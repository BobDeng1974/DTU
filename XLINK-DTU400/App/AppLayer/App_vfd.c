#define _InVfd

#include "app_vfd.h"
#include "includes.h"
/*
  ******************************************************************************
  * @�ļ�    vfd_com.c 
  * @����    ������ͱ�Ƶ��֮���ͨ��
  ******************************************************************************
*/ 

#if MONITOR_METER_ON
static u8 Meter_Writer_Flag =0;
static char Meter_Writer_Data[6]={0};
#endif

#define MODBUS_ERR_NONE         0
#define MODBUS_ERR_RCV_TIMEOUT  1
#define MODBUS_ERR_COM_ALARM    2


typedef enum {
    WRITE_PLT_INIT = 0,
    WRITE_PLT_EN1,
    WRITE_PLT_EN2,
    WRITE_PLT_EN3,
    WRITE_PLT_WRITE,
}WRITE_PLT_ENUM;


static DELAY_TIMER VfdDelay;
static DELAY_TIMER MeterDelay;
static DELAY_TIMER Meter2Delay;

u8  MainVfd_Addr = 1;
u8  Flow_Addr ;
u8  Meter_Addr ;


MODBUS_HOST_STRUCT  *pVfd = &RS4850Modbus;
MODBUS_HOST_STRUCT  *pMeter = &RS4851Modbus;
MODBUS_HOST_STRUCT  *pMeter2 = &RS4852Modbus;

static u8 RevCanBuf[12];
static u8 RevCanFlag = 0;

void    RS4850_ChgToSnd(void)
{
    RS485_0_SND_STATUS;    

    USART1->CR1 &= USART_RX_OFF;                                    //  ���չر�  
    USART1->CR1 |= USART_TX_ON;                                     //  ���Ϳ���

    USART_ClearFlag(USART1, USART_FLAG_TC | USART_FLAG_RXNE);
#if !UART1_DMA_EN
    USART_ITConfig(USART1, USART_IT_TC, ENABLE);                    //  ����TXE�ж�
#endif    
}


void    RS4850_ChgToRcv(void)
{
    USART1->CR1 &= USART_TX_OFF;                                     //  ���͹ر�  
    USART1->CR1 |= USART_RX_ON;                                      //  ���տ���
    RS485_0_RCV_STATUS;                                             //  ��������
}



void    RS4851_ChgToSnd(void)
{
    RS485_1_SND_STATUS;    

    USART3->CR1 &= USART_RX_OFF;                                    //  ���չر�  
    USART3->CR1 |= USART_TX_ON;                                     //  ���Ϳ���

    USART_ClearFlag(USART3, USART_FLAG_TC | USART_FLAG_RXNE);
    USART_ITConfig(USART3, USART_IT_TC, ENABLE);                    //  ����TXE�ж�
}

void    RS4851_ChgToRcv(void)
{
    USART3->CR1 &= USART_TX_OFF;                                     //  ���͹ر�  
    USART3->CR1 |= USART_RX_ON;                                      //  ���տ���
    RS485_1_RCV_STATUS;                                             //  ��������
}

void    RS4852_ChgToSnd(void)
{
    RS485_2_SND_STATUS;    

    UART4->CR1 &= USART_RX_OFF;                                    //  ���չر�  
    UART4->CR1 |= USART_TX_ON;                                     //  ���Ϳ���

    USART_ClearFlag(UART4, USART_FLAG_TC | USART_FLAG_RXNE);
    USART_ITConfig(UART4, USART_IT_TC, ENABLE);                    //  ����TXE�ж�
}

void    RS4852_ChgToRcv(void)
{
    UART4->CR1 &= USART_TX_OFF;                                     //  ���͹ر�  
    UART4->CR1 |= USART_RX_ON;                                      //  ���տ���
    RS485_2_RCV_STATUS;                                             //  ��������
}


void    ModbusAddrInit(MODBUS_HOST_STRUCT *pMdBus, bool bMaster, UART_COM_ENUM UartNum, char NodeAddr)
{
    memset((char *)pMdBus, 0, sizeof(MODBUS_HOST_STRUCT));
    pMdBus->UartNum = UartNum;
    pMdBus->Addr    = NodeAddr;
    pMdBus->Stat.Bits.bMaster = bMaster;
}

void    ModbusFunInit(MODBUS_HOST_STRUCT *pMdBus, pFun pChgToSnd, pFun pChgToRcv)
{
    pMdBus->pChgToSnd = pChgToSnd;
    pMdBus->pChgToRcv = pChgToRcv;

    pMdBus->pChgToRcv();
}

void    ModbusOvTmInit(MODBUS_HOST_STRUCT *pMdBus, u16 CharOvTimer, u16 PrtlOvTimer)
{
    pMdBus->CharOvTimer = CharOvTimer;
    pMdBus->PrtlOvTimer = PrtlOvTimer;
}


char    ModbusRcvTimeOut(MODBUS_HOST_STRUCT *pMdBus)
{
    char ret = MODBUS_ERR_NONE;
    
    if(pMdBus->Stat.Bits.RcvFlg) {
        if(++pMdBus->RcvTimer >= pMdBus->PrtlOvTimer) {
            pMdBus->RcvTimer = 0;
            pMdBus->Stat.Bits.RcvFlg = FALSE;
            ret = MODBUS_ERR_RCV_TIMEOUT;
        }
    }
    
    return ret;
}

char    ModbusRcvFinish(MODBUS_HOST_STRUCT *pMdBus)
{
    char ret = FALSE;

    if(pMdBus->Stat.Bits.RxStart) {
        if(++pMdBus->RcvTimer >= pMdBus->CharOvTimer) {
            pMdBus->RcvTimer = 0;
            pMdBus->Stat.Bits.RxStart = FALSE;
            pMdBus->Stat.Bits.RcvFlg = FALSE;                       //  ����ձ�ʶ
            ret = TRUE;
        }
    }

    return ret;
}

bool    IsModbusMaster(MODBUS_HOST_STRUCT *pMdBus)
{
    return pMdBus->Stat.Bits.bMaster;
}

void    ModbusUartIsr(MODBUS_HOST_STRUCT *pMdBus, char dat)
{
    if(FALSE == pMdBus->Stat.Bits.RxStart) 
    {
        pMdBus->RcvBuf[0] = dat;
        if(pMdBus->RcvBuf[0] == pMdBus->Addr) 
        {                     //  ���ַ��ȷ       
            pMdBus->RcvCnt = 1;
            pMdBus->RcvTimer = 0;                                   //  ���㳬ʱ������ 
            pMdBus->Stat.Bits.RxStart = TRUE;                       //  ��ʾ�����Ѿ���ʼ
        } 
    } 
    else 
    {
        if(pMdBus->RcvCnt < sizeof(pMdBus->RcvBuf)) 
        {
            pMdBus->RcvBuf[pMdBus->RcvCnt++] = dat;
        }
        pMdBus->RcvTimer = 0;
    }
}


char    ModbusSendNextChar(MODBUS_HOST_STRUCT *pMdBus)
{
    if(pMdBus->Stat.Bits.SndFlg) 
    {
        if(++pMdBus->SndCnt >= pMdBus->SndSize) 
        {
            pMdBus->Stat.Bits.SndFlg = FALSE;
            pMdBus->Stat.Bits.RcvFlg = TRUE;
            pMdBus->RcvCnt = 0;
            pMdBus->RcvTimer = 0;            
#if RS485_SEND_DELAY
            if(DebugUart == pMdBus->UartNum) 
            {                      //  ���Դ��ڲ�����ʱ�л�
                pMdBus->pChgToRcv();
            } 
            else 
            {
                pMdBus->DelayToRcv.Flag  = TRUE;
                pMdBus->DelayToRcv.Timer = 0;
            }
#else
            pMdBus->pChgToRcv();
#endif            
            return TRUE;
        }
        
        UartSendData(pMdBus->UartNum, &pMdBus->SndBuf[pMdBus->SndCnt], 1);
    }
    return FALSE;
}

#if RS485_SEND_DELAY
void    ModbusDelayToRcv(MODBUS_HOST_STRUCT *pMdBus, u16 Nms)
{
    if(NULL == pMdBus) {
        return;
    }
    
    if(pMdBus->DelayToRcv.Flag) 
    {
        if(++pMdBus->DelayToRcv.Timer >= Nms) 
        {
            pMdBus->DelayToRcv.Flag = FALSE;
            pMdBus->pChgToRcv();
        }
    }
}
#endif



