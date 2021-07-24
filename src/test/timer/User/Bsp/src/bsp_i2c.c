/*
*********************************************************************************************************
*
*	程序名称: 硬件IIC驱动模块
*	文件名称: bsp_i2c.c
*	版    本: v1.0.0
*   编    写: 莫利奖
*	说    明: 硬件I2C驱动
*
*	版本记录: 
*	v1.0.0: 2021年4月13日，初版
*
*	Copyright (C), 2021-2031, 深圳市风巽云科技有限公司 https://fxymcu.taobao.com
*
*********************************************************************************************************
*/

#include "bsp.h"

/* I2C1的GPIO分布
SCL: PF1 PA9  PB6 PB8
SDA: PF0 PA10 PB7 PB9
*/
/* I2C1 时钟线 */
#define I2C1_SCL_PORT GPIOB
#define I2C1_SCL_PIN GPIO_Pins_6
#define I2C1_SCL_SOURCE GPIO_PinsSource6
#define I2C1_SCL_AF GPIO_AF_1
/* I2C1 数据线 */
#define I2C1_SDA_PORT GPIOB
#define I2C1_SDA_PIN GPIO_Pins_7
#define I2C1_SDA_SOURCE GPIO_PinsSource7
#define I2C1_SDA_AF GPIO_AF_1
/* I2C1 GPIO时钟 */
#define I2C1_GPIO_RCC RCC_AHBPERIPH_GPIOB

/* I2C2的GPIO分布
SCL: PA0 PB10 PB13 PA11 PF6 PA8
SDA: PA1 PB11 PB14 PA12 PF7 PB4
*/
/* I2C2 时钟线 */
#define I2C2_SCL_PORT GPIOA
#define I2C2_SCL_PIN GPIO_Pins_0
#define I2C2_SCL_SOURCE GPIO_PinsSource0
#define I2C2_SCL_AF GPIO_AF_4
/* I2C2 数据线 */
#define I2C2_SDA_PORT GPIOA
#define I2C2_SDA_PIN GPIO_Pins_1
#define I2C2_SDA_SOURCE GPIO_PinsSource1
#define I2C2_SDA_AF GPIO_AF_4
/* I2C2 GPIO时钟 */
#define I2C2_GPIO_RCC RCC_AHBPERIPH_GPIOA

/* I2C总线超时 */
#define I2C_TIMEOUT_SET ((uint32_t)1000) /* ms */

/* I2C事件检查标志定义 */
#define I2C_EVT_CHECK_NONE ((uint32_t)0x00000000)
#define I2C_EVT_CHECK_ACKFAIL ((uint32_t)0x00000001)
#define I2C_EVT_CHECK_STOP ((uint32_t)0x00000002)

/*
*********************************************************************************************************
*	函 数 名: I2C_ClearADDRFlag
*	功能说明: 清除I2C ADDR挂起的标志
*	形    参: I2Cx: I2C设备
*	返 回 值: 无
*********************************************************************************************************
*/
void I2C_ClearADDRFlag(I2C_Type *I2Cx)
{
    __IO uint32_t tmpreg;

    tmpreg = I2Cx->STS1;
    tmpreg = I2Cx->STS2;
}

/*
*********************************************************************************************************
*	函 数 名: I2C_ClearSTOPFlag
*	功能说明: 清除I2C STOPF挂起标志
*	形    参: I2Cx: I2C设备
*	返 回 值: 无
*********************************************************************************************************
*/
void I2C_ClearSTOPFlag(I2C_Type *I2Cx)
{
    __IO uint32_t tmpreg;

    tmpreg = I2Cx->STS1;
    tmpreg = I2Cx->CTRL1 |= I2C_CTRL1_PEN;
}

