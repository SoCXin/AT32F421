/*
*********************************************************************************************************
*
*	程序名称: 片上FLASH驱动模块
*	文件名称: bsp_flash.h
*	版    本: v1.0.0
*   编    写: 莫利奖
*   日    期: 2021年4月15日
*
*	Copyright (C), 2021-2031, 深圳市风巽云科技有限公司 https://fxymcu.taobao.com
*
*********************************************************************************************************
*/

#ifndef __BSP_FLASH_H
#define __BSP_FLASH_H

/* FLASH结束地址 */
#define USER_FLASH_END_ADDRESS 0x0800FFFF

uint8_t bsp_FLASH_ErasePage(uint32_t ulFlashAddr);
uint8_t bsp_FLASH_WriteBuffer(uint32_t ulFlashAddr, uint8_t *pByte, uint32_t ulSize);
uint8_t bsp_FLASH_ReadBuffer(uint32_t ulFlashAddr, uint8_t *pByte, uint32_t ulSize);

#endif

/********************************************* End of file **********************************************/
