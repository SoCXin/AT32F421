/*
*********************************************************************************************************
*
*	��������: ERTC����ģ��
*	�ļ�����: bsp_ertc.c
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

/* ʱ��Դѡ�� */
#define ERTC_CLOCK_SOURCE_LSE 1 /* �ⲿ32.768KHzʱ��(�Ƽ�) */
#define ERTC_CLOCK_SOURCE_LSI 0 /* �ڲ�40KHzʱ��(�����) */

/*
*********************************************************************************************************
*	�� �� ��: bsp_InitHardERTC
*	����˵��: ��ʼ��ʵʱʱ��Ӳ��
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void bsp_InitHardERTC(void)
{
    ERTC_InitType ERTC_InitStructure;
    ERTC_DateType ERTC_DateStructure;
    ERTC_TimeType ERTC_TimeStructure;
    __IO uint8_t ucAsynchPrediv;
    __IO uint16_t usSynchPrediv;

    /* ʹ�ܵ�Դ������ (PWR) APB1 �ӿ�ʱ�� */
    RCC_APB1PeriphClockCmd(RCC_APB1PERIPH_PWR, ENABLE);
    /* ʹ�ܶ�ERTC���� */
    PWR_BackupAccessCtrl(ENABLE);
    /* ����ERTC�� */
    RCC_BackupResetCmd(ENABLE);
    RCC_BackupResetCmd(DISABLE);

#if ERTC_CLOCK_SOURCE_LSI == 1 /* �ڲ�40KHzʱ�� */

    /* ʹ���ڲ�40KHzʱ��Դ */
    RCC_LSICmd(ENABLE);
    /* �ȴ�ʱ��Դ���� */
    while (RCC_GetFlagStatus(RCC_FLAG_LSISTBL) == RESET)
    {
    }

    /* ѡ��ERTCʱ��Դ */
    RCC_ERTCCLKConfig(RCC_ERTCCLKSelection_LSI);
    /* ck_spre(1Hz) = ERTCCLK(LSI) /(ucAsynchPrediv + 1)*(usSynchPrediv + 1)*/
    ucAsynchPrediv = 127;
    usSynchPrediv = 319;

#elif ERTC_CLOCK_SOURCE_LSE == 1 /* ʹ���ⲿ32.768KHz */

    /* ʹ���ⲿ32.768KHzʱ��Դ */
    RCC_LSEConfig(RCC_LSE_ENABLE);
    /* �ȴ�ʱ��Դ���� */
    while (RCC_GetFlagStatus(RCC_FLAG_LSESTBL) == RESET)
    {
    }

    /* ѡ��ERTCʱ��Դ */
    RCC_ERTCCLKConfig(RCC_ERTCCLKSelection_LSE);
    /* ck_spre(1Hz) = ERTCCLK(LSE) /(ucAsynchPrediv + 1)*(usSynchPrediv + 1)*/
    ucAsynchPrediv = 127;
    usSynchPrediv = 255;
#endif

    /* ʹ��ERTCʱ�� */
    RCC_ERTCCLKCmd(ENABLE);
    /* ��λERTC�Ĵ��� */
    ERTC_Reset();
    /* �ȴ�ERTC APB�Ĵ���ͬ�� */
    ERTC_WaitForSynchro();

    /* ����ERTC���ݼĴ�����ERTCԤ��Ƶ�� */
    ERTC_InitStructure.ERTC_AsynchPrediv = ucAsynchPrediv;   /* �첽Ԥ��Ƶ */
    ERTC_InitStructure.ERTC_SynchPrediv = usSynchPrediv;     /* ͬ��Ԥ��Ƶ */
    ERTC_InitStructure.ERTC_HourFormat = ERTC_HourFormat_24; /* Сʱ��ʽ */
    ERTC_Init(&ERTC_InitStructure);

    ERTC_ClearFlag(ERTC_FLAG_ALAF);

    /* ��������: 2021��4��9�� ������ */
    ERTC_DateStructure.ERTC_Year = 0x21;
    ERTC_DateStructure.ERTC_Month = ERTC_Month_APR;
    ERTC_DateStructure.ERTC_Date = 0x09;
    ERTC_DateStructure.ERTC_WeekDay = ERTC_Week_FRI;
    ERTC_SetDateValue(ERTC_Format_BCD, &ERTC_DateStructure);

    /* ����ʱ��: 11ʱ50��25�� */
    ERTC_TimeStructure.ERTC_AMPM = ERTC_H12_AM; /* ���� */
    ERTC_TimeStructure.ERTC_Hours = 0x11;
    ERTC_TimeStructure.ERTC_Minutes = 0x50;
    ERTC_TimeStructure.ERTC_Seconds = 0x25;
    ERTC_SetTimeValue(ERTC_Format_BCD, &ERTC_TimeStructure);

    /* д��־�����ݼĴ��� */
    ERTC_WriteBackupRegister(ERTC_BKP_DT0, 0x1155);
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_InitERTC
*	����˵��: ��ʼ��ERTC
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_InitERTC(void)
{
    /* ��ȡ��־ */
    if (ERTC_ReadBackupRegister(ERTC_BKP_DT0) != 0x1155)
    {
        /* ��ʼ��ERTCӲ��  */
        bsp_InitHardERTC();
        /* ��ӡʱ�� */
        bsp_ERTC_PrintfTime();
    }
    else /* ���Ӵ��縴λ */
    {
        /* ʹ�ܵ�Դ������ (PWR) APB1 �ӿ�ʱ�� */
        RCC_APB1PeriphClockCmd(RCC_APB1PERIPH_PWR, ENABLE);
        /* ʹ�ܶ�ERTC���� */
        PWR_BackupAccessCtrl(ENABLE);
        /* �ȴ�ERTC APB�Ĵ���ͬ�� */
        ERTC_WaitForSynchro();
        /* ��ӡʱ�� */
        bsp_ERTC_PrintfTime();
    }
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_ERTC_PrintfTime
*	����˵��: ��ӡʱ��
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_ERTC_PrintfTime(void)
{
    ERTC_DateType ERTC_DateStructure;
    ERTC_TimeType ERTC_TimeStructure;

    /* ��ȡ���� */
    ERTC_GetDateValue(ERTC_Format_BIN, &ERTC_DateStructure);
    /* ��ȡ��ʱ�� */
    ERTC_GetTimeValue(ERTC_Format_BIN, &ERTC_TimeStructure);
    /* ��ӡ */
    printf("->20%0.2d��%0.2d��%0.2d��  %0.2dʱ%0.2d��%0.2d��\r\n", ERTC_DateStructure.ERTC_Year, ERTC_DateStructure.ERTC_Month, ERTC_DateStructure.ERTC_Date,
           ERTC_TimeStructure.ERTC_Hours, ERTC_TimeStructure.ERTC_Minutes, ERTC_TimeStructure.ERTC_Seconds);
}

/********************************************* End of file **********************************************/
