/**************************Copyright (c)****************************
**                            
**                                      
**                                 
**
**--------------File Info-------------------------------------------
** File name:               SPIFlashHal.c
** Last modified Date:      2017-06-30
** Last Version:            0.9.0
** Descriptions:            SPI FlashӲ����ؽӿں���
**                            
**------------------------------------------------------------------
** Created by:              chb
** Created date:            2017-06-30
** Version:                 0.9.0
** Descriptions:            SPI FlashӲ����ؽӿں���
**                            
**------------------------------------------------------------------
** Modified by:             
** Modified date:           
** Version:                 
** Descriptions:            
**
*******************************************************************/
#include "typedef.h"
#include "stm32f10x.h"
#include "SPIFlashPin.h"
#include "SPIFlashHal.h"


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
void    FlashHalInit(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    SPI_InitTypeDef SPI_InitStruct;
    
    RCC_APB2PeriphClockCmd(FLASH_GPIO_RCC, ENABLE);
    RCC_APB2PeriphClockCmd(FLASH_SPI_RCC, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);        
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
    
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Pin = FLASH_SCK_PIN;
    GPIO_Init(FLASH_SCK_PORT, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = FLASH_MOSI_PIN;
    GPIO_Init(FLASH_MOSI_PORT, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = FLASH_MISO_PIN;
    GPIO_Init(FLASH_MISO_PORT, &GPIO_InitStructure);  
    
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin = FLASH_CS_PIN;
    GPIO_Init(FLASH_CS_PORT, &GPIO_InitStructure);    
    FlashSelect(FALSE);

    SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStruct.SPI_Mode = SPI_Mode_Master;
    SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStruct.SPI_CPOL = SPI_CPOL_High;
    SPI_InitStruct.SPI_CPHA = SPI_CPHA_2Edge;
    SPI_InitStruct.SPI_NSS  = SPI_NSS_Soft;
    SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;
    SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_InitStruct.SPI_CRCPolynomial = 7;

    SPI_Init(FLASH_SPI_IDX, &SPI_InitStruct);
    SPI_Cmd(FLASH_SPI_IDX, ENABLE);
}


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
u8  FlashSPISend(u8 data)
{
    while(SPI_I2S_GetFlagStatus(FLASH_SPI_IDX, SPI_I2S_FLAG_TXE) == RESET);
    SPI_I2S_SendData(FLASH_SPI_IDX, (u16)data);

    while(SPI_I2S_GetFlagStatus(FLASH_SPI_IDX, SPI_I2S_FLAG_RXNE) == RESET);
    data = (u8)SPI_I2S_ReceiveData(FLASH_SPI_IDX);

    return data;
}


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
void    FlashSelect(u8 bSet)
{
    bSet ? GPIO_ResetBits(FLASH_CS_PORT, FLASH_CS_PIN) : GPIO_SetBits(FLASH_CS_PORT, FLASH_CS_PIN);
}