/*
*********************************************************************************************************
*	函 数 名: I2C_WaitOnFlagUntilTimeout
*	功能说明: I2C通信超时
*	形    参: 
*	          I2Cx: I2C设备 
*	          Flag: 指定要检查的I2C标志 
*	          Status: 标志状态 
*	          EventCheck: 检查标志 
*	          Timeout: 超时时间(ms)
*	返 回 值: 状态类型
*********************************************************************************************************
*/
static I2C_StatusType I2C_WaitOnFlagUntilTimeout(I2C_Type *I2Cx, uint32_t Flag, FlagStatus Status, uint32_t ulEventCheck, uint32_t ulTimeout)
{
    ulTimeout *= 100;

    while (I2C_GetFlagStatus(I2Cx, Flag) == Status)
    {
        /* 检查应答失败标志 */
        if (ulEventCheck & I2C_EVT_CHECK_ACKFAIL)
        {
            if (I2C_GetFlagStatus(I2Cx, I2C_FLAG_ACKFAIL) == SET)
            {
                /* 停止位 */
                I2C_GenerateSTOP(I2Cx, ENABLE);
                /* 清零应答失败标志 */
                I2C_ClearFlag(I2Cx, I2C_FLAG_ACKFAIL);
                return I2C_ERROR;
            }
        }

        /* 检查停止位失败标志 */
        if (ulEventCheck & I2C_EVT_CHECK_STOP)
        {
            if (I2C_GetFlagStatus(I2Cx, I2C_FLAG_STOPF) == SET)
            {
                /* 清除I2C STOPF挂起标志 */
                I2C_ClearSTOPFlag(I2Cx);
                return I2C_ERROR;
            }
        }

        /* 10us延时 */
        bsp_DelayUs(10);
        /* 检查是否超时 */
        if ((ulTimeout--) == 0)
        {
            return I2C_TIMEOUT;
        }
    }
    return I2C_OK;
}

/*
*********************************************************************************************************
*	函 数 名: bsp_InitI2C_Hard
*	功能说明: 配置硬件I2C总线
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_InitI2C_Hard(void)
{
    GPIO_InitType GPIO_InitStructure;
    I2C_InitType I2C_InitStructure;

#if I2C1_EN
    /* 使能GPIO时钟 */
    RCC_AHBPeriphClockCmd(I2C1_GPIO_RCC, ENABLE);
    /* 使能I2C1时钟 */
    RCC_APB1PeriphClockCmd(RCC_APB1PERIPH_I2C1, ENABLE);

    /* 配置GPIO */
    GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_MaxSpeed = GPIO_MaxSpeed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; /* 复用功能 */
    GPIO_InitStructure.GPIO_OutType = GPIO_OutType_OD;
    GPIO_InitStructure.GPIO_Pull = GPIO_Pull_NOPULL;
    /* SCL */
    GPIO_InitStructure.GPIO_Pins = I2C1_SCL_PIN;
    GPIO_Init(I2C1_SCL_PORT, &GPIO_InitStructure);
    /* SDA */
    GPIO_InitStructure.GPIO_Pins = I2C1_SDA_PIN;
    GPIO_Init(I2C1_SDA_PORT, &GPIO_InitStructure);

    /* 配置复用功能 */
    GPIO_PinAFConfig(I2C1_SCL_PORT, I2C1_SCL_SOURCE, I2C1_SCL_AF);
    GPIO_PinAFConfig(I2C1_SDA_PORT, I2C1_SDA_SOURCE, I2C1_SDA_AF);

    /* 配置I2C1总线 */
    I2C_StructInit(&I2C_InitStructure);
    I2C_InitStructure.I2C_BitRate = I2C1_SPEED;              /* I2C总线速度（最大400KHz） */
    I2C_InitStructure.I2C_Mode = I2C_Mode_I2CDevice;         /* I2C总线模式 */
    I2C_InitStructure.I2C_FmDutyCycle = I2C_FmDutyCycle_2_1; /* I2C总线占空比 */
    I2C_InitStructure.I2C_AddrMode = I2C_AddrMode_7bit;      /* 7位地址模式 */
    I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;              /* 使能应答 */
    I2C_Init(I2C1, &I2C_InitStructure);
    /* 使能I2C */
    I2C_Cmd(I2C1, ENABLE);
#endif

