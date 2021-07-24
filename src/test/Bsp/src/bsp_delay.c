/*
*********************************************************************************************************
*
*	程序名称: 系统延时模块
*	文件名称: bsp_delay.c
*	版    本: v1.0.0
*   编    写: 莫利奖
*	说    明: 使用ARM内核SysTick定时器进行延时
*
*	版本记录: 
*	v1.0.0: 2021年3月20日，初版
*
*	Copyright (C), 2021-2031, 深圳市风巽云科技有限公司 https://fxymcu.taobao.com
*
*********************************************************************************************************
*/

#include "bsp.h"

/* 延时计数值 */
static __IO float fUsValueCount;
static __IO float fMsValueCount;

/*
*********************************************************************************************************
*	函 数 名: bsp_InitSysTick
*	功能说明: 初始化SysTick
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_InitSysTick(void)
{
    /* 配置SysTick时钟-8分频 */
    SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
    /* 1us所需的计数值 */
    fUsValueCount = (float)SystemCoreClock / (8 * 1000000);
    /* 1ms所需的计数值 */
    fMsValueCount = fUsValueCount * 1000;
}

/*
*********************************************************************************************************
*	函 数 bsp_DelayUsUs
*	功能说明: us延时
*	形    参: ulNus: 延时值，注意最大延时值为 2^24 / fUsValueCount)
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_DelayUs(uint32_t ulNus)
{
    uint32_t ulStatus;

    SysTick->LOAD = (uint32_t)(ulNus * fUsValueCount);
    SysTick->VAL = 0x00;
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
    do
    {
        ulStatus = SysTick->CTRL;
    } while ((ulStatus & 0x01) && !(ulStatus & (1 << 16)));

    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
    SysTick->VAL = 0X00;
}

/*
*********************************************************************************************************
*	函 数 bsp_DelayMsMs
*	功能说明: ms延时
*	形    参: ulNms: 毫秒延时值
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_DelayMs(uint32_t ulNms)
{
    uint32_t ulStatus;

    while (ulNms)
    {
        SysTick->LOAD = (uint32_t)(1000 * fUsValueCount); /* 定时1000us */
        SysTick->VAL = 0x00;
        SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
        do
        {
            ulStatus = SysTick->CTRL;
        } while ((ulStatus & 0x01) && !(ulStatus & (1 << 16)));

        SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
        SysTick->VAL = 0X00;
        ulNms--;
    }
}

/********************************************* End of file **********************************************/
