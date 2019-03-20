#include <bsp.h>
//#include "os_cpu.h"
#include "app_cfg.h"
//#include "spiflash.h"
#include "app_debug.h"
#include "stm32f10x_can.h"
#include "sfud.h"

/*******************************************************************************
* Function Name  : GPIO_Configuration
* Description    : Configure GPIO Pin
* Input          : None
* Output         : None
* Return         : None
* Attention         : None
*******************************************************************************/
void GPIO_Configuration(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    //  DI
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;    
//    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
//    GPIO_InitStructure.GPIO_Pin   = KEY1_PIN;
//    GPIO_Init(KEY1_PORT, &GPIO_InitStructure);
    
    //  DO
//    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;    
//    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;    
//    GPIO_InitStructure.GPIO_Pin   = DO_JQ1_PIN;
//    GPIO_Init(DO_JQ1_PORT, &GPIO_InitStructure);
//    GPIO_InitStructure.GPIO_Pin   = DO_JQ2_PIN;
//   GPIO_Init(DO_JQ2_PORT, &GPIO_InitStructure);
//    GPIO_InitStructure.GPIO_Pin   = DO_JQ3_PIN;
//    GPIO_Init(DO_JQ3_PORT, &GPIO_InitStructure);
//    GPIO_InitStructure.GPIO_Pin   = DO_JQ4_PIN;
//    GPIO_Init(DO_JQ4_PORT, &GPIO_InitStructure);
    

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
    GPIO_InitStructure.GPIO_Pin  = RS485_0_CTL_PIN;
    GPIO_Init(RS485_0_CTL_PORT, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
    GPIO_InitStructure.GPIO_Pin  = RS485_1_CTL_PIN;
    GPIO_Init(RS485_1_CTL_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
    GPIO_InitStructure.GPIO_Pin  = RS485_2_CTL_PIN;
    GPIO_Init(RS485_2_CTL_PORT, &GPIO_InitStructure);
    //  Other
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin  = GPRS_PWRKEY_PIN;
    GPIO_Init(GPRS_PWRKEY_PORT, &GPIO_InitStructure);    
    GPIO_InitStructure.GPIO_Pin  = GPRS_POWER_PIN;
    GPIO_Init(GPRS_POWER_PORT, &GPIO_InitStructure);
    GPIO_SetBits(GPRS_POWER_PORT, GPRS_POWER_PIN);

    GPIO_InitStructure.GPIO_Pin  = GPRS_RTS_PIN;    
    GPIO_Init(GPRS_RTS_PORT, &GPIO_InitStructure);
    GPIO_ResetBits(GPRS_RTS_PORT, GPRS_RTS_PIN);
    
    GPIO_InitStructure.GPIO_Pin   = DOG_PIN;
    GPIO_Init(DOG_PORT, &GPIO_InitStructure);    

    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
    GPIO_InitStructure.GPIO_Pin  = GLED_PIN;
    GPIO_Init(GLED_PORT, &GPIO_InitStructure);
    GLED_ON();
    GLED_OFF();
    GLED_ON();
        
    GPIO_InitStructure.GPIO_Pin  = RLED_PIN;
    GPIO_Init(RLED_PORT, &GPIO_InitStructure);
    LED_ON();
    LED_OFF();
    LED_ON();

	#ifdef XLINK
	    //gprs
     GPIO_InitStructure.GPIO_Pin  = SIG3_PIN;
     GPIO_Init(GPIOB, &GPIO_InitStructure);

     GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin  = SIG1_PIN | SIG2_PIN; 
	GPIO_Init(SIG_PORT, &GPIO_InitStructure);

	NO_CSQ;
    #endif
//    GPIO_InitStructure.GPIO_Pin  = KEY1_LED_PIN;
//    GPIO_Init(KEY1_LED_PORT, &GPIO_InitStructure);   
}


void InitIWDG(void)
{
#ifdef  PRJ_RELEASE
    if(RCC_GetFlagStatus(RCC_FLAG_IWDGRST) != RESET) {
        RCC_ClearFlag();
    }  
      
    IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
    IWDG_SetPrescaler(IWDG_Prescaler_32);  
    IWDG_SetReload(1250 * 1.5);                                     //  1.5s
    IWDG_ReloadCounter();
    IWDG_Enable();
#endif  

}


void    InitUart(UART_COM_ENUM UartNum, int Baud)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    DMA_InitTypeDef DMA_InitStructure;
    
    USART_InitStructure.USART_BaudRate   = Baud;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;  
    USART_InitStructure.USART_StopBits   = USART_StopBits_1;                
    USART_InitStructure.USART_Parity     = USART_Parity_No;                   
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;  
    USART_InitStructure.USART_Mode       = USART_Mode_Rx | USART_Mode_Tx;   
    
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;    
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable; 
    
    switch(UartNum)
    {
        case COM1:  
            USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);            
            GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
            GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
            GPIO_InitStructure.GPIO_Pin   = UART1_TXD_PIN;
            GPIO_Init(UART1_TXD_PORT, &GPIO_InitStructure);            
            GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
            GPIO_InitStructure.GPIO_Pin   = UART1_RXD_PIN;
            GPIO_Init(UART1_RXD_PORT, &GPIO_InitStructure);
            USART_Init(USART1, &USART_InitStructure); 
            
#if UART1_DMA_EN
            DMA_DeInit(UART1_DMA_TX_CHL);
            DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)(&USART1->DR);
            DMA_InitStructure.DMA_Priority = DMA_Priority_Low;
            DMA_Init(UART1_DMA_TX_CHL, &DMA_InitStructure); 
            DMA_Cmd(UART1_DMA_TX_CHL, DISABLE);

            DMA_ClearFlag(UART1_DMA_TX_TC);
            DMA_ITConfig(UART1_DMA_TX_CHL, DMA_IT_TC, ENABLE);
            USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);      
