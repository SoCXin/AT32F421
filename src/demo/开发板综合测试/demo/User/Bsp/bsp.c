/*
*********************************************************************************************************
*
*	程序名称: BSP模块
*	文件名称: bsp.c
*	版    本: v1.0.0
*   编    写: 莫利奖
*	说    明: 硬件底层驱动程序的主文件
*
*	版本记录: 
*	v1.0.0: 2021年4月8日，初版
*
*	Copyright (C), 2021-2031, 深圳市风巽云科技有限公司 https://fxymcu.taobao.com
*
*********************************************************************************************************
*/

#include "bsp.h"

/*
*********************************************************************************************************
*	函 数 名: System_Init
*	功能说明: 系统初始化
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void System_Init(void)
{
    /* 优先级分组设置为4，配置0-15级抢占式优先级，0级子优先级 */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
    /* 内核进入调试状态时看门狗停止工作 */
    MCUDBG_PeriphDebugModeConfig(MCUDBG_IWDG_STOP, ENABLE);
    /* 更新时钟配置 */
    SystemCoreClockUpdate();
    /* 外设初始化 */
    bsp_InitSysTick();
	bsp_InitLED();
	bsp_InitTMR6();
	bsp_InitKEY();
	bsp_InitUART();
//	bsp_InitEXTI();
//	bsp_InitTMR3_PWM3(0);
	bsp_InitADC1();
	bsp_InitERTC();
//	bsp_InitI2C_Gpio();
	bsp_InitI2C_Hard();
	bsp_InitSPI();
}

/********************************************* End of file **********************************************/
