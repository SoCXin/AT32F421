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
*	v1.0.0: 2021年6月10日，初版
*
*	Copyright (C), 2021-2031, 深圳市风巽云科技有限公司 https://fxymcu.taobao.com
*
*********************************************************************************************************
*/

#include "bsp.h"

/* 转换缓冲 */
uint16_t ADC_ValueBuffer[dSamplingTimes][dChannel] = {0};

/*
*********************************************************************************************************
*	函 数 名: GPIO_Configuration
*	功能说明: 配置ADC, PA4，PA5，PA6作为ADC通道输入
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
static void GPIO_Configuration(void)
{
    GPIO_InitType GPIO_InitStructure;

    /* 使能GPIO时钟 */
    RCC_AHBPeriphClockCmd(RCC_AHBPERIPH_GPIOA, ENABLE);

    /* 配置IO模拟输入 */
    GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pins = GPIO_Pins_4 | GPIO_Pins_5 | GPIO_Pins_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}

/*
*********************************************************************************************************
*	函 数 名: ADC1_Configuration
*	功能说明: 配置ADC
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
static void ADC1_Configuration(void)
{
    ADC_InitType ADC_InitStructure;
    DMA_InitType DMA_InitStructure;

    /* ADC1时钟 = PCLK2/12 = 10MHz */
    RCC_ADCCLKConfig(RCC_APB2CLK_Div12);

    /* 使能ADC1时钟 */
    RCC_APB2PeriphClockCmd(RCC_APB2PERIPH_ADC1, ENABLE);
    /* 使能DMA1时钟 */
    RCC_AHBPeriphClockCmd(RCC_AHBPERIPH_DMA1, ENABLE);

    /* 配置DMA1 */
    DMA_Reset(DMA1_Channel1);
    DMA_DefaultInitParaConfig(&DMA_InitStructure);
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->RDOR;             /* DMA外设基地址 */
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&ADC_ValueBuffer;            /* DMA内存基地址 */
    DMA_InitStructure.DMA_Direction = DMA_DIR_PERIPHERALSRC;                      /* 传输方向，外设作为数据传输的目的地 */
    DMA_InitStructure.DMA_BufferSize = dChannel * dSamplingTimes;                 /* 数据大小 */
    DMA_InitStructure.DMA_PeripheralInc = DMA_PERIPHERALINC_DISABLE;              /* 外设地址寄存器不变 */
    DMA_InitStructure.DMA_MemoryInc = DMA_MEMORYINC_ENABLE;                       /* 内存地址寄存器递增 */
    DMA_InitStructure.DMA_PeripheralDataWidth = DMA_PERIPHERALDATAWIDTH_HALFWORD; /* 外设数据宽度为16位 */
    DMA_InitStructure.DMA_MemoryDataWidth = DMA_MEMORYDATAWIDTH_HALFWORD;         /* 存储器数据宽度为16位 */
    DMA_InitStructure.DMA_Mode = DMA_MODE_CIRCULAR;                               /* 循环模式 */
    DMA_InitStructure.DMA_Priority = DMA_PRIORITY_HIGH;                           /* 高优先级 */
    DMA_InitStructure.DMA_MTOM = DMA_MEMTOMEM_DISABLE;                            /* 非内存到内存 */
    DMA_Init(DMA1_Channel1, &DMA_InitStructure);
    /* 使能MDA1通道1 */
    DMA_ChannelEnable(DMA1_Channel1, ENABLE);

    /* 配置ADC1 */
    ADC_StructInit(&ADC_InitStructure);
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;          /* ADC独立模式 */
    ADC_InitStructure.ADC_ScanMode = ENABLE;                    /* 扫描模式 */
    ADC_InitStructure.ADC_ContinuousMode = DISABLE;             /* 不连续转换模式 */
    ADC_InitStructure.ADC_ExternalTrig = ADC_ExternalTrig_None; /* 软件启动 */
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;      /* 数据右对齐 */
    ADC_InitStructure.ADC_NumOfChannel = dChannel;              /* 顺序规则转换的ADC通道的数目 */
    ADC_Init(ADC1, &ADC_InitStructure);

    /* 配置ADC1通道转换顺序和时间 */
    ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 1, ADC_SampleTime_55_5);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 2, ADC_SampleTime_55_5);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_6, 3, ADC_SampleTime_55_5);

    /* 使能MDA1 */
    ADC_DMACtrl(ADC1, ENABLE);
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
}

/*
*********************************************************************************************************
*	函 数 名: ClearedAdcBuffer
*	功能说明: 清空ADC缓冲
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void ClearedAdcBuffer(void)
{
    /* 清空ADC缓冲区 */
    memset(ADC_ValueBuffer, 0, sizeof(ADC_ValueBuffer));
}

/*
*********************************************************************************************************
*	函 数 名: bsp_StartADC1
*	功能说明: 启动ADC转换
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_StartADC1(void)
{
    /* 软件触发ADC1转换一次 */
    ADC_SoftwareStartConvCtrl(ADC1, ENABLE);
}

/*
*********************************************************************************************************
*	函 数 名: bsp_InitADC1
*	功能说明: ADC初始化
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_InitADC1(void)
{
    GPIO_Configuration();
    ADC1_Configuration();
    ClearedAdcBuffer();
}

/*
*********************************************************************************************************
*	函 数 名: Get_ADC1
*	功能说明: 获取ADC数据
*	形    参: ucChannel.通道
*	返 回 值: 对应通道平均值
*********************************************************************************************************
*/
uint16_t Get_ADC1(uint8_t ucChannel)
{
    uint16_t usConst;
    uint32_t ulMax = 0, ulMin = 0xffffffff, ulSum = 0;

    for (usConst = 0; usConst < dSamplingTimes; usConst++)
    {
        if (ADC_ValueBuffer[usConst][ucChannel] > ulMax)
        {
            ulMax = ADC_ValueBuffer[usConst][ucChannel];
        }
        if (ADC_ValueBuffer[usConst][ucChannel] < ulMin)
        {
            ulMin = ADC_ValueBuffer[usConst][ucChannel];
        }
        ulSum += ADC_ValueBuffer[usConst][ucChannel];
    }

    return (ulSum - ulMax - ulMin) / (dSamplingTimes - 2);
}

/********************************************* End of file **********************************************/
