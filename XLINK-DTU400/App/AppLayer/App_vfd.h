/*
  ******************************************************************************
  * @�ļ�    vfd_com.h 
  * @����    vfd_comģ��ͷ�ļ�
  ******************************************************************************
*/ 


#ifndef __VFD_COM_H
#define __VFD_COM_H


#ifndef _InVfd
#define VfdExt  extern
#else
#define VfdExt  
#endif

#include "typedef.h"
#include "app_cfg.h"
#include "bsp.h"


#define MODBUS_MASTER   1
#define MODBUS_SLAVE    0

#define USART_TX_ON       0x0008    //�������ڷ���
#define USART_TX_OFF      0xFFF7    //�رմ��ڷ���
#define USART_RX_ON       0x0004    //�������ڽ���
#define USART_RX_OFF      0xFFFB    //�رմ��ڽ���


typedef enum {
//    VFD_OPT_NONE = 0,
    VFD_OPT_READ = 0,
    VFD_OPT_READ_WAIT,
    VFD_OPT_WRITE,
    VFD_OPT_WRITE_WAITE,    
}VFD_OPT_ENUM;



#define VFD_RCV_BUF_SIZE        100                                 //  ��Ƶ�����ջ����С,���Ǳ�Ƶ��͸������
#define VFD_SND_BUF_SIZE        100                                 //  ��Ƶ�����ͻ����С


#define  RS485_COMOK_CNTS   5
#define  RS485_COMERR_CNTS  10

typedef void (*pFun)(void);

typedef struct {
    union {
        char Flag;
        struct {
            char bMaster: 1;                                        //  ���ӱ�ʶ
            char SndFlg: 1;                                         //  Modbus���ͱ�ʶ
            char RcvFlg: 1;                                         //  Modbus���ձ�ʶ
            char RxStart: 1;                                        //  ��ʼ���ձ�ʶ
        }Bits;
    }Stat;

    UART_COM_ENUM UartNum;                                          //  Modbus���ں�
    u8  Addr;                                                       //  Modbusվ��

//    u8  SndStep;
    u8  SndSize;                                                    //  Modbus���ݷ��ͳ���
    u8  SndCnt;
    char SndBuf[VFD_SND_BUF_SIZE];                                  //  Modbus���ݷ��ͻ���

    u8   RcvCnt;                                                    //  Modbus���ݽ��ճ���
    char RcvBuf[VFD_RCV_BUF_SIZE];                                  //  Modbus���ݽ��ջ���
    
    u16 RcvTimer;                                                   //  ���ռ�ʱ��
    u16 CharOvTimer;                                                //  �ַ����ճ�ʱʱ��,��λms
    u16 PrtlOvTimer;                                                //  Э����ճ�ʱʱ��,��λms
    
//    u16 ComErrCnt;                                                  //  ͨ���쳣������

#if RS485_SEND_DELAY
    struct {
        u16 Flag: 1;
        u16 Timer: 15;
    }DelayToRcv;
#endif

    pFun pChgToSnd;
    pFun pChgToRcv;
//    pFun pWaitSndFinish;
}MODBUS_HOST_STRUCT;

typedef struct {
    struct {
        u8  bReadMainVfd: 1;                                        //  1:��������Ƶ������

//        u8 NeedRsp: 1;                                              //  ��ҪӦ��
//        u8 RcvRsp: 1;                                               //  ���յ�Ӧ��
//        u8 RspRlt: 1;                                               //  Ӧ����
//        u8 RspTimeOut: 1;                                           //  Ӧ��ʱ
    }Bits;


    u16 ReadIndex;                                                  //  ����Ƶ����
    u16 ReadAddr;                                                   //  ����ַ

    u8  WaitRspCmd;                                                 //  Ӧ������
    u16 WaitRspAddr;                                                //  Ӧ���ַ
    u8  OptLen;

    float MainVolt;
    float FanVolt;

    u8 Step;
    u8 Retry; 
//    u16 MainFreq;                                                   //  ������Ƶ������Ƶ��
//    u16 FanFreq;                                                    //  �ֻ���Ƶ������Ƶ��
}VFD_CTRL_STRCUT;
VfdExt VFD_CTRL_STRCUT VfdCtrl;
VfdExt VFD_CTRL_STRCUT MeterCtrl;
VfdExt VFD_CTRL_STRCUT Meter2Ctrl;


typedef struct {
    struct {
        char OptFlg: 1;                                             //  ������������ʶ
        char RspFlg: 1;                                             //  �ն�Ӧ���ʶ
        
        char Read:  1;                                              //  1:������   0: д����        
        char Type:  4;                                              //  1:�������� 0: ������� :3/4/5 ������д����
        
        char Opting: 1;                                             //  �������
    }Bits;

    u16 Addr;                                                       //  ��ַ
    u16 Data;                                                       //  ��ַ��Ӧֵ
}SERVER_VFD_OPT_STRUCT;


#define EVENT_FLAG_MODBUS   0x0001                                  //  Modbus���ݽ��ճ�ʱ�ж�
#define EVENT_FLAG_VFD      0x0004

#define STATUS_UNKNOW			  255
#define STOP_STATE                 0                                //  ֹͣ״̬
#define MACHINE_ALARM              1                                   //��������
#define JIAZAI_DELAY_RUN_STATE     2                                //  ������ʱ״̬
#define JIA_ZAI_RUN_STATE          3                                //  ��������״̬
#define KONG_ZAI_RUN_STATE         4                                //  ��������״̬
#define KONG_JIU_STOP_STATE        5                                //  �վ�����״̬
#define STOP_DELAY_RUN_STATE       6                                //  ͣ����ʱ״̬
#define RUN_STATE				   7
VfdExt char GbEventFlag;
VfdExt bool VfdInitFlag;

