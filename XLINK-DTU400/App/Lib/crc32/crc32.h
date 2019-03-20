/**************************Copyright (c)****************************
**                            
**                                      
**                                 
**
**--------------File Info-------------------------------------------
** File name:               crc32.c
** Last modified Date:      2017-07-03
** Last Version:            0.9.0
** Descriptions:            crc32�㷨ͷ�ļ�
**                            
**------------------------------------------------------------------
** Created by:              chb
** Created date:            2017-07-03
** Version:                 0.9.0
** Descriptions:            crc32�㷨ͷ�ļ�
**                            
**------------------------------------------------------------------
** Modified by:             
** Modified date:           
** Version:                 
** Descriptions:            
**
*******************************************************************/
#ifndef _CRC32_H_
#define _CRC32_H_


/*******************************************************************
** Function name:       CRC32CalcReset  
** Descriptions:        crc32���㸴λ
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
** Created by:          chb
** Created Date:        2017-07-03
**
** Modified by:
** Modified date:
*******************************************************************/
extern void    CRC32CalcReset(void);


/*******************************************************************
** Function name:       CRC32Calc  
** Descriptions:        ����crc32
** input parameters:    pBuf            ��crc32��������ݻ���
**                      Len             ��crc32��������ݴ�С
** output parameters:   u32             �������crc32ֵ
** Returned value:      ��
** Created by:          chb
** Created Date:        2017-07-03
**
** Modified by:
** Modified date:
*******************************************************************/
extern u32     CRC32Calc(u8 *pBuf, u16 Len);


#endif

