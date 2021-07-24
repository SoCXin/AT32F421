/*
*********************************************************************************************************
*
*	程序名称: 串口驱动模块
*	文件名称: bsp_usart.h
*	版    本: v1.0.0
*   编    写: 莫利奖
*   日    期: 2021年4月9日
*
*	Copyright (C), 2021-2031, 深圳市风巽云科技有限公司 https://fxymcu.taobao.com
*
*********************************************************************************************************
*/

#ifndef __BSP_USART_H
#define __BSP_USART_H

/* 使能 */
#define USART1_EN 1
#define USART2_EN 0

/* 波特率 */
#define USART1_BAUD 115200
#define USART2_BAUD 115200

/* 供外部调用 */
void bsp_UART_SetBaudrate(USART_Type *USARTx, uint32_t ulValue);
void bsp_UART_SendByte(USART_Type *USARTx, uint8_t ucByte);
void bsp_InitUART(void);

#endif

/********************************************* End of file **********************************************/
