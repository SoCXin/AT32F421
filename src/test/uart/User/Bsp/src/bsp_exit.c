/*
*********************************************************************************************************
*
*	程序名称: 外部中断驱动模块
*	文件名称: bsp_exit.c
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

/* 驱动引脚定义 */
#define EXIT_PORT GPIOC
#define EXIT_PIN GPIO_Pins_13
#define EXIT_GPIO_RCC RCC_AHBPERIPH_GPIOC
#define EXIT_PORT_SOURCE GPIO_PortSourceGPIOC
#define EXIT_PIN_SOURCE GPIO_PinsSource13
#define EXIT_LINE EXTI_Line13
#define EXIT_IRQ EXTI15_4_IRQn

/* 读按键口线状态 */
#define EXIT_READ() (EXIT_PORT->IPTDT & EXIT_PIN)

/*
*********************************************************************************************************
*	函 数 名: bsp_InitEXTI
*	功能说明: 初始化外部中断
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_InitEXTI(void)
{
    GPIO_InitType GPIO_InitStructure;
    EXTI_InitType EXTI_InitStructure;
    NVIC_InitType NVIC_InitStructure;

    /* 使能GPIO时钟 */
    RCC_AHBPeriphClockCmd(EXIT_GPIO_RCC, ENABLE);
    /* 使能SYSCFG时钟 */
    RCC_APB2PeriphClockCmd(RCC_APB2PERIPH_SYSCFGCOMP, ENABLE);

    /* 配置GPIO */
    GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pins = EXIT_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_Pull = GPIO_Pull_NOPULL; /* 浮空输入，已外置上拉 */
    GPIO_Init(EXIT_PORT, &GPIO_InitStructure);

    /* 配置外部中断输入源 */
    SYSCFG_EXTILineConfig(EXIT_PORT_SOURCE, EXIT_PIN_SOURCE);

    EXTI_StructInit(&EXTI_InitStructure);
    EXTI_InitStructure.EXTI_Line = EXIT_LINE;               /* 中断线 */
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;     /* 中断模式 */
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; /* 下降沿触发 */
    EXTI_InitStructure.EXTI_LineEnable = ENABLE;            /* 使能 */
    EXTI_Init(&EXTI_InitStructure);

    /* 使能EXTI全局中断 */
    NVIC_InitStructure.NVIC_IRQChannel = EXIT_IRQ;            /* IRQ通道 */
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3; /* 抢占优先级别 */
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;        /* 从优先级别 */
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;           /* 使能IRQ通道 */
    NVIC_Init(&NVIC_InitStructure);
}

/*
*********************************************************************************************************
*	函 数 名: EXTI15_4_IRQHandler
*	功能说明: 外部中断4-15
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void EXTI15_4_IRQHandler(void)
{
    if (EXTI_GetIntStatus(EXTI_Line13) != RESET)
    {
        EXTI_ClearIntPendingBit(EXTI_Line13);
        if (!EXIT_READ())
        {
            printf("->外部中断事件\r\n");
            bsp_LED_Toggle();
        }
    }
}

/********************************************* End of file **********************************************/