bool    ModbusRcvCrcChk(MODBUS_HOST_STRUCT *pMdBus)
{
    bool ret = FALSE;
    u16 CalCrc, RcvCrc;    
    u16 Len = pMdBus->RcvCnt;
    char *pBuf = pMdBus->RcvBuf;

    if(Len > 5) {
        CalCrc = CRC16_1(pBuf, Len-2);
        RcvCrc = (pBuf[Len-1] << 8) | pBuf[Len-2];
        if(CalCrc == RcvCrc) {
            ret = TRUE;
        }
    }
    
    return ret;
}



void    VfdWriteRegPacket(MODBUS_HOST_STRUCT *pMdBus, u16 Addr, u16 Data)
{
    u16 Crc;
    char *p = pMdBus->SndBuf;

    *p++ = pMdBus->Addr;
    *p++ = 0x06;
    *p++ = Addr >> 8;
    *p++ = Addr & 0xff;
    *p++ = Data >> 8;
    *p++ = Data & 0xff;
    Crc = CRC16_1(pMdBus->SndBuf, p - pMdBus->SndBuf);
    *p++ = (char)Crc;
    *p++ = (char)(Crc >> 8);
    pMdBus->SndSize = p - pMdBus->SndBuf;
}

void    VfdReadRegPacket(MODBUS_HOST_STRUCT *pMdBus, u16 Addr, u16 Len)
{
    u16 Crc;
    char *p = pMdBus->SndBuf;

    *p++ = pMdBus->Addr;
    *p++ = 0x03;
    *p++ = Addr >> 8;
    *p++ = Addr & 0xff;
    *p++ = Len >> 8;
    *p++ = Len & 0xff;
    Crc = CRC16_1(pMdBus->SndBuf, p - pMdBus->SndBuf);
    *p++ = (char)Crc;
    *p++ = (char)(Crc >> 8);
    pMdBus->SndSize = p - pMdBus->SndBuf;
}

void    VfdWriteMoreRegPacket(MODBUS_HOST_STRUCT *pMdBus, u16 Addr, u16 RegNum,u8 DataLen, u8 *Data)
{
    u8 i;
    u16 Crc;
    char *p = pMdBus->SndBuf;

    *p++ = pMdBus->Addr;
    *p++ = 0x10;
    *p++ = Addr >> 8;
    *p++ = Addr & 0xff;
    *p++ = RegNum >> 8;
    *p++ = RegNum & 0xff;
    *p++ = DataLen;
    for(i=0; i<DataLen; i++)
    {
        *p++ = *Data++;
    }
    
    Crc = CRC16_1(pMdBus->SndBuf, p - pMdBus->SndBuf);
    *p++ = (char)Crc;
    *p++ = (char)(Crc >> 8);
    pMdBus->SndSize = p - pMdBus->SndBuf;
}



void    VfdCtrlInit(void)
{   
    memset((char *)&VfdDelay, 0, sizeof(VfdDelay));
    memset((char *)&VfdCtrl,  0, sizeof(VfdCtrl));
    VfdCtrl.Bits.bReadMainVfd  = TRUE;
    VfdCtrl.ReadIndex = 0;
}

void    VfdWaitResp(u8 Cmd, u16 Addr)
{
    VfdCtrl.WaitRspCmd = Cmd;
    VfdCtrl.WaitRspAddr = Addr;
}

void    MeterWaitResp(u8 Cmd, u16 Addr)
{
    MeterCtrl.WaitRspCmd = Cmd;
    MeterCtrl.WaitRspAddr = Addr;
}


void    VfdClrTimeOut(void)
{
    VfdOptRlt.Bits.RcvRsp  = FALSE;
    VfdOptRlt.Bits.RspRlt  = FALSE;
    VfdOptRlt.Bits.RspTimeOut = FALSE;
}

void    MeterClrTimeOut(void)
{
    MeterOptRlt.Bits.RcvRsp  = FALSE;
    MeterOptRlt.Bits.RspRlt  = FALSE;
    MeterOptRlt.Bits.RspTimeOut = FALSE;
}

void    Meter2ClrTimeOut(void)
{
    Meter2OptRlt.Bits.RcvRsp  = FALSE;
    Meter2OptRlt.Bits.RspRlt  = FALSE;
    Meter2OptRlt.Bits.RspTimeOut = FALSE;
}


void    ChgVfdAddr(MODBUS_HOST_STRUCT *pMdBus, u8 NodeID)
{
    pMdBus->Addr = NodeID;
}


void  SetMeterReadIndex(void)
{
# if 0 
	u16 tmp;

	tmp = (GpSysPara->Dev.BindSatus & (3<<1))>>1;

	if ( 2== tmp) //ֻ��������
	{
		if (GpSysStat->Share.DTU_COMM_STATUS | (1<<1))
			{
				GpSysStat->Share.DTU_COMM_STATUS &= ~(1<<1);
			}
		
		MeterCtrl.ReadIndex = FLOW_TRANSIENT_FLOW;
	}
	else if (1 == tmp)  // ֻ�е��
	{
		if (GpSysStat->Share.DTU_COMM_STATUS | (1<<2))
			{
				GpSysStat->Share.DTU_COMM_STATUS &= ~(1<<2);	
			}
		MeterCtrl.ReadIndex = XY_VOLTAGE_A;
	}
	else 
	{	
		MeterCtrl.ReadIndex = XY_VOLTAGE_A;
	}
#endif 
	MeterCtrl.ReadIndex = XY_VOLTAGE_A;

}
static void MeterReadIndexChg(void)
{
	//u16 tmp;
#if 0
	tmp = (GpSysPara->Dev.BindSatus & (3<<1))>>1;
	/* ֻ�е�� */
	if (1 == tmp)
	{
	   if(++MeterCtrl.ReadIndex > XY_CT)                
            {
                MeterCtrl.ReadIndex = XY_VOLTAGE_A;        
            } 
	}
	/*  ֻ�������� */  
	else if (2 == tmp)
	{
		   if(++MeterCtrl.ReadIndex > FLOW_NUMBER)                
                {
                    MeterCtrl.ReadIndex = FLOW_TRANSIENT_FLOW;        
                } 
	}
	/* ���������� */
	else if (3== tmp)
	{
		   if(++MeterCtrl.ReadIndex > FLOW_NUMBER)                
                {
                    MeterCtrl.ReadIndex = XY_VOLTAGE_A;        
                } 
	}
#endif 
	if(++MeterCtrl.ReadIndex > FLOW_NUMBER) 			   
	 {
		 MeterCtrl.ReadIndex = XY_VOLTAGE_A;		
	 } 


}
void    ReadIndexChg(void)
{  
    switch(GpSysPara->Dev.Bits.PS) {

        case PS_MAM860C_880C:
        case PS_MAM860_870_880:
       
            if(++VfdCtrl.ReadIndex > PLT_Man_ALLOW_WRITE) {
                VfdCtrl.ReadIndex = PLT_Man_PRESSURE;        
            } 
            break;
            
        case PS_MAM6080_6090:
        case PS_MAM6080_1:
        case PS_MAM6090:
		case PS_MAM6070:
            if(++VfdCtrl.ReadIndex >= PLT_ALLOW_WRITE) {
                VfdCtrl.ReadIndex = PLT_PRESSURE;        
            }    
            break;
        case PS_HC:
        	if (++VfdCtrl.ReadIndex >H2U_ALLOW_WRITE)
        	{
        		  VfdCtrl.ReadIndex = H2U_PRESSURE;     
        	}
        	break;
        #if MONITOR_METER_ON
        case PS_TYPE_XY_1942E:
            if(++VfdCtrl.ReadIndex > XY_CT)                
            {
                VfdCtrl.ReadIndex = XY_VOLTAGE_A;        
            }    
            break;
        #endif
            
        default:
           if(++VfdCtrl.ReadIndex >= PLT_ALLOW_WRITE) {
                VfdCtrl.ReadIndex = PLT_PRESSURE;        
            } 
            break;
    }
}



void    ModbusDelay(void)
{
    u32 i;
    for(i=0; i<1000; i++);
}

void    ModbusStartSend(MODBUS_HOST_STRUCT *pMdBus)
{
    pMdBus->pChgToSnd();       
    
    ModbusDelay();                                                  //  �л�Ϊ����״̬����ʱһ��ʱ��
    
    pMdBus->RcvCnt = 0;    
    pMdBus->RcvTimer = 0;
    pMdBus->Stat.Bits.RcvFlg  = FALSE;    
    pMdBus->Stat.Bits.RxStart = FALSE;
    
    pMdBus->SndCnt = 0;
    pMdBus->Stat.Bits.SndFlg = TRUE;
    UartIntSendData(pMdBus->UartNum, pMdBus->SndBuf, pMdBus->SndSize);   //  ���͵�һ������
}

