/*
*********************************************************************************************************
*
*	程序名称: 按键驱动模块
*	文件名称: bsp_key.c
*	版    本: v1.0.0
*   编    写: 莫利奖
*	说    明: 支持单键，长键以及单键连发
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
#define KEY_PORT GPIOC
#define KEY_PIN GPIO_Pins_13
#define KEY_GPIO_RCC RCC_AHBPERIPH_GPIOC
/* 读按键口线状态 */
#define KEY_READ() (KEY_PORT->IPTDT & KEY_PIN)

/*
*********************************************************************************************************
*	函 数 名: bsp_InitKEY
*	功能说明: 配置用户按键相关的GPIO
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_InitKEY(void)
{
    GPIO_InitType GPIO_InitStructure;

    /* 使能GPIO时钟 */
    RCC_AHBPeriphClockCmd(KEY_GPIO_RCC, ENABLE);

    /* 配置GPIO */
    GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pins = KEY_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_Pull = GPIO_Pull_NOPULL; /* 浮空输入，已外置上拉 */
    GPIO_Init(KEY_PORT, &GPIO_InitStructure);
}

/*
*********************************************************************************************************
*	函 数 名: bsp_KEY_Driver
*	功能说明: 按键驱动，需要周期性调用，推荐10ms频率
*	形    参: 无
*	返 回 值: N_KEY: 无键  S_KEY: 单键  L_KEY: 长按
*********************************************************************************************************
*/
uint8_t bsp_KEY_Driver(void)
{
    static uint8_t ucKeyState = 0;
    static uint16_t usKeyTime;
#if AUTO_SEND_KEY_EN
    static uint8_t ucAutoSendKeyEnFlag;
#endif

    switch (ucKeyState)
    {
    case 0: /* 状态0-按键按下 */
        if (!KEY_READ())
        {
#if AUTO_SEND_KEY_EN
            ucAutoSendKeyEnFlag = 0;
#endif
            ucKeyState = 1;
        }
        break;

    case 1: /* 状态1-消抖再次确定 */
        if (!KEY_READ())
        {
            ucKeyState = 2;
        }
        else
        {
            ucKeyState = 0;
        }
        usKeyTime = 0;
        break;

    case 2: /* 状态2-确定状态 */
        if (KEY_READ())
        {
            ucKeyState = 0;
            return S_KEY;
        }
        else if (usKeyTime > LONG_KEY_TIME - 2) /* 减掉前两个状态时间 */
        {
            usKeyTime = 0;
            ucKeyState = 3;
            return L_KEY;
        }
        else
        {
            usKeyTime++;
        }
        break;

    case 3: /* 状态3 */
        if (KEY_READ())
        {
            ucKeyState = 0;
        }
#if AUTO_SEND_KEY_EN
        else /* 长按时间超过长键2倍判定值自动发送单键 */
        {
            if (ucAutoSendKeyEnFlag)
            {
                usKeyTime++;
                if (usKeyTime > AUTO_SEND_KEY_TIME - 1)
                {
                    usKeyTime = 0;
                    if (!KEY_READ())
                    {
                        return S_KEY;
                    }
                }
            }
            else
            {
                usKeyTime++;
                if (usKeyTime > LONG_KEY_TIME * 2 - 1)
                {
                    usKeyTime = 0;
                    ucAutoSendKeyEnFlag = 1;
                }
            }
        }
#endif
        break;
    }
    return N_KEY;
}

/********************************************* End of file **********************************************/
