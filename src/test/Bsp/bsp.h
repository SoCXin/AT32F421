/*
*********************************************************************************************************
*
*	��������: BSPģ��
*	�ļ�����: bsp.h
*	��    ��: v1.0.0
*   ��    д: Ī����
*   ��    ��: 2021��4��8��
*
*	Copyright (C), 2021-2031, �����з����ƿƼ����޹�˾ https://fxymcu.taobao.com
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

/* ���ⲿ���ú��� */
void System_Init(void);

#endif

/********************************************* End of file **********************************************/
