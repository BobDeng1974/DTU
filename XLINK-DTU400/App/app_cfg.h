/*
*********************************************************************************************************
*                                              EXAMPLE CODE
*
*                          (c) Copyright 2003-2006; Micrium, Inc.; Weston, FL
*
*               All rights reserved.  Protected by international copyright laws.
*               Knowledge of the source code may NOT be used to develop a similar product.
*               Please help us continue to provide the Embedded community with the finest
*               software available.  Your honesty is greatly appreciated.
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*
*                                      APPLICATION CONFIGURATION
*
*                                     ST Microelectronics STM32
*                                              with the
*                                   STM3210E-EVAL Evaluation Board
*
* Filename      : app_cfg.h
* Version       : V1.00
* Programmer(s) : BAN
*********************************************************************************************************
*/

#ifndef  __APP_CFG_H__
#define  __APP_CFG_H__


/*
 *  DTU-K V0.2  20160328    
        1.485�ӿ�ͨ���쳣ʱ��¼�쳣ʱ��ʱ��
        2.֧��MAM680D(������Ƶ,�����Ƶ,�¶�ƫ��20,�ܶ�ȡԤ����Ϣ,��Զ�̿��ػ�)
    DTU-K V0.3  20160330
        1.�����޿���/ͣ���¼��ϴ���BUG
    DTU-K V0.4  20160408
        1.����APN����������������
        2.Զ���޸������Ͷ˿ں��Զ��������ش��ն����в���
        3.Զ���޸�APN�������Զ�����,���ش��ն˲���
        4.����Զ�̿�/�ػ��Ͱ�����/�ػ�
        5.���Ӱ汾����,�ն˱��,GPRSλ������,С��ID,�����ر���ֵ,Ԥ��ֵ�ϴ�
        6.�ϵ�1h���ն˽�������ѯ
        7.��HMIʧ�ܴ�����Ϊ485�ӿڵ�ǰʧ�ܴ���
    DTU-K V0.5  20160430
        1.������������������ȶ���BUG     
    DTU-K V0.6  20170912
        1.�����Խ���������� 
    DTU-K V0.7  20170918
        1.����MAM-6090������
    DTU-K V0.8  20170919
        1.����Զ����������
    DTU-K V0.9  20170923
        1.����MAM-6090д�������¼�����
        2.GPRS����ʱ���ȡ
        3.�Ż�������ʱ���Ż����ִ��룬���ٴ���ռ�
        4.�����ڲ����ʱ�Ӵ���
    DTU-K V1.0  20171024
        1.�޸�ID�ŵĸ�ʽ
        2.�޸�COM��ɫָʾ�ƣ����ӵ�������ʱ��˸
        3.�޸��ⲿ���Ź�ι��
        
    DTU-K V1.01  20171104
        1.�޸�Ŀ���¶���ʾ����
        2.�޸������ۼ��õ硢  ����ۼ��õ� ��Ƶ�����ۼ��õ�  ��Ƶ����ۼ��õ� ��ʾ����
        3.�޸�ǰ���¶�������¶���Чʱ����ʾΪ0
        
    DTU-K V1.02  20171127
        1.�޸�GPRS�ź�ֵδ��ʱ�ϴ���
        2.�޸ķ��Ƶ��*10
        3.�޸������ۼ��õ硢����ۼ��õ硢��Ƶ�ۼ��õ硢��Ƶ����ۼ��õ�Ĳ�����λΪu32��
        4.�����û��������������������Ҳ����ϵ��ȫ���ϴ�һ�飬�Լ������仯ʱ���ӵ����������ϴ���
        5.�����¼����豸�ӵ硢�豸�������豸ͣ�����豸ֹͣ���������С��������С��վ����ߡ�������������ֹͣ���¼���
        6.KEYֵ�б��������ֵ60��Ϊ80.
        
    DTU-K V1.03  20171218    
        1.�������վ���ܡ�
        2.����ͳһ�������ܡ�
        3.������Ƶ����������
        4.����������д������޸�
        
   DTU-K V1.04  20171222
        1.����MAM860/870/880����������
        2.����MAM660/670/680����������
        3.����MAM100/200/260/280/300����������
        4.����ۼ��õ���С10��

   DTU-K V2.01  20180119
        1.Ӳ���汾������V2.0
        2.����2·485������CAN����
        3.�������������ݶ�ȡ����
        4.���η��������ֹͣ�¼�
        
	
	DTU-K V2.01  20180119
		1. �޸������Ʋ���������������
        
	DTU-K V2.02  2018-04-19
		
	1. ���ܱ� �������²���
	   CT(�������������) LINE_NET(���߷�ʽ --3P3L/3P4L)
	   UAB UBC UCA��
	2. �ϱ��ĵ��� ��Ϊһ�β������������CT ��ʵ�ʵ���ֵ����
	3.�ϱ��ĵ�ѹ���ݵ�ǰ���߷�ʽ�Ĳ�ͬ���Զ�ѡ���ϴ��ߵ�ѹ������  ��ѹ��
	4. ���Ĭ��վ���޸�Ϊ1�� ������վ��Ĭ���޸�Ϊ15
	5. �޸��������������
	6. ���� RS485 ͨ��״ָ̬ʾ ״̬��
	   ��ɫָʾ�� ����ָʾ ����������������� ͨ��״̬
	
	DTU-K V2.03  2018-04-26
	
	1.�����豸����ͨ��״̬��� 004
	2.������������ư� �����Ϣ


	DTU-K  V2.04 2018-04-27
	1. ������ʷ����ʱ����ӡ���ʷ����ʱ�����
	2. ������ȥ�������仯�ϱ��������仯�����ϴ�
	3. ��߲����ϱ����ȼ���ʹ�������ȼ������¼��ϱ�
	4. ��ʼ������ֻ��ע��ɹ����ϱ�һ��
	
	DTU-K  V2.05 2018-6-28 20:25:57
	1. ֧��MAM860 -- ��Ƶ   MAM860C ---- ��Ƶ
	2. ����ģʽ�£�Ĭ�϶�ȡ��������Ʋ���
	3. ���� key 436 --- ��������
	4. ����������֮ǰ���������仯�ϱ���Ϊ ���б仯�ϱ�
	5. ��ʷ����/����ʱ���ɱ仯�ϱ���Ϊ2min����ֻ�ϱ�һ��
	6. Run����ָʾ�ƻָ�Ϊ֮ǰ��״̬�� ��ָֻʾ������485���ӵ�״̬


	DTU-K V2.06  2018-07-30 
	1. ������ŵ���YN300-E Э��

	DTU-K  V2.07  2018-8-27
	1. ʱ���ϱ�ֻ�з��ӱ仯���ϱ�
	2. �޸�DTU����޸�ʧ�ܵ�Bug
	
	v2.10
	XLINK ʹ���ⲿ����
	
	v2.11 
	1.������Դ��ѹ����������û�б���������
	2. DTU�豸���ǰ׺����Ϊ X18, XLINK�豸���ǰ׺ΪXBD
	
	v2.12
	1. Spi Flash  ������Ϊͨ�õ�SFUD������֧�ָ����SPI flashоƬ
	2. ���Э�����Ϊ ��������(֮ǰΪ��ŵ���),��ʾ���ܵ���Ϊ���ĵ���
	
	
*/
#define  XLINK    //���Ӳ����xlink �����˺�

