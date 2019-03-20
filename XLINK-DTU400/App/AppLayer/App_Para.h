#ifndef _APP_PARA_H_
#define    _APP_PARA_H_

#ifndef _InPara
#define ParaExt     extern
#else
#define ParaExt  
#endif

#include "struct.h"


/***********************************************************************/

typedef enum {
    RUN_TYPE_STOP = 0,                                              //  ֹͣ״̬
    RUN_TYPE_AC_XINJIAO_DELAY,                                      //  ��Ƶ�ǽ���ʱ
    RUN_TYPE_LOAD_DELAY,                                            //  ����������ʱ
    RUN_TYPE_UNLOAD_RUN,                                            //  ��������״̬
    RUN_TYPE_LOAD_RUN,                                              //  ��������״̬
    RUN_TYPE_AC_UNLOAD_STOP,                                        //  ��Ƶ�վ�ͣ��״̬
    RUN_TYPE_STOP_DELAY,                                            //  ͣ����ʱ״̬
    RUN_TYPE_VFD_TO_AC_DELAY,                                       //  ��Ƶת��Ƶ��ʱ
}RUN_TYPE_ENUM;         

#pragma pack(1)
typedef struct {
    DEV_PARA_STRUCT Dev;
    NET_PARA_STRUCT Net;
    u32 Index;
    u16 Crc;
    
}SYS_PARA_BUF_STRUCT;
#define SYS_PARA_BUF_SIZE       sizeof(SYS_PARA_BUF_STRUCT)

typedef struct {
    MAX_TIME_STRUCT Hour;
    u32 Index;
    u16 Crc;
}USR_PARA_BUF_STRUCT;
#define USR_PARA_BUF_SIZE       sizeof(USR_PARA_BUF_STRUCT)


typedef enum {
    ADDR_JSON_INT = 0,       
    ADDR_JSON_STRING,        
    ADDR_JSON_FLOAT,          
    
}ADDR_JSON_ENUM;      



typedef struct {
    ERR_INFO_STRUCT  Error;
    u32 Index;
    u16 Crc;
}OTH_INFO_BUF_STRUCT;
#define OTH_INFO_BUF_SIZE        sizeof(OTH_INFO_BUF_STRUCT)

typedef struct {
    ALARM_FLAG_UNION    Alarm;
    PREALM_FLAG_UNION   PreAlarm;
    u16 Res;
    u32 Index;
    u16 Crc;
}ALM_INFO_BUF_STRUCT;
#define ALM_INFO_BUF_SIZE        sizeof(ALM_INFO_BUF_STRUCT)

#pragma pack()



ParaExt u16 SysParaBuf[SYS_PARA_BUF_SIZE >> 1];
ParaExt SYS_PARA_BUF_STRUCT *GpSysPara;

ParaExt u16 UsrParaBuf[USR_PARA_BUF_SIZE >> 1];
ParaExt USR_PARA_BUF_STRUCT *GpUsrPara;

ParaExt u16 OthInfoBuf[OTH_INFO_BUF_SIZE >> 1];
ParaExt OTH_INFO_BUF_STRUCT *GpOthInfo;

ParaExt u16 AlmInfoBuf[ALM_INFO_BUF_SIZE >> 1];
ParaExt ALM_INFO_BUF_STRUCT *GpAlmInfo;


#if 1
#define FLASH_SECTOR_SIZE       0x800                               //  2048
#define FLASH_PAGE_MASK         (FLASH_SECTOR_SIZE-1)               //  ����������
#define SECTOR_BEGIN_ADDR(Addr) (Addr & (~FLASH_PAGE_MASK))         //  ������ַ��ַ
#define SECTOR_OFFSET_ADDR(Addr)(Addr & FLASH_PAGE_MASK)            //  ����ƫ�Ƶ�ַ

#define SYS_PARA_SECTOR_NUM         1                                  
#define SYS_PARA_FLASH_MAX_CNT      (FLASH_SECTOR_SIZE*SYS_PARA_SECTOR_NUM/SYS_PARA_BUF_SIZE)
#define SYS_PARA_FLASH_START_ADDR   (254 << 10)                     //  254K
#define SYS_PARA_FLASH_END_ADDR     (SYS_PARA_FLASH_START_ADDR+SYS_PARA_BUF_SIZE*SYS_PARA_FLASH_MAX_CNT)

#define USR_PARA_SECTOR_NUM         1
#define USR_PARA_FLASH_MAX_CNT      (FLASH_SECTOR_SIZE*USR_PARA_SECTOR_NUM/USR_PARA_BUF_SIZE)
#define USR_PARA_FLASH_START_ADDR   (252 << 10)                     //  252K
#define USR_PARA_FLASH_END_ADDR     (USR_PARA_FLASH_START_ADDR+USR_PARA_BUF_SIZE*USR_PARA_FLASH_MAX_CNT)