/*
void    VfdOptResultAlarm(MODBUS_HOST_STRUCT *pMdBus, bool bOptStat)
{         
    static u8 CommErrCnt = 0;
    static u8 CommOkCnt  = 0;
    static u8 CommOKFlag = FALSE;


	
    if(TRUE == bOptStat) {
        CommErrCnt = 0;
        if(++CommOkCnt >= 10) {
            CommOkCnt = 10;
            CommOKFlag = TRUE;
            AlmStat.Alarm.Bits.RS485CommErr = FALSE;                //  ͨ������
        }
    } else {
        CommOkCnt = 0;      
        if(++CommErrCnt >= 20) {
            CommErrCnt = 20;
            if(!AlmStat.Alarm.Bits.RS485CommErr) {
                AlmStat.Alarm.Bits.RS485CommErr = TRUE;             //  ͨ���쳣
                RS485ErrTime = BeforeReg;
                if(CommOKFlag) {                                    //  ����ͨ�ź���쳣�żƴ�
                    (GpOthInfo->Error.RS485ErrCnt < 0xffff) ? GpOthInfo->Error.RS485ErrCnt++ : 0xffff;
                }
            }
        }
        GpSysStat->Share.RS485ErrCnt++;        
    }   
}
*/

static int GetComIndex(MODBUS_HOST_STRUCT *pMdBus)
{
	if ( pVfd == pMdBus)
		return 0;
	else if (pMeter == pMdBus)
		{
			if (pMdBus->Addr == Meter_Addr)
				return 1;
			else
				return 2;
		}
	else
			return  3;
	
}
void Rs485ComOptResultAlarm(MODBUS_HOST_STRUCT *pMdBus, bool bOptStat)
{
	static u8 CommErrCnt[4] = {0};
    static u8 CommOkCnt[4]  = {0};
    static u8 CommOKFlag[4] = {0};
	static  u16 LastDTuStatus  = 0; 
	int  index;

	
		
	index = GetComIndex(pMdBus);
	
	if(TRUE == bOptStat) {
        CommErrCnt[index] = 0;
        if(++CommOkCnt[index] >= RS485_COMOK_CNTS) {
            CommOkCnt[index] = RS485_COMOK_CNTS;
           CommOKFlag[index] = TRUE;
         //   AlmStat.Alarm.Bits.RS485CommErr = FALSE;                //  ͨ������
         //	AlmStat.Alarm.Flag &= ~(1<< (index+24));   
         	GpSysStat->Share.DTU_COMM_STATUS |= 1<<index;
        }
    } else {
        CommOkCnt[index] = 0;      
        if(++CommErrCnt[index] >= RS485_COMERR_CNTS) {
            CommErrCnt[index] = RS485_COMERR_CNTS;
          //  if(!(AlmStat.Alarm.Flag & (1<<(24+index)))) {
              //  AlmStat.Alarm.Bits.RS485CommErr = TRUE;             //  ͨ���쳣
			//	AlmStat.Alarm.Flag |= (1<<(24+index));
			  	GpSysStat->Share.DTU_COMM_STATUS &= ~(1<<index);
			//  RS485ErrTime = BeforeReg;
               if( index ==1 && CommOKFlag[index]) {                                    //  ����ͨ�ź���쳣�żƴ�
                  (GpOthInfo->Error.RS485ErrCnt < 0xffff) ? GpOthInfo->Error.RS485ErrCnt++ : 0;
               }
           // }
		//		}
      //  GpSysStat->Share.RS485ErrCnt++;        
		}   
	}

	// ͨ��״̬�����֪ͨ�ϴ�
	if (LastDTuStatus != GpSysStat->Share.DTU_COMM_STATUS)
		{
			//GbRs485ComStat = 1;
			ParaUpdate.Bits.Rs485comstatus = TRUE;
			GpSysStat->Share.UpdataParaFlag |= 0x0008; 
			LastDTuStatus = GpSysStat->Share.DTU_COMM_STATUS;
		}
}
/*
void    MeterOptResultAlarm(MODBUS_HOST_STRUCT *pMdBus, bool bOptStat)
{         
    static u8 CommErrCnt = 0;
    static u8 CommOkCnt  = 0;
    static u8 CommOKFlag = FALSE;


	
    if(TRUE == bOptStat) {
        CommErrCnt = 0;
        if(++CommOkCnt >= 10) {
            CommOkCnt = 10;
            CommOKFlag = TRUE;
            AlmStat.Alarm.Bits.RS485MeterErr = FALSE;                //  ͨ������
        }
    } else {
        CommOkCnt = 0;      
        if(++CommErrCnt >= 20) {
            CommErrCnt = 20;
            if(!AlmStat.Alarm.Bits.RS485MeterErr) {
                AlmStat.Alarm.Bits.RS485MeterErr = TRUE;             //  ͨ���쳣
                RS485ErrTime = BeforeReg;
                if(CommOKFlag) {                                    //  ����ͨ�ź���쳣�żƴ�
                    (GpOthInfo->Error.RS485ErrCnt < 0xffff) ? GpOthInfo->Error.RS485ErrCnt++ : 0xffff;
                }
            }
        }
        GpSysStat->Share.RS485ErrCnt++;        
    }   
}

void    Meter2OptResultAlarm(MODBUS_HOST_STRUCT *pMdBus, bool bOptStat)
{         
    static u8 CommErrCnt = 0;
    static u8 CommOkCnt  = 0;
    static u8 CommOKFlag = FALSE;


	
    if(TRUE == bOptStat) {
        CommErrCnt = 0;
        if(++CommOkCnt >= 10) {
            CommOkCnt = 10;
            CommOKFlag = TRUE;
            AlmStat.Alarm.Bits.RS485CommErr = FALSE;                //  ͨ������
        }
    } else {
        CommOkCnt = 0;      
        if(++CommErrCnt >= 20) {
            CommErrCnt = 20;
            if(!AlmStat.Alarm.Bits.RS485CommErr) {
                AlmStat.Alarm.Bits.RS485CommErr = TRUE;             //  ͨ���쳣
                RS485ErrTime = BeforeReg;
                if(CommOKFlag) {                                    //  ����ͨ�ź���쳣�żƴ�
                    (GpOthInfo->Error.RS485ErrCnt < 0xffff) ? GpOthInfo->Error.RS485ErrCnt++ : 0xffff;
                }
            }
        }
        GpSysStat->Share.RS485ErrCnt++;        
    }   
}
*/
void    VfdWriteDataDebug(void)
{
#if USE_UART_DEBUG_INFO
//    bool bVfdDebugFlag;

//    bVfdDebugFlag = AppDebug.Bits.Vfd;
//    AppDebug.Bits.Vfd = TRUE;
//    DebugHexMsg("Write Data ", pVfd->SndBuf, pVfd->SndSize);
//    AppDebug.Bits.Vfd = bVfdDebugFlag;
#endif    
}


void    SetRemoteOptResult(bool bRlt)
{
    if(VfdWhoUse.Bits.Remote) {
        VfdWhoUse.Bits.Remote = FALSE;                              //  Զ�̱�Ƶ����д��������
        SvrVfdOpt.Bits.RspFlg = bRlt;                               //  �ò������
    }
}


void    VfdSetWhoUse(VFD_USE_ENUM Who, char bRead, char bMain, u16 Addr) 
{
    
    switch(Who) {
        case VFD_USE_REMOTE:
            VfdWhoUse.Bits.Remote = TRUE;
            break;
//        case VFD_USE_LEARN:
//            VfdWhoUse.Bits.Learn = TRUE;
//            break;   
        case VFD_USE_PARASET:
            VfdWhoUse.Bits.ParSet = TRUE;
            break; 
        default:
            VfdWhoUse.Bits.Local = TRUE;
            break;     
        
    }
    
    VfdWhoUse.Bits.Read = bRead;
    VfdWhoUse.Bits.Main = bMain;  
    VfdOptAddr = Addr;
}


