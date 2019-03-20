/**************************Copyright (c)****************************
**                            
**                                      
**                                 
**
**--------------File Info-------------------------------------------
** File name:               SPIFlashUsr.h
** Last modified Date:      2017-06-30
** Last Version:            0.9.0
** Descriptions:            SPI Flash�������û��ӿں���ͷ�ļ�
**							
**------------------------------------------------------------------
** Created by:              chb
** Created date:            2017-06-30
** Version:                 0.9.0
** Descriptions:            ����ucoss
**							
**------------------------------------------------------------------
** Modified by:             
** Modified date:           
** Version:                 
** Descriptions:            
**
*******************************************************************/
#ifndef _SPI_FLASH_USR_H_
#define _SPI_FLASH_USR_H_


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
extern void    FlashLockCreate(void);


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
extern void    FlashLockPend(void);


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
extern void    FlashLockPost(void);


#endif