#if I2C2_EN
    /* 使能GPIO时钟 */
    RCC_AHBPeriphClockCmd(I2C2_GPIO_RCC, ENABLE);
    /* 使能I2C2时钟 */
    RCC_APB1PeriphClockCmd(RCC_APB1PERIPH_I2C2, ENABLE);

    /* 配置GPIO */
    GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_MaxSpeed = GPIO_MaxSpeed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; /* 复用功能 */
    GPIO_InitStructure.GPIO_OutType = GPIO_OutType_OD;
    GPIO_InitStructure.GPIO_Pull = GPIO_Pull_NOPULL;
    /* SCL */
    GPIO_InitStructure.GPIO_Pins = I2C2_SCL_PIN;
    GPIO_Init(I2C2_SCL_PORT, &GPIO_InitStructure);
    /* SDA */
    GPIO_InitStructure.GPIO_Pins = I2C2_SDA_PIN;
    GPIO_Init(I2C2_SDA_PORT, &GPIO_InitStructure);

    /* 配置复用功能 */
    GPIO_PinAFConfig(I2C2_SCL_PORT, I2C2_SCL_SOURCE, I2C2_SCL_AF);
    GPIO_PinAFConfig(I2C2_SDA_PORT, I2C2_SDA_SOURCE, I2C2_SDA_AF);

    /* 配置I2C2总线 */
    I2C_StructInit(&I2C_InitStructure);
    I2C_InitStructure.I2C_BitRate = I2C1_SPEED;              /* I2C总线速度（最大400KHz） */
    I2C_InitStructure.I2C_Mode = I2C_Mode_I2CDevice;         /* I2C总线模式 */
    I2C_InitStructure.I2C_FmDutyCycle = I2C_FmDutyCycle_2_1; /* I2C总线占空比 */
    I2C_InitStructure.I2C_AddrMode = I2C_AddrMode_7bit;      /* 7位地址模式 */
    I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;              /* 使能应答 */
    I2C_Init(I2C2, &I2C_InitStructure);
    /* 使能I2C2 */
    I2C_Cmd(I2C2, ENABLE);
#endif
}

