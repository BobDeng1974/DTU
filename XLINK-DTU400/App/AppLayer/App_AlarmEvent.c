#define _InAlmEvt
#include "app_alarmevent.h"
#include "includes.h"


//#define MIN_LACKWATER_ALM_EN            FALSE
//#define MIN_LACKWATER_ALM_MINUTE        3

//#define MIN_LACKPHASE_ALM_EN            FALSE
//#define MIN_LACKPHASE_ALM_MINUTE        3

//#define MIN_OVERMAXCURR_ALM_EN          FALSE
//#define MIN_OVERMAXCURR_ALM_MINUTE      3

//#define MIN_CURRIMBALANCE_ALM_EN        FALSE
//#define MIN_CURRIMBALANCE_ALM_MINUTE    3

//#define MIN_FANOVERMAXCURR_ALM_EN       FALSE
//#define MIN_FANOVERMAXCURR_ALM_MINUTE   3

//#define MIN_FANCURRIMBALANCE_ALM_EN     FALSE
//#define MIN_FANCURRIMBALANCE_ALM_MINUTE 3

//#define MIN_OVERMAXVOLT_ALM_EN          FALSE
//#define MIN_OVERMAXVOLT_ALM_MINUTE      3   

//#define MIN_OVERMINVOLT_ALM_EN          FALSE
//#define MIN_OVERMINVOLT_ALM_MINUTE      3

//#define MIN_OVERMAXPRES1_ALM_EN         FALSE
//#define MIN_OVERMAXPRES1_ALM_MINUTE     3

//#define MIN_OVERMAXTEMP1_ALM_EN         FALSE
//#define MIN_OVERMAXTEMP1_ALM_MINUTE     3

//#define MIN_OVERMINTEMP_ALM_EN          FALSE
//#define MIN_OVERMINTEMP_ALM_MINUTE      3

//#define MAIN_CURRENT_FAULT_ALM_EN       FALSE
//#define MAIN_CURRENT_FAULT_ALM_MINUTE   3

#define FAN_CURRENT_FAULT_ALM_EN        TRUE
#define FAN_CURRENT_FAULT_ALM_MINUTE    3


typedef union {
    u32 Flag;
    struct {
        u32 Flg: 1;                                                 //  ���˱�ʶ
        u32 Nms: 10;                                                //  �����ʱ
        u32 Sec: 6;                                                 //  ���ʱ
        u32 Min: 7;                                                 //  �ּ�ʱ,���127����
        u32 MaxMin: 7;
        u32 Res:  1;
    }Bits;
}ALM_TIMER_UNION;

typedef struct {

#if FAN_CURRENT_FAULT_ALM_EN
    ALM_TIMER_UNION FanCurFault;
#endif

}ALM_INV_STRUCT;
static ALM_INV_STRUCT  AlmFltTimer;


static char    AlarmFilterTimeOver(ALM_TIMER_UNION *pAlmTimer, u16 Nms)
{
    if(pAlmTimer->Bits.Flg) 
    {
        if(pAlmTimer->Bits.Nms + Nms >= 1000) 
        {
            pAlmTimer->Bits.Nms = 0;
            if(++pAlmTimer->Bits.Sec >= 60) 
            {
                pAlmTimer->Bits.Sec = 0;
                if(++pAlmTimer->Bits.Min >= pAlmTimer->Bits.MaxMin) 
                {
                    pAlmTimer->Bits.Min = 0;
                    pAlmTimer->Bits.Flg = FALSE;
                    return TRUE;
                }
            }  
        } 
        else 
        {
            pAlmTimer->Bits.Nms += Nms;
        }
    }

    return FALSE;
}

void    AlarmFilterTimer(u16 Nms)
{

#if FAN_CURRENT_FAULT_ALM_EN
    if(AlarmFilterTimeOver(&AlmFltTimer.FanCurFault, Nms)) 
    {
        AlmMask.Alarm.Bits.FanCurrFault = TRUE;
    }
#endif
}

static u32 LastAlmFlg;                                          //  ��һ�α���״̬
static u32 LastPreAlmFlg;                                       //  ��һ��Ԥ��״̬
static u32 LastDevAlmFlg;                                       //  ��һ��Ԥ��״̬