void    Task_VfdComm(MODBUS_HOST_STRUCT *pMdBus, u16 Nms)
{
    u16 OptAddr;
    u16 WriteData;    
    
    if(!VfdInitFlag) {                                              //  �ӿ�δ��ʼ��
        SvrVfdOpt.Addr = 1009;
        SvrVfdOpt.Data = 8;
        return;
    }

    if(EnterPltParaWrite) {
        PltParaWrite(pVfd, Nms);
        return;
    }    

    if((VFD_OPT_READ_WAIT != VfdCtrl.Step) && (VFD_OPT_WRITE_WAITE != VfdCtrl.Step)) 
    {
        /*
         *  Զ��д��Ƶ��
         */
        if(RemoteWritePara) 
        {
            RemoteWritePara = FALSE;
            switch(GpSysPara->Dev.Bits.PS) {
                case PS_MAM_KY_G:
                case PS_MAM_KY_C:
                case PS_MAM260:
                case PS_MAM260A:
                case PS_MAM280:
                case PS_MAM660_670_680:    
                case PS_MAM860_870_880:            
                case PS_MAM890: 
                    EnterPltParaWrite = FALSE;
                    break;
                case PS_MAM6080_6090:
                case PS_MAM6080_1:
                case PS_MAM6090:
				case PS_MAM6070:
				case PS_MAM860C_880C:  
				case PS_HC:
                    EnterPltParaWrite = TRUE;
                    break;
                default:
                    EnterPltParaWrite = FALSE;
                    break;
            }            
            return;
        }     

        /*
         *  Զ�̿��ػ�
         */
        if(bRemoteBoot || bRemoteStop) 
        {                    
            OptAddr = 9;                                            //  д��ַ
            if(bRemoteBoot) 
            {
                bRemoteBoot = FALSE;
                RmtBootTimer.Bits.Flag = TRUE;
                RmtBootTimer.Bits.Timer = 0;
                WriteData = 0x01;                                   //  Զ�̿���
            } 
            else if(bRemoteStop) 
            {
                bRemoteStop = FALSE;   
                RmtStopTimer.Bits.Flag = TRUE;
                RmtStopTimer.Bits.Timer = 0;
                WriteData = 0x02;                                   //  Զ��ͣ��
            }
            
            ChgVfdAddr(pMdBus, MainVfd_Addr);
            VfdWriteRegPacket(pMdBus, OptAddr, WriteData);                    
            VfdWaitResp(0x06, OptAddr); 
            VfdCtrl.Step = VFD_OPT_WRITE;            
            VfdSetWhoUse(VFD_USE_LOCAL, VFD_WRITE, VFD_MAIN, OptAddr);
        }
    }
    
#if MONITOR_METER_ON    
    if(Meter_Writer_Flag) //д�������
    {    
        Meter_Writer_Flag =0;
        OptAddr = 47;            
        ChgVfdAddr(pMdBus, MainVfd_Addr);    
        VfdWriteMoreRegPacket(pMdBus, OptAddr,0x03,0x06, (u8 *)Meter_Writer_Data);        
        VfdWaitResp(0x10, OptAddr); 
        VfdCtrl.Step = VFD_OPT_WRITE;            
        VfdSetWhoUse(VFD_USE_LOCAL, VFD_WRITE, VFD_MAIN, OptAddr);
    }
#endif    
    
    switch(VfdCtrl.Step) {          
        case VFD_OPT_READ:
            if(VfdDelay.Bits.Flag) {                                //  �������ʱ
                VfdDelay.Bits.Timer += Nms;
                if(VfdDelay.Bits.Timer >= VFD_READ_OPT_INTEVAL) {
                    VfdDelay.Val = 0;
                }
            }

            if(!VfdDelay.Bits.Flag) {                               //  �Ƕ�������ʱ״̬
                VfdCtrl.Retry = 0;
                VfdClrTimeOut();    

                OptAddr = GetPltReadAddr();       
                                  
                ChgVfdAddr(pMdBus, MainVfd_Addr);
                VfdReadRegPacket(pMdBus, OptAddr, 1);
                VfdWaitResp(0x03, OptAddr); 
                ModbusStartSend(pMdBus);
                VfdSetWhoUse(VFD_USE_LOCAL, VFD_READ, VFD_MAIN, OptAddr);
                VfdCtrl.Step = VFD_OPT_READ_WAIT;
            }
            break;
        case VFD_OPT_READ_WAIT:            
            if(VfdOptRlt.Bits.RcvRsp && VfdOptRlt.Bits.RspRlt) {        //  �����ݳɹ�
                VfdDelay.Bits.Flag = TRUE;
                SetRemoteOptResult(TRUE);
                Rs485ComOptResultAlarm(pMdBus, TRUE);
                ReadIndexChg();                                     //  ����һ��ַ
                VfdCtrl.Step = VFD_OPT_READ;
            } else if(VfdOptRlt.Bits.RspTimeOut || (VfdOptRlt.Bits.RcvRsp && (!VfdOptRlt.Bits.RspRlt))) { //  ������ʧ��
               if(++VfdCtrl.Retry >= VFD_READ_RETRY_NUM) {
                    VfdCtrl.Retry = 0;
                    SetRemoteOptResult(FALSE);
                    Rs485ComOptResultAlarm(pMdBus, FALSE);
                    ReadIndexChg();                         
                    VfdCtrl.Step = VFD_OPT_READ;
                } else {                                            //  ����
                    VfdClrTimeOut();    
                    ModbusStartSend(pMdBus);
                }
            } 
            break;
        case VFD_OPT_WRITE:
            VfdCtrl.Retry = 0;
            VfdClrTimeOut();    
            ModbusStartSend(pMdBus);
            VfdCtrl.Step = VFD_OPT_WRITE_WAITE;
            break;
        case VFD_OPT_WRITE_WAITE:
            if(VfdOptRlt.Bits.RcvRsp && VfdOptRlt.Bits.RspRlt) {    //  �����ݳɹ�
                VfdDelay.Bits.Flag = TRUE;
                SetRemoteOptResult(TRUE);
                Rs485ComOptResultAlarm(pMdBus, TRUE);
                VfdCtrl.Step = VFD_OPT_READ;
            } else if(VfdOptRlt.Bits.RspTimeOut || (VfdOptRlt.Bits.RcvRsp && (!VfdOptRlt.Bits.RspRlt))) { //  ������ʧ��
                if(++VfdCtrl.Retry >= VFD_WRITE_RETRY_NUM) {
                    VfdCtrl.Retry = 0;
                    SetRemoteOptResult(FALSE);
                    Rs485ComOptResultAlarm(pMdBus, FALSE);
                    VfdCtrl.Step = VFD_OPT_READ;
                } else {                                            //  ����
                    VfdClrTimeOut();    
                    ModbusStartSend(pMdBus);
                }
            } 
            break;
        default:
            VfdCtrl.Step = VFD_OPT_READ;
            break;

    }
    
}



