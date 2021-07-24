/*
*********************************************************************************************************
*
*	��������: ��ʱ������ģ��
*	�ļ�����: bsp_timer.c
*	��    ��: v1.0.0
*   ��    д: Ī����
*	˵    ��: ���ö�ʱ�����ڶ�ʱģʽ
*
*	�汾��¼: 
*	v1.0.0: 2021��4��9�գ�����
*
*	Copyright (C), 2021-2031, �����з����ƿƼ����޹�˾ https://fxymcu.taobao.com
*
*********************************************************************************************************
*/

#include "bsp.h"

/* ϵͳʱ���� */
uint8_t ucSys10msFlag = 0;

/*
*********************************************************************************************************
*	�� �� ��: bsp_InitTMR6
*	����˵��: TMR6��ʼ��
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_InitTMR6(void)
{
    TMR_TimerBaseInitType TMR_TMReBaseStructure;
    NVIC_InitType NVIC_InitStructure;

    /* ʹ��TMR6ʱ�� */
    RCC_APB1PeriphClockCmd(RCC_APB1PERIPH_TMR6, ENABLE);

    /* ����TMR6 */
    TMR_TimeBaseStructInit(&TMR_TMReBaseStructure);
    TMR_TMReBaseStructure.TMR_Period = 10000 - 1;              /* �Զ���װ�ؼĴ���ֵ����ʱ10ms */
    TMR_TMReBaseStructure.TMR_DIV = 120 - 1;                   /* ��Ƶ��120��Ƶ TMR6ʱ�� = 1MHz */
    TMR_TMReBaseStructure.TMR_ClockDivision = 0;               /* TMR6ʱ�ӳ�Ƶ */
    TMR_TMReBaseStructure.TMR_CounterMode = TMR_CounterDIR_Up; /* ���ϼ��� */
    TMR_TimeBaseInit(TMR6, &TMR_TMReBaseStructure);

    /* ��������жϱ�־ */
    TMR_ClearITPendingBit(TMR6, TMR_INT_Overflow);
    /* ʹ������ж� */
    TMR_INTConfig(TMR6, TMR_INT_Overflow, ENABLE);
    /* ʹ��TMR2 */
    TMR_Cmd(TMR6, ENABLE);

    /* ʹ��TMR2ȫ���ж� */
    NVIC_InitStructure.NVIC_IRQChannel = TMR6_GLOBAL_IRQn;    /* IRQͨ�� */
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; /* ��ռ���ȼ��� */
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;        /* �����ȼ��� */
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;           /* ʹ��IRQͨ�� */
    NVIC_Init(&NVIC_InitStructure);
}

/*
*********************************************************************************************************
*	�� �� ��: TMR6_GLOBAL_IRQHandler
*	����˵��: TIMR6�ж�
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void TMR6_GLOBAL_IRQHandler(void)
{
    if (TMR_GetINTStatus(TMR6, TMR_INT_Overflow) != RESET)
    {
        /* ��������жϱ�־ */
        TMR_ClearITPendingBit(TMR6, TMR_INT_Overflow);
        ucSys10msFlag = 1;
    }
}

/********************************************* End of file **********************************************/
