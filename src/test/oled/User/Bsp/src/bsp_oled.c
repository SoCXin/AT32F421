/*
*********************************************************************************************************
*
*	��������: OLEDģ��(SSD1306оƬ)����
*	�ļ�����: bsp_oled.c
*	��    ��: v1.0.0
*   ��    д: Ī����
*	˵    ��: ����ģ��ʵ�����������ͬ������
*
*	�汾��¼: 
*	v1.0.0: 2021��4��20�գ�����
*
*	Copyright (C), 2021-2031, �����з����ƿƼ����޹�˾ https://fxymcu.taobao.com
*
*********************************************************************************************************
*/

#include "bsp.h"
#include "font.h"

/* OLED������ʽ */
#define OLED_DRI_MODE 2 /* 0: GPIOģ��IIC 1: Ӳ��IIC 2: SPI */

/* ��λ */
#define OLED_RESET_PORT GPIOB
#define OLED_RESET_PIN GPIO_Pins_1
/* ����/���� */
#define OLED_DC_PORT GPIOB
#define OLED_DC_PIN GPIO_Pins_2
/* GPIOʱ�� */
#define OLED_GPIO_RCC RCC_AHBPERIPH_GPIOB

/* �����궨�� */
#define OLED_RESET_1() (OLED_RESET_PORT->BSRE = OLED_RESET_PIN)
#define OLED_RESET_0() (OLED_RESET_PORT->BRE = OLED_RESET_PIN)

#define OLED_DC_1() (OLED_DC_PORT->BSRE = OLED_DC_PIN)
#define OLED_DC_0() (OLED_DC_PORT->BRE = OLED_DC_PIN)

/* OLED��ʾ���� */
#define DIR_180 0 /* 0: �������� 1: ��ת180�� */

