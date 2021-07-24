/*
*********************************************************************************************************
*
*	��������: SPI��������ģ��
*	�ļ�����: bsp_spi.c
*	��    ��: v1.0.0
*   ��    д: Ī����
*	˵    ��: SPI��������
*
*	�汾��¼: 
*	v1.0.0: 2021��3��31�գ�����
*
*	Copyright (C), 2021-2031, �����з����ƿƼ����޹�˾ https://fxymcu.taobao.com
*
*********************************************************************************************************
*/

#include "bsp.h"

/* SPI1��GPIO�ֲ�
NSS:  PA4 PA15
SCK:  PA5 PB3
MISO: PA6 PB4
MOSI: PA7 PB5
*/
/* Ƭѡ */
#define SPI1_NSS_PORT GPIOA
#define SPI1_NSS_PIN GPIO_Pins_4
/* ʱ�� */
#define SPI1_SCK_PORT GPIOA
#define SPI1_SCK_PIN GPIO_Pins_5
#define SPI1_SCK_SOURCE GPIO_PinsSource5
#define SPI1_SCK_AF GPIO_AF_0
/* ����ӳ� */
#define SPI1_MISO_PORT GPIOA
#define SPI1_MISO_PIN GPIO_Pins_6
#define SPI1_MISO_SOURCE GPIO_PinsSource6
#define SPI1_MISO_AF GPIO_AF_0
/* �������� */
#define SPI1_MOSI_PORT GPIOA
#define SPI1_MOSI_PIN GPIO_Pins_7
#define SPI1_MOSI_SOURCE GPIO_PinsSource7
#define SPI1_MOSI_AF GPIO_AF_0
/* GPIOʱ�� */
#define SPI1_GPIO_RCC RCC_AHBPERIPH_GPIOA

/* �����궨�� */
#define SPI1_NSS_1() (SPI1_NSS_PORT->BSRE = SPI1_NSS_PIN)
#define SPI1_NSS_0() (SPI1_NSS_PORT->BRE = SPI1_NSS_PIN)

#define SPI1_SCK_1() (SPI1_SCK_PORT->BSRE = SPI1_SCK_PIN)
#define SPI1_SCK_0() (SPI1_SCK_PORT->BRE = SPI1_SCK_PIN)

#define SPI1_MISO_READ() (SPI1_MISO_PORT->IPTDT & SPI1_MISO_PIN)

#define SPI1_MOSI_1() (SPI1_MOSI_PORT->BSRE = SPI1_MOSI_PIN)
#define SPI1_MOSI_0() (SPI1_MOSI_PORT->BRE = SPI1_MOSI_PIN)

/*
SPI����ģʽ
CPOL: �涨��SCKʱ���źſ���״̬�ĵ�ƽ
CPHA: �涨����������SCKʱ�ӵ�ǰһ��ʱ���ػ��Ǻ�һ��ʱ���ر�����

ģʽ0: CPOL = 0��CPHA = 0 SCK����Ϊ�͵�ƽ��������ǰһ��ʱ������Ч(����ȡ)
ģʽ1: CPOL = 0��CPHA = 1 SCK����Ϊ�͵�ƽ�������ں�һ��ʱ������Ч(����ȡ)
ģʽ2: CPOL = 1��CPHA = 0 SCK����Ϊ�ߵ�ƽ��������ǰһ��ʱ������Ч(����ȡ)
ģʽ3: CPOL = 1��CPHA = 1 SCK����Ϊ�ߵ�ƽ�������ں�һ��ʱ������Ч(����ȡ)
*/
#define SPI1_MODE 0

/* SPI2��GPIO�ֲ�
NSS:  PB12 PA15 PB9
SCK:  PB10 PB13 PB3 PB1
MISO: PB14 PA13 PB4
MOSI: PB15 PA14 PB5
*/
/* Ƭѡ */
#define SPI2_NSS_PORT GPIOB
#define SPI2_NSS_PIN GPIO_Pins_12
/* ʱ�� */
#define SPI2_SCK_PORT GPIOB
#define SPI2_SCK_PIN GPIO_Pins_13
#define SPI2_SCK_SOURCE GPIO_PinsSource13
#define SPI2_SCK_AF GPIO_AF_0
/* ����ӳ� */
#define SPI2_MISO_PORT GPIOB
#define SPI2_MISO_PIN GPIO_Pins_14
#define SPI2_MISO_SOURCE GPIO_PinsSource14
#define SPI2_MISO_AF GPIO_AF_0
/* �������� */
#define SPI2_MOSI_PORT GPIOB
#define SPI2_MOSI_PIN GPIO_Pins_15
#define SPI2_MOSI_SOURCE GPIO_PinsSource15
#define SPI2_MOSI_AF GPIO_AF_0
/* GPIOʱ�� */
#define SPI2_GPIO_RCC RCC_AHBPERIPH_GPIOB

