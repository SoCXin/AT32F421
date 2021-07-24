/*
*********************************************************************************************************
*
*	程序名称: 硬件IIC驱动模块
*	文件名称: bsp_i2c.h
*	版    本: v1.0.0
*   编    写: 莫利奖
*   日    期: 2021年4月13日
*
*	Copyright (C), 2021-2031, 深圳市风巽云科技有限公司 https://fxymcu.taobao.com
*
*********************************************************************************************************
*/

#ifndef __BSP_I2C_H
#define __BSP_I2C_H

/* 使能 */
#define I2C1_EN 1
#define I2C2_EN 1

/* I2C速度定义 */
#define I2C1_SPEED 400000 /* HZ */
#define I2C2_SPEED 400000 /* HZ */

/* I2C状态枚举 */
typedef enum
{
    I2C_OK = 0,

    I2C_ERROR_STEP_1,
    I2C_ERROR_STEP_2,
    I2C_ERROR_STEP_3,
    I2C_ERROR_STEP_4,
    I2C_ERROR_STEP_5,
    I2C_ERROR_STEP_6,
    I2C_ERROR_STEP_7,
    I2C_ERROR_STEP_8,
    I2C_ERROR_STEP_9,
    I2C_ERROR_STEP_10,
    I2C_ERROR_STEP_11,
    I2C_ERROR_STEP_12,

    I2C_BUSY = 100,
    I2C_TIMEOUT = 101,
    I2C_ERROR = 102,
} I2C_StatusType;

/* 供外部调用函数 */
void bsp_InitI2C_Hard(void);
I2C_StatusType bsp_I2C_ReadBuffer(I2C_Type *I2Cx, uint8_t ucSlaveAddr, uint8_t *pByteBuf, uint16_t usDataLen);
I2C_StatusType bsp_I2C_SendBuffer(I2C_Type *I2Cx, uint8_t ucSlaveAddr, uint8_t *pByteBuf, uint16_t usDataLen);
I2C_StatusType bsp_I2C_ReadAddrBuffer(I2C_Type *I2Cx, uint8_t ucSlaveAddr, uint8_t *pDataAddr, uint8_t ucAddrLen, uint8_t *pByteBuf, uint16_t usDataLen);
I2C_StatusType bsp_I2C_SendAddrBuffer(I2C_Type *I2Cx, uint8_t ucSlaveAddr, uint8_t *pDataAddr, uint8_t ucAddrLen, uint8_t *pByteBuf, uint16_t usDataLen);

#endif

/********************************************* End of file **********************************************/
