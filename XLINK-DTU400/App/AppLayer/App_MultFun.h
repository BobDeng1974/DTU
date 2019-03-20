#ifndef _MULT_FUN_H_
#define _MULT_FUN_H_


#ifndef _InMult
#define MultExt  extern
#else
#define MultExt  
#endif

#include "struct.h"

typedef struct {
    struct {
        u32 Flag: 1;                                                //  ��ʱ��ʶ
        u32 Hour: 7;                                                //  Сʱ����
        u32 Min:  6;                                                //  ���Ӽ���
        u32 Sec:  6;                                                //  �����
    }Cnt;

    struct {
        u32 CSQ1: 5;                                                //  GPRS����ʱ���ź�
        u32 CSQ2: 5;
        u32 CSQ3: 5;
        u32 ErrCode1: 4;                                            //  GPRS������1
        u32 ErrCode2: 4;                                            //  GPRS������2
        u32 ErrCode3: 4;                                            //  GPRS������3
    }Info;
}BEFORE_REG_INFO;

MultExt BEFORE_REG_INFO BeforeReg;
MultExt BEFORE_REG_INFO RS485ErrTime;
MultExt u32 MoneyCheckTimer;

MultExt DELAY_TIMER RmtBootTimer;
MultExt DELAY_TIMER RmtStopTimer;
MultExt DELAY_TIMER CfgDelay;
MultExt DELAY_TIMER AlmDealDelay;   

MultExt void    RegTimerStop(void);
MultExt void    RegTimerErrCode(void);
MultExt void    MultFunInit(void);
MultExt void    Task_MultFun_1s(void);

#endif