#define ID_HEADER_LEN   3    

#ifdef XLINK
#define PRODUCT_MODLE  "X-LINK-1"
#define APP_VERSION    "LNK_V2.12"             //  ����汾��ƽ̨��ʾ��
#define ID_HEADER       "XBD"

#else
#define PRODUCT_MODLE   "X-DTU"                 //  ��Ʒ�ͺ�
#define APP_VERSION     "DTU_V2.12"             //  ����汾��ƽ̨��ʾ��
#define ID_HEADER       "X18"
 

#endif
#define PRODUCT_VERSION         "V2.12"                 //  Զ����������汾 �Ƚ�

#define PCB_VERSION             "DTU_V2.0"              //  Ӳ���汾
#define SERVER_TYPE             1                         // ������ƽ̨���ͣ�0δ֪��1��£�2��������3��߻� �����ڻ�ȡ�ض�������������˿ڣ�


#define MAIN_VFD_NODE_ADDR      1                       //  ������Ƶ��Ĭ�ϵ�ַ
#define FAN_VFD_NODE_ADDR       2                       //  �����Ƶ��Ĭ�ϵ�ַ
#define VFD_READ_OPT_INTEVAL    50                      //  ��Ƶ�������ݼ��
#define METER_READ_OPT_INTEVAL  500                     //  ��Ƶ�������ݼ��


