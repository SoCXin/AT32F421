/*
*********************************************************************************************************
*
*	程序名称: SPI总线驱动模块
*	文件名称: bsp_spi.c
*	版    本: v1.0.0
*   编    写: 莫利奖
*	说    明: SPI总线驱动
*
*	版本记录: 
*	v1.0.0: 2021年3月31日，初版
*
*	Copyright (C), 2021-2031, 深圳市风巽云科技有限公司 https://fxymcu.taobao.com
*
*********************************************************************************************************
*/

#include "bsp.h"

/* SPI1的GPIO分布
NSS:  PA4 PA15
SCK:  PA5 PB3
MISO: PA6 PB4
MOSI: PA7 PB5
*/
/* 片选 */
#define SPI1_NSS_PORT GPIOA
#define SPI1_NSS_PIN GPIO_Pins_4
/* 时钟 */
#define SPI1_SCK_PORT GPIOA
#define SPI1_SCK_PIN GPIO_Pins_5
#define SPI1_SCK_SOURCE GPIO_PinsSource5
#define SPI1_SCK_AF GPIO_AF_0
/* 主入从出 */
#define SPI1_MISO_PORT GPIOA
#define SPI1_MISO_PIN GPIO_Pins_6
#define SPI1_MISO_SOURCE GPIO_PinsSource6
#define SPI1_MISO_AF GPIO_AF_0
/* 主出从入 */
#define SPI1_MOSI_PORT GPIOA
#define SPI1_MOSI_PIN GPIO_Pins_7
#define SPI1_MOSI_SOURCE GPIO_PinsSource7
#define SPI1_MOSI_AF GPIO_AF_0
/* GPIO时钟 */
#define SPI1_GPIO_RCC RCC_AHBPERIPH_GPIOA

/* 操作宏定义 */
#define SPI1_NSS_1() (SPI1_NSS_PORT->BSRE = SPI1_NSS_PIN)
#define SPI1_NSS_0() (SPI1_NSS_PORT->BRE = SPI1_NSS_PIN)

#define SPI1_SCK_1() (SPI1_SCK_PORT->BSRE = SPI1_SCK_PIN)
#define SPI1_SCK_0() (SPI1_SCK_PORT->BRE = SPI1_SCK_PIN)

#define SPI1_MISO_READ() (SPI1_MISO_PORT->IPTDT & SPI1_MISO_PIN)

#define SPI1_MOSI_1() (SPI1_MOSI_PORT->BSRE = SPI1_MOSI_PIN)
#define SPI1_MOSI_0() (SPI1_MOSI_PORT->BRE = SPI1_MOSI_PIN)

/*
SPI总线模式
CPOL: 规定了SCK时钟信号空闲状态的电平
CPHA: 规定了数据是在SCK时钟的前一个时钟沿还是后一个时钟沿被采样

模式0: CPOL = 0，CPHA = 0 SCK空闲为低电平，数据在前一个时钟沿有效(被读取)
模式1: CPOL = 0，CPHA = 1 SCK空闲为低电平，数据在后一个时钟沿有效(被读取)
模式2: CPOL = 1，CPHA = 0 SCK空闲为高电平，数据在前一个时钟沿有效(被读取)
模式3: CPOL = 1，CPHA = 1 SCK空闲为高电平，数据在后一个时钟沿有效(被读取)
*/
#define SPI1_MODE 0

/* SPI2的GPIO分布
NSS:  PB12 PA15 PB9
SCK:  PB10 PB13 PB3 PB1
MISO: PB14 PA13 PB4
MOSI: PB15 PA14 PB5
*/
/* 片选 */
#define SPI2_NSS_PORT GPIOB
#define SPI2_NSS_PIN GPIO_Pins_12
/* 时钟 */
#define SPI2_SCK_PORT GPIOB
#define SPI2_SCK_PIN GPIO_Pins_13
#define SPI2_SCK_SOURCE GPIO_PinsSource13
#define SPI2_SCK_AF GPIO_AF_0
/* 主入从出 */
#define SPI2_MISO_PORT GPIOB
#define SPI2_MISO_PIN GPIO_Pins_14
#define SPI2_MISO_SOURCE GPIO_PinsSource14
#define SPI2_MISO_AF GPIO_AF_0
/* 主出从入 */
#define SPI2_MOSI_PORT GPIOB
#define SPI2_MOSI_PIN GPIO_Pins_15
#define SPI2_MOSI_SOURCE GPIO_PinsSource15
#define SPI2_MOSI_AF GPIO_AF_0
/* GPIO时钟 */
#define SPI2_GPIO_RCC RCC_AHBPERIPH_GPIOB

/* 操作宏定义 */
#define SPI2_NSS_1() (SPI2_NSS_PORT->BSRE = SPI2_NSS_PIN)
#define SPI2_NSS_0() (SPI2_NSS_PORT->BRE = SPI2_NSS_PIN)

#define SPI2_SCK_1() (SPI2_SCK_PORT->BSRE = SPI2_SCK_PIN)
#define SPI2_SCK_0() (SPI2_SCK_PORT->BRE = SPI2_SCK_PIN)