/*
*********************************************************************************************************
*	函 数 名: bsp_I2C_ReadBuffer
*	功能说明: I2C总线读取数据
*	形    参: 
*	          I2Cx: I2C设备
*	          ucSlaveAddr: 从机地址
*	          pByteBuf: 数据缓冲
*	          usDataLen: 数据长度
*	返 回 值: 状态
*********************************************************************************************************
*/
I2C_StatusType bsp_I2C_ReadBuffer(I2C_Type *I2Cx, uint8_t ucSlaveAddr, uint8_t *pByteBuf, uint16_t usDataLen)
{
    /* 等待I2C总线空闲标志 */
    if (I2C_WaitOnFlagUntilTimeout(I2Cx, I2C_FLAG_BUSYF, SET, I2C_EVT_CHECK_NONE, I2C_TIMEOUT_SET) != I2C_OK)
    {
        return I2C_ERROR_STEP_1;
    }

    /* 禁止POS */
    I2C_NACKPositionConfig(I2Cx, I2C_NACKPosition_Current);
    /* 使能应答 */
    I2C_AcknowledgeConfig(I2Cx, ENABLE);

    /* 起始位 */
    I2C_GenerateSTART(I2Cx, ENABLE);
    /* 等待操作标志 */
    if (I2C_WaitOnFlagUntilTimeout(I2Cx, I2C_FLAG_STARTF, RESET, I2C_EVT_CHECK_NONE, I2C_TIMEOUT_SET) != I2C_OK)
    {
        /* 停止位 */
        I2C_GenerateSTOP(I2Cx, ENABLE);
        return I2C_ERROR_STEP_2;
    }

    /* 设备地址 */
    I2C_Send7bitAddress(I2Cx, ucSlaveAddr, I2C_Direction_Receive);
    /* 等待操作标志 */
    if (I2C_WaitOnFlagUntilTimeout(I2Cx, I2C_FLAG_ADDRF, RESET, I2C_EVT_CHECK_ACKFAIL, I2C_TIMEOUT_SET) != I2C_OK)
    {
        /* 停止位 */
        I2C_GenerateSTOP(I2Cx, ENABLE);
        return I2C_ERROR_STEP_3;
    }
    /* 清零ADDRF标志 */
    I2C_ClearADDRFlag(I2Cx);

    if (usDataLen == 1)
    {
        /* 禁止应答 */
        I2C_AcknowledgeConfig(I2Cx, DISABLE);
        /* 停止位 */
        I2C_GenerateSTOP(I2Cx, ENABLE);
    }
    else if (usDataLen == 2)
    {
        /*使能POS */
        I2C_NACKPositionConfig(I2Cx, I2C_NACKPosition_Next);
        /* 禁止应答 */
        I2C_AcknowledgeConfig(I2Cx, DISABLE);
    }
    else
    {
        /* 使能应答 */
        I2C_AcknowledgeConfig(I2Cx, ENABLE);
    }

    while (usDataLen)
    {
        if (usDataLen < 4)
        {
            if (usDataLen == 1)
            {
                /* 等待操作标志 */
                if (I2C_WaitOnFlagUntilTimeout(I2Cx, I2C_FLAG_RDNE, RESET, I2C_EVT_CHECK_NONE, I2C_TIMEOUT_SET) != I2C_OK)
                {
                    /* 停止位 */
                    I2C_GenerateSTOP(I2Cx, ENABLE);
                    return I2C_ERROR_STEP_4;
                }

                /* 读取数据 */
                (*pByteBuf++) = I2C_ReceiveData(I2Cx);
                usDataLen = 0;
            }
            else if (usDataLen == 2)
            {
                /* 等待操作标志 */
                if (I2C_WaitOnFlagUntilTimeout(I2Cx, I2C_FLAG_BTFF, RESET, I2C_EVT_CHECK_NONE, I2C_TIMEOUT_SET) != I2C_OK)
                {
                    /* 停止位 */
                    I2C_GenerateSTOP(I2Cx, ENABLE);
                    return I2C_ERROR_STEP_5;
                }

                /* 停止位 */
                I2C_GenerateSTOP(I2Cx, ENABLE);

                /* 读取数据 */
                (*pByteBuf++) = I2C_ReceiveData(I2Cx);
                /* 读取数据 */
                (*pByteBuf++) = I2C_ReceiveData(I2Cx);
                usDataLen = 0;
            }
            else
            {
                /* 等待操作标志 */
                if (I2C_WaitOnFlagUntilTimeout(I2Cx, I2C_FLAG_BTFF, RESET, I2C_EVT_CHECK_NONE, I2C_TIMEOUT_SET) != I2C_OK)
                {
                    /* 停止位 */
                    I2C_GenerateSTOP(I2Cx, ENABLE);
                    return I2C_ERROR_STEP_6;
                }

                /* 禁止应答 */
                I2C_AcknowledgeConfig(I2Cx, DISABLE);

                /* 读取数据 */
                (*pByteBuf++) = I2C_ReceiveData(I2Cx);

                /* 等待操作标志 */
                if (I2C_WaitOnFlagUntilTimeout(I2Cx, I2C_FLAG_BTFF, RESET, I2C_EVT_CHECK_NONE, I2C_TIMEOUT_SET) != I2C_OK)
                {
                    /* 停止位 */
                    I2C_GenerateSTOP(I2Cx, ENABLE);
                    return I2C_ERROR_STEP_7;
                }

                /* 停止位 */
                I2C_GenerateSTOP(I2Cx, ENABLE);

                /* 读取数据 */
                (*pByteBuf++) = I2C_ReceiveData(I2Cx);
                /* 读取数据 */
                (*pByteBuf++) = I2C_ReceiveData(I2Cx);
                usDataLen = 0;
            }
        }
        else
        {
            /* 等待操作标志 */
            if (I2C_WaitOnFlagUntilTimeout(I2Cx, I2C_FLAG_RDNE, RESET, I2C_EVT_CHECK_NONE, I2C_TIMEOUT_SET) != I2C_OK)
            {
                /* 停止位 */
                I2C_GenerateSTOP(I2Cx, ENABLE);
                return I2C_ERROR_STEP_8;
            }

            /* 读取数据 */
            (*pByteBuf++) = I2C_ReceiveData(I2Cx);
            usDataLen--;
        }
    }
    return I2C_OK;
}