void    Task_MeterComm(MODBUS_HOST_STRUCT *pMdBus, u16 Nms)
{
    u16 OptAddr;
    u8  DataLen;
//    u16 WriteData;    
    
#if MONITOR_METER_ON    
    if(Meter_Writer_Flag) //д�������
    {    
        Meter_Writer_Flag =0;
        OptAddr = 47;            
        ChgVfdAddr(pMdBus, Meter_Addr);    
        VfdWriteMoreRegPacket(pMdBus, OptAddr,0x03,0x06, (u8 *)Meter_Writer_Data);        
        MeterWaitResp(0x10, OptAddr); 
        MeterCtrl.Step = VFD_OPT_WRITE;            
    //    VfdSetWhoUse(VFD_USE_LOCAL, VFD_WRITE, VFD_MAIN, OptAddr);
        MeterWhoUse.Bits.Read =  VFD_WRITE;
        MeterWhoUse.Bits.Local = TRUE;
    }
#endif    
    
    switch(MeterCtrl.Step) 
    {          
        case VFD_OPT_READ:
            if(MeterDelay.Bits.Flag) 
            {                                //  �������ʱ
                MeterDelay.Bits.Timer += Nms;
                if(MeterDelay.Bits.Timer >= METER_READ_OPT_INTEVAL) 
                {
                    MeterDelay.Val = 0;
                }
            }
            else
            {                               //  �Ƕ�������ʱ״̬
                MeterCtrl.Retry = 0;
                MeterClrTimeOut();                                      
                GetMeterReadAddr(&OptAddr,&DataLen);       
                if(MeterCtrl.ReadIndex > XY_CT)                
                { 
                    Flow_Addr = GpSysPara->Dev.FlowNum;
                    ChgVfdAddr(pMdBus, Flow_Addr);//��������
                }
                else
                {
                    Meter_Addr = GpSysPara->Dev.MeterNum;
                    ChgVfdAddr(pMdBus, Meter_Addr);//�����
                }
                VfdReadRegPacket(pMdBus, OptAddr, DataLen/2);
                MeterWaitResp(0x03, OptAddr); 
                ModbusStartSend(pMdBus);
//                VfdSetWhoUse(VFD_USE_LOCAL, VFD_READ, VFD_MAIN, OptAddr);
                MeterWhoUse.Bits.Read =  VFD_READ;
                MeterWhoUse.Bits.Local = TRUE;
                
                MeterCtrl.Step = VFD_OPT_READ_WAIT;
            }
            break;
            
        case VFD_OPT_READ_WAIT:            
            if(MeterOptRlt.Bits.RcvRsp && MeterOptRlt.Bits.RspRlt) 
            {        //  �����ݳɹ�
                MeterDelay.Bits.Flag = TRUE;
              //  SetRemoteOptResult(TRUE);
               Rs485ComOptResultAlarm(pMdBus, TRUE);
			 	 MeterReadIndexChg();//  ����һ��ַ  
              //  ReadIndexChg();  //  ����һ��ַ  
           //       if(++MeterCtrl.ReadIndex > FLOW_NUMBER)                
           //     {
          //          MeterCtrl.ReadIndex = XY_VOLTAGE_A;        
            //    } 
                MeterCtrl.Step = VFD_OPT_READ;
            } 
            else if(MeterOptRlt.Bits.RspTimeOut || (MeterOptRlt.Bits.RcvRsp && (!MeterOptRlt.Bits.RspRlt))) 
            { //  ������ʧ��
               if(++MeterCtrl.Retry >= VFD_READ_RETRY_NUM) 
               {
                    MeterCtrl.Retry = 0;
                   // SetRemoteOptResult(FALSE);
                    Rs485ComOptResultAlarm(pMdBus, FALSE);
                   // ReadIndexChg(); 
                    MeterReadIndexChg();//  ����һ��ַ  
//                       if(++MeterCtrl.ReadIndex > FLOW_NUMBER)                
//                    {
//                        MeterCtrl.ReadIndex = XY_VOLTAGE_A;        
//                    } 
                    MeterCtrl.Step = VFD_OPT_READ;
               } 
               else 
               {                                            //  ����
                    MeterClrTimeOut();    
                    ModbusStartSend(pMdBus);
               }
            } 
            break;
            
        case VFD_OPT_WRITE:
            MeterCtrl.Retry = 0;
            MeterClrTimeOut();    
            ModbusStartSend(pMdBus);
            MeterCtrl.Step = VFD_OPT_WRITE_WAITE;
            break;
        
        case VFD_OPT_WRITE_WAITE:
            if(MeterOptRlt.Bits.RcvRsp && MeterOptRlt.Bits.RspRlt) 
            {    //  �����ݳɹ�
                MeterDelay.Bits.Flag = TRUE;
             //   SetRemoteOptResult(TRUE);
                Rs485ComOptResultAlarm(pMdBus, TRUE);
                MeterCtrl.Step = VFD_OPT_READ;
            } 
            else if(MeterOptRlt.Bits.RspTimeOut || (MeterOptRlt.Bits.RcvRsp && (!MeterOptRlt.Bits.RspRlt))) 
            { //  ������ʧ��
                if(++MeterCtrl.Retry >= VFD_WRITE_RETRY_NUM) 
                {
                    MeterCtrl.Retry = 0;
                //    SetRemoteOptResult(FALSE);
                    Rs485ComOptResultAlarm(pMdBus, FALSE);
                    MeterCtrl.Step = VFD_OPT_READ;
                } 
                else 
                {                                            //  ����
                    MeterClrTimeOut();    
                    ModbusStartSend(pMdBus);
                }
            } 
            break;
        default:
            MeterCtrl.Step = VFD_OPT_READ;
            break;
    }
}


void    Task_Meter2Comm(MODBUS_HOST_STRUCT *pMdBus, u16 Nms)
{
    u16 OptAddr;
    u8  DataLen;
    
    
    switch(Meter2Ctrl.Step) 
    {          
        case VFD_OPT_READ:
            if(Meter2Delay.Bits.Flag) 
            {                                //  �������ʱ
                Meter2Delay.Bits.Timer += Nms;
                if(Meter2Delay.Bits.Timer >= METER_READ_OPT_INTEVAL) 
                {
                    Meter2Delay.Val = 0;
                }
            }
            else
            {                               //  �Ƕ�������ʱ״̬
                Meter2Ctrl.Retry = 0;
                Meter2ClrTimeOut();                                      
                GetMeter2ReadAddr(&OptAddr,&DataLen);       
                if(Meter2Ctrl.ReadIndex > XY_CT)                
                { 
                    Flow_Addr = GpSysPara->Dev.FlowNum;
                    ChgVfdAddr(pMdBus, Flow_Addr);//��������
                }
                else
                {
                    Meter_Addr = GpSysPara->Dev.MeterNum;
                    ChgVfdAddr(pMdBus, Meter_Addr);//�����
                }
                VfdReadRegPacket(pMdBus, OptAddr, DataLen/2);
                MeterWaitResp(0x03, OptAddr); 
                ModbusStartSend(pMdBus);
//                VfdSetWhoUse(VFD_USE_LOCAL, VFD_READ, VFD_MAIN, OptAddr);
                Meter2WhoUse.Bits.Read =  VFD_READ;
                Meter2WhoUse.Bits.Local = TRUE;
                
                Meter2Ctrl.Step = VFD_OPT_READ_WAIT;
            }
            break;
            
        case VFD_OPT_READ_WAIT:            
            if(Meter2OptRlt.Bits.RcvRsp && Meter2OptRlt.Bits.RspRlt) 
            {        //  �����ݳɹ�
                Meter2Delay.Bits.Flag = TRUE;
              //  SetRemoteOptResult(TRUE);
             //   Rs485ComOptResultAlarm(pMdBus, TRUE);
              //  ReadIndexChg();  //  ����һ��ַ  
                  if(++Meter2Ctrl.ReadIndex > FLOW_NUMBER)                
                {
                    Meter2Ctrl.ReadIndex = XY_VOLTAGE_A;        
                } 
                Meter2Ctrl.Step = VFD_OPT_READ;
            } 
            else if(Meter2OptRlt.Bits.RspTimeOut || (Meter2OptRlt.Bits.RcvRsp && (!Meter2OptRlt.Bits.RspRlt))) 
            { //  ������ʧ��
               if(++Meter2Ctrl.Retry >= VFD_READ_RETRY_NUM) 
               {
                    Meter2Ctrl.Retry = 0;
                   // SetRemoteOptResult(FALSE);
                  //  Rs485ComOptResultAlarm(pMdBus, FALSE);
                   // ReadIndexChg(); 
                       if(++Meter2Ctrl.ReadIndex > FLOW_NUMBER)                
                    {
                        Meter2Ctrl.ReadIndex = XY_VOLTAGE_A;        
                    } 
                    Meter2Ctrl.Step = VFD_OPT_READ;
               } 
               else 
               {                                            //  ����
                    Meter2ClrTimeOut();    
                    ModbusStartSend(pMdBus);
               }
            } 
            break;
            
        case VFD_OPT_WRITE:
            Meter2Ctrl.Retry = 0;
            Meter2ClrTimeOut();    
            ModbusStartSend(pMdBus);
            Meter2Ctrl.Step = VFD_OPT_WRITE_WAITE;
            break;
        
        case VFD_OPT_WRITE_WAITE:
            if(Meter2OptRlt.Bits.RcvRsp && Meter2OptRlt.Bits.RspRlt) 
            {    //  �����ݳɹ�
                Meter2Delay.Bits.Flag = TRUE;
             //   SetRemoteOptResult(TRUE);
             //   Rs485ComOptResultAlarm(pMdBus, TRUE);
                Meter2Ctrl.Step = VFD_OPT_READ;
            } 
            else if(Meter2OptRlt.Bits.RspTimeOut || (Meter2OptRlt.Bits.RcvRsp && (!Meter2OptRlt.Bits.RspRlt))) 
            { //  ������ʧ��
                if(++Meter2Ctrl.Retry >= VFD_WRITE_RETRY_NUM) 
                {
                    Meter2Ctrl.Retry = 0;
                //    SetRemoteOptResult(FALSE);
                //    Rs485ComOptResultAlarm(pMdBus, FALSE);
                    Meter2Ctrl.Step = VFD_OPT_READ;
                } 
                else 
                {                                            //  ����
                    Meter2ClrTimeOut();    
                    ModbusStartSend(pMdBus);
                }
            } 
            break;
        default:
            Meter2Ctrl.Step = VFD_OPT_READ;
            break;
    }
}



