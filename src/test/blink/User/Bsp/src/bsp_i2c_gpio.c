/*
*********************************************************************************************************
*
*	程序名称: I2C总线驱动模块
*	文件名称: bsp_i2c_gpio.c
*	版    本: v1.0.0
*   编    写: 莫利奖
*	说    明: GPIO模拟I2C总线
*
*	版本记录: 
*	v1.0.0: 2021年4月9日，初版
*
*	Copyright (C), 2021-2031, 深圳市风巽云科技有限公司 https://fxymcu.taobao.com
*
*********************************************************************************************************
*/

#include "bsp.h"

/* 驱动引脚定义 */
/* 时钟 */
#define I2C_SCL_PORT GPIOB
#define I2C_SCL_PIN GPIO_Pins_6
/* 数据 */
#define I2C_SDA_PORT GPIOB
#define I2C_SDA_PIN GPIO_Pins_7
/* GPIO时钟 */
#define I2C_GPIO_RCC RCC_AHBPERIPH_GPIOB

/* 操作宏定义 */
#define I2C_SCL_1() (I2C_SCL_PORT->BSRE = I2C_SCL_PIN)
#define I2C_SCL_0() (I2C_SCL_PORT->BRE = I2C_SCL_PIN)

#define I2C_SDA_1() (I2C_SDA_PORT->BSRE = I2C_SDA_PIN)
#define I2C_SDA_0() (I2C_SDA_PORT->BRE = I2C_SDA_PIN)

#define I2C_SCL_READ() (I2C_SCL_PORT->IPTDT & I2C_SCL_PIN)
#define I2C_SDA_READ() (I2C_SDA_PORT->IPTDT & I2C_SDA_PIN)

/*
*********************************************************************************************************
*	函 数 名：bsp_InitI2C_Gpio
*	功能说明：配置I2C总线的GPIO，采用模拟IO的方式实现
*	形    参：无
*	返 回 值：无
*********************************************************************************************************
*/
void bsp_InitI2C_Gpio(void)
{
    GPIO_InitType GPIO_InitStructure;

    /* 使能GPIO时钟 */
    RCC_AHBPeriphClockCmd(I2C_GPIO_RCC, ENABLE);

    /* 配置GPIO */
    GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OutType = GPIO_OutType_OD; /* 开漏输出，外置上拉 */
    GPIO_InitStructure.GPIO_MaxSpeed = GPIO_MaxSpeed_50MHz;
    /* SCL */
    GPIO_InitStructure.GPIO_Pins = I2C_SCL_PIN;
    GPIO_Init(I2C_SCL_PORT, &GPIO_InitStructure);
    /* SDA */
    GPIO_InitStructure.GPIO_Pins = I2C_SDA_PIN;
    GPIO_Init(I2C_SDA_PORT, &GPIO_InitStructure);

    /* 复位I2C总线上设备处于待机模式 */
    bsp_I2C_Stop();
}

/*
*********************************************************************************************************
*	函 数 名：I2C_Delay
*	功能说明：I2C总线位延迟
*			  系统时钟120MHz：
			  ucNop = 25  IIC时钟大概400KHz 
			  ucNop = 55  IIC时钟大概200KHz
*	形    参：无
*	返 回 值：无
*********************************************************************************************************
*/
static void I2C_Delay(void)
{
    uint8_t ucNop = 25;

    do
    {
/* Arm Compiler 5 */
#if defined(__CC_ARM)
        __nop();
/* IAR或者GCC */
#else
        asm("nop");
#endif
    } while (ucNop--);
}

/*
*********************************************************************************************************
*	函 数 名：bsp_I2C_Start
*	功能说明：CPU发起I2C总线启动信号
*	形    参：无
*	返 回 值：无
*********************************************************************************************************
*/
void bsp_I2C_Start(void)
{
    I2C_SDA_1();
    I2C_SCL_1();
    I2C_Delay();
    I2C_SDA_0();
    I2C_Delay();
    I2C_SCL_0();
    I2C_Delay();
}

/*
*********************************************************************************************************
*	函 数 名：bsp_I2C_Start
*	功能说明：CPU发起I2C总线停止信号
*	形    参：无
*	返 回 值：无
*********************************************************************************************************
*/
void bsp_I2C_Stop(void)
{
    I2C_SDA_0();
    I2C_Delay();
    I2C_SCL_1();
    I2C_Delay();
    I2C_SDA_1();
    I2C_Delay();
}

