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
*	v1.0.0: 2021��4��9�գ�����
*
*	Copyright (C), 2021-2031, �����з����ƿƼ����޹�˾ https://fxymcu.taobao.com
*
*********************************************************************************************************
*/

#include "bsp.h"

/*
*********************************************************************************************************
*	�� �� ��: bsp_InitADC1Channel
*	����˵��: ����ADC1ģʽ��ָ��ͨ��
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void bsp_InitADC1Channel(void)
{
    ADC_InitType ADC_InitStructure;

    /* ADC1ʱ�� = PCLK2/12 = 10MHz */
    RCC_ADCCLKConfig(RCC_APB2CLK_Div12);
    /* ʹ��ADC1ʱ�� */
    RCC_APB2PeriphClockCmd(RCC_APB2PERIPH_ADC1, ENABLE);

    /* ADC1 ���� */
    ADC_StructInit(&ADC_InitStructure);
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;          /* ����ģʽ */
    ADC_InitStructure.ADC_ScanMode = ENABLE;                    /* ����ת�� */
    ADC_InitStructure.ADC_ContinuousMode = DISABLE;             /* ����ת�� */
    ADC_InitStructure.ADC_ExternalTrig = ADC_ExternalTrig_None; /* ������� */
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;      /* ת������Ҷ��� */
    ADC_InitStructure.ADC_NumOfChannel = 1;                     /* һ��ת��ͨ�� */
    ADC_Init(ADC1, &ADC_InitStructure);

    /* ����ADCͨ��ת��˳���ʱ�� */
    ADC_RegularChannelConfig(ADC1, ADC_Channel_TempSensor, 1, ADC_SampleTime_55_5); /* ����ʱ�� = 55.5 + 12.5 = 68����  68 * ��1/10�� = 6.8us */
    /* ʹ���¶ȴ�����ͨ�� */
    ADC_TempSensorVrefintCtrl(ENABLE);

    /* ����ж���ɱ�־ */
    ADC_ClearINTPendingBit(ADC1, ADC_INT_EC);
    /* ʹ��ADC1ת������ж� */
    ADC_INTConfig(ADC1, ADC_INT_EC, ENABLE);
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

    /* �������ADC1ת��һ�� */
    ADC_SoftwareStartConvCtrl(ADC1, ENABLE);
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_InitADC1
*	����˵��: ��ʼ��ADC1
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_InitADC1(void)
{
    bsp_InitADC1Channel();
}

/*
*********************************************************************************************************
*	�� �� ��: Get_ADC
*	����˵��: ��ȡADCֵ
*	��    ��: ��
*	�� �� ֵ: ADCֵ
*********************************************************************************************************
*/
uint16_t Get_ADC(void)
{
    /* �������ADC1ת�� */
    ADC_SoftwareStartConvCtrl(ADC1, ENABLE);
    /* �ȴ�ת����� */
    while (ADC_GetSoftwareStartConvStatus(ADC1));
    /* ���ع�����ת����� */
    return ADC_GetDualModeConversionValue();
}

/********************************************* End of file **********************************************/
