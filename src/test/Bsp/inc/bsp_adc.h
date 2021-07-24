/*
*********************************************************************************************************
*
*	程序名称: ADC1驱动模块
*	文件名称: bsp_adc.h
*	版    本: v1.0.0
*   编    写: 莫利奖
*   日    期: 2021年6月10日
*
*	Copyright (C), 2021-2031, 深圳市风巽云科技有限公司 https://fxymcu.taobao.com
*
*********************************************************************************************************
*/

#ifndef __BSP_ADC_H
#define __BSP_ADC_H

/* 通道数 */
#define dChannel 3
/* 采样次数 */
#define dSamplingTimes 18

void ClearedAdcBuffer(void);
void bsp_StartADC1(void);
void bsp_InitADC1(void);
uint16_t Get_ADC1(uint8_t ucChannel);

#endif

/********************************************* End of file **********************************************/
