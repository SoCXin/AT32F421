/*
*********************************************************************************************************
*
*	程序名称: ERTC驱动模块
*	文件名称: bsp_ertc.c
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

/* 时钟源选择 */
#define ERTC_CLOCK_SOURCE_LSE 1 /* 外部32.768KHz时钟(推荐) */
#define ERTC_CLOCK_SOURCE_LSI 0 /* 内部40KHz时钟(有误差) */

/*
*********************************************************************************************************
*	函 数 名: bsp_InitHardERTC
*	功能说明: 初始化实时时钟硬件
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
static void bsp_InitHardERTC(void)
{
    ERTC_InitType ERTC_InitStructure;
    ERTC_DateType ERTC_DateStructure;
    ERTC_TimeType ERTC_TimeStructure;
    __IO uint8_t ucAsynchPrediv;
    __IO uint16_t usSynchPrediv;

    /* 使能电源控制器 (PWR) APB1 接口时钟 */
    RCC_APB1PeriphClockCmd(RCC_APB1PERIPH_PWR, ENABLE);
    /* 使能对ERTC访问 */
    PWR_BackupAccessCtrl(ENABLE);
    /* 重置ERTC域 */
    RCC_BackupResetCmd(ENABLE);
    RCC_BackupResetCmd(DISABLE);

#if ERTC_CLOCK_SOURCE_LSI == 1 /* 内部40KHz时钟 */

    /* 使能内部40KHz时钟源 */
    RCC_LSICmd(ENABLE);
    /* 等待时钟源就绪 */
    while (RCC_GetFlagStatus(RCC_FLAG_LSISTBL) == RESET)
    {
    }

    /* 选择ERTC时钟源 */
    RCC_ERTCCLKConfig(RCC_ERTCCLKSelection_LSI);
    /* ck_spre(1Hz) = ERTCCLK(LSI) /(ucAsynchPrediv + 1)*(usSynchPrediv + 1)*/
    ucAsynchPrediv = 127;
    usSynchPrediv = 319;

#elif ERTC_CLOCK_SOURCE_LSE == 1 /* 使用外部32.768KHz */

    /* 使能外部32.768KHz时钟源 */
    RCC_LSEConfig(RCC_LSE_ENABLE);
    /* 等待时钟源就绪 */
    while (RCC_GetFlagStatus(RCC_FLAG_LSESTBL) == RESET)
    {
    }

    /* 选择ERTC时钟源 */
    RCC_ERTCCLKConfig(RCC_ERTCCLKSelection_LSE);
    /* ck_spre(1Hz) = ERTCCLK(LSE) /(ucAsynchPrediv + 1)*(usSynchPrediv + 1)*/
    ucAsynchPrediv = 127;
    usSynchPrediv = 255;
#endif

    /* 使能ERTC时钟 */
    RCC_ERTCCLKCmd(ENABLE);
    /* 复位ERTC寄存器 */
    ERTC_Reset();
    /* 等待ERTC APB寄存器同步 */
    ERTC_WaitForSynchro();

    /* 配置ERTC数据寄存器和ERTC预分频器 */
    ERTC_InitStructure.ERTC_AsynchPrediv = ucAsynchPrediv;   /* 异步预分频 */
    ERTC_InitStructure.ERTC_SynchPrediv = usSynchPrediv;     /* 同步预分频 */
    ERTC_InitStructure.ERTC_HourFormat = ERTC_HourFormat_24; /* 小时格式 */
    ERTC_Init(&ERTC_InitStructure);

    ERTC_ClearFlag(ERTC_FLAG_ALAF);

    /* 设置日期: 2021年4月9日 星期五 */
    ERTC_DateStructure.ERTC_Year = 0x21;
    ERTC_DateStructure.ERTC_Month = ERTC_Month_APR;
    ERTC_DateStructure.ERTC_Date = 0x09;
    ERTC_DateStructure.ERTC_WeekDay = ERTC_Week_FRI;
    ERTC_SetDateValue(ERTC_Format_BCD, &ERTC_DateStructure);

    /* 设置时间: 11时50分25秒 */
    ERTC_TimeStructure.ERTC_AMPM = ERTC_H12_AM; /* 上午 */
    ERTC_TimeStructure.ERTC_Hours = 0x11;
    ERTC_TimeStructure.ERTC_Minutes = 0x50;
    ERTC_TimeStructure.ERTC_Seconds = 0x25;
    ERTC_SetTimeValue(ERTC_Format_BCD, &ERTC_TimeStructure);

    /* 写标志到备份寄存器 */
    ERTC_WriteBackupRegister(ERTC_BKP_DT0, 0x1155);
}

/*
*********************************************************************************************************
*	函 数 名: bsp_InitERTC
*	功能说明: 初始化ERTC
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_InitERTC(void)
{
    /* 读取标志 */
    if (ERTC_ReadBackupRegister(ERTC_BKP_DT0) != 0x1155)
    {
        /* 初始化ERTC硬件  */
        bsp_InitHardERTC();
        /* 打印时钟 */
        bsp_ERTC_PrintfTime();
    }
    else /* 板子带电复位 */
    {
        /* 使能电源控制器 (PWR) APB1 接口时钟 */
        RCC_APB1PeriphClockCmd(RCC_APB1PERIPH_PWR, ENABLE);
        /* 使能对ERTC访问 */
        PWR_BackupAccessCtrl(ENABLE);
        /* 等待ERTC APB寄存器同步 */
        ERTC_WaitForSynchro();
        /* 打印时钟 */
        bsp_ERTC_PrintfTime();
    }
}

/*
*********************************************************************************************************
*	函 数 名: bsp_ERTC_PrintfTime
*	功能说明: 打印时间
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_ERTC_PrintfTime(void)
{
    ERTC_DateType ERTC_DateStructure;
    ERTC_TimeType ERTC_TimeStructure;

    /* 读取日期 */
    ERTC_GetDateValue(ERTC_Format_BIN, &ERTC_DateStructure);
    /* 读取日时钟 */
    ERTC_GetTimeValue(ERTC_Format_BIN, &ERTC_TimeStructure);
    /* 打印 */
    printf("->20%0.2d年%0.2d月%0.2d日  %0.2d时%0.2d分%0.2d秒\r\n", ERTC_DateStructure.ERTC_Year, ERTC_DateStructure.ERTC_Month, ERTC_DateStructure.ERTC_Date,
           ERTC_TimeStructure.ERTC_Hours, ERTC_TimeStructure.ERTC_Minutes, ERTC_TimeStructure.ERTC_Seconds);
}

/********************************************* End of file **********************************************/
