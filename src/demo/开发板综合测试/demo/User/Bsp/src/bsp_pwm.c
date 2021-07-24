/*
*********************************************************************************************************
*
*	��������: PWM����ģ��
*	�ļ�����: bsp_pwm.c
*	��    ��: v1.0.0
*   ��    д: Ī����
*	˵    ��: 
*
*	�汾��¼: 
*	v1.0.0: 2021��4��8�գ�����
*
*	Copyright (C), 2021-2031, �����з����ƿƼ����޹�˾ https://fxymcu.taobao.com
*
*********************************************************************************************************
*/

#include "bsp.h"

/* �������Ŷ���-TMR3_CH3 */
#define T3_PWM3_PORT GPIOB
#define T3_PWM3_PIN GPIO_Pins_0
#define T3_PWM3_SOURCE GPIO_PinsSource0
#define T3_PWM3_AF GPIO_AF_1
#define T3_PWM3_GPIO_RCC RCC_AHBPERIPH_GPIOB

/* ���� */
#define CYCLE 100

/*
*********************************************************************************************************
*	�� �� ��: bsp_InitTMR3_PWM3
*	����˵��: ����TMR3ͨ��3��PWM��ص�GPIO
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_InitTMR3_PWM3(uint8_t ucPwmDuty)
{
    GPIO_InitType GPIO_InitStructure;
    TMR_TimerBaseInitType TMR_TMReBaseStructure;
    TMR_OCInitType TMR_OCInitStructure;

    /* ʹ��GPIOʱ�� */
    RCC_AHBPeriphClockCmd(T3_PWM3_GPIO_RCC, ENABLE);
    /* ʹ��TMR3ʱ�� */
    RCC_APB1PeriphClockCmd(RCC_APB1PERIPH_TMR3, ENABLE);

    /* ����GPIO */
    GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pins = T3_PWM3_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; /* ����ģʽ */
	GPIO_InitStructure.GPIO_OutType = GPIO_OutType_PP;
    GPIO_InitStructure.GPIO_MaxSpeed = GPIO_MaxSpeed_50MHz;
    GPIO_Init(T3_PWM3_PORT, &GPIO_InitStructure);

    /* ���ø��ù��� */
    GPIO_PinAFConfig(T3_PWM3_PORT, T3_PWM3_SOURCE, T3_PWM3_AF);

    /* ����TMR3 */
    TMR_TimeBaseStructInit(&TMR_TMReBaseStructure);
    TMR_TMReBaseStructure.TMR_Period = CYCLE - 1;              /* PWM���� */
    TMR_TMReBaseStructure.TMR_DIV = 120 - 1;                   /* 120��Ƶ����1MHz */
    TMR_TMReBaseStructure.TMR_ClockDivision = 0;               /* ʱ�ӳ�Ƶ */
    TMR_TMReBaseStructure.TMR_CounterMode = TMR_CounterDIR_Up; /* ���ϼ��� */
    TMR_TimeBaseInit(TMR3, &TMR_TMReBaseStructure);

    /* ����PWM��TMR3_CH3��� */
    TMR_OCStructInit(&TMR_OCInitStructure);
    TMR_OCInitStructure.TMR_OCMode = TMR_OCMode_PWM1;                           /* PWMģʽ */
    TMR_OCInitStructure.TMR_OutputState = TMR_OutputState_Enable;               /* ʹ����� */
    TMR_OCInitStructure.TMR_Pulse = (uint32_t)((float)ucPwmDuty / 100 * CYCLE); /* PWMռ�ձ� */
    TMR_OCInitStructure.TMR_OCPolarity = TMR_OCPolarity_High;                   /* �ߵ�ƽ��Ч */
    TMR_OC3Init(TMR3, &TMR_OCInitStructure);

    /* ʹ�ܶ�ʱ��TIM3��CCM�ϵ�����ֵ */
    TMR_OC3PreloadConfig(TMR3, TMR_OCPreload_Enable);
    /* ʹ��TIM3��AR�ϵ�Ԥװ�ؼĴ��� */
    TMR_ARPreloadConfig(TMR3, ENABLE);

    /* ʹ��TMR3 */
    TMR_Cmd(TMR3, ENABLE);
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_TMR3_PWM3_SetDuty
*	����˵��: ����PWMռ�ձ�
*	��    ��: ucDuty: 0-100%
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_TMR3_PWM3_SetDuty(uint8_t ucDuty)
{
    TMR_SetCompare3(TMR3, (uint32_t)((float)ucDuty / 100 * CYCLE));
}

/********************************************* End of file **********************************************/
