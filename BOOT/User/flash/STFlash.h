/**************************Copyright (c)****************************
**                            
**                                      
**                                 
**
**--------------File Info-------------------------------------------
** File name:               ST Flash.h
** Last modified Date:      2017-06-30
** Last Version:            0.9.0
** Descriptions:            ST MCU�ڲ�Flash�����⺯��ͷ�ļ�
**							
**------------------------------------------------------------------
** Created by:              chb
** Created date:            2017-06-30
** Version:                 0.9.0
** Descriptions:            ST MCU�ڲ�Flash�����⺯��ͷ�ļ�
**							
**------------------------------------------------------------------
** Modified by:             
** Modified date:           
** Version:                 
** Descriptions:            
**
*******************************************************************/
#ifndef _STM_FLASH_H_
#define _STM_FLASH_H_


/*******************************************************************
** Function name:       STFlashRead  
** Descriptions:        ST MCU�ڲ�flash������
** input parameters:    Addr        ��������ʼ��ַ
**                      pBuf        �����ݻ���
**                      Len         �����ݳ���
** output parameters:   pBuf        �����ݻ���
** Returned value:      ��
** Created by:          chb
** Created Date:        2017-06-30
**
** Modified by:
** Modified date:
*******************************************************************/
void	STFlashRead(u32 Addr, u8 *pBuf, u16 Len);


/*******************************************************************
** Function name:       STFlashWrite  
** Descriptions:        ST MCU�ڲ�flashд����
** input parameters:    Addr        д������ʼ��ַ
**                      pBuf        д���ݻ���
**                      Len         д���ݳ���
** output parameters:   ��
** Returned value:      TRUE        �ɹ�
**                      FALSE       ʧ��
** Created by:          chb
** Created Date:        2017-06-30
**
** Modified by:
** Modified date:
*******************************************************************/
u8  STFlashWrite(u32 Addr, u8 *pBuf, u16 Len);


/*******************************************************************
** Function name:       STFlashErase  
** Descriptions:        ST MCU�ڲ�flash��������
** input parameters:    Addr        ������ʼ��ַ
** output parameters:   ��
** Returned value:      TRUE        �ɹ�
**                      FALSE       ʧ��
** Created by:          chb
** Created Date:        2017-06-30
**
** Modified by:
** Modified date:
*******************************************************************/
u8  STFlashErase(u32 Addr);


#endif