#endif          
            USART_ClearITPendingBit(USART1, USART_IT_RXNE);
            USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); 

            USART_Cmd(USART1, ENABLE);
            break;

        case COM2:
            USART_ITConfig(USART2, USART_IT_RXNE, DISABLE); 
#ifdef  UART2_REMAP
            GPIO_PinRemapConfig(GPIO_Remap_USART2, ENABLE);
#endif                        
            GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
            GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
            GPIO_InitStructure.GPIO_Pin   = UART2_TXD_PIN;
            GPIO_Init(UART2_TXD_PORT, &GPIO_InitStructure);
            GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
            GPIO_InitStructure.GPIO_Pin   = UART2_RXD_PIN;
            GPIO_Init(UART2_RXD_PORT, &GPIO_InitStructure);                      
            USART_Init(USART2, &USART_InitStructure);   

#if UART2_DMA_EN
            DMA_DeInit(UART2_DMA_TX_CHL);
            DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)(&USART2->DR);
            DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
            DMA_Init(UART2_DMA_TX_CHL, &DMA_InitStructure); 
            DMA_Cmd(UART2_DMA_TX_CHL, DISABLE);

            DMA_ClearFlag(UART2_DMA_TX_TC);
            DMA_ITConfig(UART2_DMA_TX_CHL, DMA_IT_TC, ENABLE);
            USART_DMACmd(USART2, USART_DMAReq_Tx, ENABLE);      
#endif
            
            USART_ClearITPendingBit(USART2, USART_IT_RXNE);
            USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);             
            USART_Cmd(USART2, ENABLE);
            break;

        case COM3:
            USART_ITConfig(USART3, USART_IT_RXNE, DISABLE); 
                       
            GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
            GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
            GPIO_InitStructure.GPIO_Pin   = UART3_TXD_PIN;
            GPIO_Init(UART3_TXD_PORT, &GPIO_InitStructure);
            GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
            GPIO_InitStructure.GPIO_Pin   = UART3_RXD_PIN;
            GPIO_Init(UART3_RXD_PORT, &GPIO_InitStructure);                      
            USART_Init(USART3, &USART_InitStructure);   
            
            USART_ClearITPendingBit(USART3, USART_IT_RXNE);
            USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);             
            USART_Cmd(USART3, ENABLE);
            break;

        case COM4:
            USART_ITConfig(UART4, USART_IT_RXNE, DISABLE); 
                       
            GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
            GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
            GPIO_InitStructure.GPIO_Pin   = UART4_TXD_PIN;
            GPIO_Init(UART4_TXD_PORT, &GPIO_InitStructure);
            GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
            GPIO_InitStructure.GPIO_Pin   = UART4_RXD_PIN;
            GPIO_Init(UART4_RXD_PORT, &GPIO_InitStructure);                      
            USART_Init(UART4, &USART_InitStructure);    
            
            USART_ClearITPendingBit(UART4, USART_IT_RXNE);
            USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);               
            USART_Cmd(UART4, ENABLE);
            break;

 
        default:
            break;
    }
}