void    AlarmEventInit(void)
{
    /*
     *  ��Ҫ�ϴ��ı�������
     */
    memset((char *)&AlmMask, 0xff, sizeof(AlmMask));  
    AlmMask.AlmSnd.Flag = 0;
    AlmMask.AlmSnd.Bits.Reserved = TRUE;//  ���������������б���֮���δ֪���屨������ַ8 bit0
    AlmMask.AlmSnd.Bits.LackWater = TRUE;//  ȱˮ����
    AlmMask.AlmSnd.Bits.LackPhase = TRUE;    //ȱ��
    AlmMask.AlmSnd.Bits.PluseError = TRUE;    //�������
    
    AlmMask.AlmSnd.Bits.MainVfdAlarm = TRUE;   //  ������Ƶ������
    AlmMask.AlmSnd.Bits.MainCurrFault = TRUE;  //  �����������ϱ���
    AlmMask.AlmSnd.Bits.CurrImbalance = TRUE;  //  ����������ƽ��
    
    AlmMask.AlmSnd.Bits.FanVfdAlarm = TRUE;    //  �����Ƶ������  
    AlmMask.AlmSnd.Bits.FanCurrFault = TRUE;    //  ����������ϱ���
    AlmMask.AlmSnd.Bits.FanCurrImbalance = TRUE;    //  ���������ƽ��
    
    AlmMask.AlmSnd.Bits.OverMaxPres1 = TRUE;    //  ѹ�����ޱ���
    AlmMask.AlmSnd.Bits.PresSen1Fault = TRUE;   //  ѹ���������𻵱���
    AlmMask.AlmSnd.Bits.OverMaxTemp1 = TRUE;    //  �¶ȳ��ޱ���
       
    AlmMask.AlmSnd.Bits.TempSen1Fault = TRUE;   //  �¶ȴ������𻵱���
    AlmMask.AlmSnd.Bits.OverMinTemp = TRUE;     //  �¶ȳ��ͱ���
    
    
    AlmMask.AlmSnd.Bits.TempImbalance = TRUE;   //  �¶�ʧ������
    AlmMask.AlmSnd.Bits.OverMaxUsrHour = TRUE;  //  ����ʹ��ʱ�ޱ���
    AlmMask.AlmSnd.Bits.SupplyVoltageOverMax = TRUE; // ��ѹ���߱���
    AlmMask.AlmSnd.Bits.SupplyVoltageOverMin = TRUE; // ��ѹ���ͱ���
    
    
//    AlmMask.AlmSnd.Bits.RS485CommErr = TRUE;    //  RS485���������ͨ���쳣
//    AlmMask.AlmSnd.Bits.RS485MeterErr = TRUE;    //  RS485����ܱ��ͨ���쳣
//    AlmMask.AlmSnd.Bits.RS485FlowErr = TRUE;    //  RS485�������Ƶ�ͨ���쳣
// 
    memset((char *)&AlmStat,  0, sizeof(AlmStat));
    if(GpSysPara->Dev.Bits.AlmSaveFlg) {
        AlmStat.Alarm.Flag    =  GpAlmInfo->Alarm.Flag;
        AlmStat.PreAlarm.Flag =  GpAlmInfo->PreAlarm.Flag;
        
        AlmMask.Alarm.Flag    = ~GpAlmInfo->Alarm.Flag;
        AlmMask.PreAlarm.Flag = ~GpAlmInfo->PreAlarm.Flag;
    }
    LastAlmFlg    = AlmStat.Alarm.Flag;
    LastPreAlmFlg = AlmStat.PreAlarm.Flag;
    
    memset((char *)&SysEvent, 0, sizeof(SysEvent));

    /*
     *  ��ͬ�������ʱ��
     */
    memset((char *)&AlmFltTimer, 0, sizeof(AlmFltTimer));

#if FAN_CURRENT_FAULT_ALM_EN
    AlmFltTimer.FanCurFault.Bits.MaxMin  = FAN_CURRENT_FAULT_ALM_MINUTE;
#endif

//    AlmStat.Alarm.Bits.MainVfdAlarm = TRUE;
//    AlmStat.Alarm.Bits.FanVfdAlarm = TRUE;

//    AlmStat.DevAlarm.Bits.DevPowerOn = TRUE;
//    AlmStat.DevAlarm.Bits.DevStartUp = TRUE;
//    AlmStat.DevAlarm.Bits.DevShutDown = TRUE;
//    
//    AlmStat.DevAlarm.Bits.DevStop = TRUE;
//    AlmStat.DevAlarm.Bits.LoadingRun = TRUE;
//    AlmStat.DevAlarm.Bits.IdlerRun = TRUE;
//    AlmStat.DevAlarm.Bits.DevDormancy = TRUE;
//    AlmStat.DevAlarm.Bits.FanRun = TRUE;
//    AlmStat.DevAlarm.Bits.FanStop = TRUE;
}


