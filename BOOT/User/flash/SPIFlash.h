/**************************Copyright (c)****************************
**                            
**                                      
**                                 
**
**--------------File Info-------------------------------------------
** File name:               SPIFlash.h
** Last modified Date:      2017-06-30
** Last Version:            0.9.0
** Descriptions:            SPI Flash�����⺯��ͷ�ļ�
**							
**------------------------------------------------------------------
** Created by:              chb
** Created date:            2017-06-30
** Version:                 0.9.0
** Descriptions:            SPI Flash�����⺯��ͷ�ļ�
**							
**------------------------------------------------------------------
** Modified by:             
** Modified date:           
** Version:                 
** Descriptions:            
**
*******************************************************************/
#ifndef _SPI_FLASH_H_
#define _SPI_FLASH_H_


/*******************************************************************
**  SPI Flash���ͺ궨��
*******************************************************************/
typedef enum {
    FLASH_MDL_UNKNOW = 0,                                           //  Flash���Ͳ�ʶ��
    FLASH_SST25VF016,
    FLASH_SST25VF032,  
    FLASH_W25Q16,                                                   
    FLASH_W25Q32,                                                   
}FLASH_MDL_ENUM;


/*******************************************************************
** Function name:       SPIFlashInit  
** Descriptions:        SPI Flash��ʼ��
** input parameters:    ��
** output parameters:   ��
** Returned value:      FLASH_MDL_ENUM      Flash����
** Created Date:        2017-06-30
** Created by:          chb
**
** Modified by:
** Modified date:
*******************************************************************/
extern FLASH_MDL_ENUM    SPIFlashInit(void);


/*******************************************************************
** Function name:       SPIFlashRead  
** Descriptions:        SPI Flash������
** input parameters:    Addr    ��������ʼ��ַ
**                      pBuf    ��ȡ���ݻ���
**                      RdLen   ��ȡ���ݳ���
** output parameters:   pBuf    ��ȡ���ݻ���      
** Returned value:      ��
** Created Date:        2017-06-30
** Created by:          chb
**
** Modified by:
** Modified date:
*******************************************************************/
extern void    SPIFlashRead(u32 Addr, u8 *pBuf, u16 RdLen);


/*******************************************************************
** Function name:       SPIFlashWrite  
** Descriptions:        SPI Flashд����
** input parameters:    Addr    д������ʼ��ַ
**                      pBuf    д���ݻ���
**                      RdLen   д���ݳ���
** output parameters:   ��     
** Returned value:      ��
** Created Date:        2017-06-30
** Created by:          chb
**
** Modified by:
** Modified date:
*******************************************************************/
extern void    SPIFlashWrite(u32 Addr, u8 *pBuf, u16 WrLen);


/*******************************************************************
** Function name:       SPIFlashErase4K  
** Descriptions:        SPI Flash 4K��������
** input parameters:    Addr    ������ʼ��ַ
** output parameters:   ��     
** Returned value:      ��
** Created Date:        2017-06-30
** Created by:          chb
**
** Modified by:
** Modified date:
*******************************************************************/
extern void    SPIFlashErase4K(u32 Addr);


/*******************************************************************
** Function name:       SPIFlashErase64K  
** Descriptions:        SPI Flash 64K��������
** input parameters:    Addr    ������ʼ��ַ
** output parameters:   ��     
** Returned value:      ��
** Created Date:        2017-06-30
** Created by:          chb
**
** Modified by:
** Modified date:
*******************************************************************/
extern void    SPIFlashErase64K(u32 Addr);


/*******************************************************************
** Function name:       SPIFlashChipErase  
** Descriptions:        SPI Flash��Ƭ��������
** input parameters:    ��
** output parameters:   ��     
** Returned value:      ��
** Created Date:        2017-06-30
** Created by:          chb
**
** Modified by:
** Modified date:
*******************************************************************/
extern void    SPIFlashChipErase(void);


/*******************************************************************
** Function name:       SPIFlashLibVersion  
** Descriptions:        ��ȡSPI Flash��汾��Ϣ
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��汾��Ϣ�ַ���
** Created by:          chb
** Created Date:        2017-06-30
**
** Modified by:
** Modified date:
*******************************************************************/
extern char    *SPIFlashLibVersion(void);


#endif