/*
*********************************************************************************************************
*	�� �� ��: bsp_InitOLEDGpio
*	����˵��: ����OLED��ص�GPIO
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_InitOLEDGpio(void)
{
    GPIO_InitType GPIO_InitStructure;

    /* ʹ��GPIOʱ�� */
    RCC_AHBPeriphClockCmd(OLED_GPIO_RCC, ENABLE);

    /* ���õ�ƽ */
    GPIO_ResetBits(OLED_RESET_PORT, OLED_RESET_PIN);
    GPIO_ResetBits(OLED_DC_PORT, OLED_DC_PIN);

    /* ����GPIO */
    GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OutType = GPIO_OutType_PP; /* ������� */
    GPIO_InitStructure.GPIO_MaxSpeed = GPIO_MaxSpeed_50MHz;
    /* RESET */
    GPIO_InitStructure.GPIO_Pins = OLED_RESET_PIN;
    GPIO_Init(OLED_RESET_PORT, &GPIO_InitStructure);
    /* DC */
    GPIO_InitStructure.GPIO_Pins = OLED_DC_PIN;
    GPIO_Init(OLED_DC_PORT, &GPIO_InitStructure);
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_OLED_WriteByte
*	����˵��: ��OLEDд��һ���ֽ�-IIC���߷�ʽ
*	��    ��: 
*	          ucData: ����
*	          ucCmd: ����
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void bsp_OLED_WriteByte(uint8_t ucData, uint8_t ucCmd)
{
#if OLED_DRI_MODE == 2
    bsp_SPI_NSS(SPI1, 0);
    if (ucCmd == OLED_DATA)
    {
        OLED_DC_1();
    }
    else
    {
        OLED_DC_0();
    }
    bsp_SPI_Bus(SPI1, ucData);
    bsp_SPI_NSS(SPI1, 1);
#elif OLED_DRI_MODE == 1
    uint8_t ucaByteBuf[2];

    ucaByteBuf[0] = ucCmd;
    ucaByteBuf[1] = ucData;
    bsp_I2C_SendBuffer(I2C1, OLED_ADDR, ucaByteBuf, 2);
#else
    bsp_I2C_Start();
    bsp_I2C_SendByte(OLED_ADDR);
    bsp_I2C_WaitAck();
    bsp_I2C_SendByte(ucCmd);
    bsp_I2C_WaitAck();
    bsp_I2C_SendByte(ucData);
    bsp_I2C_WaitAck();
    bsp_I2C_Stop();
#endif
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_OLED_SetPos
*	����˵��: ��������
*	��    ��: 
*			usX : X���꣨0-127��
*			usY : Y���꣨0-7��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void bsp_OLED_SetPos(uint8_t ucX, uint8_t ucY)
{
    bsp_OLED_WriteByte(0xb0 + ucY, OLED_CMD);                 /* ����ҳ��ַ */
    bsp_OLED_WriteByte(((ucX & 0xf0) >> 4) | 0x10, OLED_CMD); /* �����е�ַ�ĵ�4λ */
    bsp_OLED_WriteByte(ucX & 0x0f, OLED_CMD);                 /* �����е�ַ�ĸ�4λ */
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_OLED_DispOn
*	����˵��: ����ʾ
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_OLED_DispOn(void)
{
    bsp_OLED_WriteByte(0x8d, OLED_CMD); /* ��ɱ����� */
    bsp_OLED_WriteByte(0x14, OLED_CMD); /* �򿪳���*/
    bsp_OLED_WriteByte(0xaf, OLED_CMD); /* ��OLED��� */
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_OLED_DispOff
*	����˵��: �ر���ʾ
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_OLED_DispOff(void)
{
    bsp_OLED_WriteByte(0x8d, OLED_CMD); /* ��ɱ����� */
    bsp_OLED_WriteByte(0x10, OLED_CMD); /* �رճ��� */
    bsp_OLED_WriteByte(0xae, OLED_CMD); /* �ر�OLED��� */
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_OLED_SetContrast
*	����˵��: ���öԱȶ�
*	��    ��: ucValue: 0-255
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_OLED_SetContrast(uint8_t ucValue)
{
    bsp_OLED_WriteByte(0x81, OLED_CMD);    /* ���öԱȶ����� */
    bsp_OLED_WriteByte(ucValue, OLED_CMD); /* ���öԱȶȲ��� */
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_OLED_SetDir
*	����˵��: ������ʾ����
*	��    ��: ucDir = 1 ��ʾ��������0��ʾ��ת180��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_OLED_SetDir(uint8_t ucDir)
{
    if (ucDir)
    {
        bsp_OLED_WriteByte(0xa1, OLED_CMD); /* A0 : �е�ַ0ӳ�䵽SEG0; A1 : �е�ַ127ӳ�䵽SEG0 */
        bsp_OLED_WriteByte(0xc8, OLED_CMD); /* C0 : ����ɨ��,��COM0��COM63;  C8 : ����ɨ��, �� COM63�� COM0 */
    }
    else
    {
        bsp_OLED_WriteByte(0xa0, OLED_CMD); /* A0 : �е�ַ0ӳ�䵽SEG0; A1 : �е�ַ127ӳ�䵽SEG0 */
        bsp_OLED_WriteByte(0xc0, OLED_CMD); /* C0 : ����ɨ��,��COM0��COM63;  C8 : ����ɨ��, �� COM63�� COM0 */
    }
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_InitOLED
*	����˵��: ��ʼ��OLED��
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_InitOLED(void)
{
    /* ��ʼ��GPIO */
    bsp_InitOLEDGpio();
    /* ��λ */
    OLED_RESET_0();
    bsp_DelayMs(100);
    OLED_RESET_1();
    bsp_DelayMs(100);

    /* ����SSD1306оƬ��ʼ�� */
    bsp_OLED_WriteByte(0xae, OLED_CMD); /* �ر�OLED�����ʾ */

    bsp_OLED_WriteByte(0x00, OLED_CMD); /* �����е�ַ��4bit */
    bsp_OLED_WriteByte(0x10, OLED_CMD); /* �����е�ַ��4bit */
    bsp_OLED_WriteByte(0x40, OLED_CMD); /* ������ʼ�е�ַ����5bit 0-63�������� */
    bsp_OLED_WriteByte(0xb0, OLED_CMD); /* ����ҳ��ַ */

    bsp_OLED_WriteByte(0x20, OLED_CMD); /* �ڴ�Ѱַģʽ���� */
    bsp_OLED_WriteByte(0x02, OLED_CMD); /* [1:0]: 00.�е�ַģʽ; 01.�е�ַģʽ; 10.ҳ��ַģʽ; Ĭ��ҳ��ַ */

    bsp_OLED_WriteByte(0x81, OLED_CMD); /* ���öԱȶ����� */
    bsp_OLED_WriteByte(0xcf, OLED_CMD); /* ���öԱȶȲ�����ȱʡCF */

#if DIR_180
    bsp_OLED_WriteByte(0xa0, OLED_CMD); /* A0 : �е�ַ0ӳ�䵽SEG0; A1 : �е�ַ127ӳ�䵽SEG0 */
    bsp_OLED_WriteByte(0xc0, OLED_CMD); /* C0 : ����ɨ��,��COM0��COM63;  C8 : ����ɨ��, �� COM63�� COM0 */
#else
    bsp_OLED_WriteByte(0xa1, OLED_CMD); /* A0 : �е�ַ0ӳ�䵽SEG0; A1 : �е�ַ127ӳ�䵽SEG0 */
    bsp_OLED_WriteByte(0xc8, OLED_CMD); /* C0 : ����ɨ��,��COM0��COM63;  C8 : ����ɨ��, �� COM63�� COM0 */
#endif

    bsp_OLED_WriteByte(0xa6, OLED_CMD); /* A6 : ����������ʾģʽ; A7 : ����Ϊ����ģʽ */

    bsp_OLED_WriteByte(0xa8, OLED_CMD); /* ����COM·������ */
    bsp_OLED_WriteByte(0x3f, OLED_CMD); /* 1 ->��63+1��· */

    bsp_OLED_WriteByte(0xd3, OLED_CMD); /* ������ʾƫ������ */
    bsp_OLED_WriteByte(0x00, OLED_CMD); /* ��ƫ�� */

    bsp_OLED_WriteByte(0xd5, OLED_CMD); /* ������ʾʱ�ӷ�Ƶϵ��/��Ƶ������ */
    bsp_OLED_WriteByte(0x80, OLED_CMD); /* ���÷�Ƶϵ������4bit�Ƿ�Ƶϵ������4bit����Ƶ�� */

    bsp_OLED_WriteByte(0xd9, OLED_CMD); /* ����Ԥ����������� */
    bsp_OLED_WriteByte(0xf1, OLED_CMD); /* [3:0],PHASE 1; [7:4],PHASE 2; */

    bsp_OLED_WriteByte(0xda, OLED_CMD); /* ����COM��Ӳ�����߷�ʽ���� */
    bsp_OLED_WriteByte(0x12, OLED_CMD);

    bsp_OLED_WriteByte(0xdb, OLED_CMD); /* ���� vcomh ��ѹ�������� */
    bsp_OLED_WriteByte(0x40, OLED_CMD); /* [6:4] 000 = 0.65 x VCC; 0.77 x VCC (RESET); 0.83 x VCC  */

    bsp_OLED_WriteByte(0x8d, OLED_CMD); /* ���ó������� */
    bsp_OLED_WriteByte(0x14, OLED_CMD); /* 0x14 �򿪳��� 0x10�رճ��� */
    bsp_OLED_WriteByte(0xaf, OLED_CMD); /* ��OLED��� */

    OLED_ClrScr(0x00); /* ���� */
}

/*
*********************************************************************************************************
*	�� �� ��: OLED_ClrScr
*	����˵��: ����
*	��    ��: ucData: ȫ��д�������
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void OLED_ClrScr(uint8_t ucData)
{
    uint8_t ucY, ucX;

    for (ucY = 0; ucY < 8; ucY++)
    {
        bsp_OLED_WriteByte(0xb0 + ucY, OLED_CMD);
        bsp_OLED_WriteByte(0x00, OLED_CMD);
        bsp_OLED_WriteByte(0x10, OLED_CMD);
        for (ucX = 0; ucX < 128; ucX++)
        {
            bsp_OLED_WriteByte(ucData, OLED_DATA);
        }
    }
}

/*
*********************************************************************************************************
*	�� �� ��: OLED_DispChar
*	����˵��: ָ��λ����ʾһ���ַ�
*	��    ��: 
*			usX : X���꣨0-127��
*			usY : Y���꣨0-7��
*			c	: ASCII�ַ�
*           ucFont: ���� 8/16
*			ucMode: ��ʾģʽ: 0.���ס�1.����
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void OLED_DispChar(uint8_t ucX, uint8_t ucY, char c, uint8_t ucFont, uint8_t ucMode)
{
    uint8_t ucCnt;

    c -= 32; /* ascii���ǰ32���������ַ� */

    if (ucFont == 8)
    {
        bsp_OLED_SetPos(ucX, ucY); /* �趨���� */
        for (ucCnt = 0; ucCnt < 6; ucCnt++)
        {
            if (ucMode)
            {
                bsp_OLED_WriteByte(en5x7[c * 6 + ucCnt], OLED_DATA);
            }
            else
            {
                bsp_OLED_WriteByte(~en5x7[c * 6 + ucCnt], OLED_DATA);
            }
        }
    }
    else if (ucFont == 16)
    {
        bsp_OLED_SetPos(ucX, ucY); /* �趨���� */
        for (ucCnt = 0; ucCnt < 8; ucCnt++)
        {
            if (ucMode)
            {
                bsp_OLED_WriteByte(en8x16[c * 16 + ucCnt], OLED_DATA);
            }
            else
            {
                bsp_OLED_WriteByte(~en8x16[c * 16 + ucCnt], OLED_DATA);
            }
        }
        bsp_OLED_SetPos(ucX, ucY + 1); /* �趨��һҳ���� */
        for (ucCnt = 0; ucCnt < 8; ucCnt++)
        {
            if (ucMode)
            {
                bsp_OLED_WriteByte(en8x16[c * 16 + 8 + ucCnt], OLED_DATA);
            }
            else
            {
                bsp_OLED_WriteByte(~en8x16[c * 16 + 8 + ucCnt], OLED_DATA);
            }
        }
    }
}

/*
*********************************************************************************************************
*	�� �� ��: OLED_DispChStr
*	����˵��: ָ��λ����ʾһ������
*	��    ��: 
*			usX : X���꣨0-127��
*			usY : Y���꣨0-7��
*			pCh : ���ֻ���
*			ucMode: ��ʾģʽ: 0.���ס�1.����
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void OLED_DispChStr(uint8_t ucX, uint8_t ucY, char *pCh, uint8_t ucMode)
{
    uint8_t ucTemp, ucCnt;
    uint16_t usFont;

    for (usFont = 0; usFont < FontMax; usFont++) /* �Խ����ֿ�,ѭ����ѯ����,һ��������Ҫ�����ֽ� */
    {
        if ((Codechinese16x16[usFont].Index[0] == pCh[0]) && (Codechinese16x16[usFont].Index[1] == pCh[1])) /* Ѱ�Ҷ�Ӧ���� */
        {
            for (ucCnt = 0; ucCnt < 32; ucCnt++) /* һ������32���ֽ� */
            {
                ucTemp = Codechinese16x16[usFont].Msk[ucCnt];
                if (ucCnt < 16)
                {
                    bsp_OLED_SetPos(ucX + ucCnt, ucY); /* �趨���� */
                    if (ucMode)
                    {
                        bsp_OLED_WriteByte(ucTemp, OLED_DATA);
                    }
                    else
                    {
                        bsp_OLED_WriteByte(~ucTemp, OLED_DATA);
                    }
                }
                else
                {
                    bsp_OLED_SetPos(ucX + ucCnt - 16, ucY + 1); /* �趨��һҳ���� */
                    if (ucMode)
                    {
                        bsp_OLED_WriteByte(ucTemp, OLED_DATA);
                    }
                    else
                    {
                        bsp_OLED_WriteByte(~ucTemp, OLED_DATA);
                    }
                }
            }
        }
    }
}

/*
*********************************************************************************************************
*	�� �� ��: OLED_DispStr
*	����˵��: ָ��λ����ʾ�ַ���
*	��    ��: 
*			usX : X���꣨0-127��
*			usY : Y���꣨0-7��
*			pStr: �ַ���ָ��
*			ucMode: ��ʾģʽ: 0.���ס�1.����
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void OLED_DispStr(uint8_t ucX, uint8_t ucY, char *pStr, uint8_t ucMode)
{
    uint8_t bChFlag = 0;

    while (*pStr) /* ����δ���� */
    {
        if (!bChFlag)
        {
            if (*pStr > 0x7e) /* ���� */

            {
                bChFlag = 1;
            }
            else /* ASCII�ַ� */
            {
                OLED_DispChar(ucX, ucY, *pStr, 16, ucMode);
                pStr++;
                ucX += 8;
            }
        }
        else /* ���� */
        {
            bChFlag = 0;
            OLED_DispChStr(ucX, ucY, pStr, ucMode);
            pStr += 2;
            ucX += 16;
        }
    }
}

/*
*********************************************************************************************************
*	�� �� ��: BitXOR
*	����˵��: ����λ���M^N��
*	��    ��: ucM��ucN
*	�� �� ֵ: λ�����
*********************************************************************************************************
*/
uint16_t BitXOR(uint8_t ucM, uint8_t ucN)
{
    uint16_t usResult = 1;

    while (ucN--)
    {
        usResult *= ucM;
    }

    return usResult;
}

/*
*********************************************************************************************************
*	�� �� ��: OLED_DispNumber
*	����˵��: ��ʾ���ݴ�
*	��    ��: 
*			usX : X���꣨0-127��
*			usY : Y���꣨0-7��
*			usNumber : ����
*			ucLen    : ���ݳ���
*			dp       : С����λ�ã�û��С��������д0��
*           ucMode   : ��ʾģʽ: 0.���ס�1.����
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void OLED_DispNumber(uint8_t ucX, uint8_t ucY, uint16_t usNumber, uint8_t ucLen, uint8_t dp, uint8_t ucMode)
{
    uint8_t ucCnt, ucTemp;
    uint8_t bDispFlag = 0;

    for (ucCnt = 0; ucCnt < ucLen; ucCnt++)
    {
        ucTemp = (usNumber / BitXOR(10, ucLen - ucCnt - 1)) % 10;
        if (dp != 0 && ucCnt == dp) /* �ж�����С���� */
        {
            OLED_DispChar(ucX + 8 * ucCnt, ucY, '.', 16, ucMode); /* ��ʾС���� */
            ucLen += 1;
            continue;
        }
        if (bDispFlag == 0 && ucCnt < (ucLen - 1))
        {
            if (ucTemp == 0)
            {
                OLED_DispChar(ucX + 8 * ucCnt, ucY, '0', 16, ucMode);
                continue;
            }
            else
            {
                bDispFlag = 1;
            }
        }
        OLED_DispChar(ucX + 8 * ucCnt, ucY, ucTemp + '0', 16, ucMode); /* ��ʾ���� */
    }
}

/*
*********************************************************************************************************
*	�� �� ��: OLED_DrawBMP
*	����˵��: ��ʾ��ɫͼƬ
*	��    ��: 
*			usX : ͼƬX���꣨0-127��
*			usY : ͼƬY���꣨0-7��
*			usWidth  : ͼƬ���
*			usHeight : ͼƬ�߶ȣ�8�ı�����
*			ptr      : ��ɫͼƬ����ָ��
*           ucMode   : ��ʾģʽ: 0.���ס�1.����
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void OLED_DrawBMP(uint8_t ucX, uint8_t ucY, uint8_t ucWidth, uint8_t ucHeight, const uint8_t *ptr, uint8_t ucMode)
{
    uint16_t j = 0;
    uint8_t xCnt, yCnt;

    ucX %= 128;
    ucY %= 7;
    ucHeight /= 8;
    for (yCnt = ucY; yCnt < ucY + ucHeight; yCnt++)
    {
        bsp_OLED_SetPos(ucX, ucY + yCnt);
        for (xCnt = ucX; xCnt < ucX + ucWidth; xCnt++)
        {
            if (ucMode)
            {
                bsp_OLED_WriteByte(ptr[j++], OLED_DATA);
            }
            else
            {
                bsp_OLED_WriteByte(~ptr[j++], OLED_DATA);
            }
        }
    }
}

/********************************************* End of file **********************************************/
