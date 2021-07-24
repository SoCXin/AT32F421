/*
*********************************************************************************************************
*
*	程序名称: OLED模块(SSD1306芯片)驱动
*	文件名称: bsp_oled.c
*	版    本: v1.0.0
*   编    写: 莫利奖
*	说    明: 根据模块实际情况开启不同的驱动
*
*	版本记录: 
*	v1.0.0: 2021年4月20日，初版
*
*	Copyright (C), 2021-2031, 深圳市风巽云科技有限公司 https://fxymcu.taobao.com
*
*********************************************************************************************************
*/

#include "bsp.h"
#include "font.h"

/* OLED驱动方式 */
#define OLED_DRI_MODE 2 /* 0: GPIO模拟IIC 1: 硬件IIC 2: SPI */

/* 复位 */
#define OLED_RESET_PORT GPIOB
#define OLED_RESET_PIN GPIO_Pins_1
/* 数据/命令 */
#define OLED_DC_PORT GPIOB
#define OLED_DC_PIN GPIO_Pins_2
/* GPIO时钟 */
#define OLED_GPIO_RCC RCC_AHBPERIPH_GPIOB

/* 操作宏定义 */
#define OLED_RESET_1() (OLED_RESET_PORT->BSRE = OLED_RESET_PIN)
#define OLED_RESET_0() (OLED_RESET_PORT->BRE = OLED_RESET_PIN)

#define OLED_DC_1() (OLED_DC_PORT->BSRE = OLED_DC_PIN)
#define OLED_DC_0() (OLED_DC_PORT->BRE = OLED_DC_PIN)

/* OLED显示方向 */
#define DIR_180 0 /* 0: 正常方向 1: 旋转180度 */

/*
*********************************************************************************************************
*	函 数 名: bsp_InitOLEDGpio
*	功能说明: 配置OLED相关的GPIO
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_InitOLEDGpio(void)
{
    GPIO_InitType GPIO_InitStructure;

    /* 使能GPIO时钟 */
    RCC_AHBPeriphClockCmd(OLED_GPIO_RCC, ENABLE);

    /* 设置电平 */
    GPIO_ResetBits(OLED_RESET_PORT, OLED_RESET_PIN);
    GPIO_ResetBits(OLED_DC_PORT, OLED_DC_PIN);

    /* 配置GPIO */
    GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OutType = GPIO_OutType_PP; /* 推挽输出 */
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
*	函 数 名: bsp_OLED_WriteByte
*	功能说明: 向OLED写入一个字节-IIC总线方式
*	形    参: 
*	          ucData: 数据
*	          ucCmd: 命令
*	返 回 值: 无
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
*	函 数 名: bsp_OLED_SetPos
*	功能说明: 设置坐标
*	形    参: 
*			usX : X坐标（0-127）
*			usY : Y坐标（0-7）
*	返 回 值: 无
*********************************************************************************************************
*/
static void bsp_OLED_SetPos(uint8_t ucX, uint8_t ucY)
{
    bsp_OLED_WriteByte(0xb0 + ucY, OLED_CMD);                 /* 设置页地址 */
    bsp_OLED_WriteByte(((ucX & 0xf0) >> 4) | 0x10, OLED_CMD); /* 设置列地址的低4位 */
    bsp_OLED_WriteByte(ucX & 0x0f, OLED_CMD);                 /* 设置列地址的高4位 */
}

/*
*********************************************************************************************************
*	函 数 名: bsp_OLED_DispOn
*	功能说明: 打开显示
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_OLED_DispOn(void)
{
    bsp_OLED_WriteByte(0x8d, OLED_CMD); /* 电荷泵设置 */
    bsp_OLED_WriteByte(0x14, OLED_CMD); /* 打开充电泵*/
    bsp_OLED_WriteByte(0xaf, OLED_CMD); /* 打开OLED面板 */
}

/*
*********************************************************************************************************
*	函 数 名: bsp_OLED_DispOff
*	功能说明: 关闭显示
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_OLED_DispOff(void)
{
    bsp_OLED_WriteByte(0x8d, OLED_CMD); /* 电荷泵设置 */
    bsp_OLED_WriteByte(0x10, OLED_CMD); /* 关闭充电泵 */
    bsp_OLED_WriteByte(0xae, OLED_CMD); /* 关闭OLED面板 */
}

/*
*********************************************************************************************************
*	函 数 名: bsp_OLED_SetContrast
*	功能说明: 设置对比度
*	形    参: ucValue: 0-255
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_OLED_SetContrast(uint8_t ucValue)
{
    bsp_OLED_WriteByte(0x81, OLED_CMD);    /* 设置对比度命令 */
    bsp_OLED_WriteByte(ucValue, OLED_CMD); /* 设置对比度参数 */
}

