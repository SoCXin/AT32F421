/*
*********************************************************************************************************
*
*	程序名称: 串口驱动模块
*	文件名称: bsp_usart.c
*	版    本: v1.0.0
*   编    写: 莫利奖
*	说    明: 
*
*	版本记录: 
*	v1.0.0: 2021年4月9日，初版
*
*	Copyright (C), 2021-2031, 深圳市风巽云科技有限公司 https://fxymcu.taobao.com
*
*********************************************************************************************************
*/

#include "bsp.h"

/* 串口1的GPIO分布
TXD: PA9 PB6
RXD: PA10 PB7
*/
#define USART1_TX_PORT GPIOA
#define USART1_TX_PIN GPIO_Pins_9
#define USART1_TX_SOURCE GPIO_PinsSource9
#define USART1_TX_AF GPIO_AF_1

#define USART1_RX_PORT GPIOA
#define USART1_RX_PIN GPIO_Pins_10
#define USART1_RX_SOURCE GPIO_PinsSource10
#define USART1_RX_AF GPIO_AF_1

#define USART1_GPIO_RCC RCC_AHBPERIPH_GPIOA

/* 串口2的GPIO分布
TXD: PA2 PA14
RXD: PA3 PA15 PB0
*/
#define USART2_TX_PORT GPIOA
#define USART2_TX_PIN GPIO_Pins_2
#define USART2_TX_SOURCE GPIO_PinsSource2
#define USART2_TX_AF GPIO_AF_1

#define USART2_RX_PORT GPIOA
#define USART2_RX_PIN GPIO_Pins_3
#define USART2_RX_SOURCE GPIO_PinsSource3
#define USART2_RX_AF GPIO_AF_1

#define USART2_GPIO_RCC RCC_AHBPERIPH_GPIOA

/*
*********************************************************************************************************
*	函 数 名: bsp_InitHardUART
*	功能说明: 配置串口的硬件参数
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
static void bsp_InitHardUART(void)
{
    GPIO_InitType GPIO_InitStructure;
    USART_InitType USART_InitStructure;

#if USART1_EN
    /* 使能GPIO时钟 */
    RCC_AHBPeriphClockCmd(USART1_GPIO_RCC, ENABLE);
    /* 使能USART1时钟 */
    RCC_APB2PeriphClockCmd(RCC_APB2PERIPH_USART1, ENABLE);

    /* 配置GPIO */
    GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; /* 复用功能 */
    GPIO_InitStructure.GPIO_MaxSpeed = GPIO_MaxSpeed_50MHz;
    /* 配置TX */
    GPIO_InitStructure.GPIO_Pins = USART1_TX_PIN;
    GPIO_InitStructure.GPIO_OutType = GPIO_OutType_PP;
    GPIO_Init(USART1_TX_PORT, &GPIO_InitStructure);
    /* 配置RX */
    GPIO_InitStructure.GPIO_Pins = USART1_RX_PIN;
    GPIO_InitStructure.GPIO_Pull = GPIO_Pull_PU;
    GPIO_Init(USART1_RX_PORT, &GPIO_InitStructure);

    /* 配置复用功能 */
    GPIO_PinAFConfig(USART1_TX_PORT, USART1_TX_SOURCE, USART1_TX_AF);
    GPIO_PinAFConfig(USART1_RX_PORT, USART1_RX_SOURCE, USART1_RX_AF);

    /* 配置参数 */
    USART_InitStructure.USART_BaudRate = USART1_BAUD;                               /* 波特率 */
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;                     /* 数据位 */
    USART_InitStructure.USART_StopBits = USART_StopBits_1;                          /* 停止位 */
    USART_InitStructure.USART_Parity = USART_Parity_No;                             /* 检验位 */
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; /* 硬件流控 */
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;                 /* 模式 */
    USART_Init(USART1, &USART_InitStructure);
    /* 使能串口1接收中断 */
    USART_INTConfig(USART1, USART_INT_RDNE, ENABLE);
    /* 使能串口 */
    USART_Cmd(USART1, ENABLE);
#endif

#if USART2_EN
    /* 使能GPIO时钟 */
    RCC_AHBPeriphClockCmd(USART2_GPIO_RCC, ENABLE);
    /* 使能USART2时钟 */
    RCC_APB1PeriphClockCmd(RCC_APB1PERIPH_USART2, ENABLE);

    /* 配置GPIO */
    GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; /* 复用功能 */
    GPIO_InitStructure.GPIO_MaxSpeed = GPIO_MaxSpeed_50MHz;
    /* 配置TX */
    GPIO_InitStructure.GPIO_Pins = USART2_TX_PIN;
    GPIO_InitStructure.GPIO_OutType = GPIO_OutType_PP;
    GPIO_Init(USART2_TX_PORT, &GPIO_InitStructure);
    /* 配置RX */
    GPIO_InitStructure.GPIO_Pins = USART2_RX_PIN;
    GPIO_InitStructure.GPIO_Pull = GPIO_Pull_PU;
    GPIO_Init(USART2_RX_PORT, &GPIO_InitStructure);

    /* 配置复用功能 */
    GPIO_PinAFConfig(USART2_TX_PORT, USART2_TX_SOURCE, USART2_TX_AF);
    GPIO_PinAFConfig(USART2_RX_PORT, USART2_RX_SOURCE, USART2_RX_AF);

    /* 配置参数 */
    USART_InitStructure.USART_BaudRate = USART2_BAUD;                               /* 波特率 */
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;                     /* 数据位 */
    USART_InitStructure.USART_StopBits = USART_StopBits_1;                          /* 停止位 */
    USART_InitStructure.USART_Parity = USART_Parity_No;                             /* 检验位 */
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; /* 硬件流控 */
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;                 /* 模式 */
    USART_Init(USART2, &USART_InitStructure);
    /* 使能串口1接收中断 */
    USART_INTConfig(USART2, USART_INT_RDNE, ENABLE);
    /* 使能串口 */
    USART_Cmd(USART2, ENABLE);