/*
*********************************************************************************************************
*	函 数 名: bsp_I2C_SendBuffer
*	功能说明: I2C总线发送数据
*	形    参: 
*	          I2Cx: I2C设备
*	          ucSlaveAddr: 从机地址
*	          pByteBuf: 数据缓冲
*	          usDataLen: 数据长度
*	返 回 值: 状态
*********************************************************************************************************
*/
I2C_StatusType bsp_I2C_SendBuffer(I2C_Type *I2Cx, uint8_t ucSlaveAddr, uint8_t *pByteBuf, uint16_t usDataLen)
{
    /* 等待I2C总线空闲标志 */
    if (I2C_WaitOnFlagUntilTimeout(I2Cx, I2C_FLAG_BUSYF, SET, I2C_EVT_CHECK_NONE, I2C_TIMEOUT_SET) != I2C_OK)
    {
        return I2C_ERROR_STEP_1;
    }

    /* 禁止POS */
    I2C_NACKPositionConfig(I2Cx, I2C_NACKPosition_Current);

    /* 起始位 */
    I2C_GenerateSTART(I2Cx, ENABLE);
    /* 等待操作标志 */
    if (I2C_WaitOnFlagUntilTimeout(I2Cx, I2C_FLAG_STARTF, RESET, I2C_EVT_CHECK_NONE, I2C_TIMEOUT_SET) != I2C_OK)
    {
        /* 停止位 */
        I2C_GenerateSTOP(I2Cx, ENABLE);
        return I2C_ERROR_STEP_2;
    }

    /* 设备地址 */
    I2C_Send7bitAddress(I2Cx, ucSlaveAddr, I2C_Direction_Transmit);
    /* 等待操作标志 */
    if (I2C_WaitOnFlagUntilTimeout(I2Cx, I2C_FLAG_ADDRF, RESET, I2C_EVT_CHECK_ACKFAIL, I2C_TIMEOUT_SET) != I2C_OK)
    {
        /* 停止位 */
        I2C_GenerateSTOP(I2Cx, ENABLE);
        return I2C_ERROR_STEP_3;
    }

    /* 清零ADDRF标志 */
    I2C_ClearADDRFlag(I2Cx);

    while (usDataLen)
    {
        /* 等待发送标志 */
        if (I2C_WaitOnFlagUntilTimeout(I2Cx, I2C_FLAG_TDE, RESET, I2C_EVT_CHECK_ACKFAIL, I2C_TIMEOUT_SET) != I2C_OK)
        {
            /* 停止位 */
            I2C_GenerateSTOP(I2Cx, ENABLE);
            return I2C_ERROR_STEP_4;
        }
        /* 发送数据 */
        I2C_SendData(I2Cx, (*pByteBuf++));
        usDataLen--;
    }

    /* 等待操作标志 */
    if (I2C_WaitOnFlagUntilTimeout(I2Cx, I2C_FLAG_BTFF, RESET, I2C_EVT_CHECK_ACKFAIL, I2C_TIMEOUT_SET) != I2C_OK)
    {
        /* 停止位 */
        I2C_GenerateSTOP(I2Cx, ENABLE);
        return I2C_ERROR_STEP_5;
    }

    /* 停止位 */
    I2C_GenerateSTOP(I2Cx, ENABLE);
    return I2C_OK;
}

