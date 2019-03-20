/**************************Copyright (c)****************************
**                            
**                                      
**                                 
**
**--------------File Info-------------------------------------------
** File name:               SPIFlashHal.c
** Last modified Date:      2017-06-30
** Last Version:            0.9.0
** Descriptions:            SPI FlashӲ����ؽӿں���ͷ�ļ�
**							
**------------------------------------------------------------------
** Created by:              chb
** Created date:            2017-06-30
** Version:                 0.9.0
** Descriptions:            SPI FlashӲ����ؽӿں���ͷ�ļ�
**							
**------------------------------------------------------------------
** Modified by:             
** Modified date:           
** Version:                 
** Descriptions:            
**
*******************************************************************/
#ifndef _SPI_FLASH_HAL_H_
#define _SPI_FLASH_HAL_H_


/*******************************************************************
** Function name:       FlashHalInit  
** Descriptions:        SPIӲ���ӿڳ�ʼ��
** input parameters:    ��
** output parameters:   ��     
** Returned value:      ��
** Created Date:        2017-06-30
** Created by:          chb
**
** Modified by:
** Modified date:
*******************************************************************/
extern void    FlashHalInit(void);


/*******************************************************************
** Function name:       FlashSPISend  
** Descriptions:        SPI�ֽ����ݷ���
** input parameters:    data            �����͵��ֽ�����
** output parameters:   ��     
** Returned value:      ��
** Created Date:        2017-06-30
** Created by:          chb
**
** Modified by:
** Modified date:
*******************************************************************/
extern u8      FlashSPISend(u8 data);


/*******************************************************************
** Function name:       FlashSelect  
** Descriptions:        SPIƬѡ����
** input parameters:    bSet            SPIƬѡ���Ʊ�ʶ
** output parameters:   ��     
** Returned value:      ��
** Created Date:        2017-06-30
** Created by:          chb
**
** Modified by:
** Modified date:
*******************************************************************/
extern void    FlashSelect(u8 bSet);

#endif


