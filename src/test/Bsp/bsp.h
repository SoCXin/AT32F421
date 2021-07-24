/*
*********************************************************************************************************
*
*	程序名称: BSP模块
*	文件名称: bsp.h
*	版    本: v1.0.0
*   编    写: 莫利奖
*   日    期: 2021年4月8日
*
*	Copyright (C), 2021-2031, 深圳市风巽云科技有限公司 https://fxymcu.taobao.com
*
*********************************************************************************************************
*/

#ifndef __BSP_H
#define __BSP_H

#include <at32f4xx.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "bsp_delay.h"
#include "bsp_led.h"
#include "bsp_flash.h"
#include "bsp_usart.h"
#include "bsp_timer.h"
#include "bsp_key.h"
#include "bsp_pwm.h"
#include "bsp_exit.h"
#include "bsp_adc.h"
//#include "bsp_ertc.h"
//#include "bsp_i2c_gpio.h"
//#include "bsp_i2c.h"
//#include "bsp_spi.h"

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

/* 供外部调用函数 */
void System_Init(void);

#endif

/********************************************* End of file **********************************************/
