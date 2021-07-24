/*
*********************************************************************************************************
*
*	��������: ADC1����ģ��
*	�ļ�����: bsp_adc.c
*	��    ��: v1.0.0
*   ��    д: Ī����
*	˵    ��: 
*
*	�汾��¼: 
*	v1.0.0: 2021��6��10�գ�����
*
*	Copyright (C), 2021-2031, �����з����ƿƼ����޹�˾ https://fxymcu.taobao.com
*
*********************************************************************************************************
*/

#include "bsp.h"

/* ת������ */
uint16_t ADC_ValueBuffer[dSamplingTimes][dChannel] = {0};

/*
*********************************************************************************************************
*	�� �� ��: GPIO_Configuration
*	����˵��: ����ADC, PA4��PA5��PA6��ΪADCͨ������
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void GPIO_Configuration(void)
{
    GPIO_InitType GPIO_InitStructure;

    /* ʹ��GPIOʱ�� */
    RCC_AHBPeriphClockCmd(RCC_AHBPERIPH_GPIOA, ENABLE);

    /* ����IOģ������ */
    GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pins = GPIO_Pins_4 | GPIO_Pins_5 | GPIO_Pins_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}

/*
*********************************************************************************************************
*	�� �� ��: ADC1_Configuration
*	����˵��: ����ADC
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void ADC1_Configuration(void)
{
    ADC_InitType ADC_InitStructure;
    DMA_InitType DMA_InitStructure;

    /* ADC1ʱ�� = PCLK2/12 = 10MHz */
    RCC_ADCCLKConfig(RCC_APB2CLK_Div12);

    /* ʹ��ADC1ʱ�� */
    RCC_APB2PeriphClockCmd(RCC_APB2PERIPH_ADC1, ENABLE);
    /* ʹ��DMA1ʱ�� */
    RCC_AHBPeriphClockCmd(RCC_AHBPERIPH_DMA1, ENABLE);

    /* ����DMA1 */
    DMA_Reset(DMA1_Channel1);
    DMA_DefaultInitParaConfig(&DMA_InitStructure);
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->RDOR;             /* DMA�������ַ */
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&ADC_ValueBuffer;            /* DMA�ڴ����ַ */
    DMA_InitStructure.DMA_Direction = DMA_DIR_PERIPHERALSRC;                      /* ���䷽��������Ϊ���ݴ����Ŀ�ĵ� */
    DMA_InitStructure.DMA_BufferSize = dChannel * dSamplingTimes;                 /* ���ݴ�С */
    DMA_InitStructure.DMA_PeripheralInc = DMA_PERIPHERALINC_DISABLE;              /* �����ַ�Ĵ������� */
    DMA_InitStructure.DMA_MemoryInc = DMA_MEMORYINC_ENABLE;                       /* �ڴ��ַ�Ĵ������� */
    DMA_InitStructure.DMA_PeripheralDataWidth = DMA_PERIPHERALDATAWIDTH_HALFWORD; /* �������ݿ��Ϊ16λ */
    DMA_InitStructure.DMA_MemoryDataWidth = DMA_MEMORYDATAWIDTH_HALFWORD;         /* �洢�����ݿ��Ϊ16λ */
    DMA_InitStructure.DMA_Mode = DMA_MODE_CIRCULAR;                               /* ѭ��ģʽ */
    DMA_InitStructure.DMA_Priority = DMA_PRIORITY_HIGH;                           /* �����ȼ� */
    DMA_InitStructure.DMA_MTOM = DMA_MEMTOMEM_DISABLE;                            /* ���ڴ浽�ڴ� */
    DMA_Init(DMA1_Channel1, &DMA_InitStructure);
    /* ʹ��MDA1ͨ��1 */
    DMA_ChannelEnable(DMA1_Channel1, ENABLE);

    /* ����ADC1 */
    ADC_StructInit(&ADC_InitStructure);
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;          /* ADC����ģʽ */
    ADC_InitStructure.ADC_ScanMode = ENABLE;                    /* ɨ��ģʽ */
    ADC_InitStructure.ADC_ContinuousMode = DISABLE;             /* ������ת��ģʽ */
    ADC_InitStructure.ADC_ExternalTrig = ADC_ExternalTrig_None; /* ������� */
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;      /* �����Ҷ��� */
    ADC_InitStructure.ADC_NumOfChannel = dChannel;              /* ˳�����ת����ADCͨ������Ŀ */
    ADC_Init(ADC1, &ADC_InitStructure);

    /* ����ADC1ͨ��ת��˳���ʱ�� */
    ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 1, ADC_SampleTime_55_5);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 2, ADC_SampleTime_55_5);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_6, 3, ADC_SampleTime_55_5);

    /* ʹ��MDA1 */
    ADC_DMACtrl(ADC1, ENABLE);
    /* ʹ��ADC1 */
    ADC_Ctrl(ADC1, ENABLE);

    /* ����ADC1У׼ */
    ADC_RstCalibration(ADC1);
    /* �ȴ�������� */
    while (ADC_GetResetCalibrationStatus(ADC1));

    /* ��ʼУ׼ */
    ADC_StartCalibration(ADC1);
    /* �ȴ�У׼��� */
    while (ADC_GetCalibrationStatus(ADC1));
}

/*
*********************************************************************************************************
*	�� �� ��: ClearedAdcBuffer
*	����˵��: ���ADC����
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void ClearedAdcBuffer(void)
{
    /* ���ADC������ */
    memset(ADC_ValueBuffer, 0, sizeof(ADC_ValueBuffer));
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_StartADC1
*	����˵��: ����ADCת��
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_StartADC1(void)
{
    /* �������ADC1ת��һ�� */
    ADC_SoftwareStartConvCtrl(ADC1, ENABLE);
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_InitADC1
*	����˵��: ADC��ʼ��
*	��    ��: ��
*	�� �� ֵ: ��
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
*	�� �� ��: Get_ADC1
*	����˵��: ��ȡADC����
*	��    ��: ucChannel.ͨ��
*	�� �� ֵ: ��Ӧͨ��ƽ��ֵ
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