void    InitCan(void)
{
    NVIC_InitTypeDef    Can_NVIC_InitStructure;
    GPIO_InitTypeDef    Can_GPIO_InitStructure;

    //--------- IO�˿ڳ�ʼ�� ----------------------------
      /* Configure CAN pin: RX (PA.11) */
    Can_GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    Can_GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOA, &Can_GPIO_InitStructure);
      /* Configure CAN pin: TX (PA.12) */
    Can_GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
    Can_GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//GPIO_Speed_50MHz
    Can_GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &Can_GPIO_InitStructure);

    //----------�жϳ�ʼ�� ---------------------------------------
      /* Enable CAN RX0 interrupt IRQ channel */
    Can_NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;
    Can_NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    Can_NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;
    Can_NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&Can_NVIC_InitStructure);

      /* Enable CAN TX interrupt IRQ channel */
    Can_NVIC_InitStructure.NVIC_IRQChannel = USB_HP_CAN1_TX_IRQn;
    Can_NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    Can_NVIC_InitStructure.NVIC_IRQChannelSubPriority = 5;
    Can_NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&Can_NVIC_InitStructure);


}

void Can_Config(void)
{
    CAN_InitTypeDef CAN_InitStructure;
    RCC_ClocksTypeDef       RCC_ClocksStatus;
    CAN_FilterInitTypeDef  CAN_FilterInitStructure;


    CAN_DeInit(CAN1);
    CAN_StructInit(&CAN_InitStructure);

    //---------- ����CAN�Ĳ����� -----------------------------
    RCC_GetClocksFreq(&RCC_ClocksStatus);//���PCLK1Ƶ��

    CAN_InitStructure.CAN_TTCM = DISABLE; //ʹ��/ʧ�� ʱ�䴥��ͨѶģʽ.0:ʱ�䴥��ͨ��ģʽ�ر�;
    CAN_InitStructure.CAN_ABOM = ENABLE;  //ʹ��/ʧ�� �Զ����߹���.    1:һ����ص�128��11����������λ,�Զ��˳�����״̬;
    CAN_InitStructure.CAN_AWUM = ENABLE;  //ʹ��/ʧ�� �Զ�����ģʽ.    1:Ӳ����⵽CAN����ʱ�Զ��뿪����ģʽ;
    CAN_InitStructure.CAN_NART = DISABLE; //ʹ��/ʧ�� ���Զ��ش���ģʽ.0:CANӲ������ʧ�ܺ��һֱ�ط�ֱ�����ͳɹ�;
    CAN_InitStructure.CAN_RFLM = DISABLE; //ʹ��/ʧ�� �ܽ���FIFO����ģʽ.0:����FIFO����,��һ������ı��Ľ�����ǰһ��;
    CAN_InitStructure.CAN_TXFP = ENABLE;  //ʹ��/ʧ�� ����FIFO���ȼ�.    1:�ɷ��������˳��(ʱ���Ⱥ�˳��)���������ȼ�.
    
    CAN_InitStructure.CAN_Mode= CAN_Mode_Normal;//CAN_Mode_Normal;
    CAN_InitStructure.CAN_SJW= CAN_SJW_1tq;
    CAN_InitStructure.CAN_BS1= CAN_BS1_9tq;        //
    CAN_InitStructure.CAN_BS2= CAN_BS2_2tq;        //
    CAN_InitStructure.CAN_Prescaler=24;
    CAN_Init(CAN1, &CAN_InitStructure);  


    CAN_FilterInitStructure.CAN_FilterNumber = 0;                          //ָ���˴���ʼ���Ĺ����������ķ�Χ��1��13
    CAN_FilterInitStructure.CAN_FilterMode    = CAN_FilterMode_IdMask;    //����������ʼ��Ϊ��ʶ������λģʽ
    CAN_FilterInitStructure.CAN_FilterScale  = CAN_FilterScale_16bit;    //�����˹�����λ��

    CAN_FilterInitStructure.CAN_FilterIdHigh = 0x0000;         //������ ��ʶ��(16λλ��)
    CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x0000;     //������ ���α�ʶ��
    
    CAN_FilterInitStructure.CAN_FilterIdLow  = 0x0000;
    CAN_FilterInitStructure.CAN_FilterMaskIdLow   = 0x0000;  //���α�ʶ�� 0:���Ƚ�; 1:����ƥ��
 
    CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_FIFO0;     //ͨ�����˲�����Ϣ���������� FIFO0 ��
    CAN_FilterInitStructure.CAN_FilterActivation        = ENABLE;   //ʹ��/ʧ�� ������
    CAN_FilterInit(&CAN_FilterInitStructure);

    CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);        //
}
    

