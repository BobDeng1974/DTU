#ifndef    _APP_DEBUG_H_
#define    _APP_DEBUG_H_

#ifndef    _InDebug
#define    DebugExt    extern
#else
#define    DebugExt    
#endif

typedef enum {
    DEBUG_NONE = 0,
    DEBUG_AT,
    DEBUG_GPRS,
    DEBUG_SMS,
    DEBUG_YUE,
    DEBUG_PRO,
    DEBUG_MONITOR,
//    DEBUG_UPDATE,
    DEBUG_ADC,
    DEBUG_TASK,
    DEBUG_VFD,
    DEBUG_PARA,
    DEBUG_CTRL,
    DEBUG_MODBUS,
    DEBUG_HMI,
    DEBUG_IO,
    DEBUG_UPGRADE,
	DEBUG_SFUD
}DEBUG_ENUM;


typedef union {
    int Flag;
    struct {
        int Enable: 1;                                              //  �������ʹ��
        int TaskStack:   1;                                         //  �����ջ
        int ATCmd:       1;                                         //  AT����  
        int GprsRcvData: 1;                                         //  GRPS��������
        int GprsSndData: 1;                                         //  GRPS��������
        int SMSRcvData:  1;                                         //  ���Ž���
        int SMSSndData:  1;                                         //  ���ŷ���
        int EmailInf:    1;                                         //  �ʼ���������  
        
        int FtpInf:      1;                                         //  FTP����
        int WindMaster:  1;                                         //  ����������
        int Adc:         1;                                         //  ADC����
        int Vfd:         1;                                         //  Vfd����
        int Ctrl:        1;
        int HMIModbus:   1;
        int IO:          1;
    }Bits;
}APP_DEBUG_UNION;


typedef struct{
    
    char FlashModel[4];
    char FlashName[16];
    char SimCard[4];
    char GprsModel[4];
    int GprsSignal;
}DEBUG_CHECK_OUT;

DebugExt  DEBUG_CHECK_OUT   DebugCheck;
DebugExt  APP_DEBUG_UNION   AppDebug;


DebugExt void    AppDebugInit(void);
DebugExt void    Debug(char* pMsg);
DebugExt void    DisableAllDebug(void);
DebugExt void    DebugMsg(DEBUG_ENUM Type, char *msg, ...);
DebugExt void    DebugHexMsg(char *pHead, char *pHex, short Len);
DebugExt void 	 DebugStrMsg(char *pHead, char * pStr, short len);

DebugExt void    RS485CfgResp(char *pBuf, int Len);
DebugExt void    RS485SndDly(void);
//DebugExt void	Debug(char* pMsg);

#endif







