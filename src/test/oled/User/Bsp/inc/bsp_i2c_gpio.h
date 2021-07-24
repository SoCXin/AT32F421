/*
*********************************************************************************************************
*
*	程序名称: I2C总线驱动模块
*	文件名称: bsp_i2c_gpio.h
*	版    本: v1.0.0
*   编    写: 莫利奖
*   日    期: 2021年4月9日
*
*	Copyright (C), 2021-2031, 深圳市风巽云科技有限公司 https://fxymcu.taobao.com
*
*********************************************************************************************************
*/

#ifndef _BSP_I2C_GPIO_H
#define _BSP_I2C_GPIO_H

#define I2C_WR 0 /* 写控制bit */
#define I2C_RD 1 /* 读控制bit */

void bsp_InitI2C_Gpio(void);
void bsp_I2C_Start(void);
void bsp_I2C_Stop(void);
void bsp_I2C_SendByte(uint8_t _ucByte);
uint8_t bsp_I2C_ReadByte(void);
uint8_t bsp_I2C_WaitAck(void);
void bsp_I2C_Ack(void);
void bsp_I2C_NAck(void);
uint8_t bsp_I2C_CheckDevice(uint8_t _Address);

#endif

/********************************************* End of file **********************************************/