/* �����궨�� */
#define SPI2_NSS_1() (SPI2_NSS_PORT->BSRE = SPI2_NSS_PIN)
#define SPI2_NSS_0() (SPI2_NSS_PORT->BRE = SPI2_NSS_PIN)

#define SPI2_SCK_1() (SPI2_SCK_PORT->BSRE = SPI2_SCK_PIN)
#define SPI2_SCK_0() (SPI2_SCK_PORT->BRE = SPI2_SCK_PIN)

#define SPI2_MISO_READ() (SPI2_MISO_PORT->IPTDT & SPI2_MISO_PIN)

#define SPI2_MOSI_1() (SPI2_MOSI_PORT->BSRE = SPI2_MOSI_PIN)
#define SPI2_MOSI_0() (SPI2_MOSI_PORT->BRE = SPI2_MOSI_PIN)

/*
SPI����ģʽ
CPOL: �涨��SCKʱ���źſ���״̬�ĵ�ƽ
CPHA: �涨����������SCKʱ�ӵ�ǰһ��ʱ���ػ��Ǻ�һ��ʱ���ر�����

ģʽ0: CPOL = 0��CPHA = 0 SCK����Ϊ�͵�ƽ��������ǰһ��ʱ������Ч(����ȡ)
ģʽ1: CPOL = 0��CPHA = 1 SCK����Ϊ�͵�ƽ�������ں�һ��ʱ������Ч(����ȡ)
ģʽ2: CPOL = 1��CPHA = 0 SCK����Ϊ�ߵ�ƽ��������ǰһ��ʱ������Ч(����ȡ)
ģʽ3: CPOL = 1��CPHA = 1 SCK����Ϊ�ߵ�ƽ�������ں�һ��ʱ������Ч(����ȡ)
*/
#define SPI2_MODE 0

/*
*********************************************************************************************************
*	�� �� ��: bsp_InitSPI
*	����˵��: ����SPI
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_InitSPI(void)
{
    GPIO_InitType GPIO_InitStructure;
    SPI_InitType SPI_InitStructure;

#if SPI1_EN
    /* ʹ��GPIOʱ�� */
    RCC_AHBPeriphClockCmd(SPI1_GPIO_RCC, ENABLE);
    /* ʹ��SPI1ʱ�� */
    RCC_APB2PeriphClockCmd(RCC_APB2PERIPH_SPI1, ENABLE);

    /* ���õ�ƽ-��ֹƬѡ */
    GPIO_SetBits(SPI1_NSS_PORT, SPI1_NSS_PIN);

    /* ����SPI1 GPIO*/
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

    /* ���ø��ù��� */
    GPIO_PinAFConfig(SPI1_SCK_PORT, SPI1_SCK_SOURCE, SPI1_SCK_AF);
    GPIO_PinAFConfig(SPI1_MISO_PORT, SPI1_MISO_SOURCE, SPI1_MISO_AF);
    GPIO_PinAFConfig(SPI1_MOSI_PORT, SPI1_MOSI_SOURCE, SPI1_MOSI_AF);

    /* ����SPI1ģʽ */
    SPI_DefaultInitParaConfig(&SPI_InitStructure);
    SPI_InitStructure.SPI_TransMode = SPI_TRANSMODE_FULLDUPLEX; /* ȫ˫�� */
    SPI_InitStructure.SPI_Mode = SPI_MODE_MASTER;               /* ���� */
    SPI_InitStructure.SPI_FrameSize = SPI_FRAMESIZE_8BIT;       /* 8λ */