/*
*********************************************************************************************************
*	函 数 名：bsp_I2C_SendByte
*	功能说明：CPU向I2C总线设备发送8bit数据
*	形    参：ucByte：等待发送的字节
*	返 回 值：无
*********************************************************************************************************
*/
void bsp_I2C_SendByte(uint8_t ucByte)
{
    uint8_t i;

    /* 先高后低位 */
    for (i = 0; i < 8; i++)
    {
        if (ucByte & 0x80)
        {
            I2C_SDA_1();
        }
        else
        {
            I2C_SDA_0();
        }
        I2C_Delay();
        I2C_SCL_1();
        I2C_Delay();
        I2C_SCL_0();
        I2C_SCL_0();
        if (i == 7)
        {
            I2C_SDA_1();
        }
        ucByte <<= 1;
    }
}

/*
*********************************************************************************************************
*	函 数 名：bsp_I2C_ReadByte
*	功能说明：CPU从I2C总线设备读取8bit数据
*	形    参：无
*	返 回 值：读到的数据
*********************************************************************************************************
*/
uint8_t bsp_I2C_ReadByte(void)
{
    uint8_t i;
    uint8_t ucByte = 0;

    /* 读到第1个bit为数据的bit7 */
    for (i = 0; i < 8; i++)
    {
        ucByte <<= 1;
        I2C_SCL_1();
        I2C_Delay();
        if (I2C_SDA_READ())
        {
            ucByte++;
        }
        I2C_SCL_0();
        I2C_Delay();
    }
    return ucByte;
}

/*
*********************************************************************************************************
*	函 数 名：bsp_I2C_WaitAck
*	功能说明：CPU产生一个时钟，并读取器件的ACK应答信号
*	形    参：无
*	返 回 值：0.表示正确应答；1.表示无器件响应
*********************************************************************************************************
*/
uint8_t bsp_I2C_WaitAck(void)
{
    uint8_t re;

    I2C_SDA_1();
    I2C_Delay();
    I2C_SCL_1();
    I2C_Delay();
    if (I2C_SDA_READ())
    {
        re = 1;
    }
    else
    {
        re = 0;
    }
    I2C_SCL_0();
    I2C_Delay();
    return re;
}

/*
*********************************************************************************************************
*	函 数 名：bsp_I2C_Ack
*	功能说明：CPU产生一个ACK信号
*	形    参：无
*	返 回 值：无
*********************************************************************************************************
*/
void bsp_I2C_Ack(void)
{
    I2C_SDA_0();
    I2C_Delay();
    I2C_SCL_1();
    I2C_Delay();
    I2C_SCL_0();
    I2C_Delay();
    I2C_SDA_1();
    I2C_Delay();
}

/*
*********************************************************************************************************
*	函 数 名：bsp_I2C_NAck
*	功能说明：CPU产生1个NACK信号
*	形    参：无
*	返 回 值：无
*********************************************************************************************************
*/
void bsp_I2C_NAck(void)
{
    I2C_SDA_1();
    I2C_Delay();
    I2C_SCL_1();
    I2C_Delay();
    I2C_SCL_0();
    I2C_Delay();
}

/*
*********************************************************************************************************
*	函 数 名：bsp_I2C_CheckDevice
*	功能说明：发送设备地址，检测该设备是否存在
*	形    参：ucAddress：设备的I2C总线地址
*	返 回 值：1.设备不存在 0.设备存在
*********************************************************************************************************
*/
uint8_t bsp_I2C_CheckDevice(uint8_t ucAddress)
{
    uint8_t ucAck = 1;

    I2C_SDA_1();
    I2C_SCL_1();
    I2C_Delay();
    if (I2C_SDA_READ() && I2C_SCL_READ()) /* 总线空闲状态 */
    {
        bsp_I2C_Start();
        bsp_I2C_SendByte(ucAddress | I2C_WR);
        ucAck = bsp_I2C_WaitAck();
        bsp_I2C_Stop();
    }
    return ucAck;
}

/********************************************* End of file **********************************************/