#define SPI2_MISO_READ() (SPI2_MISO_PORT->IPTDT & SPI2_MISO_PIN)

#define SPI2_MOSI_1() (SPI2_MOSI_PORT->BSRE = SPI2_MOSI_PIN)
#define SPI2_MOSI_0() (SPI2_MOSI_PORT->BRE = SPI2_MOSI_PIN)

/*
SPI总线模式
CPOL: 规定了SCK时钟信号空闲状态的电平
CPHA: 规定了数据是在SCK时钟的前一个时钟沿还是后一个时钟沿被采样

模式0: CPOL = 0，CPHA = 0 SCK空闲为低电平，数据在前一个时钟沿有效(被读取)
模式1: CPOL = 0，CPHA = 1 SCK空闲为低电平，数据在后一个时钟沿有效(被读取)
模式2: CPOL = 1，CPHA = 0 SCK空闲为高电平，数据在前一个时钟沿有效(被读取)
模式3: CPOL = 1，CPHA = 1 SCK空闲为高电平，数据在后一个时钟沿有效(被读取)
*/
#define SPI2_MODE 0

/*
*********************************************************************************************************
*	函 数 名: bsp_InitSPI
*	功能说明: 配置SPI
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_InitSPI(void)
{
    GPIO_InitType GPIO_InitStructure;
    SPI_InitType SPI_InitStructure;

#if SPI1_EN
    /* 使能GPIO时钟 */
    RCC_AHBPeriphClockCmd(SPI1_GPIO_RCC, ENABLE);
    /* 使能SPI1时钟 */
    RCC_APB2PeriphClockCmd(RCC_APB2PERIPH_SPI1, ENABLE);

    /* 设置电平-禁止片选 */
    GPIO_SetBits(SPI1_NSS_PORT, SPI1_NSS_PIN);

    /* 配置SPI1 GPIO*/
    GPIO_InitStructure.GPIO_MaxSpeed = GPIO_MaxSpeed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OutType = GPIO_OutType_PP;
    /* SCK */
    GPIO_InitStructure.GPIO_Pins = SPI1_SCK_PIN;
    GPIO_Init(SPI1_SCK_PORT, &GPIO_InitStructure);
    /* MOSI */
    GPIO_InitStructure.GPIO_Pins = SPI1_MOSI_PIN;
    GPIO_Init(SPI1_MOSI_PORT, &GPIO_InitStructure);
    /* MISO */
    GPIO_InitStructure.GPIO_Pins = SPI1_MISO_PIN;
    GPIO_InitStructure.GPIO_Pull = GPIO_Pull_PD;
    GPIO_Init(SPI1_MISO_PORT, &GPIO_InitStructure);
    /* NSS */
    GPIO_InitStructure.GPIO_Pins = SPI1_NSS_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OutType = GPIO_OutType_PP;
    GPIO_Init(SPI1_NSS_PORT, &GPIO_InitStructure);

    /* 配置复用功能 */
    GPIO_PinAFConfig(SPI1_SCK_PORT, SPI1_SCK_SOURCE, SPI1_SCK_AF);
    GPIO_PinAFConfig(SPI1_MISO_PORT, SPI1_MISO_SOURCE, SPI1_MISO_AF);
    GPIO_PinAFConfig(SPI1_MOSI_PORT, SPI1_MOSI_SOURCE, SPI1_MOSI_AF);

    /* 配置SPI1模式 */
    SPI_DefaultInitParaConfig(&SPI_InitStructure);
    SPI_InitStructure.SPI_TransMode = SPI_TRANSMODE_FULLDUPLEX; /* 全双工 */
    SPI_InitStructure.SPI_Mode = SPI_MODE_MASTER;               /* 主机 */
    SPI_InitStructure.SPI_FrameSize = SPI_FRAMESIZE_8BIT;       /* 8位 */
#if (SPI1_MODE == 0 || SPI1_MODE == 1)
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_LOW; /* 时钟空闲为低电平 */
#else
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_HIGH;  /* 时钟空闲为高电平 */
#endif
#if (SPI1_MODE == 0 || SPI1_MODE == 2)
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1EDGE; /* 数据在前一个时钟沿有效(被读取) */
#else
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_2EDGE; /* 数据在后一个时钟沿有效(被读取) */
#endif
    SPI_InitStructure.SPI_NSSSEL = SPI_NSSSEL_SOFT;    /* 片选由软件控制 */
    SPI_InitStructure.SPI_MCLKP = SPI_MCLKP_2;         /* 时钟 */
    SPI_InitStructure.SPI_FirstBit = SPI_FIRSTBIT_MSB; /* 高位在先 */
    SPI_InitStructure.SPI_CPOLY = 7;                   /* CRC计算的多项式 */
    SPI_Init(SPI1, &SPI_InitStructure);

    /* 使能SPI1 */
    SPI_Enable(SPI1, ENABLE);
#endif