pFun VfdChgToSnd = RS4850_ChgToSnd;
pFun VfdChgToRcv = RS4850_ChgToRcv;

pFun MeterChgToSnd = RS4851_ChgToSnd;
pFun MeterChgToRcv = RS4851_ChgToRcv;

pFun Meter2ChgToSnd = RS4852_ChgToSnd;
pFun Meter2ChgToRcv = RS4852_ChgToRcv;

void    Task_VfdDeal(void)
{    
    u8  Cmd;
    u16 Len;
    u16 Data, Addr;

    if(!VfdInitFlag) {
        return;
    }
    
    if(GbEventFlag) {
        GbEventFlag = FALSE;
        if(ModbusRcvFinish(pVfd)) {            
            VfdOptRlt.Bits.RcvRsp = TRUE;
            VfdOptRlt.Bits.RspRlt = FALSE;

            Cmd = pVfd->RcvBuf[1];            
            if(0x03 == Cmd) {                                       //  ������Ӧ��
                if(2 != pVfd->RcvBuf[2]) {                          //  �����Ĵ�����
                    return;
                }
                
                Len = 5 + pVfd->RcvBuf[2];
                if(pVfd->RcvCnt > Len) {
                    pVfd->RcvCnt = Len;                             //  �������ݹ���
                }   
            } else if(0x06 == Cmd) {                                //  д����Ӧ��
                pVfd->RcvCnt = (pVfd->RcvCnt > 8) ? 8 : pVfd->RcvCnt;//  �������ݹ���
                Addr = (pVfd->RcvBuf[2] << 8) | pVfd->RcvBuf[3];
                Data = (pVfd->RcvBuf[4] << 8) | pVfd->RcvBuf[5];
            } else if(0x10 == Cmd){
                pVfd->RcvCnt = (pVfd->RcvCnt > 8) ? 8 : pVfd->RcvCnt;//  �������ݹ���
                Addr = (pVfd->RcvBuf[2] << 8) | pVfd->RcvBuf[3];
                Data = (pVfd->RcvBuf[4] << 8) | pVfd->RcvBuf[5];
            }
            else {
                return;
            }

            if(!ModbusRcvCrcChk(pVfd)) {
                return;
            }
            
            if(0x03 == Cmd) {
                if(VfdWhoUse.Bits.Read) {
                    VfdOptRlt.Bits.RspRlt = TRUE;    
                    if(VfdWhoUse.Bits.Remote) {
                        SvrVfdOpt.Data = Data;
                        return;                                        //  Զ�̶���Ƶ���ɹ�,�˳�
                    }
                }
        
                Data = (pVfd->RcvBuf[3] << 8) | pVfd->RcvBuf[4]; 
                switch(GpSysPara->Dev.Bits.PS) {
                    case PS_MAM_KY_G:
                    case PS_MAM_KY_C:
                    case PS_MAM260:
                    case PS_MAM260A:
                    case PS_MAM280:
                    case PS_MAM660_670_680:    
                    case PS_MAM860_870_880:
                    case PS_MAM860C_880C:  
                    case PS_MAM890: 
                        PltDevMan860DataPrase(VfdCtrl.ReadIndex, Data, GpSysPara->Dev.Bits.PS);
                        break;
                    case PS_MAM6080_6090:
                    case PS_MAM6080_1:
                    case PS_MAM6090:
					case PS_MAM6070:
                        PltDevDataPrase(VfdCtrl.ReadIndex, Data, GpSysPara->Dev.Bits.PS);
                        break;
                    case PS_HC:
                    	PltDevH2UParse(VfdCtrl.ReadIndex, Data);
                    	break;
                    #if MONITOR_METER_ON
                    case PS_TYPE_XY_1942E:
//                        XYDevDataDeal(Data, GpSysPara->Dev.Bits.PS);
                        break;
                    #endif
                    
                    default:
                     PltDevDataPrase(VfdCtrl.ReadIndex, Data, GpSysPara->Dev.Bits.PS);
                        break;
                }                
            } else if(0x06 == Cmd) {
                if(!VfdWhoUse.Bits.Read) {
                    VfdOptRlt.Bits.RspRlt = TRUE;
                    if(VfdWhoUse.Bits.Remote) {
                        switch(GpSysPara->Dev.Bits.PS) {
//                            case PS_MAM_KY_G:
//                            case PS_MAM_KY_C:
//                            case PS_MAM260:
//                            case PS_MAM260A:
//                            case PS_MAM280:
//                            case PS_MAM660_670_680:    
//                            case PS_MAM860_870_880:
//                            case PS_MAM860C_880C:
//                            case PS_MAM890: 
//                                break;
                            case PS_MAM6080_6090:
                            case PS_MAM6080_1:
                            case PS_MAM6090:
                            case PS_MAM6070:
                                if((0xf000 != Addr) && (0xe000 != Addr) && (0xd000 != Addr)) {
                                    SvrVfdOpt.Addr = Addr;              //  ������У���ַ
                                    SvrVfdOpt.Data = Data;
                                }
                                break;
                            default:
                                break;
                        }
                        return;                                        //  Զ��д��Ƶ���ɹ�,�˳�
                    }            
                } 
            } else if(0x10 == Cmd) {
                if(!VfdWhoUse.Bits.Read) {
                    VfdOptRlt.Bits.RspRlt = TRUE;
                    if(VfdWhoUse.Bits.Remote) {
                        switch(GpSysPara->Dev.Bits.PS) {
//                            case PS_MAM_KY_G:
//                            case PS_MAM_KY_C:
//                            case PS_MAM260:
//                            case PS_MAM260A:
//                            case PS_MAM280:
//                            case PS_MAM660_670_680:    
//                            case PS_MAM860_870_880:
//                            case PS_MAM860C_880C:
//                            case PS_MAM890: 
//                            case PS_TYPE_XY_1942E:
//                                break;
                            case PS_MAM6080_6090:
                            case PS_MAM6080_1:
                            case PS_MAM6090:
                            case PS_MAM6070:
                                if((0xf000 != Addr) && (0xe000 != Addr) && (0xd000 != Addr)) {
                                    SvrVfdOpt.Addr = Addr;              //  ������У���ַ
                                    SvrVfdOpt.Data = Data;
                                }
                                break;
                            default:
                                break;
                        }
                        return;                                        //  Զ��д��Ƶ���ɹ�,�˳�
                    }            
                } 
            }
//            memset(pVfd->RcvBuf, 0, pVfd->RcvCnt);  
            
        } else if(IsModbusMaster(pVfd)) {
            if(ModbusRcvTimeOut(pVfd)) {
                VfdOptRlt.Bits.RspTimeOut = TRUE;
            }
        }
    }
}

static u8 RecDataBuff[20];

void    Task_MeterDeal(void)
{    
    u8  Cmd,Temp;
    u16 Len;
//    u16 Data, Addr;

    
    if(GbMeterEventFlag) 
    {
        GbMeterEventFlag = FALSE;
        if(ModbusRcvFinish(pMeter)) 
        {            
            MeterOptRlt.Bits.RcvRsp = TRUE;
            MeterOptRlt.Bits.RspRlt = FALSE;

            Cmd = pMeter->RcvBuf[1];            
            if(0x03 == Cmd) 
            {                                       //  ������Ӧ��
                if(2 > pMeter->RcvBuf[2]) {          //  �����Ĵ�����
                    return;
               }
                
                Len = pMeter->RcvBuf[2];
                if(pMeter->RcvCnt > (Len +5)) 
                {
                    pMeter->RcvCnt = Len+5 ;                             //  �������ݹ���
                }   
            } 
            else if(0x06 == Cmd) 
            {                                //  д����Ӧ��
                pMeter->RcvCnt = (pMeter->RcvCnt > 8) ? 8 : pMeter->RcvCnt;//  �������ݹ���
//                Addr = (pMeter->RcvBuf[2] << 8) | pMeter->RcvBuf[3];
//                Data = (pMeter->RcvBuf[4] << 8) | pMeter->RcvBuf[5];
            } 
            else if(0x10 == Cmd)
            {
                pMeter->RcvCnt = (pMeter->RcvCnt > 8) ? 8 : pMeter->RcvCnt;//  �������ݹ���
//                Addr = (pMeter->RcvBuf[2] << 8) | pMeter->RcvBuf[3];
//                Data = (pMeter->RcvBuf[4] << 8) | pMeter->RcvBuf[5];
            }
            else 
            {
                return;
            }

            if(!ModbusRcvCrcChk(pMeter)) 
            {
                return;
            }
            
            if(0x03 == Cmd) 
            {
                if(MeterWhoUse.Bits.Read) 
                {
                    MeterOptRlt.Bits.RspRlt = TRUE;    
                }
//                Data = (pMeter->RcvBuf[3] << 8) | pMeter->RcvBuf[4]; 
                
                memcpy(RecDataBuff, &pMeter->RcvBuf[3], Len);              
                if(Len == 2) 
                {
                    Temp = RecDataBuff[0];
                    RecDataBuff[0]=RecDataBuff[1];
                    RecDataBuff[1] = Temp;
                }
                else if(Len == 4)
                {            
                    Temp = RecDataBuff[0];
                    RecDataBuff[0]=RecDataBuff[3]; RecDataBuff[3]=Temp;
                    Temp = RecDataBuff[2];
                    RecDataBuff[2]=RecDataBuff[1]; RecDataBuff[1]=Temp;
                }
                XYDevDataDeal(RecDataBuff, Len, GpSysPara->Dev.Bits.PS);
                             
            } 
            else if(0x06 == Cmd) 
            {
                if(!MeterWhoUse.Bits.Read) 
                {
                    MeterOptRlt.Bits.RspRlt = TRUE;
                 
                    return;                                        //  Զ��д��Ƶ���ɹ�,�˳�
                               
                } 
            } 
            else if(0x10 == Cmd) 
            {
                if(!MeterWhoUse.Bits.Read) 
                {
                    MeterOptRlt.Bits.RspRlt = TRUE;                                   
                        return;                                        //  Զ��д��Ƶ���ɹ�,�˳�                            
                } 
            }
//            memset(pVfd->RcvBuf, 0, pVfd->RcvCnt);  
            
        } 
        else if(ModbusRcvTimeOut(pMeter)) 
        {
            MeterOptRlt.Bits.RspTimeOut = TRUE;
        }
        
    }
}