#define OTH_INFO_SECTOR_NUM         1                       
#define OTH_INFO_FLASH_MAX_CNT      (FLASH_SECTOR_SIZE*OTH_INFO_SECTOR_NUM/OTH_INFO_BUF_SIZE)
#define OTH_INFO_FLASH_START_ADDR   (250 << 10)                     //  250K
#define OHT_INFO_FLASH_END_ADDR     (OTH_INFO_FLASH_START_ADDR+OTH_INFO_BUF_SIZE*OTH_INFO_FLASH_MAX_CNT)

#define ALM_INFO_SECTOR_NUM         5                       
#define ALM_INFO_FLASH_MAX_CNT      (FLASH_SECTOR_SIZE*ALM_INFO_SECTOR_NUM/ALM_INFO_BUF_SIZE)
#define ALM_INFO_FLASH_START_ADDR   (240 << 10)                      //  240K
#define ALM_INFO_FLASH_END_ADDR     (ALM_INFO_FLASH_START_ADDR+ALM_INFO_BUF_SIZE*ALM_INFO_FLASH_MAX_CNT)

#else
#define FLASH_SECTOR_SIZE       0x400                               //  1024
#define FLASH_PAGE_MASK         (FLASH_SECTOR_SIZE-1)               //  ����������
#define SECTOR_BEGIN_ADDR(Addr) (Addr & (~FLASH_PAGE_MASK))         //  ������ַ��ַ
#define SECTOR_OFFSET_ADDR(Addr)(Addr & FLASH_PAGE_MASK)            //  ����ƫ�Ƶ�ַ

#define SYS_PARA_SECTOR_NUM         1                                  
#define SYS_PARA_FLASH_MAX_CNT      (FLASH_SECTOR_SIZE*SYS_PARA_SECTOR_NUM/SYS_PARA_BUF_SIZE)
#define SYS_PARA_FLASH_START_ADDR   (63 << 10)                      //  63K
#define SYS_PARA_FLASH_END_ADDR     (SYS_PARA_FLASH_START_ADDR+SYS_PARA_BUF_SIZE*SYS_PARA_FLASH_MAX_CNT)

#define USR_PARA_SECTOR_NUM         1
#define USR_PARA_FLASH_MAX_CNT      (FLASH_SECTOR_SIZE*USR_PARA_SECTOR_NUM/USR_PARA_BUF_SIZE)
#define USR_PARA_FLASH_START_ADDR   (62 << 10)                      //  62K
#define USR_PARA_FLASH_END_ADDR     (USR_PARA_FLASH_START_ADDR+USR_PARA_BUF_SIZE*USR_PARA_FLASH_MAX_CNT)

#define OTH_INFO_SECTOR_NUM         1                       
#define OTH_INFO_FLASH_MAX_CNT      (FLASH_SECTOR_SIZE*OTH_INFO_SECTOR_NUM/OTH_INFO_BUF_SIZE)
#define OTH_INFO_FLASH_START_ADDR   (61 << 10)                      //  61K
#define OHT_INFO_FLASH_END_ADDR     (OTH_INFO_FLASH_START_ADDR+OTH_INFO_BUF_SIZE*OTH_INFO_FLASH_MAX_CNT)

#define ALM_INFO_SECTOR_NUM         2                       
#define ALM_INFO_FLASH_MAX_CNT      (FLASH_SECTOR_SIZE*ALM_INFO_SECTOR_NUM/ALM_INFO_BUF_SIZE)
#define ALM_INFO_FLASH_START_ADDR   (59 << 10)                      //  56K
#define ALM_INFO_FLASH_END_ADDR     (ALM_INFO_FLASH_START_ADDR+ALM_INFO_BUF_SIZE*ALM_INFO_FLASH_MAX_CNT)
#endif

typedef struct {
    u16 OutFreq;                                                    //  ���Ƶ��
    u16 OutSpeed;                                                   //  ���ת��
    u16 OutCurrent;                                                 //  �������
    u16 OutVolt;                                                    //  �����ѹ
    u16 OutPower;                                                   //  �������
    u16 LineVolt;                                                   //  ĸ�ߵ�ѹ
    u16 AlarmVal;                                                   //  ��������
    /*************����Ϊ�ѹ滮***********************/

    u16 A_Current;                                                  //  A�����
    u16 B_Current;                                                  //  B�����
    u16 C_Current;                                                  //  C�����
    u16 RateCurrent;

    u32 Power_Consume_Total;                                        //�����ۼ��õ�
    u32 Pfreq_Power_Consume_Total;                                    //��Ƶ�����ۼ��õ�
    u16 Cfreq_Press;                                                //��Ƶѹ��
    
}MAIN_STAT_STRUCT;