#if SPI2_EN
    /* 使能GPIO时钟 */
    RCC_AHBPeriphClockCmd(SPI2_GPIO_RCC, ENABLE);
    /* 使能SPI2时钟 */
    RCC_APB1PeriphClockCmd(RCC_APB1PERIPH_SPI2, ENABLE);

    /* 设置电平-禁止片选 */
    GPIO_SetBits(SPI2_NSS_PORT, SPI2_NSS_PIN);

    /* 配置SPI2 GPIO*/
    GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_MaxSpeed = GPIO_MaxSpeed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OutType = GPIO_OutType_PP;
    /* SCK */
    GPIO_InitStructure.GPIO_Pins = SPI2_SCK_PIN;
    GPIO_Init(SPI2_SCK_PORT, &GPIO_InitStructure);
    /* MOSI */
    GPIO_InitStructure.GPIO_Pins = SPI2_MOSI_PIN;
    GPIO_Init(SPI2_MOSI_PORT, &GPIO_InitStructure);
    /* MISO */
    GPIO_InitStructure.GPIO_Pins = SPI2_MISO_PIN;
    GPIO_InitStructure.GPIO_Pull = GPIO_Pull_PD;
    GPIO_Init(SPI2_MISO_PORT, &GPIO_InitStructure);
    /* NSS */
    GPIO_InitStructure.GPIO_Pins = SPI2_NSS_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OutType = GPIO_OutType_PP;
    GPIO_Init(SPI2_NSS_PORT, &GPIO_InitStructure);

    /* 配置复用功能 */
    GPIO_PinAFConfig(SPI2_SCK_PORT, SPI2_SCK_SOURCE, SPI2_SCK_AF);
    GPIO_PinAFConfig(SPI2_MISO_PORT, SPI2_MISO_SOURCE, SPI2_MISO_AF);
    GPIO_PinAFConfig(SPI2_MOSI_PORT, SPI2_MOSI_SOURCE, SPI2_MOSI_AF);

    /* 配置SPI2模式 */
    SPI_DefaultInitParaConfig(&SPI_InitStructure);
    SPI_InitStructure.SPI_TransMode = SPI_TRANSMODE_FULLDUPLEX; /* 全双工 */
    SPI_InitStructure.SPI_Mode = SPI_MODE_MASTER;               /* 主机 */
    SPI_InitStructure.SPI_FrameSize = SPI_FRAMESIZE_8BIT;       /* 8位 */
#if (SPI2_MODE == 0 || SPI2_MODE == 1)
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_LOW; /* 时钟空闲为低电平 */
#else
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_HIGH;  /* 时钟空闲为高电平 */
#endif
#if (SPI2_MODE == 0 || SPI2_MODE == 2)
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1EDGE; /* 数据在前一个时钟沿有效(被读取) */
#else
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_2EDGE; /* 数据在后一个时钟沿有效(被读取) */
#endif
    SPI_InitStructure.SPI_NSSSEL = SPI_NSSSEL_SOFT;    /* 片选由软件控制 */
    SPI_InitStructure.SPI_MCLKP = SPI_MCLKP_2;         /* 时钟 */
    SPI_InitStructure.SPI_FirstBit = SPI_FIRSTBIT_MSB; /* 高位在先 */
    SPI_InitStructure.SPI_CPOLY = 7;                   /* CRC计算的多项式 */
    SPI_Init(SPI2, &SPI_InitStructure);

    /* 使能SPI2 */
    SPI_Enable(SPI2, ENABLE);
#endif
}

/*
*********************************************************************************************************
*	函 数 名: bsp_SPI_NSS
*	功能说明: SPI片选
*	形    参: 
*	          SPIx: SPI总线设备
*	          ucLevel: 1.高电平 0.电平
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_SPI_NSS(SPI_Type *SPIx, uint8_t ucLevel)
{
    if (SPIx == SPI1)
    {
        if (ucLevel)
        {
            SPI1_NSS_1();
        }
        else
        {
            SPI1_NSS_0();
        }
    }
    else if (SPIx == SPI2)
    {
        if (ucLevel)
        {
            SPI2_NSS_1();
        }
        else
        {
            SPI2_NSS_0();
        }
    }
}

/*
*********************************************************************************************************
*	函 数 名: bsp_SPI_Bus
*	功能说明: SPI总线收发数据
*	形    参:
*	          SPIx: SPI总线设备
*	          ucSendByte: 发送字节
*	返 回 值: 读取到的字节
*********************************************************************************************************
*/
uint8_t bsp_SPI_Bus(SPI_Type *SPIx, uint8_t ucSendByte)
{
    uint8_t ucReadByte;

    /* 等待发送完成 */
    while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TE) == RESET)
    {
    }
    /* 发送数据 */
    SPI_I2S_TxData(SPIx, ucSendByte);
    /* 等待接收数据 */
    while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_RNE) == RESET)
    {
    }
    /* 读取数据 */
    ucReadByte = SPI_I2S_RxData(SPIx);

    return ucReadByte;
}

/********************************************* End of file **********************************************/