/*
void    AlarmEventDeal(void)
{
    u32 ChgFlg;
    ALARM_FLAG_UNION NewAlm, DisAlm;
//    static RESET_TYPE_ENUM  LastResetType;                          //  ��һ�θ�λ״̬
//    static BOOT_TYPE_ENUM   LastBootType;                           //  ��һ�ο���״̬
//    static STOP_TYPE_ENUM   LastStopType;                           //  ��һ��ͣ��״̬
    
    if(LastAlmFlg != AlmStat.Alarm.Flag) {
        ChgFlg = AlmStat.Alarm.Flag ^ LastAlmFlg;                   //  �仯λ
        NewAlm.Flag = ChgFlg & AlmMask.Alarm.Flag;                  //  �²�����λ
        DisAlm.Flag = ChgFlg ^ NewAlm.Flag;                         //  ��ʧ��λ
                       
        if(NewAlm.Flag) 
        {                                           //  �������±���Դ 
            SysEvent.Alarm.Flag |= NewAlm.Flag & AlmMask.AlmSnd.Flag;//  �ϴ������¼�
            if(SysEvent.Alarm.Flag) 
            {
                SysEvent.Flag.Bits.Alarm = TRUE;                    //  �ñ�����ʶ
//                if(SysEvent.Alarm.Bits.MainVfdAlarm) {                      
//                    SysEvent.MainVfdAlmVal = AlmStat.MainVfdAlmVal; //  ������Ƶ��������ֵ
//                }
//                if(SysEvent.Alarm.Bits.FanVfdAlarm) {                   
//                    SysEvent.FanVfdAlmVal = AlmStat.FanVfdAlmVal;   //  �����Ƶ����ֵ
//                }
            }            
            
            AlmMask.Alarm.Flag ^= NewAlm.Flag;                      //  �����´���ͬ�ı���

        } 
        
        if(DisAlm.Flag) 
        {                                           //  �б���Դ��ʧ 
            SysEvent.Alarm.Flag &= ~DisAlm.Flag;                    //  �¼�δ�ϴ�,����Դ��ʧ,��������¼� 
            if((!SysEvent.Alarm.Flag) && (!SysEvent.PreAlarm.Flag)) 
            {
                SysEvent.Flag.Bits.Alarm = FALSE;
            }
            
            AlmMask.Alarm.Flag |= DisAlm.Flag;                      //  �´οɲ��������¼�
        }
    
        LastAlmFlg = AlmStat.Alarm.Flag;
        ParaFat.Bits.AlmInfo = TRUE;
    }

    if(LastPreAlmFlg != AlmStat.PreAlarm.Flag) 
    {
        ChgFlg = AlmStat.PreAlarm.Flag ^ LastPreAlmFlg;                
        NewAlm.Flag = ChgFlg & AlmMask.PreAlarm.Flag;                   
        DisAlm.Flag = ChgFlg ^ NewAlm.Flag;                                  
                       
        if(NewAlm.Flag)   
        {                                               
            SysEvent.PreAlarm.Flag |= NewAlm.Flag;  
            SysEvent.Flag.Bits.Alarm = TRUE;                        //  �ñ�����ʶ           
            AlmMask.PreAlarm.Flag ^= NewAlm.Flag;                               
        } 
        
        if(DisAlm.Flag) 
        {                                                         
            AlmMask.PreAlarm.Flag |= DisAlm.Flag;

            SysEvent.PreAlarm.Flag &= ~DisAlm.Flag;                 //  �¼�δ�ϴ�,Ԥ����Դ��ʧ,���Ԥ���¼� 
            if((!SysEvent.Alarm.Flag) && (!SysEvent.PreAlarm.Flag)) 
            {
                SysEvent.Flag.Bits.Alarm = FALSE;
            }
        }
    
        LastPreAlmFlg = AlmStat.PreAlarm.Flag;
        ParaFat.Bits.AlmInfo = TRUE;
    }  


    if(LastDevAlmFlg != AlmStat.DevAlarm.Flag) 
    {
        ChgFlg = AlmStat.DevAlarm.Flag ^ LastDevAlmFlg;                
        NewAlm.Flag = ChgFlg & AlmMask.DevAlarm.Flag;                   
        DisAlm.Flag = ChgFlg ^ NewAlm.Flag;                                  
                       
        if(NewAlm.Flag) 
        {                                               
            SysEvent.DevAlarm.Flag |= NewAlm.Flag;  
            SysEvent.Flag.Bits.Alarm = TRUE;                        //  �ñ�����ʶ           
            AlmMask.DevAlarm.Flag ^= NewAlm.Flag;                               
        } 
        
        if(DisAlm.Flag) 
        {                                                         
            AlmMask.DevAlarm.Flag |= DisAlm.Flag;

            SysEvent.DevAlarm.Flag &= ~DisAlm.Flag;                 //  �¼�δ�ϴ�,Ԥ����Դ��ʧ,���Ԥ���¼� 
            if((!SysEvent.Alarm.Flag) && (!SysEvent.PreAlarm.Flag)&& (!SysEvent.DevAlarm.Flag)) 
            {
                SysEvent.Flag.Bits.Alarm = FALSE;
            }
        }
    
        LastDevAlmFlg = AlmStat.DevAlarm.Flag;
        ParaFat.Bits.AlmInfo = TRUE;
    }  

    


    if(AlmStat.Flag.Bits.Boot) 
    {
        AlmStat.Flag.Bits.Boot = FALSE;
        if(AlmStat.BootType) 
        {            
            SysEvent.BootType = AlmStat.BootType;
            SysEvent.Flag.Bits.Boot = TRUE;
        }        
    }
    
    if(AlmStat.Flag.Bits.Stop) 
    {
        AlmStat.Flag.Bits.Stop = FALSE;
        if(AlmStat.StopType) 
        {
            SysEvent.Flag.Bits.Stop = TRUE;
            SysEvent.StopType = AlmStat.StopType;
        }
    }

    if(AlmStat.Flag.Bits.Reset) 
    {
        AlmStat.Flag.Bits.Reset = FALSE;
        if(AlmStat.ResetType) 
        {
            SysEvent.Flag.Bits.Reset = TRUE;
            SysEvent.ResetType = AlmStat.ResetType;
        }
    }
}


*/