void    Init485Uart(int Baud)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
//    DMA_InitTypeDef DMA_InitStructure;
    
    USART_InitStructure.USART_BaudRate   = Baud;
    USART_InitStructure.USART_WordLength = USART_WordLength_9b;  
    USART_InitStructure.USART_StopBits   = USART_StopBits_1;                
    USART_InitStructure.USART_Parity     = USART_Parity_Even;      //PLT������ żУ��             
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;  
    USART_InitStructure.USART_Mode       = USART_Mode_Rx | USART_Mode_Tx;   
    
    USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);            
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Pin   = UART1_TXD_PIN;
    GPIO_Init(UART1_TXD_PORT, &GPIO_InitStructure);            
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Pin   = UART1_RXD_PIN;
    GPIO_Init(UART1_RXD_PORT, &GPIO_InitStructure);
    USART_Init(USART1, &USART_InitStructure); 
    

    USART_ClearITPendingBit(USART1, USART_IT_RXNE);
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); 

    USART_Cmd(USART1, ENABLE);        
}

void    InterruptConfig(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    
//    /* Configure the NVIC Preemption Priority Bits */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

    /* Enable the USARTy Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel    = USART1_IRQn;            //  VFD
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel    = USART2_IRQn;            //  GPRS  
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel    = USART3_IRQn;            //  GPRS  
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;    
    NVIC_Init(&NVIC_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel    = UART4_IRQn;            //  GPRS  
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;    
    NVIC_Init(&NVIC_InitStructure);
    
#if UART1_DMA_EN
    NVIC_InitStructure.NVIC_IRQChannel    = DMA1_Channel4_IRQn;     //  UART1
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;   
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;          
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                                        
    NVIC_Init(&NVIC_InitStructure);
#endif    

#if UART2_DMA_EN
    NVIC_InitStructure.NVIC_IRQChannel    = DMA1_Channel7_IRQn;     //  UART2
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;   
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;          
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                                        
    NVIC_Init(&NVIC_InitStructure);
#endif
    NVIC_SetPriority(SysTick_IRQn, 15);  //1111��������ȼ���1ms�ж�һ��
}

static void systick_init(void) 
{ 
    RCC_ClocksTypeDef rcc_clocks; 
    RCC_GetClocksFreq(&rcc_clocks);        
    SysTick_Config(rcc_clocks.HCLK_Frequency / 1000);  
}

void    HardWareInit(void)
{
    //FLASH_MDL_ENUM FlashMdl;

    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA |
                            RCC_APB2Periph_GPIOB |
                            RCC_APB2Periph_GPIOC |
                            RCC_APB2Periph_GPIOD |
                            RCC_APB2Periph_GPIOE |
                            RCC_APB2Periph_AFIO  |
                            RCC_APB2Periph_SPI1  |
                            RCC_APB2Periph_USART1, ENABLE);

    RCC_APB1PeriphClockCmd( RCC_APB1Periph_USART2, ENABLE);
    RCC_APB1PeriphClockCmd( RCC_APB1Periph_USART3, ENABLE);
    RCC_APB1PeriphClockCmd( RCC_APB1Periph_UART4, ENABLE);
    RCC_APB1PeriphClockCmd( RCC_APB1Periph_CAN1, ENABLE);
    RCC_AHBPeriphClockCmd(  RCC_AHBPeriph_DMA1,    ENABLE);         //  ����AHB����ʱ��
                         
    systick_init();
    
    GPIO_Configuration();      
    InitIWDG();
    
    InitUart(DebugUart, 9600); 

    RS485_0_RCV_STATUS;
    
    InitUart(GPRSUart, 115200); 

    InitUart(RS4851Uart, 9600); 
    InitUart(RS4852Uart, 9600); 
    InitCan();
    Can_Config();
    
    InterruptConfig();
	
	
 
    if (sfud_init() == SFUD_SUCCESS) {
      memcpy(DebugCheck.FlashModel, "OK ", 3); 
      memcpy(DebugCheck.FlashName,sfud_get_device_table()->chip.name, 15); 
    }
    else
    {
    	 memcpy(DebugCheck.FlashModel, "ERR", 3); 
    }

	#if 0 	
    FlashMdl = SPIFlashInit();
    if((FLASH_SST25VF016 == FlashMdl) || (FLASH_SST25VF032 == FlashMdl))
    {
        memcpy(DebugCheck.FlashModel, "OK ", 3); 
    }
    else if((FLASH_W25Q16 == FlashMdl) || (FLASH_W25Q32 == FlashMdl))
    {
        memcpy(DebugCheck.FlashModel, "OK ", 3); 
    }
    else
    {
        memcpy(DebugCheck.FlashModel, "ERR", 3); 
    }
    #endif 
    FeedDog();
}


void    FeedDog(void)
{

#ifdef  PRJ_RELEASE
    static char WdFlag = 0;
    IWDG_ReloadCounter();
    WdFlag ? DOG_HIGH() : DOG_LOW();
    WdFlag = !WdFlag;
#endif
}


