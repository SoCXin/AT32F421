/*
*********************************************************************************************************
*
*	��������: ��������ģ��
*	�ļ�����: bsp_key.c
*	��    ��: v1.0.0
*   ��    д: Ī����
*	˵    ��: ֧�ֵ����������Լ���������
*
*	�汾��¼: 
*	v1.0.0: 2021��4��9�գ�����
*
*	Copyright (C), 2021-2031, �����з����ƿƼ����޹�˾ https://fxymcu.taobao.com
*
*********************************************************************************************************
*/

#include "bsp.h"

/* �������Ŷ��� */
#define KEY_PORT GPIOC
#define KEY_PIN GPIO_Pins_13
#define KEY_GPIO_RCC RCC_AHBPERIPH_GPIOC
/* ����������״̬ */
#define KEY_READ() (KEY_PORT->IPTDT & KEY_PIN)

/*
*********************************************************************************************************
*	�� �� ��: bsp_InitKEY
*	����˵��: �����û�������ص�GPIO
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_InitKEY(void)
{
    GPIO_InitType GPIO_InitStructure;

    /* ʹ��GPIOʱ�� */
    RCC_AHBPeriphClockCmd(KEY_GPIO_RCC, ENABLE);

    /* ����GPIO */
    GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pins = KEY_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_Pull = GPIO_Pull_NOPULL; /* �������룬���������� */
    GPIO_Init(KEY_PORT, &GPIO_InitStructure);
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_KEY_Driver
*	����˵��: ������������Ҫ�����Ե��ã��Ƽ�10msƵ��
*	��    ��: ��
*	�� �� ֵ: N_KEY: �޼�  S_KEY: ����  L_KEY: ����
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
    case 0: /* ״̬0-�������� */
        if (!KEY_READ())
        {
#if AUTO_SEND_KEY_EN
            ucAutoSendKeyEnFlag = 0;
#endif
            ucKeyState = 1;
        }
        break;

    case 1: /* ״̬1-�����ٴ�ȷ�� */
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

    case 2: /* ״̬2-ȷ��״̬ */
        if (KEY_READ())
        {
            ucKeyState = 0;
            return S_KEY;
        }
        else if (usKeyTime > LONG_KEY_TIME - 2) /* ����ǰ����״̬ʱ�� */
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

    case 3: /* ״̬3 */
        if (KEY_READ())
        {
            ucKeyState = 0;
        }
#if AUTO_SEND_KEY_EN
        else /* ����ʱ�䳬������2���ж�ֵ�Զ����͵��� */
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