typedef struct {
    u16 OutFreq;                                                    //  ���Ƶ��
    u16 OutSpeed;                                                   //  ���ת��
    u16 OutCurrent;                                                 //  �������
    u16 OutVolt;                                                    //  �����ѹ
    u16 OutPower;                                                   //  �������
    u16 LineVolt;                                                   //  ĸ�ߵ�ѹ
    u16 AlarmVal;                                                   //  ��������
    /*************����Ϊ�ѹ滮***********************/

    u16 A_Current;                                                  //  A�����
    u16 B_Current;                                                  //  B�����
    u16 C_Current;                                                  //  C�����

    u16 StartTemp;
    u16 StopTemp;
    u16 RateCurrent;
    u16 PreAlmTemp;
    u16 AlmTemp;

    u32 Power_Consume_Total;                                        //����ۼ��õ�
    u32 Pfreq_Power_Consume_Total;                                    //��Ƶ����ۼ��õ�
    u16 Cfreq_Temp;                                                    //�����Ƶ�¶�
    
}FAN_STAT_STRUCT;

typedef struct {
    u16 MaxOilFiltHour;                                                //  ������ʹ��ʱ��
    u16 MaxOilSepaHour;                                                //  �ͷ���ʹ��ʱ��
    u16 MaxGasFiltHour;                                                //  ������ʹ��ʱ��
    u16 MaxlubeHour;                                                   //  ����ʹ��ʱ��
    u16 MaxGreaseHour; 
    u16 MaxUseHour;
    u16 MaxWaterFiltHour;												//ˮ����ʱ��
}MAX_HOURS_STRUCT;

typedef struct {
    SHARE_STAT_STRUCT   Share;
    RUN_HOUR_STRUCT     Run;
    
    MAIN_STAT_STRUCT    Main;
    FAN_STAT_STRUCT     Fan;
    
    VFD_PARA_STRUCT        VFD;
    
    XY_METER_STAT_STRUCT MeterData;
    
    FLOW_METER_STAT_STRUCT FlowData;
    
    MAM6080_OTHER_PARA_STRUCT OtherData;
    
}SYS_STAT_BUF_STRUCT;

#define SYS_STAT_BUF_SIZE           sizeof(SYS_STAT_BUF_STRUCT)

ParaExt u16 SysStatBuf[SYS_STAT_BUF_SIZE >> 1];
ParaExt SYS_STAT_BUF_STRUCT *GpSysStat;


typedef enum {
    PARA_TYPE_SYS = 1,                                              //  ϵͳ����
    PARA_TYPE_USR,
    PARA_TYPE_ALM,
    PARA_TYPE_OHT,
}PARA_TYPE_ENUM;    


typedef struct {
    struct {
        u16 SysPara:  1;                                            //  ϵͳ�����洢��ʶ
        u16 UsrPara:  1;                                            //  �û������洢��ʶ
//        u16 NetPara:  1;                                            //  ��������洢��ʶ
        u16 AlmInfo:  1;                                            //  ������Ϣ�洢��ʶ
        u16 OthInfo:  1;                                            //  ������Ϣ�洢��ʶ
    }Bits;
    
    u32 SysParaAddr;
    u32 UsrParaAddr;
//    u32 NetParaAddr;
    u32 AlmInfoAddr;
    u32 OthInfoAddr;
}PARA_FAT_STRUCT;
ParaExt PARA_FAT_STRUCT    ParaFat;


ParaExt char    crc8Cal(char* pBuf, short Len);
ParaExt void    SysParaDefaultSet(char bInitID);
ParaExt void    SysParaRead(void);
ParaExt void    SysParaStorage(void);
ParaExt char    SysVarRead(u16 ReadAddr, char *pBuf, u16 *pLen, ADDR_TYPE_ENUM *pType);
ParaExt char    SysVarWrite(u16 WriteAddr, u16 WriteLen, char *pBuf, u16 *pWriteLen);
ParaExt char    FieldValOffset(u16 BgnAddr, u16 EndAddr, u16 Addr, u16 *pOffset);
ParaExt void    DevParaWrite(PARA_TYPE_ENUM ParaTyp);
ParaExt void    SysVarInit(void);
ParaExt ADDR_JSON_ENUM  AddrJsonType(u16 Addr, u8* point_len);
ParaExt char   SpecAddrRead(u16 ReadAddr, char *pBuf, u16 *pLen, ADDR_TYPE_ENUM *pType);
#endif