/*
*********************************************************************************************************
*	函 数 名: bsp_OLED_SetDir
*	功能说明: 设置显示方向
*	形    参: ucDir = 1 表示正常方向，0表示翻转180度
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_OLED_SetDir(uint8_t ucDir)
{
    if (ucDir)
    {
        bsp_OLED_WriteByte(0xa1, OLED_CMD); /* A0 : 列地址0映射到SEG0; A1 : 列地址127映射到SEG0 */
        bsp_OLED_WriteByte(0xc8, OLED_CMD); /* C0 : 正常扫描,从COM0到COM63;  C8 : 反向扫描, 从 COM63至 COM0 */
    }
    else
    {
        bsp_OLED_WriteByte(0xa0, OLED_CMD); /* A0 : 列地址0映射到SEG0; A1 : 列地址127映射到SEG0 */
        bsp_OLED_WriteByte(0xc0, OLED_CMD); /* C0 : 正常扫描,从COM0到COM63;  C8 : 反向扫描, 从 COM63至 COM0 */
    }
}

/*
*********************************************************************************************************
*	函 数 名: bsp_InitOLED
*	功能说明: 初始化OLED屏
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_InitOLED(void)
{
    /* 初始化GPIO */
    bsp_InitOLEDGpio();
    /* 复位 */
    OLED_RESET_0();
    bsp_DelayMs(100);
    OLED_RESET_1();
    bsp_DelayMs(100);

    /* 根据SSD1306芯片初始化 */
    bsp_OLED_WriteByte(0xae, OLED_CMD); /* 关闭OLED面板显示 */

    bsp_OLED_WriteByte(0x00, OLED_CMD); /* 设置列地址低4bit */
    bsp_OLED_WriteByte(0x10, OLED_CMD); /* 设置列地址高4bit */
    bsp_OLED_WriteByte(0x40, OLED_CMD); /* 设置起始行地址（低5bit 0-63），行数 */
    bsp_OLED_WriteByte(0xb0, OLED_CMD); /* 设置页地址 */

    bsp_OLED_WriteByte(0x20, OLED_CMD); /* 内存寻址模式命令 */
    bsp_OLED_WriteByte(0x02, OLED_CMD); /* [1:0]: 00.列地址模式; 01.行地址模式; 10.页地址模式; 默认页地址 */

    bsp_OLED_WriteByte(0x81, OLED_CMD); /* 设置对比度命令 */
    bsp_OLED_WriteByte(0xcf, OLED_CMD); /* 设置对比度参数，缺省CF */

#if DIR_180
    bsp_OLED_WriteByte(0xa0, OLED_CMD); /* A0 : 列地址0映射到SEG0; A1 : 列地址127映射到SEG0 */
    bsp_OLED_WriteByte(0xc0, OLED_CMD); /* C0 : 正常扫描,从COM0到COM63;  C8 : 反向扫描, 从 COM63至 COM0 */
#else
    bsp_OLED_WriteByte(0xa1, OLED_CMD); /* A0 : 列地址0映射到SEG0; A1 : 列地址127映射到SEG0 */
    bsp_OLED_WriteByte(0xc8, OLED_CMD); /* C0 : 正常扫描,从COM0到COM63;  C8 : 反向扫描, 从 COM63至 COM0 */
#endif

    bsp_OLED_WriteByte(0xa6, OLED_CMD); /* A6 : 设置正常显示模式; A7 : 设置为反显模式 */

    bsp_OLED_WriteByte(0xa8, OLED_CMD); /* 设置COM路数命令 */
    bsp_OLED_WriteByte(0x3f, OLED_CMD); /* 1 ->（63+1）路 */

    bsp_OLED_WriteByte(0xd3, OLED_CMD); /* 设置显示偏移命令 */
    bsp_OLED_WriteByte(0x00, OLED_CMD); /* 无偏移 */

    bsp_OLED_WriteByte(0xd5, OLED_CMD); /* 设置显示时钟分频系数/振荡频率命令 */
    bsp_OLED_WriteByte(0x80, OLED_CMD); /* 设置分频系数，高4bit是分频系数，低4bit是振荡频率 */

    bsp_OLED_WriteByte(0xd9, OLED_CMD); /* 设置预充电周期命令 */
    bsp_OLED_WriteByte(0xf1, OLED_CMD); /* [3:0],PHASE 1; [7:4],PHASE 2; */

    bsp_OLED_WriteByte(0xda, OLED_CMD); /* 设置COM脚硬件接线方式命令 */
    bsp_OLED_WriteByte(0x12, OLED_CMD);

    bsp_OLED_WriteByte(0xdb, OLED_CMD); /* 设置 vcomh 电压倍率命令 */
    bsp_OLED_WriteByte(0x40, OLED_CMD); /* [6:4] 000 = 0.65 x VCC; 0.77 x VCC (RESET); 0.83 x VCC  */

    bsp_OLED_WriteByte(0x8d, OLED_CMD); /* 设置充电泵命令 */
    bsp_OLED_WriteByte(0x14, OLED_CMD); /* 0x14 打开充电泵 0x10关闭充电泵 */
    bsp_OLED_WriteByte(0xaf, OLED_CMD); /* 打开OLED面板 */

    OLED_ClrScr(0x00); /* 清屏 */
}