/*
*********************************************************************************************************
*	函 数 名: bsp_I2C_ReadAddrBuffer
*	功能说明: I2C总线读取指定地址数据
*	形    参: 
*	          I2Cx: I2C设备 
*	          ucSlaveAddr: 从机地址 
*	          pDataAddr: 数据地址 
*	          ucAddrLen: 地址长度 
*	          pByteBuf: 数据缓冲 
*	          usDataLen: 数据长度
*	返 回 值: 状态
*********************************************************************************************************
*/
I2C_StatusType bsp_I2C_ReadAddrBuffer(I2C_Type *I2Cx, uint8_t ucSlaveAddr, uint8_t *pDataAddr, uint8_t ucAddrLen, uint8_t *pByteBuf, uint16_t usDataLen)
{
    /* 等待I2C总线空闲标志 */
    if (I2C_WaitOnFlagUntilTimeout(I2Cx, I2C_FLAG_BUSYF, SET, I2C_EVT_CHECK_NONE, I2C_TIMEOUT_SET) != I2C_OK)
    {
        return I2C_ERROR_STEP_1;
    }

    /* 禁止POS */
    I2C_NACKPositionConfig(I2Cx, I2C_NACKPosition_Current);

    /* 起始位*/
    I2C_GenerateSTART(I2Cx, ENABLE);
    /* 等待操作标志 */
    if (I2C_WaitOnFlagUntilTimeout(I2Cx, I2C_FLAG_STARTF, RESET, I2C_EVT_CHECK_NONE, I2C_TIMEOUT_SET) != I2C_OK)
    {
        /* 停止位 */
        I2C_GenerateSTOP(I2Cx, ENABLE);
        return I2C_ERROR_STEP_2;
    }

    /* 发送从机地址 */
    I2C_Send7bitAddress(I2Cx, ucSlaveAddr, I2C_Direction_Transmit);
    /* 等待操作标志 */
    if (I2C_WaitOnFlagUntilTimeout(I2Cx, I2C_FLAG_ADDRF, RESET, I2C_EVT_CHECK_ACKFAIL, I2C_TIMEOUT_SET) != I2C_OK)
    {
        /* 停止位 */
        I2C_GenerateSTOP(I2Cx, ENABLE);
        return I2C_ERROR_STEP_3;
    }

    /* 清零ADDRF标志 */
    I2C_ClearADDRFlag(I2Cx);

    /* 数据地址 */
    while (ucAddrLen)
    {
        /* 等待操作标志 */
        if (I2C_WaitOnFlagUntilTimeout(I2Cx, I2C_FLAG_TDE, RESET, I2C_EVT_CHECK_ACKFAIL, I2C_TIMEOUT_SET) != I2C_OK)
        {
            /* 停止位 */
            I2C_GenerateSTOP(I2Cx, ENABLE);
            return I2C_ERROR_STEP_4;
        }
        /* 发送数据 */
        I2C_SendData(I2Cx, (*pDataAddr++));
        ucAddrLen--;
    }

    /* 发送起始信号 */
    I2C_GenerateSTART(I2Cx, ENABLE);
    /* 等待操作标志 */
    if (I2C_WaitOnFlagUntilTimeout(I2Cx, I2C_FLAG_STARTF, RESET, I2C_EVT_CHECK_NONE, I2C_TIMEOUT_SET) != I2C_OK)
    {
        /* 停止位 */
        I2C_GenerateSTOP(I2Cx, ENABLE);
        return I2C_ERROR_STEP_5;
    }

    /* 发送从机地址-读 */
    I2C_Send7bitAddress(I2Cx, ucSlaveAddr, I2C_Direction_Receive);
    /* 等待操作标志 */
    if (I2C_WaitOnFlagUntilTimeout(I2Cx, I2C_FLAG_ADDRF, RESET, I2C_EVT_CHECK_ACKFAIL, I2C_TIMEOUT_SET) != I2C_OK)
    {
        /* 停止位 */
        I2C_GenerateSTOP(I2Cx, ENABLE);
        return I2C_ERROR_STEP_6;
    }

    /* 清零ADDRF标志 */
    I2C_ClearADDRFlag(I2Cx);

    if (usDataLen == 1)
    {
        /* 禁止应答 */
        I2C_AcknowledgeConfig(I2Cx, DISABLE);
        /* 停止位 */
        I2C_GenerateSTOP(I2Cx, ENABLE);
    }
    else if (usDataLen == 2)
    {
        /* 使能POS */
        I2C_NACKPositionConfig(I2Cx, I2C_NACKPosition_Next);
        /* 禁止应答 */
        I2C_AcknowledgeConfig(I2Cx, DISABLE);
    }
    else
    {
        /* 使能应答 */
        I2C_AcknowledgeConfig(I2Cx, ENABLE);
    }
    /* 读取数据 */
    while (usDataLen)
    {
        if (usDataLen < 4)
        {
            if (usDataLen == 1)
            {
                /* 等待操作标志 */
                if (I2C_WaitOnFlagUntilTimeout(I2Cx, I2C_FLAG_RDNE, RESET, I2C_EVT_CHECK_NONE, I2C_TIMEOUT_SET) != I2C_OK)
                {
                    /* 停止位 */
                    I2C_GenerateSTOP(I2Cx, ENABLE);
                    return I2C_ERROR_STEP_7;
                }

                /* 读取数据 */
                (*pByteBuf++) = I2C_ReceiveData(I2Cx);
                usDataLen = 0;
            }
            else if (usDataLen == 2)
            {
                /* 等待操作标志 */
                if (I2C_WaitOnFlagUntilTimeout(I2Cx, I2C_FLAG_BTFF, RESET, I2C_EVT_CHECK_NONE, I2C_TIMEOUT_SET) != I2C_OK)
                {
                    /* 停止位 */
                    I2C_GenerateSTOP(I2Cx, ENABLE);
                    return I2C_ERROR_STEP_8;
                }

                /* 停止位 */
                I2C_GenerateSTOP(I2Cx, ENABLE);

                /* 读取数据 */
                (*pByteBuf++) = I2C_ReceiveData(I2Cx);
                /* 读取数据 */
                (*pByteBuf++) = I2C_ReceiveData(I2Cx);
                usDataLen = 0;
            }
            else
            {
                /* 等待操作标志 */
                if (I2C_WaitOnFlagUntilTimeout(I2Cx, I2C_FLAG_BTFF, RESET, I2C_EVT_CHECK_NONE, I2C_TIMEOUT_SET) != I2C_OK)
                {
                    /* 停止位 */
                    I2C_GenerateSTOP(I2Cx, ENABLE);
                    return I2C_ERROR_STEP_9;
                }

                /* 禁止应答 */
                I2C_AcknowledgeConfig(I2Cx, DISABLE);

                /* 读取数据 */
                (*pByteBuf++) = I2C_ReceiveData(I2Cx);

                /* 等待操作标志 */
                if (I2C_WaitOnFlagUntilTimeout(I2Cx, I2C_FLAG_BTFF, RESET, I2C_EVT_CHECK_NONE, I2C_TIMEOUT_SET) != I2C_OK)
                {
                    /* 停止位 */
                    I2C_GenerateSTOP(I2Cx, ENABLE);
                    return I2C_ERROR_STEP_10;
                }

                /* 停止位 */
                I2C_GenerateSTOP(I2Cx, ENABLE);

                /* 读取数据 */
                (*pByteBuf++) = I2C_ReceiveData(I2Cx);
                /* 读取数据 */
                (*pByteBuf++) = I2C_ReceiveData(I2Cx);
                usDataLen = 0;
            }
        }
        else
        {
            /* 等待操作标志 */
            if (I2C_WaitOnFlagUntilTimeout(I2Cx, I2C_FLAG_RDNE, RESET, I2C_EVT_CHECK_NONE, I2C_TIMEOUT_SET) != I2C_OK)
            {
                /* 停止位 */
                I2C_GenerateSTOP(I2Cx, ENABLE);
                return I2C_ERROR_STEP_11;
            }

            /* 读取数据 */
            (*pByteBuf++) = I2C_ReceiveData(I2Cx);
            usDataLen--;
        }
    }
    return I2C_OK;
}

