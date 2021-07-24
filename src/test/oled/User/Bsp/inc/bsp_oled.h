/*
*********************************************************************************************************
*
*	��������: OLEDģ��(SSD1306оƬ)����
*	�ļ�����: bsp_oled.h
*	��    ��: v1.0.0
*   ��    д: Ī����
*   ��    ��: 2021��4��20��
*
*	Copyright (C), 2021-2031, �����з����ƿƼ����޹�˾ https://fxymcu.taobao.com
*
*********************************************************************************************************
*/

#ifndef __BSP_OLED_H
#define __BSP_OLED_H

/* �궨�� */
#define OLED_ADDR 0x78 /* OLED�豸��ַ */
#define OLED_CMD 0x00  /* д���� */
#define OLED_DATA 0x40 /* д���� */

/* ���ⲿ���� */
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
