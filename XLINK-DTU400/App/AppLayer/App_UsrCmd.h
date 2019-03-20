#ifndef _APP_USR_CMD_H_
#define _APP_USR_CMD_H_

#ifndef _InUsrCmd
#define UsrCmdExt   extern
#else
#define UsrCmdExt
#endif

#include "uart.h"
#include "app_debug.h"

typedef struct {
    char Uart1: 1;
    char Uart2: 1;
    char Uart3: 1;
    char Uart5: 1;
}RCV_STRUCT;

UsrCmdExt RCV_STRUCT GbRcvData;

#define UART_TEST_BUF_SIZE      100
UsrCmdExt char    UartTestBuf[UART_TEST_BUF_SIZE];
UsrCmdExt short   UartBufLen;
UsrCmdExt char    GbRS485TstFlg;


/*
 *  ��������
 *  *RK,SET,03,15-09-10 17:07:00#����ϵͳʱ��
 *  *RK,QUERY,00#��ѯ���в���
 *  *RK,CTRL,01,01#����ϵͳ 
 *  *RK,TEST,01,01#����RS485���Թ���
 *  *RK,DEBUG,00,01#����������Ϣ
 */
#define USR_CMD_SET         "SET"
typedef enum {
    CMD_SET_ID = 0x01,                                              //  �����ն�ID          0x01
    CMD_SET_SERVER,                                                 //  ���÷�������Ϣ      0x02
    CMD_SET_TIME,                                                   //  Уʱ                0x03
    CMD_SET_STMPADDR,                                               //  �����ʼ���ַ        0x04
    CMD_SET_FTPINF,                                                 //  ����FTP��Ϣ         0x05
    CMD_SET_SMSALMEN,                                               //  ���ö��ű�������    0x06
    CMD_SET_ALMTEL,                                                 //  ���ñ����绰����    0x07
    CMD_SET_LOW_VOLT,                                               //  ���õ�ѹ��ֵ        0x08
}CMD_SET_ENUM;


#define USR_CMD_QUERY       "QUERY"
typedef enum {
    CMD_QUERY_ALL = 0x00,                                           //  ��ѯ���в���        0x00
    CMD_QUERY_ID,                                                   //  ��ѯ�ն�ID          0x01
    CMD_QUERY_SERVER,                                               //  ��ѯ��������Ϣ      0x02
    CMD_QUERY_TIME,                                                 //  ��ѯʱ��            0x03
    CMD_QUERY_STMPADDR,                                             //  ��ѯ�ʼ���ַ        0x04
}CMD_QUERY_ENUM;


#define USR_CMD_CTRL        "CTRL"
typedef enum {
    CMD_CTRL_RESET = 0x01,                                          //  �����豸            0x01
    CMD_CTRL_UPDATE,
}CMD_CTRL_ENUM;


#define USR_CMD_TEST        "TEST"
typedef enum {
    CMD_TEST_RS485 = 0x01,                                          //  485�˿ڲ���         0x01
    CMD_TEST_DO,                                                    //  �̵����������      0x02
    CMD_TEST_EMAIL,                                                 //  �����ʼ�����        0x03
    CMD_TEST_FTP,                                                   //  FTP�ļ����Ͳ���     0x04
    CMD_TEST_YECX,                                                  //  ����ѯ            0x05
}CMD_TEST_ENUM;


#define USR_CMD_DEBUG       "DEBUG"
typedef enum {
    CMD_DEBUG_EN = 0x00,                                            //  ����������Ϣ        0x00
    CMD_DEBUG_STACK,                                                //  �����ջ            0x01                            
    CMD_DEBUG_AT,                                                   //  ATָ��              0x02
    CMD_DEBUG_GPRS_RX,                                              //  ���շ���������      0x03
    CMD_DEBUG_GPRS_TX,                                              //  ���ͷ���������      0x04
    CMD_DEBUG_SMS_RX,                                               //  ���Ž�������        0x05
    CMD_DEBUG_SMS_TX,                                               //  ���ŷ�������        0x06
    CMD_DEBUG_EMAIL,                                                //  EMAIL��������       0x07
    CMD_DEBUG_FTP,                                                  //  FTP��������         0x08
    CMD_DEBUG_WDMST,                                                //  ���ٴ���������      0x09
    CMD_DEBUG_ADC,                                                  //  ADC����             0x0A
    CMD_DEBUG_VFD,                                                  //  ��Ƶ������          0x0B
    CMD_DEBUG_HMI_MODBUS,
}CMD_DEBUG_ENUM;


UsrCmdExt   void    TaskUartUsrCmd(void);
UsrCmdExt   void    TaskRS485Test(void);
UsrCmdExt   char    UsrCmdParse(char *pRd, char *pRsp);
UsrCmdExt   char    SMSCmdParse(char *pRd, char *pRsp);



#endif