/*
*********************************************************************************************************
*	函 数 名: bsp_I2C_SendAddrBuffer
*	功能说明: I2C总线发送指定地址数据
*	形    参: 
*	          I2Cx: I2C设备 
*	          ucSlaveAddr: 从机地址 
*	          pDataAddr: 数据地址 
*	          ucAddrLen: 地址长度 
*	          pByteBuf: 数据缓冲 
*	          usDataLen: 数据长度
*	返 回 值: 状态
*********************************************************************************************************
*/
I2C_StatusType bsp_I2C_SendAddrBuffer(I2C_Type *I2Cx, uint8_t ucSlaveAddr, uint8_t *pDataAddr, uint8_t ucAddrLen, uint8_t *pByteBuf, uint16_t usDataLen)
{
    /* 等待I2C总线空闲标志 */
    if (I2C_WaitOnFlagUntilTimeout(I2Cx, I2C_FLAG_BUSYF, SET, I2C_EVT_CHECK_NONE, I2C_TIMEOUT_SET) != I2C_OK)
    {
        return I2C_ERROR_STEP_1;
    }

    /* 禁止POS */
    I2C_NACKPositionConfig(I2Cx, I2C_NACKPosition_Current);

    /* 起始位 */
    I2C_GenerateSTART(I2Cx, ENABLE);
    /* 等待操作标志 */
    if (I2C_WaitOnFlagUntilTimeout(I2Cx, I2C_FLAG_STARTF, RESET, I2C_EVT_CHECK_NONE, I2C_TIMEOUT_SET) != I2C_OK)
    {
        /* 停止位 */
        I2C_GenerateSTOP(I2Cx, ENABLE);
        return I2C_ERROR_STEP_2;
    }

    /* 设备地址 */
    I2C_Send7bitAddress(I2Cx, ucSlaveAddr, I2C_Direction_Transmit);
    /* 等待操作标志 */
    if (I2C_WaitOnFlagUntilTimeout(I2Cx, I2C_FLAG_ADDRF, RESET, I2C_EVT_CHECK_ACKFAIL, I2C_TIMEOUT_SET) != I2C_OK)
    {
        /* 停止位 */
        I2C_GenerateSTOP(I2Cx, ENABLE);
        return I2C_ERROR_STEP_3;
    }

    /* 清零ADDRF标志 */
    I2C_ClearADDRFlag(I2Cx);

    /* 地址 */
    while (ucAddrLen)
    {
        /* 等待发送标志 */
        if (I2C_WaitOnFlagUntilTimeout(I2Cx, I2C_FLAG_TDE, RESET, I2C_EVT_CHECK_ACKFAIL, I2C_TIMEOUT_SET) != I2C_OK)
        {
            /* 停止位 */
            I2C_GenerateSTOP(I2Cx, ENABLE);
            return I2C_ERROR_STEP_4;
        }
        /* 发送数据 */
        I2C_SendData(I2Cx, (*pDataAddr++));
        ucAddrLen--;
    }
    /* 数据 */
    while (usDataLen)
    {
        /* 等待发送标志 */
        if (I2C_WaitOnFlagUntilTimeout(I2Cx, I2C_FLAG_TDE, RESET, I2C_EVT_CHECK_ACKFAIL, I2C_TIMEOUT_SET) != I2C_OK)
        {
            /* 停止位 */
            I2C_GenerateSTOP(I2Cx, ENABLE);
            return I2C_ERROR_STEP_5;
        }
        /* 发送数据 */
        I2C_SendData(I2Cx, (*pByteBuf++));
        usDataLen--;
    }

    /* 等待操作标志 */
    if (I2C_WaitOnFlagUntilTimeout(I2Cx, I2C_FLAG_BTFF, RESET, I2C_EVT_CHECK_ACKFAIL, I2C_TIMEOUT_SET) != I2C_OK)
    {
        /* 停止位 */
        I2C_GenerateSTOP(I2Cx, ENABLE);
        return I2C_ERROR_STEP_6;
    }

    /* 停止位 */
    I2C_GenerateSTOP(I2Cx, ENABLE);
    return I2C_OK;
}

/********************************************* End of file **********************************************/
