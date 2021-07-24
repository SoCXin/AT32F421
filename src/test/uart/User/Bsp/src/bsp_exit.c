/*
*********************************************************************************************************
*
*	��������: �ⲿ�ж�����ģ��
*	�ļ�����: bsp_exit.c
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

/* �������Ŷ��� */
#define EXIT_PORT GPIOC
#define EXIT_PIN GPIO_Pins_13
#define EXIT_GPIO_RCC RCC_AHBPERIPH_GPIOC
#define EXIT_PORT_SOURCE GPIO_PortSourceGPIOC
#define EXIT_PIN_SOURCE GPIO_PinsSource13
#define EXIT_LINE EXTI_Line13
#define EXIT_IRQ EXTI15_4_IRQn

/* ����������״̬ */
#define EXIT_READ() (EXIT_PORT->IPTDT & EXIT_PIN)

/*
*********************************************************************************************************
*	�� �� ��: bsp_InitEXTI
*	����˵��: ��ʼ���ⲿ�ж�
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_InitEXTI(void)
{
    GPIO_InitType GPIO_InitStructure;
    EXTI_InitType EXTI_InitStructure;
    NVIC_InitType NVIC_InitStructure;

    /* ʹ��GPIOʱ�� */
    RCC_AHBPeriphClockCmd(EXIT_GPIO_RCC, ENABLE);
    /* ʹ��SYSCFGʱ�� */
    RCC_APB2PeriphClockCmd(RCC_APB2PERIPH_SYSCFGCOMP, ENABLE);

    /* ����GPIO */
    GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pins = EXIT_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_Pull = GPIO_Pull_NOPULL; /* �������룬���������� */
    GPIO_Init(EXIT_PORT, &GPIO_InitStructure);

    /* �����ⲿ�ж�����Դ */
    SYSCFG_EXTILineConfig(EXIT_PORT_SOURCE, EXIT_PIN_SOURCE);

    EXTI_StructInit(&EXTI_InitStructure);
    EXTI_InitStructure.EXTI_Line = EXIT_LINE;               /* �ж��� */
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;     /* �ж�ģʽ */
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; /* �½��ش��� */
    EXTI_InitStructure.EXTI_LineEnable = ENABLE;            /* ʹ�� */
    EXTI_Init(&EXTI_InitStructure);

    /* ʹ��EXTIȫ���ж� */
    NVIC_InitStructure.NVIC_IRQChannel = EXIT_IRQ;            /* IRQͨ�� */
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3; /* ��ռ���ȼ��� */
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;        /* �����ȼ��� */
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;           /* ʹ��IRQͨ�� */
    NVIC_Init(&NVIC_InitStructure);
}

/*
*********************************************************************************************************
*	�� �� ��: EXTI15_4_IRQHandler
*	����˵��: �ⲿ�ж�4-15
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void EXTI15_4_IRQHandler(void)
{
    if (EXTI_GetIntStatus(EXTI_Line13) != RESET)
    {
        EXTI_ClearIntPendingBit(EXTI_Line13);
        if (!EXIT_READ())
        {
            printf("->�ⲿ�ж��¼�\r\n");
            bsp_LED_Toggle();
        }
    }
}

/********************************************* End of file **********************************************/
