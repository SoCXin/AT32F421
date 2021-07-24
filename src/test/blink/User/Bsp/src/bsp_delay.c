/*
*********************************************************************************************************
*
*	��������: ϵͳ��ʱģ��
*	�ļ�����: bsp_delay.c
*	��    ��: v1.0.0
*   ��    д: Ī����
*	˵    ��: ʹ��ARM�ں�SysTick��ʱ��������ʱ
*
*	�汾��¼: 
*	v1.0.0: 2021��3��20�գ�����
*
*	Copyright (C), 2021-2031, �����з����ƿƼ����޹�˾ https://fxymcu.taobao.com
*
*********************************************************************************************************
*/

#include "bsp.h"

/* ��ʱ����ֵ */
static __IO float fUsValueCount;
static __IO float fMsValueCount;

/*
*********************************************************************************************************
*	�� �� ��: bsp_InitSysTick
*	����˵��: ��ʼ��SysTick
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_InitSysTick(void)
{
    /* ����SysTickʱ��-8��Ƶ */
    SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
    /* 1us����ļ���ֵ */
    fUsValueCount = (float)SystemCoreClock / (8 * 1000000);
    /* 1ms����ļ���ֵ */
    fMsValueCount = fUsValueCount * 1000;
}

/*
*********************************************************************************************************
*	�� �� bsp_DelayUsUs
*	����˵��: us��ʱ
*	��    ��: ulNus: ��ʱֵ��ע�������ʱֵΪ 2^24 / fUsValueCount)
*	�� �� ֵ: ��
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
*	�� �� bsp_DelayMsMs
*	����˵��: ms��ʱ
*	��    ��: ulNms: ������ʱֵ
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_DelayMs(uint32_t ulNms)
{
    uint32_t ulStatus;

    while (ulNms)
    {
        SysTick->LOAD = (uint32_t)(1000 * fUsValueCount); /* ��ʱ1000us */
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