#define VFD_READ_RETRY_NUM      3                       //  ��Ƶ�����������Դ���
#define VFD_WRITE_RETRY_NUM     5                       //  ��Ƶ��д�������Դ���


#define PRO_DATA_BIG_ENDIAN     1                       //  Э�����ݴ��ģʽ
#define PRO_NOT_USE_CRC         1                       //  ��ʹ��CRC,���ֶ�Ϊ0

#define USE_RTC_EN              0                       //  ʹ��RTCʱ��

#define PROTOCOL_USE_ONE_SNNO   1                       //  1:ֻʹ��Э��ͷ��ˮ��,��Ϣ��������ˮ����Ϣ


#define RS485_SEND_DELAY        0                       //  RS485���ݷ��ͺ���ʱ�л�Ϊ����״̬

#define USE_UART_DEBUG_INFO     1

#define  DEBUG_USE_STR          1              //   GPRS ���͵����� ���ַ�����ӡ
	
											     //  �����ӡʮ������



/*
*********************************************************************************************************
*                                       MODULE ENABLE / DISABLE
*********************************************************************************************************
*/
#define CUSTOMER_BD
//#define CUSTOMER_XWL

#ifdef  CUSTOMER_BD

#define SERVER_DOMAIN   "bd.qhxwl.com"
#define SERVER_PORT     9003
#endif


#ifdef  CUSTOMER_XWL
#define SERVER_DOMAIN   "test.qhxwl.com"
//#define SERVER_DOMAIN   "xwl.test.qhxwl.com"
#define SERVER_PORT     9003

//#define SERVER_DOMAIN   "test.qhxwl.com"
//#define SERVER_PORT     9003
//#define SERVER_DOMAIN   "main.test.qhxwl.com"        // ͳһ������������������ڻ�ȡ�ض��������˿�
//#define SERVER_PORT     9000
#endif

#define SST_UPDATE_ADDR         (4<<10)             // ������ʼ��ַ: 4K
#define SSTFLASH_PAGE_SIZE      4096




/*
*********************************************************************************************************
*                                            TASK PRIORITIES
*********************************************************************************************************
*/


#define TASK_SVRCOMM_EN  
#define TASK_VFDCOMM_EN
#define TASK_VFDDEAL_EN

    
#define TASK_START_PRIO                    1
#define TASK_VFDDEAL_PRIO               4
#define TASK_VFDCOMM_PRIO               5

#define TASK_PRODEAL_PRIO               6
#define TASK_SVRCOMM_PRIO                7
#define TASK_PROSEND_PRIO               8


//#define  OS_PROBE_TASK_PRIO              (OS_LOWEST_PRIO - 3)
#define  OS_TASK_TMR_PRIO                (OS_LOWEST_PRIO - 2)

/*
*********************************************************************************************************
*                                            TASK STACK SIZES
*                             Size of the task stacks (# of OS_STK entries)
*********************************************************************************************************
*/


//#define TASK_START_STK_SIZE         120
//#define TASK_SVRCOMM_STK_SIZE       150
//#define TASK_PRODEAL_STK_SIZE       150
//#define TASK_PROSEND_STK_SIZE       100
//#define TASK_VFDCOMM_STK_SIZE       100
//#define TASK_VFDDEAL_STK_SIZE       100




#endif
