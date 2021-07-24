/*
*********************************************************************************************************
*
*	程序名称: ADC1驱动模块
*	文件名称: bsp_adc.c
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

/*
*********************************************************************************************************
*	函 数 名: bsp_InitADC1Channel
*	功能说明: 配置ADC1模式及指定通道
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
static void bsp_InitADC1Channel(void)
{
    ADC_InitType ADC_InitStructure;

    /* ADC1时钟 = PCLK2/12 = 10MHz */
    RCC_ADCCLKConfig(RCC_APB2CLK_Div12);
    /* 使能ADC1时钟 */
    RCC_APB2PeriphClockCmd(RCC_APB2PERIPH_ADC1, ENABLE);

    /* ADC1 配置 */
    ADC_StructInit(&ADC_InitStructure);
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;          /* 独立模式 */
    ADC_InitStructure.ADC_ScanMode = ENABLE;                    /* 单次转换 */
    ADC_InitStructure.ADC_ContinuousMode = DISABLE;             /* 连续转换 */
    ADC_InitStructure.ADC_ExternalTrig = ADC_ExternalTrig_None; /* 软件触发 */
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;      /* 转换结果右对齐 */
    ADC_InitStructure.ADC_NumOfChannel = 1;                     /* 一个转换通道 */
    ADC_Init(ADC1, &ADC_InitStructure);

    /* 配置ADC通道转换顺序和时间 */
    ADC_RegularChannelConfig(ADC1, ADC_Channel_TempSensor, 1, ADC_SampleTime_55_5); /* 采样时间 = 55.5 + 12.5 = 68周期  68 * （1/10） = 6.8us */
    /* 使能温度传感器通道 */
    ADC_TempSensorVrefintCtrl(ENABLE);

    /* 清除中断完成标志 */
    ADC_ClearINTPendingBit(ADC1, ADC_INT_EC);
    /* 使能ADC1转换完成中断 */
    ADC_INTConfig(ADC1, ADC_INT_EC, ENABLE);
    /* 使能ADC1 */
    ADC_Ctrl(ADC1, ENABLE);

    /* 重置ADC1校准 */
    ADC_RstCalibration(ADC1);
    /* 等待重置完成 */
    while (ADC_GetResetCalibrationStatus(ADC1));

    /* 开始校准 */
    ADC_StartCalibration(ADC1);
    /* 等待校准完成 */
    while (ADC_GetCalibrationStatus(ADC1));

    /* 软件触发ADC1转换一次 */
    ADC_SoftwareStartConvCtrl(ADC1, ENABLE);
}

/*
*********************************************************************************************************
*	函 数 名: bsp_InitADC1
*	功能说明: 初始化ADC1
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_InitADC1(void)
{
    bsp_InitADC1Channel();
}

/*
*********************************************************************************************************
*	函 数 名: Get_ADC
*	功能说明: 读取ADC值
*	形    参: 无
*	返 回 值: ADC值
*********************************************************************************************************
*/
uint16_t Get_ADC(void)
{
    /* 软件触发ADC1转换 */
    ADC_SoftwareStartConvCtrl(ADC1, ENABLE);
    /* 等待转换完成 */
    while (ADC_GetSoftwareStartConvStatus(ADC1));
    /* 返回规则组转换结果 */
    return ADC_GetDualModeConversionValue();
}

/********************************************* End of file **********************************************/