static u8 RecDataBuff2[20];

void    Task_Meter2Deal(void)
{    
    u8  Cmd,Temp;
    u16 Len;
//    u16 Data, Addr;

    
    if(GbMeter2EventFlag) 
    {
        GbMeter2EventFlag = FALSE;
        if(ModbusRcvFinish(pMeter2)) 
        {            
            Meter2OptRlt.Bits.RcvRsp = TRUE;
            Meter2OptRlt.Bits.RspRlt = FALSE;

            Cmd = pMeter2->RcvBuf[1];            
            if(0x03 == Cmd) 
            {                                       //  ������Ӧ��
                if(2 > pMeter2->RcvBuf[2]) {          //  �����Ĵ�����
                    return;
               }
                
                Len = pMeter2->RcvBuf[2];
                if(pMeter2->RcvCnt > (Len +5)) 
                {
                    pMeter2->RcvCnt = Len+5 ;                             //  �������ݹ���
                }   
            } 
            else if(0x06 == Cmd) 
            {                                //  д����Ӧ��
                pMeter2->RcvCnt = (pMeter2->RcvCnt > 8) ? 8 : pMeter2->RcvCnt;//  �������ݹ���
           //     Addr = (pMeter2->RcvBuf[2] << 8) | pMeter2->RcvBuf[3];
           //     Data = (pMeter2->RcvBuf[4] << 8) | pMeter2->RcvBuf[5];
            } 
            else if(0x10 == Cmd)
            {
                pMeter2->RcvCnt = (pMeter2->RcvCnt > 8) ? 8 : pMeter2->RcvCnt;//  �������ݹ���
            //    Addr = (pMeter2->RcvBuf[2] << 8) | pMeter2->RcvBuf[3];
            //    Data = (pMeter2->RcvBuf[4] << 8) | pMeter2->RcvBuf[5];
            }
            else 
            {
                return;
            }

            if(!ModbusRcvCrcChk(pMeter2)) 
            {
                return;
            }
            
            if(0x03 == Cmd) 
            {
                if(Meter2WhoUse.Bits.Read) 
                {
                    Meter2OptRlt.Bits.RspRlt = TRUE;    
                }
                //Data = (pMeter2->RcvBuf[3] << 8) | pMeter2->RcvBuf[4]; 
                
                memcpy(RecDataBuff2, &pMeter2->RcvBuf[3], Len);              
                if(Len == 2) 
                {
                    Temp = RecDataBuff2[0];
                    RecDataBuff2[0]=RecDataBuff2[1];
                    RecDataBuff2[1] = Temp;
                }
                else if(Len == 4)
                {            
                    Temp = RecDataBuff2[0];
                    RecDataBuff2[0]=RecDataBuff2[3]; RecDataBuff2[3]=Temp;
                    Temp = RecDataBuff2[2];
                    RecDataBuff2[2]=RecDataBuff2[1]; RecDataBuff2[1]=Temp;
                }
                XYDevDataDeal(RecDataBuff2, Len, GpSysPara->Dev.Bits.PS);
                             
            } 
            else if(0x06 == Cmd) 
            {
                if(!Meter2WhoUse.Bits.Read) 
                {
                    Meter2OptRlt.Bits.RspRlt = TRUE;
                 
                    return;                                        //  Զ��д��Ƶ���ɹ�,�˳�
                               
                } 
            } 
            else if(0x10 == Cmd) 
            {
                if(!Meter2WhoUse.Bits.Read) 
                {
                    Meter2OptRlt.Bits.RspRlt = TRUE;                                   
                        return;                                        //  Զ��д��Ƶ���ɹ�,�˳�                            
                } 
            }
//            memset(pVfd->RcvBuf, 0, pVfd->RcvCnt);  
            
        } 
        else if(ModbusRcvTimeOut(pMeter2)) 
        {
            Meter2OptRlt.Bits.RspTimeOut = TRUE;
        }
        
    }
    
    //**********����CAN���շ�*************//
    if(RevCanFlag)
    {
        RevCanFlag = 0;
        DriveCan_sendData(RevCanBuf);
    }
}



void    RS485ComInit(void)
{
    ModbusAddrInit(pVfd, MODBUS_SLAVE, VfdUart, 1);
    ModbusOvTmInit(pVfd, 10, 300);
    ModbusFunInit(pVfd, VfdChgToSnd, VfdChgToRcv);
}

void    Task_VfdInit(void)
{
    if(VfdInitFlag) {
        return;
    }
    Init485Uart(9600); 
    VfdCtrlInit();
    ModbusAddrInit(pVfd, MODBUS_MASTER, VfdUart, 1);
    ModbusOvTmInit(pVfd, 10, 300);
    ModbusFunInit(pVfd, VfdChgToSnd, VfdChgToRcv);

    VfdInitFlag = TRUE;
}


void    Task_MeterInit(void)
{
       InitUart(RS4851Uart, 9600); 
    
  //  VfdCtrlInit();
    ModbusAddrInit(pMeter, MODBUS_MASTER, RS4851Uart, 1);
    ModbusOvTmInit(pMeter, 20, 500);
    ModbusFunInit(pMeter, MeterChgToSnd, MeterChgToRcv);
	SetMeterReadIndex(); 
}


void    Task_Meter2Init(void)
{
       InitUart(RS4852Uart, 9600); 
    
  //  VfdCtrlInit();
    ModbusAddrInit(pMeter2, MODBUS_MASTER, RS4852Uart, 1);
    ModbusOvTmInit(pMeter2, 20, 500);
    ModbusFunInit(pMeter2, Meter2ChgToSnd, Meter2ChgToRcv);

}



