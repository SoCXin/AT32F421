/*
*********************************************************************************************************
*
*	程序名称: 按键驱动模块
*	文件名称: bsp_key.h
*	版    本: v1.0.0
*   编    写: 莫利奖
*   日    期: 2021年4月9日
*
*	Copyright (C), 2021-2031, 深圳市风巽云科技有限公司 https://fxymcu.taobao.com
*
*********************************************************************************************************
*/

#ifndef __BSP_KEY_H
#define __BSP_KEY_H

/* 按键枚举值 */
typedef enum
{
    N_KEY, /* 无键 */
    S_KEY, /* 单键 */
    L_KEY  /* 长键 */
} KEY_Type;

#define LONG_KEY_TIME 150     /* 判定为长键时间 = N*10ms */
#define AUTO_SEND_KEY_TIME 10 /* 自动发送长键间隔时间 = N*10ms */
#define AUTO_SEND_KEY_EN 1    /* 使能自动发送长键 */

/* 供外部调用函数 */
void bsp_InitKEY(void);
uint8_t bsp_KEY_Driver(void);

#endif

/********************************************* End of file **********************************************/