/*
*********************************************************************************************************
*	函 数 名: OLED_ClrScr
*	功能说明: 清屏
*	形    参: ucData: 全屏写入的数据
*	返 回 值: 无
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
*	函 数 名: OLED_DispChar
*	功能说明: 指定位置显示一个字符
*	形    参: 
*			usX : X坐标（0-127）
*			usY : Y坐标（0-7）
*			c	: ASCII字符
*           ucFont: 字体 8/16
*			ucMode: 显示模式: 0.反白、1.正常
*	返 回 值: 无
*********************************************************************************************************
*/
void OLED_DispChar(uint8_t ucX, uint8_t ucY, char c, uint8_t ucFont, uint8_t ucMode)
{
    uint8_t ucCnt;

    c -= 32; /* ascii码表前32个不常用字符 */

    if (ucFont == 8)
    {
        bsp_OLED_SetPos(ucX, ucY); /* 设定坐标 */
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
        bsp_OLED_SetPos(ucX, ucY); /* 设定坐标 */
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
        bsp_OLED_SetPos(ucX, ucY + 1); /* 设定下一页坐标 */
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
*	函 数 名: OLED_DispChStr
*	功能说明: 指定位置显示一个汉字
*	形    参: 
*			usX : X坐标（0-127）
*			usY : Y坐标（0-7）
*			pCh : 汉字缓冲
*			ucMode: 显示模式: 0.反白、1.正常
*	返 回 值: 无
*********************************************************************************************************
*/
void OLED_DispChStr(uint8_t ucX, uint8_t ucY, char *pCh, uint8_t ucMode)
{
    uint8_t ucTemp, ucCnt;
    uint16_t usFont;

    for (usFont = 0; usFont < FontMax; usFont++) /* 自建汉字库,循环查询内码,一个汉字需要两个字节 */
    {
        if ((Codechinese16x16[usFont].Index[0] == pCh[0]) && (Codechinese16x16[usFont].Index[1] == pCh[1])) /* 寻找对应汉字 */
        {
            for (ucCnt = 0; ucCnt < 32; ucCnt++) /* 一个汉字32个字节 */
            {
                ucTemp = Codechinese16x16[usFont].Msk[ucCnt];
                if (ucCnt < 16)
                {
                    bsp_OLED_SetPos(ucX + ucCnt, ucY); /* 设定坐标 */
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
                    bsp_OLED_SetPos(ucX + ucCnt - 16, ucY + 1); /* 设定下一页坐标 */
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
*	函 数 名: OLED_DispStr
*	功能说明: 指定位置显示字符串
*	形    参: 
*			usX : X坐标（0-127）
*			usY : Y坐标（0-7）
*			pStr: 字符串指针
*			ucMode: 显示模式: 0.反白、1.正常
*	返 回 值: 无
*********************************************************************************************************
*/
void OLED_DispStr(uint8_t ucX, uint8_t ucY, char *pStr, uint8_t ucMode)
{
    uint8_t bChFlag = 0;

    while (*pStr) /* 数据未结束 */
    {
        if (!bChFlag)
        {
            if (*pStr > 0x7e) /* 汉字 */

            {
                bChFlag = 1;
            }
            else /* ASCII字符 */
            {
                OLED_DispChar(ucX, ucY, *pStr, 16, ucMode);
                pStr++;
                ucX += 8;
            }
        }
        else /* 汉字 */
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
*	函 数 名: BitXOR
*	功能说明: 数据位异或（M^N）
*	形    参: ucM，ucN
*	返 回 值: 位异或结果
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
*	函 数 名: OLED_DispNumber
*	功能说明: 显示数据串
*	形    参: 
*			usX : X坐标（0-127）
*			usY : Y坐标（0-7）
*			usNumber : 数据
*			ucLen    : 数据长度
*			dp       : 小数点位置（没有小数点则填写0）
*           ucMode   : 显示模式: 0.反白、1.正常
*	返 回 值: 无
*********************************************************************************************************
*/
void OLED_DispNumber(uint8_t ucX, uint8_t ucY, uint16_t usNumber, uint8_t ucLen, uint8_t dp, uint8_t ucMode)
{
    uint8_t ucCnt, ucTemp;
    uint8_t bDispFlag = 0;

    for (ucCnt = 0; ucCnt < ucLen; ucCnt++)
    {
        ucTemp = (usNumber / BitXOR(10, ucLen - ucCnt - 1)) % 10;
        if (dp != 0 && ucCnt == dp) /* 判断有无小数点 */
        {
            OLED_DispChar(ucX + 8 * ucCnt, ucY, '.', 16, ucMode); /* 显示小数点 */
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
        OLED_DispChar(ucX + 8 * ucCnt, ucY, ucTemp + '0', 16, ucMode); /* 显示数据 */
    }
}

/*
*********************************************************************************************************
*	函 数 名: OLED_DrawBMP
*	功能说明: 显示单色图片
*	形    参: 
*			usX : 图片X坐标（0-127）
*			usY : 图片Y坐标（0-7）
*			usWidth  : 图片宽度
*			usHeight : 图片高度（8的倍数）
*			ptr      : 单色图片点阵指针
*           ucMode   : 显示模式: 0.反白、1.正常
*	返 回 值: 无
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
