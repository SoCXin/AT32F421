/*
*********************************************************************************************************
*
*	��������: ��������ģ��
*	�ļ�����: bsp_usart.c
*	��    ��: v1.0.0
*   ��    д: Ī����
*	˵    ��: 
*
*	�汾��¼: 
*	v1.0.0: 2021��4��9�գ�����
*
*	Copyright (C), 2021-2031, �����з����ƿƼ����޹�˾ https://fxymcu.taobao.com
*
*********************************************************************************************************
*/

#include "bsp.h"

/* ����1��GPIO�ֲ�
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

/* ����2��GPIO�ֲ�
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
*	�� �� ��: bsp_InitHardUART
*	����˵��: ���ô��ڵ�Ӳ������
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void bsp_InitHardUART(void)
{
    GPIO_InitType GPIO_InitStructure;
    USART_InitType USART_InitStructure;

#if USART1_EN
    /* ʹ��GPIOʱ�� */
    RCC_AHBPeriphClockCmd(USART1_GPIO_RCC, ENABLE);
    /* ʹ��USART1ʱ�� */
    RCC_APB2PeriphClockCmd(RCC_APB2PERIPH_USART1, ENABLE);

    /* ����GPIO */
    GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; /* ���ù��� */
    GPIO_InitStructure.GPIO_MaxSpeed = GPIO_MaxSpeed_50MHz;
    /* ����TX */
    GPIO_InitStructure.GPIO_Pins = USART1_TX_PIN;
    GPIO_InitStructure.GPIO_OutType = GPIO_OutType_PP;
    GPIO_Init(USART1_TX_PORT, &GPIO_InitStructure);
    /* ����RX */
    GPIO_InitStructure.GPIO_Pins = USART1_RX_PIN;
    GPIO_InitStructure.GPIO_Pull = GPIO_Pull_PU;
    GPIO_Init(USART1_RX_PORT, &GPIO_InitStructure);

    /* ���ø��ù��� */
    GPIO_PinAFConfig(USART1_TX_PORT, USART1_TX_SOURCE, USART1_TX_AF);
    GPIO_PinAFConfig(USART1_RX_PORT, USART1_RX_SOURCE, USART1_RX_AF);

    /* ���ò��� */
    USART_InitStructure.USART_BaudRate = USART1_BAUD;                               /* ������ */
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;                     /* ����λ */
    USART_InitStructure.USART_StopBits = USART_StopBits_1;                          /* ֹͣλ */
    USART_InitStructure.USART_Parity = USART_Parity_No;                             /* ����λ */
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; /* Ӳ������ */
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;                 /* ģʽ */
    USART_Init(USART1, &USART_InitStructure);
    /* ʹ�ܴ���1�����ж� */
    USART_INTConfig(USART1, USART_INT_RDNE, ENABLE);
    /* ʹ�ܴ��� */
    USART_Cmd(USART1, ENABLE);
#endif

#if USART2_EN
    /* ʹ��GPIOʱ�� */
    RCC_AHBPeriphClockCmd(USART2_GPIO_RCC, ENABLE);
    /* ʹ��USART2ʱ�� */
    RCC_APB1PeriphClockCmd(RCC_APB1PERIPH_USART2, ENABLE);

    /* ����GPIO */
    GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; /* ���ù��� */
    GPIO_InitStructure.GPIO_MaxSpeed = GPIO_MaxSpeed_50MHz;
    /* ����TX */
    GPIO_InitStructure.GPIO_Pins = USART2_TX_PIN;
    GPIO_InitStructure.GPIO_OutType = GPIO_OutType_PP;
    GPIO_Init(USART2_TX_PORT, &GPIO_InitStructure);
    /* ����RX */
    GPIO_InitStructure.GPIO_Pins = USART2_RX_PIN;
    GPIO_InitStructure.GPIO_Pull = GPIO_Pull_PU;
    GPIO_Init(USART2_RX_PORT, &GPIO_InitStructure);

    /* ���ø��ù��� */
    GPIO_PinAFConfig(USART2_TX_PORT, USART2_TX_SOURCE, USART2_TX_AF);
    GPIO_PinAFConfig(USART2_RX_PORT, USART2_RX_SOURCE, USART2_RX_AF);

    /* ���ò��� */
    USART_InitStructure.USART_BaudRate = USART2_BAUD;                               /* ������ */
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;                     /* ����λ */
    USART_InitStructure.USART_StopBits = USART_StopBits_1;                          /* ֹͣλ */
    USART_InitStructure.USART_Parity = USART_Parity_No;                             /* ����λ */
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; /* Ӳ������ */
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;                 /* ģʽ */
    USART_Init(USART2, &USART_InitStructure);
    /* ʹ�ܴ���1�����ж� */
    USART_INTConfig(USART2, USART_INT_RDNE, ENABLE);
    /* ʹ�ܴ��� */
    USART_Cmd(USART2, ENABLE);
