/*
*********************************************************************************************************
*
*	程序名称: OLED模块(SSD1306芯片)驱动
*	文件名称: bsp_oled.h
*	版    本: v1.0.0
*   编    写: 莫利奖
*   日    期: 2021年4月20日
*
*	Copyright (C), 2021-2031, 深圳市风巽云科技有限公司 https://fxymcu.taobao.com
*
*********************************************************************************************************
*/

#ifndef __BSP_OLED_H
#define __BSP_OLED_H

/* 宏定义 */
#define OLED_ADDR 0x78 /* OLED设备地址 */
#define OLED_CMD 0x00  /* 写命令 */
#define OLED_DATA 0x40 /* 写数据 */

/* 供外部调用 */
void bsp_OLED_DispOn(void);
void bsp_OLED_DispOff(void);
void bsp_OLED_SetContrast(uint8_t ucValue);
void bsp_OLED_SetDir(uint8_t ucDir);
void bsp_InitOLED(void);
void OLED_ClrScr(uint8_t ucData);
void OLED_DispChar(uint8_t ucX, uint8_t ucY, char c, uint8_t ucFont, uint8_t ucMode);
void OLED_DispChStr(uint8_t ucX, uint8_t ucY, char *pCh, uint8_t ucMode);
void OLED_DispStr(uint8_t ucX, uint8_t ucY, char *str, uint8_t ucMode);
uint16_t BitXOR(uint8_t ucM, uint8_t ucN);
void OLED_DispNumber(uint8_t ucX, uint8_t ucY, uint16_t usNumber, uint8_t ucLen, uint8_t dp, uint8_t ucMode);
void OLED_DrawBMP(uint8_t ucX, uint8_t ucY, uint8_t ucWidth, uint8_t ucHeight, const uint8_t *ptr, uint8_t ucMode);

#endif

/********************************************* End of file **********************************************/