void	AlarmEventDeal(void)
{
	u32 ChgFlg;
	u8  i;
	ALARM_FLAG_UNION NewAlm;
	ALARM_FLAG_UNION DisAlm;  
//	static u32 TmpLastAlmStat, TmpAlmstat;
	/* ����ֹͣ1min �к��ֹ�ϱ����� */
	//  if (STOP_STATE == GpSysStat->Share.RunState && ( 0 == StopFilterTimer) )
//	{
//		return ;
//	}  
	if(LastAlmFlg != AlmStat.Alarm.Flag) 
	{
//		TmpLastAlmStat  = LastAlmFlg;
//		TmpAlmstat = AlmStat.Alarm.Flag;
		ChgFlg = AlmStat.Alarm.Flag ^ LastAlmFlg;					//	�仯λ
		NewAlm.Flag = ChgFlg &  AlmStat.Alarm.Flag;					//	�²�����λ
		DisAlm.Flag = ChgFlg ^ AlmStat.Alarm.Flag;					// ����ʧ��λ
					   
		if(NewAlm.Flag) 
		{											//	�������±���Դ 
			SysEvent.Alarm.Flag |= NewAlm.Flag & AlmMask.AlmSnd.Flag;//  �ϴ������¼�
			if(SysEvent.Alarm.Flag) 
			{


				for (i=0; i<32; i++)
				{
					if ((SysEvent.Alarm.Flag & (1<<i)) && AlmFilterTimer[i] == 0)
					SysEvent.Flag.Bits.Alarm = TRUE;					//	�ñ�����ʶ
				}
				
			}			 
		} 

		if (DisAlm.Flag)
		{
				SysEvent.Alarm.Flag &= ~(DisAlm.Flag);
		}

		LastAlmFlg  = AlmStat.Alarm.Flag;
		ParaFat.Bits.AlmInfo = TRUE;
	}

	if(LastPreAlmFlg != AlmStat.PreAlarm.Flag) 
	  {
		  ChgFlg = AlmStat.PreAlarm.Flag ^ LastPreAlmFlg;				 
		  NewAlm.Flag = ChgFlg & AlmStat.PreAlarm.Flag; 				  
		  DisAlm.Flag = ChgFlg ^ AlmStat.PreAlarm.Flag;					// ����ʧ��λ				   
						 
		  if(NewAlm.Flag)	
		  { 											  
			  SysEvent.PreAlarm.Flag |= NewAlm.Flag ;   
			  	for (i=0; i<2; i++)
				{
					if ((SysEvent.PreAlarm.Flag  & (1<<i)) && PreAlmFilterTimer[i] == 0)
					SysEvent.Flag.Bits.Alarm = TRUE;					//	�ñ�����ʶ
				}
							  
		  }


		 if (DisAlm.Flag)
	  	{
	  		SysEvent.PreAlarm.Flag &= ~(DisAlm.Flag);
	  	}
		  
		  LastPreAlmFlg = AlmStat.PreAlarm.Flag;
		  ParaFat.Bits.AlmInfo = TRUE;
	  }  

	if(LastDevAlmFlg != AlmStat.DevAlarm.Flag) 
	  {
		  ChgFlg = AlmStat.DevAlarm.Flag ^ LastDevAlmFlg;				 
		  NewAlm.Flag = ChgFlg & AlmStat.DevAlarm.Flag; 				  
								   
						 
		  if(NewAlm.Flag) 
		  { 											  
			  SysEvent.DevAlarm.Flag |= NewAlm.Flag;  
			  SysEvent.Flag.Bits.Alarm = TRUE;						  //  �ñ�����ʶ									  
		  } 
		  
	  
		  LastDevAlmFlg = AlmStat.DevAlarm.Flag;
		  ParaFat.Bits.AlmInfo = TRUE;
	  }  

	
    if(AlmStat.Flag.Bits.Boot) 
    {
        AlmStat.Flag.Bits.Boot = FALSE;
        if(AlmStat.BootType) 
        {            
            SysEvent.BootType = AlmStat.BootType;
            SysEvent.Flag.Bits.Boot = TRUE;
        }        
    }
    
    if(AlmStat.Flag.Bits.Stop) 
    {
        AlmStat.Flag.Bits.Stop = FALSE;
        if(AlmStat.StopType) 
        {
            SysEvent.Flag.Bits.Stop = TRUE;
            SysEvent.StopType = AlmStat.StopType;
        }
    }

    if(AlmStat.Flag.Bits.Reset) 
    {
        AlmStat.Flag.Bits.Reset = FALSE;
        if(AlmStat.ResetType) 
        {
            SysEvent.Flag.Bits.Reset = TRUE;
            SysEvent.ResetType = AlmStat.ResetType;
        }
    }
}

