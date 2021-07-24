/*
*********************************************************************************************************
*
*	��������: ADC��ȡ�ⲿ��ͨ������
*	�ļ�����: main.c
*   MCU�ͺ� : AT32F413RBT7 FLASH:128KB SRAM:64KB
*   ����ƽ̨: ������AT32F421������
*	��    ��: v1.0.3
*   ��    д: Ī����
*	˵    ��: ��ʾADC��ȡ�ⲿ��ͨ������
*
*	�汾��¼: 
*	v1.0.0: 2020��11��25�գ�����
*	v1.0.1: 2021��3��3�գ�������׼����1.2.9
*   v1.0.2: 2021��4��16�գ�������׼����1.3.0���淶ʾ��
*	v1.0.3: 2021��6��10�գ�ADC��ȡ�ⲿ�����ͨ��ʾ��
*
*	Copyright (C), 2021-2031, �����з����ƿƼ����޹�˾ https://fxymcu.taobao.com
*
*********************************************************************************************************
*/

#include "bsp.h"

/* ���������������̷������� */
#define EXAMPLE_NAME "ADC��ȡ�ⲿ��ͨ������"
#define EXAMPLE_DATE "2021��6��10��"
#define DEMO_VER "v1.0.2"

uint16_t usAdc[dChannel] = {0};
uint8_t ucSamplingCnt = 0;

/* �������� */
static void PrintfLogo(void);

/*
*********************************************************************************************************
*	�� �� ��: main
*	����˵��: ��׼c�������
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
int main(void)
{
    uint8_t ucTimeCnt = 0;

    /* ϵͳ��ʼ�� */
    System_Init();
    /* ������Ϣ */
    PrintfLogo();

    while (1)
    {
        /* ϵͳ10msʱ�� */
        if (ucSys10msFlag)
        {
            ucSys10msFlag = 0;
			/* ADCת�� */
            if (ucSamplingCnt > dSamplingTimes - 1)
            {
                ucSamplingCnt = 0;
                usAdc[0] = Get_ADC1(0);
                usAdc[1] = Get_ADC1(1);
                usAdc[2] = Get_ADC1(2);
                ClearedAdcBuffer();
            }
            else
            {
                ucSamplingCnt++;
                /* ����ADCת�� */
                bsp_StartADC1();
            }
            /* ��ӡADֵ */
            ucTimeCnt++;
            if (ucTimeCnt > 99)
            {
                ucTimeCnt = 0;
                printf("->usAdc[0] = %d usAdc[1] = %d usAdc[2] = %d\r\n", usAdc[0], usAdc[1], usAdc[2]);
                bsp_LED_Toggle();
            }
        }
    }
}

/*
*********************************************************************************************************
*	�� �� ��: PrintfLogo
*	����˵��: ��ӡ�������ƺ����̷�������
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void PrintfLogo(void)
{
    uint32_t CPU_Sn0, CPU_Sn1, CPU_Sn2;

    printf("-------------------------------------------------------------\r\n");
    printf("->��������: %s\r\n", EXAMPLE_NAME); /* ��ӡ�������� */
    printf("->���̰汾: %s\r\n", DEMO_VER);     /* ��ӡ���̰汾 */
    printf("->��������: %s\r\n", EXAMPLE_DATE); /* ��ӡ�������� */

    /* UID */
    CPU_Sn0 = *(__IO uint32_t *)(0x1FFFF7E8);
    CPU_Sn1 = *(__IO uint32_t *)(0x1FFFF7E8 + 4);
    CPU_Sn2 = *(__IO uint32_t *)(0x1FFFF7E8 + 8);

    printf("->CPU: AT32F421C8T7, LQFP48, ��Ƶ: %dMHz\r\n", SystemCoreClock / 1000000);
    printf("->FLASH: 64Kbyte, SRAM: 16Kbyte\r\n");
    printf("->UID: %08X %08X %08X\n\r", CPU_Sn2, CPU_Sn1, CPU_Sn0);

    /* ��汾 */
    printf("->��汾: V1.3.0\r\n");
    printf("-------------------------------------------------------------\r\n");
}

/*
*********************************************************************************************************
*	�� �� ��: assert_failed
*	����˵��: ���Ժ������궨����at32f4xx_conf.h�ļ�
*	��    ��: 
*	          file: Դ�����ļ����ƣ�__FILE__ ��ʾԴ�����ļ�����
*	          line: �����кţ�__LINE__ ��ʾԴ�����кţ�
*	�� �� ֵ: ��
*********************************************************************************************************
*/
#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t *file, uint32_t line)
{
    /* �û����������Լ��Ĵ��뱨��Դ�����ļ����ʹ����кţ�����ʹ�ô��ڽ������ļ����кŷ��������� */
    printf("Wrong parameters value: file %s on line %d\r\n", file, line);

    /* ��ѭ�������ڲ������� */
    while (1)
    {
    }
}
#endif

/********************************************* End of file **********************************************/