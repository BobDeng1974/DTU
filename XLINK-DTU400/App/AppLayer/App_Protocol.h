#ifndef _APP_PROTOCOL_H_
#define _APP_PROTOCOL_H_

#ifndef _InPro
#define ProExt  extern
#else
#define ProExt
#endif

#include "struct.h"
#include "app_rtc.h"

#define GPRS_PRO_HEAD_FLAG          0x2A
#define GPRS_PRO_END_FLAG           0x23

#define GPRS_PRO_VERSION            0x01                                                        //ͨ��Э��汾

#define PRO_MSGHEAD_SIZE            17                              //  ��Ϣͷ����
#define PRO_BEFOR_MSGBODY_SIZE      21                              //  ��Ϣ��֮ǰ�ĳ���
#define PRO_MSGEND_CRC_SIZE         3                               //  CRC����Ϣβ�ĳ���
#define PRO_MIN_SIZE                (PRO_BEFOR_MSGBODY_SIZE+PRO_MSGEND_CRC_SIZE)
 
//#define TERMINAL_GENERAL_ACK        0x0001                          //  �ն�ͨ��Ӧ��

#define PRO_CMD_MASK                0x7ff
 
#define TERMINAL_REG                0x001                          //  �ն�ע��
#define TERMINAL_REQ_RESP           0x801

#define TERMINAL_HEART              0x002                          //  �ն�����
#define TERMINAL_HEART_RESP         0x802

#define TERMINAL_TIME_PARA          0x003
#define TERMINAL_TIME_PARA_RESP     0x803

#define TERMINAL_STATUS_UPLOAD      0x004                        //�����и��±仯�������ϴ�
#define TERMINAL_STATUS_RESP        0x804

#define SERVER_GET_DEV_DATA         0x805                          //  ��ȡ�ն�����
#define SERVER_GET_DEV_DATA_RESP    0x005

#define SERVER_SET_PARA             0x806                          //  ��������
#define SERVER_SET_PARA_RESP        0x006

#define TERMINAL_EVENT_UPLOAD       0x007                          //  �¼��ϴ�
#define TERMINAL_EVENT_RESP         0x807 


#define SERVER_VFD_OPT              0x811
#define SERVER_VFD_RESP             0x011

#define SERVER_CONTROL              0x812
#define SERVER_CONTROL_RESP         0x012



#define SERVER_UPDATE_REQUEST       0x813                          //  ��������
#define TERMINAL_UPDATE_RESP        0x013                          //  ��ȡ�������ݰ�
#define TERMINAL_UPDATE              0x050                             //  ��ȡ�������ݰ�

#define TERMINAL_UPDATE_Result      0x014                          //  ��ȡ�������ݰ�

#define SERVER_DOMAIN_CFG              0x815                          //�����������˿��ض���


#define SERVER_MSG_PUSH        		0X816    //ϵͳ��Ϣ����
#define SERVER_MSG_PUSH_RESP   		0X016    //ϵͳ��Ϣ����Ӧ��

#define HEARTDAT_RSP 1                                                                                        //����Ӧ���ʶ��1����ҪӦ��0������ҪӦ��

typedef enum {
    ACK_SUCCESS = 0,                                                //  �ɹ�
    ACK_FAIL,                                                       //  ʧ��
    ACK_NOT_SUPPORT,                                                //  ��֧��
}ACK_ENUM;



typedef enum {
    EVENT_TYPE_NONE = 0,
    EVENT_TYPE_ALARM,                                               //  �����¼�
    EVENT_TYPE_BOOT,                                                //  �����¼�
    EVENT_TYPE_STOP,                                                //  ͣ���¼�
    EVENT_TYPE_RESET,                                               //  ��λ�¼�
}EVENT_TYPE_ENUM;

typedef enum {
    TRANS_TYPE_NONE = 0,
    TRANS_TYPE_UPDATE = 01,                                         //  Զ���������
    TRANS_TYPE_SMS_RLS,                                             //  ���ŷ��ͽ��
    TRANS_TYPE_MAINVFD,                                             //  ������Ƶ͸������              
    TRANS_TYPE_FANVFD,                                              //  �����Ƶ͸������
    TRANS_TYPE_ALMSMS,                                              //  ��������
    TRANS_TYPE_YECX,                                                //  ����ѯ����
    TRANS_TYPE_ALMCLR,                                              //  �������
}TRANS_TYPE_ENUM;


