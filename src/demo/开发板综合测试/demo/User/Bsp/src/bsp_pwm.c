/*
*********************************************************************************************************
*
*	程序名称: PWM驱动模块
*	文件名称: bsp_pwm.c
*	版    本: v1.0.0
*   编    写: 莫利奖
*	说    明: 
*
*	版本记录: 
*	v1.0.0: 2021年4月8日，初版
*
*	Copyright (C), 2021-2031, 深圳市风巽云科技有限公司 https://fxymcu.taobao.com
*
*********************************************************************************************************
*/

#include "bsp.h"

/* 驱动引脚定义-TMR3_CH3 */
#define T3_PWM3_PORT GPIOB
#define T3_PWM3_PIN GPIO_Pins_0
#define T3_PWM3_SOURCE GPIO_PinsSource0
#define T3_PWM3_AF GPIO_AF_1
#define T3_PWM3_GPIO_RCC RCC_AHBPERIPH_GPIOB

/* 周期 */
#define CYCLE 100

/*
*********************************************************************************************************
*	函 数 名: bsp_InitTMR3_PWM3
*	功能说明: 配置TMR3通道3的PWM相关的GPIO
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_InitTMR3_PWM3(uint8_t ucPwmDuty)
{
    GPIO_InitType GPIO_InitStructure;
    TMR_TimerBaseInitType TMR_TMReBaseStructure;
    TMR_OCInitType TMR_OCInitStructure;

    /* 使能GPIO时钟 */
    RCC_AHBPeriphClockCmd(T3_PWM3_GPIO_RCC, ENABLE);
    /* 使能TMR3时钟 */
    RCC_APB1PeriphClockCmd(RCC_APB1PERIPH_TMR3, ENABLE);

    /* 配置GPIO */
    GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pins = T3_PWM3_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; /* 复用模式 */
	GPIO_InitStructure.GPIO_OutType = GPIO_OutType_PP;
    GPIO_InitStructure.GPIO_MaxSpeed = GPIO_MaxSpeed_50MHz;
    GPIO_Init(T3_PWM3_PORT, &GPIO_InitStructure);

    /* 配置复用功能 */
    GPIO_PinAFConfig(T3_PWM3_PORT, T3_PWM3_SOURCE, T3_PWM3_AF);

    /* 配置TMR3 */
    TMR_TimeBaseStructInit(&TMR_TMReBaseStructure);
    TMR_TMReBaseStructure.TMR_Period = CYCLE - 1;              /* PWM周期 */
    TMR_TMReBaseStructure.TMR_DIV = 120 - 1;                   /* 120分频，即1MHz */
    TMR_TMReBaseStructure.TMR_ClockDivision = 0;               /* 时钟除频 */
    TMR_TMReBaseStructure.TMR_CounterMode = TMR_CounterDIR_Up; /* 向上计数 */
    TMR_TimeBaseInit(TMR3, &TMR_TMReBaseStructure);

    /* 配置PWM从TMR3_CH3输出 */
    TMR_OCStructInit(&TMR_OCInitStructure);
    TMR_OCInitStructure.TMR_OCMode = TMR_OCMode_PWM1;                           /* PWM模式 */
    TMR_OCInitStructure.TMR_OutputState = TMR_OutputState_Enable;               /* 使能输出 */
    TMR_OCInitStructure.TMR_Pulse = (uint32_t)((float)ucPwmDuty / 100 * CYCLE); /* PWM占空比 */
    TMR_OCInitStructure.TMR_OCPolarity = TMR_OCPolarity_High;                   /* 高电平有效 */
    TMR_OC3Init(TMR3, &TMR_OCInitStructure);

    /* 使能定时器TIM3在CCM上的周期值 */
    TMR_OC3PreloadConfig(TMR3, TMR_OCPreload_Enable);
    /* 使能TIM3在AR上的预装载寄存器 */
    TMR_ARPreloadConfig(TMR3, ENABLE);

    /* 使能TMR3 */
    TMR_Cmd(TMR3, ENABLE);
}

/*
*********************************************************************************************************
*	函 数 名: bsp_TMR3_PWM3_SetDuty
*	功能说明: 设置PWM占空比
*	形    参: ucDuty: 0-100%
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_TMR3_PWM3_SetDuty(uint8_t ucDuty)
{
    TMR_SetCompare3(TMR3, (uint32_t)((float)ucDuty / 100 * CYCLE));
}

/********************************************* End of file **********************************************/