#endif
}

/*
*********************************************************************************************************
*	函 数 名: bsp_UART_ConfigNVIC
*	功能说明: 配置串口硬件中断.
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
static void bsp_UART_ConfigNVIC(void)
{
    NVIC_InitType NVIC_InitStructure;

#if USART1_EN
    /* 使能USART1中断 */
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;         /* IRQ通道 */
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; /* 抢占优先级别 */
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;        /* 从优先级别 */
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;           /* 使能IRQ通道 */
    NVIC_Init(&NVIC_InitStructure);
#endif

#if USART2_EN
    /* 使能USART2中断 */
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;         /* IRQ通道 */
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; /* 抢占优先级别 */
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;        /* 从优先级别 */
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;           /* 使能IRQ通道 */
    NVIC_Init(&NVIC_InitStructure);
#endif
}

/*
*********************************************************************************************************
*	函 数 名: bsp_UART_SetBaudrate
*	功能说明: 设置串口波特率，适用于数据8位，1个停止位，无校验位，无硬件流控
*	形    参: USARTx.串口 ulValue.波特率
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_UART_SetBaudrate(USART_Type *USARTx, uint32_t ulValue)
{
    USART_InitType USART_InitStructure;

    USART_InitStructure.USART_BaudRate = ulValue;                                   /* 波特率 */
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;                     /* 数据位 */
    USART_InitStructure.USART_StopBits = USART_StopBits_1;                          /* 停止位 */
    USART_InitStructure.USART_Parity = USART_Parity_No;                             /* 检验位 */
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; /* 硬件流控 */
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;                 /* 模式 */
    USART_Init(USARTx, &USART_InitStructure);
}

/*
*********************************************************************************************************
*	函 数 名: bsp_UART_SendByte
*	功能说明: 串口发送一个字节
*	形    参: USARTx: 串口号=USART1，USART2 ucByte: 数据
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_UART_SendByte(USART_Type *USARTx, uint8_t ucByte)
{
    /* 发送数据 */
    USART_SendData(USARTx, ucByte);
    /* 等待发送结束 */
    while (USART_GetFlagStatus(USART1, USART_FLAG_TRAC) == RESET)
    {
    }
}

/*
*********************************************************************************************************
*	函 数 名: bsp_InitUART
*	功能说明: 串口初始化
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_InitUART(void)
{
    bsp_InitHardUART();
    bsp_UART_ConfigNVIC();
}

/*
*********************************************************************************************************
*	函 数 名: USART1_IRQHandler USART2_IRQHandler
*	功能说明: USART中断服务程序
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
#if USART1_EN
void USART1_IRQHandler(void)
{
    volatile uint8_t ucData = 0; /* 禁止优化，有利于在线仿真观察收到的数据 */

    if (USART_GetITStatus(USART1, USART_INT_RDNE) != RESET)
    {
        /* 清除接收中断标志 */
        USART_ClearFlag(USART1, USART_FLAG_RDNE);
        /* 读取数据 */
        ucData = USART_ReceiveData(USART1);
    }
}
#endif

#if USART2_EN
void USART2_IRQHandler(void)
{
    volatile uint8_t ucData = 0; /* 禁止优化，有利于在线仿真观察收到的数据 */

    if (USART_GetITStatus(USART2, USART_INT_RDNE) != RESET)
    {
        /* 清除接收中断标志 */
        USART_ClearFlag(USART1, USART_FLAG_RDNE);
        /* 读取数据 */
        ucData = USART_ReceiveData(USART2);
    }
}
#endif

/*
*********************************************************************************************************
*	函 数 名: fputc
*	功能说明: 重定义putc函数，可以使用printf函数从串口1打印输出
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
int fputc(int ch, FILE *f)
{
    /* 发送数据 */
    USART_SendData(USART1, (uint16_t)ch);
    /* 等待发送结束 */
    while (USART_GetFlagStatus(USART1, USART_FLAG_TRAC) == RESET)
    {
    }

    return ch;
}

/*
*********************************************************************************************************
*	函 数 名: fgetc
*	功能说明: 重定义getc函数，可以使用getchar函数从串口1输入数据
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
int fgetc(FILE *f)
{
    /* 等待接收到数据 */
    while (USART_GetFlagStatus(USART1, USART_FLAG_RDNE) == RESET)
    {
    }

    return (int)USART_ReceiveData(USART1);
}

/********************************************* End of file **********************************************/