typedef union {
    char Flag;
    struct {
        char Alarm: 1;                                              //  ������ʶ
        char Boot:  1;                                              //  ������ʶ
        char Stop:  1;                                              //  ͣ����ʶ
        char Reset: 1;                                              //  ������ʶ
//      char Heart: 1;                                                //  ����
    }Bits;  
}SYS_EVENT_UNION;

#pragma pack(1)
typedef struct {
    SYS_EVENT_UNION   Flag;                                      
    ALARM_FLAG_UNION  Alarm;                                        //  ��������
    PREALM_FLAG_UNION PreAlarm;                                     //  Ԥ������
    DEVALM_FLAG_UNION DevAlarm;                                     //  Ԥ������
    RESET_TYPE_ENUM ResetType;                                      //  ��λ����
//    u16 MainVfdAlmVal;                                            //  ������Ƶ����ֵ
//    u16 FanVfdAlmVal;                                             //  �����Ƶ����ֵ
    BOOT_TYPE_ENUM  BootType;                                       //  ��������
    STOP_TYPE_ENUM  StopType;                                       //  ͣ������
//    SYS_TIME_STRUCT PwrDwnTime;                                   //  ����ʱ��  
    OTHERS_ALM_FLAG_UNION OthersAlarm;                      // ������������
}SYS_EVENT_STRUCT;

typedef struct {
    ALARM_FLAG_UNION  Alarm;                                        //  ����������
    ALARM_FLAG_UNION  AlmSnd;                                       //  ���ϴ���λ��0
    PREALM_FLAG_UNION PreAlarm;                                     //  Ԥ��������
    DEVALM_FLAG_UNION DevAlarm;                                     //  �豸״̬�¼�
}ALARM_MASK_STRUCT;

#pragma pack()


ProExt SYS_EVENT_STRUCT    AlmStat;                                 //  ����״̬,������ʵʱֵ
ProExt SYS_EVENT_STRUCT    SysEvent;                                //  �¼���ʶ         
ProExt ALARM_MASK_STRUCT   AlmMask;                                 //  ��������


typedef struct {
    bool Flag;
    u8  Cnt;
}SMS_CNT_STRUCT;
ProExt SMS_CNT_STRUCT  SMSStatistics;

typedef union {
    u32 Flag;
    struct {
        u32 Send:   1;
        u32 Resend: 1;                                             //  ��������޸ĺ�����ش�
        u32 Index : 6;
        u32 Timer: 24;
    }Bits;
}PARA_UPLOAD_UNION;
ProExt PARA_UPLOAD_UNION ParaUpload;

typedef union{
	u32 val;
	struct {
		u32  Rs485comstatus:	1;   // 485ͨ��״̬�ı�
		u32  Rs485ErrCnts:		1;	// Rs485ͨ�Ŵ������
		u32  MatainAlarmVal:	1;	// �����¼�����ֵ
		u32  MachineState:		1;	// ����״̬����״̬�ı�
		u32	 Reserved:			28; //Ԥ��
		}Bits;
}PARA_UPDATE_UNION;
ProExt  PARA_UPDATE_UNION ParaUpdate;

ProExt bool GbUpLoadAllPara;
ProExt DELAY_TIMER RebootTime;

typedef union
{
	u32 val;
	struct {
			u32 flag:1;
			u32  timer:31 ; 
		}Bits;
}SysTimer;


ProExt SysTimer  InitParaUp;
ProExt bool GbeInitParaSend;





ProExt void    ClearHeartTimer(void);
ProExt void    ProSendInit(void);
ProExt void    DevStatToSystem(void);
ProExt void    Task_SvrComm(void);
ProExt void    GPRSDataDeal(char *pBuf);
ProExt void    GprsProtocolSend(u16 Nms);
ProExt char    IsReg(void);
ProExt void    RebootDelay(u16 Nms);
ProExt void   Task_ProDeal(void);
ProExt void Task_ProSend(u16 Nms);
ProExt char    isGprsCon(void);

ProExt void JSON_RtcTimeGet(char *pBuf);
ProExt void  GetNULLData(char *pBuf, u16 *pLen);
ProExt u32 RegDelay;            //  ע����ʱ��                                
//ProExt bool ServerCmd_RtcAndPs_Deal(char **pBuf);
ProExt bool ServerCmd_Head_Deal(char **pBuf, u8 SetTimeFlag);
ProExt void    SetParaSaveFlag(u16 Addr);
#endif