/* ������Ԥ���ϱ� Ƶ�ʿ���
* ��Ӧ�Ķ�ʱ��ʱ�䲻Ϊ0�����ϱ�
*/
void Alarm_Filter_Deal(void )
{
	u16 i;
	//Alarm
	for (i=0; i<sizeof(AlmFilterTimer)/sizeof(AlmFilterTimer[0]); i++)
	{
		if (AlmFilterTimer[i] != 0)
		{
			AlmFilterTimer[i]--;
		}
	}

	// PreAlarm
	for (i=0; i<sizeof(PreAlmFilterTimer)/sizeof(PreAlmFilterTimer[0]); i++)
	{

		if (PreAlmFilterTimer[i] != 0)
		{
			PreAlmFilterTimer[i]--;
		}
	}
}


void Start_Stop_Filter(void)
{
	if (StartFilterTimer != 0)
	{
		StartFilterTimer--;
	}

	if (StopFilterTimer !=0)
	{
	
		StopFilterTimer--;
	}
}
/**
	@para  pbuf: Ҫ׷�ӱ���������Ϣ�Ļ�����
	@para  alm_bit: ����������λ
*/
//void PackAlarmInfo(char *pbuf,  int alm_bit)
//{
//	if (5 == i)
//	{
//		sprintf(&pBuf[strlen(pBuf)], "\"8305\":{\"2300\":\"%d\",\"46\":\"%d\",\"42\":\"%d\"},"
//        ,GpSysStat->VFD.MainVfd_Model, GpSysStat->VFD.MainVfd_AlarmValue, GpSysStat->Main.OutCurrent);            
//	}
//}


