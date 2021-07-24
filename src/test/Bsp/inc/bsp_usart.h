/*
*********************************************************************************************************
*
*	��������: ��������ģ��
*	�ļ�����: bsp_usart.h
*	��    ��: v1.0.0
*   ��    д: Ī����
*   ��    ��: 2021��4��9��
*
*	Copyright (C), 2021-2031, �����з����ƿƼ����޹�˾ https://fxymcu.taobao.com
*
*********************************************************************************************************
*/

#ifndef __BSP_USART_H
#define __BSP_USART_H

/* ʹ�� */
#define USART1_EN 1
#define USART2_EN 0

/* ������ */
#define USART1_BAUD 115200
#define USART2_BAUD 115200

/* ���ⲿ���� */
void bsp_UART_SetBaudrate(USART_Type *USARTx, uint32_t ulValue);
void bsp_UART_SendByte(USART_Type *USARTx, uint8_t ucByte);
void bsp_InitUART(void);

#endif

/********************************************* End of file **********************************************/
