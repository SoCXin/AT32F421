/*
*********************************************************************************************************
*
*	程序名称: SPI总线驱动模块
*	文件名称: bsp_spi.h
*	版    本: v1.0.0
*   编    写: 莫利奖
*   日    期: 2021年3月31日
*
*	Copyright (C), 2021-2031, 深圳市风巽云科技有限公司 https://fxymcu.taobao.com
*
*********************************************************************************************************
*/

#ifndef __BSP_SPI_H
#define __BSP_SPI_H

/* 使能 */
#define SPI1_EN 1
#define SPI2_EN 1

/* 供外部调用函数 */
void bsp_InitSPI(void);
void bsp_SPI_NSS(SPI_Type* SPIx, uint8_t ucLevel);
uint8_t bsp_SPI_Bus(SPI_Type* SPIx, uint8_t ucSendByte);

#endif

/********************************************* End of file **********************************************/
