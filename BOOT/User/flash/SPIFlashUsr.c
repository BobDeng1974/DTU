/**************************Copyright (c)****************************
**                            
**                                      
**                                 
**
**--------------File Info-------------------------------------------
** File name:               SPIFlashUsr.c
** Last modified Date:      2017-06-30
** Last Version:            0.9.0
** Descriptions:            SPI Flash�������û��ӿں���
**							
**------------------------------------------------------------------
** Created by:              chb
** Created date:            2017-06-30
** Version:                 0.9.0
** Descriptions:            ����ucoss,ʹ��ucossʱ����:LIB_USE_UCOSS
**							
**------------------------------------------------------------------
** Modified by:             
** Modified date:           
** Version:                 
** Descriptions:            
**
*******************************************************************/
#include "typedef.h"

//#define LIB_USE_UCOSS                                             //  ʹ��ucoss-ii

#ifdef  LIB_USE_UCOSS
#include  <ucos_ii.h>
static  OS_EVENT    *FlashLock = NULL;                              //  
#endif


/*******************************************************************
** Function name:       FlashLockCreate  
** Descriptions:        ��������ʼ��
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
** Created by:          chb
** Created Date:        2017-06-30
**
** Modified by:
** Modified date:
*******************************************************************/
void    FlashLockCreate(void)
{
#ifdef  LIB_USE_UCOSS
    FlashLock = OSSemCreate(1);
    if(NULL == FlashLock) {
        while(1);
    }
#endif 
}


/*******************************************************************
** Function name:       FlashLockPend  
** Descriptions:        �������ȴ�
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
** Created by:          chb
** Created Date:        2017-06-30
**
** Modified by:
** Modified date:
*******************************************************************/
void    FlashLockPend(void)
{
#ifdef  LIB_USE_UCOSS
    u8  err = 0;  
    OSSemPend(FlashLock, 0, &err);
#endif    
}


/*******************************************************************
** Function name:       FlashLockPost  
** Descriptions:        �������ͷ�
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
** Created by:          chb
** Created Date:        2017-06-30
**
** Modified by:
** Modified date:
*******************************************************************/
void    FlashLockPost(void)
{
#ifdef  LIB_USE_UCOSS
    OSSemPost(FlashLock);
#endif    
}



