/*
*********************************************************************************************************
*
*	��������: ��������ģ��
*	�ļ�����: bsp_key.h
*	��    ��: v1.0.0
*   ��    д: Ī����
*   ��    ��: 2021��4��9��
*
*	Copyright (C), 2021-2031, �����з����ƿƼ����޹�˾ https://fxymcu.taobao.com
*
*********************************************************************************************************
*/

#ifndef __BSP_KEY_H
#define __BSP_KEY_H

/* ����ö��ֵ */
typedef enum
{
    N_KEY, /* �޼� */
    S_KEY, /* ���� */
    L_KEY  /* ���� */
} KEY_Type;

#define LONG_KEY_TIME 150     /* �ж�Ϊ����ʱ�� = N*10ms */
#define AUTO_SEND_KEY_TIME 10 /* �Զ����ͳ������ʱ�� = N*10ms */
#define AUTO_SEND_KEY_EN 1    /* ʹ���Զ����ͳ��� */

/* ���ⲿ���ú��� */
void bsp_InitKEY(void);
uint8_t bsp_KEY_Driver(void);

#endif

/********************************************* End of file **********************************************/