void    PltParaWrite(MODBUS_HOST_STRUCT *pMdBus, u16 Nms)
{
    u16 OptAddr;
    u16 WriteData;
    char ReadVfdFlg = FALSE;
    static char ParaStep = 0;
    
    if(!VfdInitFlag) {                                              //  �ӿ�δ��ʼ��
        return;
    }

    if(!EnterPltParaWrite) {
        return;
    }

    if((VFD_OPT_READ_WAIT != VfdCtrl.Step) && (VFD_OPT_WRITE_WAITE != VfdCtrl.Step)) {
        switch(ParaStep) {
            case WRITE_PLT_INIT:
            	if (GpSysPara->Dev.Bits.PS == PS_MAM860C_880C || GpSysPara->Dev.Bits.PS == PS_MAM860_870_880)
                {
                	VfdCtrl.ReadIndex = PLT_Man_ALLOW_WRITE;    
                }
                else
                {
                	VfdCtrl.ReadIndex = PLT_ALLOW_WRITE;       
                }
                if (GpSysPara->Dev.Bits.PS == PS_HC)
                {
                	OptAddr = 164;
                }
                else
                {
               		 OptAddr = 20; //GetPltReadAddr();
                }
                ReadVfdFlg = TRUE;
                break;                
            case WRITE_PLT_EN1:
                OptAddr = GpSysPara->Dev.PasswordAdrr[0];//0xf000;
                WriteData = GpSysPara->Dev.PasswordData[0];//0xd6f0;
                break;
            case WRITE_PLT_EN2:
                OptAddr = GpSysPara->Dev.PasswordAdrr[1];//0xe000;
                WriteData = GpSysPara->Dev.PasswordData[1];//0xc5df;
                break;
            case WRITE_PLT_EN3:
                OptAddr = GpSysPara->Dev.PasswordAdrr[2];//0xd000;
                WriteData = GpSysPara->Dev.PasswordData[2];//0xb4ce;
                break;
            case WRITE_PLT_WRITE:    
             
                if(Server_Writer_Data_SendBuf(&OptAddr, &WriteData) == FALSE)
                {
                    EnterPltParaWrite = FALSE;
                    VfdCtrl.Step = VFD_OPT_READ;
                    ParaStep = 0;                    
                }
                break;
            default:
                ParaStep = 0;
                EnterPltParaWrite = FALSE;
                break;
        }        

        if(ReadVfdFlg) {
            ChgVfdAddr(pMdBus, MainVfd_Addr);
            VfdReadRegPacket(pMdBus, OptAddr, 1);
            VfdWaitResp(0x03, OptAddr);
            ModbusStartSend(pMdBus);
            VfdCtrl.Step = VFD_OPT_READ_WAIT;
            VfdSetWhoUse(VFD_USE_LOCAL, VFD_READ, VFD_MAIN, OptAddr);

        } else if(ParaStep != 0){        
            ChgVfdAddr(pMdBus, MainVfd_Addr);
            if(WRITE_PLT_WRITE == ParaStep) {
                VfdWriteRegPacket(pMdBus, OptAddr, WriteData);            
                VfdWaitResp(0x06, OptAddr); 
            } else {
                VfdWriteRegPacket(pMdBus, OptAddr, WriteData);                
                VfdWaitResp(0x06, OptAddr);
            }
            VfdCtrl.Step = VFD_OPT_WRITE;
         //   VfdSetWhoUse(VFD_USE_REMOTE, VFD_WRITE, VFD_MAIN, OptAddr);
            VfdSetWhoUse(VFD_USE_LOCAL, VFD_WRITE, VFD_MAIN, OptAddr);
        }else{
            return;
        }
    }
    
    switch(VfdCtrl.Step) {        
        case VFD_OPT_READ:
            VfdCtrl.Retry = 0;
            VfdClrTimeOut();   
            ModbusStartSend(pMdBus);
            VfdCtrl.Step = VFD_OPT_READ_WAIT;
            break;
        case VFD_OPT_READ_WAIT:
            if(VfdOptRlt.Bits.RcvRsp && VfdOptRlt.Bits.RspRlt) {    //  �����ݳɹ�
                Rs485ComOptResultAlarm(pMdBus, TRUE);
                if(PltAlowWrite) {                                  //  ����д����
                    ParaStep = WRITE_PLT_WRITE;       
                } else {
                    ParaStep = WRITE_PLT_EN1;      
                }
                VfdCtrl.Step = VFD_OPT_READ;
            } else if(VfdOptRlt.Bits.RspTimeOut || (VfdOptRlt.Bits.RcvRsp && (!VfdOptRlt.Bits.RspRlt))) { //  ������ʧ��
               if(++VfdCtrl.Retry >= VFD_READ_RETRY_NUM) {
                    VfdCtrl.Retry = 0;
                    SetRemoteOptResult(FALSE);
                    Rs485ComOptResultAlarm(pMdBus, FALSE);
                    VfdCtrl.Step = VFD_OPT_READ;
                    ParaStep = 0;
                    EnterPltParaWrite = FALSE;                      //  �˳�����д����
                } else {                                            //  ����
                    VfdClrTimeOut();    
                    ModbusStartSend(pMdBus);
                }
            } 
            break;
        case VFD_OPT_WRITE:
            VfdCtrl.Retry = 0;
            VfdClrTimeOut();    
            ModbusStartSend(pMdBus);
            VfdCtrl.Step = VFD_OPT_WRITE_WAITE;
            break;
        case VFD_OPT_WRITE_WAITE:
            if(VfdOptRlt.Bits.RcvRsp && VfdOptRlt.Bits.RspRlt) {    //  �����ݳɹ�
                Rs485ComOptResultAlarm(pMdBus, TRUE);
                if(++ParaStep > WRITE_PLT_WRITE) {                  //  д�������
                    ParaStep = 0;                    
                 //   EnterPltParaWrite = FALSE;                      //  �˳�����д���� 
                 //   SetRemoteOptResult(TRUE);                    
                }
                VfdCtrl.Step = VFD_OPT_READ;
            } else if(VfdOptRlt.Bits.RspTimeOut || (VfdOptRlt.Bits.RcvRsp && (!VfdOptRlt.Bits.RspRlt))) { //  ������ʧ��
                if(++VfdCtrl.Retry >= VFD_WRITE_RETRY_NUM) {
                    VfdCtrl.Retry = 0;
                    SetRemoteOptResult(FALSE);
                    Rs485ComOptResultAlarm(pMdBus, FALSE);
                    VfdCtrl.Step = VFD_OPT_READ;
                    ParaStep = 0;
                //    EnterPltParaWrite = FALSE;                      //  �˳�����д���� 
                } else {                                            //  ����
                    VfdClrTimeOut();    
                    ModbusStartSend(pMdBus);
                }
            } 
            break;
        default:
            VfdCtrl.Step = VFD_OPT_READ;
            break;
    }
}

#if MONITOR_METER_ON

void Vfd_Meter_Write_Data(uint32_t Data)
{
    Meter_Writer_Data[0] = (Data>>24) & 0xff;
    Meter_Writer_Data[1] = (Data>>16) & 0xff;
    
    Meter_Writer_Data[2] = (Data>>8) & 0xff;
    Meter_Writer_Data[3] = Data & 0xff;
    
    Meter_Writer_Data[4] = 0x00;
    Meter_Writer_Data[5] = 0x00;
    
    Meter_Writer_Flag = 1;
}
#endif


void CanBus_ISR_Rx(CanRxMsg *uCanRxMsg)
{

    if(uCanRxMsg->IDE == CAN_ID_STD)
    {

    //    tmp32  =  (uCanRxMsg->StdId);
        RevCanFlag = 1;
        *((u32 *)RevCanBuf)   =   uCanRxMsg->StdId;
        *((u32 *)(&RevCanBuf[4])) = *((u32 *)(uCanRxMsg->Data));
        *((u32 *)(&RevCanBuf[8])) = *((u32 *)(&uCanRxMsg->Data[4]));
    }
}



/********************************************************************
* ���� : DriveCan_sendData
* ���� : �������ݵ� CAN ����.
*
* ���� : sendData : ���͵�����(10�ֽ�);
* ��� : err      : 0:�ɹ�; 1:��������; 2:��Դ����;
*
* ��ע: (�޸ļ�¼���ݡ�ʱ��)
*       1.�������ݸ�ʽΪ{[ID10..ID3], [ID2..ID0, RTR, DLC3..DLC0], [data1], [data2],
*                         [data3], [data4], [data5], [data6], [data7], [data8]}
*****************************************************************************/
u8 DriveCan_sendData(u8 *sendData)
{
    u8 MBNumberTemp;
    CanTxMsg  txMsg;

    if(sendData == NULL) 
    {
        assert_param(0);
        return(0x01);//��������
    }

//    CAN_WakeUp(CAN1);//��CAN����

    // transmit 1 message
    txMsg.StdId = (u32)((sendData[1]<<8) | sendData[0]);
    txMsg.IDE   =  CAN_ID_STD;   //��׼֡
    txMsg.RTR   =  CAN_RTR_DATA; //����֡
    txMsg.DLC   =  8;

    *((u32 *)(txMsg.Data))       = *((u32 *)(&sendData[4]));
    *((u32 *)(&(txMsg.Data[4]))) = *((u32 *)(&sendData[8]));

//    CAN_ITConfig(CAN1, CAN_IT_TME, ENABLE);  //�����жϷ���   CAN->IER |= 0x0001;
    MBNumberTemp = CAN_Transmit(CAN1, &txMsg);

    return((MBNumberTemp != CAN_NO_MB)?0: 2);
}


/*********************************** �ļ����� *********************************/