VfdExt char GbMeterEventFlag;
VfdExt char GbMeter2EventFlag;

VfdExt  bool bRemoteBoot;                                           //  Զ�̿�������
VfdExt  bool bRemoteStop;                                           //  Զ��ͣ������

typedef enum {
    VFD_WRITE = 0,
    VFD_READ,
}VFD_OPT;

typedef enum {
    VFD_FAN = 0,
    VFD_MAIN,
}VFD_TYPE_ENUM;

typedef enum {
    VFD_USE_REMOTE = 0,
//    VFD_USE_LEARN,
    VFD_USE_PARASET,
    VFD_USE_LOCAL,
}VFD_USE_ENUM;


typedef union {
    char Flag;
    struct {
        char Remote: 1;                                             //  1: Զ�̲���
//      char Learn:  1;                                             //  1: ��ѧϰ
        char ParSet: 1;                                             //  1: ��������
        char Local:  1;                                             //  1: �Կ�

        char Main: 1;                                               //  0: ������� 1: �������� 
        char Read:  1;                                              //  1: ������   0: д����
    }Bits;
}WHO_USE_ENUM;

VfdExt  WHO_USE_ENUM VfdWhoUse; 
VfdExt  WHO_USE_ENUM MeterWhoUse; 
VfdExt  WHO_USE_ENUM Meter2WhoUse; 


typedef union {
    char Flag;
    struct {
        char RcvRsp: 1;                                               //  ���յ�Ӧ��
        char RspRlt: 1;                                               //  Ӧ����
        char RspTimeOut: 1;                                           //  Ӧ��ʱ
    }Bits;        
}OPT_RLT_ENUM;

VfdExt  OPT_RLT_ENUM VfdOptRlt;  
VfdExt  OPT_RLT_ENUM MeterOptRlt;  
VfdExt  OPT_RLT_ENUM Meter2OptRlt; 


VfdExt  u16 VfdOptAddr;
VfdExt  void    VfdSetWhoUse(VFD_USE_ENUM Who, char bRead, char bMain, u16 Addr); 

VfdExt  u8  MainVfd_Addr;
VfdExt  SERVER_VFD_OPT_STRUCT   SvrVfdOpt;
VfdExt  MODBUS_HOST_STRUCT  RS4850Modbus;

VfdExt MODBUS_HOST_STRUCT  *pVfd;

VfdExt  MODBUS_HOST_STRUCT  RS4851Modbus;
VfdExt MODBUS_HOST_STRUCT  *pMeter;

VfdExt  MODBUS_HOST_STRUCT  RS4852Modbus;
VfdExt MODBUS_HOST_STRUCT  *pMeter2;


VfdExt bool    ModbusRcvCrcChk(MODBUS_HOST_STRUCT *pMdBus);
VfdExt char    ModbusRcvFinish(MODBUS_HOST_STRUCT *pMdBus);


VfdExt char    ModbusSendNextChar(MODBUS_HOST_STRUCT *pMdBus);
VfdExt void    ModbusUartIsr(MODBUS_HOST_STRUCT *pMdBus, char dat);


VfdExt bool    IsModbusMaster(MODBUS_HOST_STRUCT *pMdBus);
VfdExt char    ModbusRcvTimeOut(MODBUS_HOST_STRUCT *pMdBus/*, char *pErrType*/);
VfdExt void    ModbusOvTmInit(MODBUS_HOST_STRUCT *pMdBus, u16 CharOvTimer, u16 PrtlOvTimer);
VfdExt void    ModbusAddrInit(MODBUS_HOST_STRUCT *pMdBus, bool bMaster, UART_COM_ENUM UartNum, char NodeAddr);
VfdExt void    ModbusFunInit(MODBUS_HOST_STRUCT *pMdBus, pFun pChgToSnd,/* pFun pWaitSndOver,*/ pFun pChgToRcv);

VfdExt  void    ModbusStartSend(MODBUS_HOST_STRUCT *pMdBus);
void    ModbusDelayToRcv(MODBUS_HOST_STRUCT *pMdBus, u16 Nms);

VfdExt  void    Task_VfdInit(void);
VfdExt  void    Task_VfdDeal(void);
VfdExt  void    Task_VfdComm(MODBUS_HOST_STRUCT *pMdBus, u16 Nms);
VfdExt  void    VfdWriteMoreRegPacket(MODBUS_HOST_STRUCT *pMdBus, u16 Addr, u16 RegNum,u8 DataLen, u8 *Data);
VfdExt  void     Vfd_Meter_Write_Data(uint32_t Data);
VfdExt  void    RS485ComInit(void);
void    Task_MeterInit(void);
void    Task_MeterComm(MODBUS_HOST_STRUCT *pMdBus, u16 Nms);
void    Task_MeterDeal(void);
void  SetMeterReadIndex(void);

void    Task_Meter2Init(void);
void    Task_Meter2Comm(MODBUS_HOST_STRUCT *pMdBus, u16 Nms);
void    Task_Meter2Deal(void);
u8 DriveCan_sendData(u8 *sendData);
void CanBus_ISR_Rx(CanRxMsg *uCanRxMsg);

#endif


/*********************************** �ļ����� *********************************/