#endif
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_UART_ConfigNVIC
*	����˵��: ���ô���Ӳ���ж�.
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void bsp_UART_ConfigNVIC(void)
{
    NVIC_InitType NVIC_InitStructure;

#if USART1_EN
    /* ʹ��USART1�ж� */
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;         /* IRQͨ�� */
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; /* ��ռ���ȼ��� */
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;        /* �����ȼ��� */
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;           /* ʹ��IRQͨ�� */
    NVIC_Init(&NVIC_InitStructure);
#endif

#if USART2_EN
    /* ʹ��USART2�ж� */
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;         /* IRQͨ�� */
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; /* ��ռ���ȼ��� */
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;        /* �����ȼ��� */
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;           /* ʹ��IRQͨ�� */
    NVIC_Init(&NVIC_InitStructure);
#endif
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_UART_SetBaudrate
*	����˵��: ���ô��ڲ����ʣ�����������8λ��1��ֹͣλ����У��λ����Ӳ������
*	��    ��: USARTx.���� ulValue.������
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_UART_SetBaudrate(USART_Type *USARTx, uint32_t ulValue)
{
    USART_InitType USART_InitStructure;

    USART_InitStructure.USART_BaudRate = ulValue;                                   /* ������ */
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;                     /* ����λ */
    USART_InitStructure.USART_StopBits = USART_StopBits_1;                          /* ֹͣλ */
    USART_InitStructure.USART_Parity = USART_Parity_No;                             /* ����λ */
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; /* Ӳ������ */
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;                 /* ģʽ */
    USART_Init(USARTx, &USART_InitStructure);
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_UART_SendByte
*	����˵��: ���ڷ���һ���ֽ�
*	��    ��: USARTx: ���ں�=USART1��USART2 ucByte: ����
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_UART_SendByte(USART_Type *USARTx, uint8_t ucByte)
{
    /* �������� */
    USART_SendData(USARTx, ucByte);
    /* �ȴ����ͽ��� */
    while (USART_GetFlagStatus(USART1, USART_FLAG_TRAC) == RESET)
    {
    }
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_InitUART
*	����˵��: ���ڳ�ʼ��
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_InitUART(void)
{
    bsp_InitHardUART();
    bsp_UART_ConfigNVIC();
}

/*
*********************************************************************************************************
*	�� �� ��: USART1_IRQHandler USART2_IRQHandler
*	����˵��: USART�жϷ������
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
#if USART1_EN
void USART1_IRQHandler(void)
{
    volatile uint8_t ucData = 0; /* ��ֹ�Ż������������߷���۲��յ������� */

    if (USART_GetITStatus(USART1, USART_INT_RDNE) != RESET)
    {
        /* ��������жϱ�־ */
        USART_ClearFlag(USART1, USART_FLAG_RDNE);
        /* ��ȡ���� */
        ucData = USART_ReceiveData(USART1);
    }
}
#endif

#if USART2_EN
void USART2_IRQHandler(void)
{
    volatile uint8_t ucData = 0; /* ��ֹ�Ż������������߷���۲��յ������� */

    if (USART_GetITStatus(USART2, USART_INT_RDNE) != RESET)
    {
        /* ��������жϱ�־ */
        USART_ClearFlag(USART1, USART_FLAG_RDNE);
        /* ��ȡ���� */
        ucData = USART_ReceiveData(USART2);
    }
}
#endif

/*
*********************************************************************************************************
*	�� �� ��: fputc
*	����˵��: �ض���putc����������ʹ��printf�����Ӵ���1��ӡ���
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
int fputc(int ch, FILE *f)
{
    /* �������� */
    USART_SendData(USART1, (uint16_t)ch);
    /* �ȴ����ͽ��� */
    while (USART_GetFlagStatus(USART1, USART_FLAG_TRAC) == RESET)
    {
    }

    return ch;
}

/*
*********************************************************************************************************
*	�� �� ��: fgetc
*	����˵��: �ض���getc����������ʹ��getchar�����Ӵ���1��������
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
int fgetc(FILE *f)
{
    /* �ȴ����յ����� */
    while (USART_GetFlagStatus(USART1, USART_FLAG_RDNE) == RESET)
    {
    }

    return (int)USART_ReceiveData(USART1);
}

/********************************************* End of file **********************************************/
