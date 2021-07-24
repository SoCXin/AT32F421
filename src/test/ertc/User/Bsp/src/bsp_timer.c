/*
*********************************************************************************************************
*
*	程序名称: 定时器驱动模块
*	文件名称: bsp_timer.c
*	版    本: v1.0.0
*   编    写: 莫利奖
*	说    明: 配置定时器处于定时模式
*
*	版本记录: 
*	v1.0.0: 2021年4月9日，初版
*
*	Copyright (C), 2021-2031, 深圳市风巽云科技有限公司 https://fxymcu.taobao.com
*
*********************************************************************************************************
*/

#include "bsp.h"

/* 系统时间标记 */
uint8_t ucSys10msFlag = 0;

/*
*********************************************************************************************************
*	函 数 名: bsp_InitTMR6
*	功能说明: TMR6初始化
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_InitTMR6(void)
{
    TMR_TimerBaseInitType TMR_TMReBaseStructure;
    NVIC_InitType NVIC_InitStructure;

    /* 使能TMR6时钟 */
    RCC_APB1PeriphClockCmd(RCC_APB1PERIPH_TMR6, ENABLE);

    /* 配置TMR6 */
    TMR_TimeBaseStructInit(&TMR_TMReBaseStructure);
    TMR_TMReBaseStructure.TMR_Period = 10000 - 1;              /* 自动重装载寄存器值，定时10ms */
    TMR_TMReBaseStructure.TMR_DIV = 120 - 1;                   /* 分频器120分频 TMR6时钟 = 1MHz */
    TMR_TMReBaseStructure.TMR_ClockDivision = 0;               /* TMR6时钟除频 */
    TMR_TMReBaseStructure.TMR_CounterMode = TMR_CounterDIR_Up; /* 向上计数 */
    TMR_TimeBaseInit(TMR6, &TMR_TMReBaseStructure);

    /* 清零溢出中断标志 */
    TMR_ClearITPendingBit(TMR6, TMR_INT_Overflow);
    /* 使能溢出中断 */
    TMR_INTConfig(TMR6, TMR_INT_Overflow, ENABLE);
    /* 使能TMR2 */
    TMR_Cmd(TMR6, ENABLE);

    /* 使能TMR2全局中断 */
    NVIC_InitStructure.NVIC_IRQChannel = TMR6_GLOBAL_IRQn;    /* IRQ通道 */
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; /* 抢占优先级别 */
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;        /* 从优先级别 */
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;           /* 使能IRQ通道 */
    NVIC_Init(&NVIC_InitStructure);
}

/*
*********************************************************************************************************
*	函 数 名: TMR6_GLOBAL_IRQHandler
*	功能说明: TIMR6中断
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void TMR6_GLOBAL_IRQHandler(void)
{
    if (TMR_GetINTStatus(TMR6, TMR_INT_Overflow) != RESET)
    {
        /* 清零溢出中断标志 */
        TMR_ClearITPendingBit(TMR6, TMR_INT_Overflow);
        ucSys10msFlag = 1;
    }
}

/********************************************* End of file **********************************************/