#if (SPI1_MODE == 0 || SPI1_MODE == 1)
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_LOW; /* ʱ�ӿ���Ϊ�͵�ƽ */
#else
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_HIGH;  /* ʱ�ӿ���Ϊ�ߵ�ƽ */
#endif
#if (SPI1_MODE == 0 || SPI1_MODE == 2)
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1EDGE; /* ������ǰһ��ʱ������Ч(����ȡ) */
#else
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_2EDGE; /* �����ں�һ��ʱ������Ч(����ȡ) */
#endif
    SPI_InitStructure.SPI_NSSSEL = SPI_NSSSEL_SOFT;    /* Ƭѡ��������� */
    SPI_InitStructure.SPI_MCLKP = SPI_MCLKP_2;         /* ʱ�� */
    SPI_InitStructure.SPI_FirstBit = SPI_FIRSTBIT_MSB; /* ��λ���� */
    SPI_InitStructure.SPI_CPOLY = 7;                   /* CRC����Ķ���ʽ */
    SPI_Init(SPI1, &SPI_InitStructure);

    /* ʹ��SPI1 */
    SPI_Enable(SPI1, ENABLE);
#endif

#if SPI2_EN
    /* ʹ��GPIOʱ�� */
    RCC_AHBPeriphClockCmd(SPI2_GPIO_RCC, ENABLE);
    /* ʹ��SPI2ʱ�� */
    RCC_APB1PeriphClockCmd(RCC_APB1PERIPH_SPI2, ENABLE);

    /* ���õ�ƽ-��ֹƬѡ */
    GPIO_SetBits(SPI2_NSS_PORT, SPI2_NSS_PIN);

    /* ����SPI2 GPIO*/
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

    /* ���ø��ù��� */
    GPIO_PinAFConfig(SPI2_SCK_PORT, SPI2_SCK_SOURCE, SPI2_SCK_AF);
    GPIO_PinAFConfig(SPI2_MISO_PORT, SPI2_MISO_SOURCE, SPI2_MISO_AF);
    GPIO_PinAFConfig(SPI2_MOSI_PORT, SPI2_MOSI_SOURCE, SPI2_MOSI_AF);

    /* ����SPI2ģʽ */
    SPI_DefaultInitParaConfig(&SPI_InitStructure);
    SPI_InitStructure.SPI_TransMode = SPI_TRANSMODE_FULLDUPLEX; /* ȫ˫�� */
    SPI_InitStructure.SPI_Mode = SPI_MODE_MASTER;               /* ���� */
    SPI_InitStructure.SPI_FrameSize = SPI_FRAMESIZE_8BIT;       /* 8λ */
#if (SPI2_MODE == 0 || SPI2_MODE == 1)
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_LOW; /* ʱ�ӿ���Ϊ�͵�ƽ */
#else
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_HIGH;  /* ʱ�ӿ���Ϊ�ߵ�ƽ */
#endif
#if (SPI2_MODE == 0 || SPI2_MODE == 2)
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1EDGE; /* ������ǰһ��ʱ������Ч(����ȡ) */
#else
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_2EDGE; /* �����ں�һ��ʱ������Ч(����ȡ) */
#endif
    SPI_InitStructure.SPI_NSSSEL = SPI_NSSSEL_SOFT;    /* Ƭѡ��������� */
    SPI_InitStructure.SPI_MCLKP = SPI_MCLKP_2;         /* ʱ�� */
    SPI_InitStructure.SPI_FirstBit = SPI_FIRSTBIT_MSB; /* ��λ���� */
    SPI_InitStructure.SPI_CPOLY = 7;                   /* CRC����Ķ���ʽ */
    SPI_Init(SPI2, &SPI_InitStructure);

    /* ʹ��SPI2 */
    SPI_Enable(SPI2, ENABLE);
#endif
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_SPI_NSS
*	����˵��: SPIƬѡ
*	��    ��: 
*	          SPIx: SPI�����豸
*	          ucLevel: 1.�ߵ�ƽ 0.��ƽ
*	�� �� ֵ: ��
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
*	�� �� ��: bsp_SPI_Bus
*	����˵��: SPI�����շ�����
*	��    ��:
*	          SPIx: SPI�����豸
*	          ucSendByte: �����ֽ�
*	�� �� ֵ: ��ȡ�����ֽ�
*********************************************************************************************************
*/
uint8_t bsp_SPI_Bus(SPI_Type *SPIx, uint8_t ucSendByte)
{
    uint8_t ucReadByte;

    /* �ȴ�������� */
    while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TE) == RESET)
    {
    }
    /* �������� */
    SPI_I2S_TxData(SPIx, ucSendByte);
    /* �ȴ��������� */
    while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_RNE) == RESET)
    {
    }
    /* ��ȡ���� */
    ucReadByte = SPI_I2S_RxData(SPIx);

    return ucReadByte;
}

/********************************************* End of file **********************************************/
