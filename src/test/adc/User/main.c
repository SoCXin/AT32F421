/*
*********************************************************************************************************
*
*	程序名称: ADC读取外部多通道输入
*	文件名称: main.c
*   MCU型号 : AT32F413RBT7 FLASH:128KB SRAM:64KB
*   运行平台: 风巽云AT32F421开发板
*	版    本: v1.0.3
*   编    写: 莫利奖
*	说    明: 演示ADC读取外部多通道输入
*
*	版本记录: 
*	v1.0.0: 2020年11月25日，初版
*	v1.0.1: 2021年3月3日，升级标准库至1.2.9
*   v1.0.2: 2021年4月16日，升级标准库至1.3.0，规范示例
*	v1.0.3: 2021年6月10日，ADC读取外部输入多通道示例
*
*	Copyright (C), 2021-2031, 深圳市风巽云科技有限公司 https://fxymcu.taobao.com
*
*********************************************************************************************************
*/

#include "bsp.h"

/* 定义例程名和例程发布日期 */
#define EXAMPLE_NAME "ADC读取外部多通道输入"
#define EXAMPLE_DATE "2021年6月10日"
#define DEMO_VER "v1.0.2"

uint16_t usAdc[dChannel] = {0};
uint8_t ucSamplingCnt = 0;

/* 函数声明 */
static void PrintfLogo(void);

/*
*********************************************************************************************************
*	函 数 名: main
*	功能说明: 标准c程序入口
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
int main(void)
{
    uint8_t ucTimeCnt = 0;

    /* 系统初始化 */
    System_Init();
    /* 板子信息 */
    PrintfLogo();

    while (1)
    {
        /* 系统10ms时基 */
        if (ucSys10msFlag)
        {
            ucSys10msFlag = 0;
			/* ADC转换 */
            if (ucSamplingCnt > dSamplingTimes - 1)
            {
                ucSamplingCnt = 0;
                usAdc[0] = Get_ADC1(0);
                usAdc[1] = Get_ADC1(1);
                usAdc[2] = Get_ADC1(2);
                ClearedAdcBuffer();
            }
            else
            {
                ucSamplingCnt++;
                /* 启动ADC转换 */
                bsp_StartADC1();
            }
            /* 打印AD值 */
            ucTimeCnt++;
            if (ucTimeCnt > 99)
            {
                ucTimeCnt = 0;
                printf("->usAdc[0] = %d usAdc[1] = %d usAdc[2] = %d\r\n", usAdc[0], usAdc[1], usAdc[2]);
                bsp_LED_Toggle();
            }
        }
    }
}

/*
*********************************************************************************************************
*	函 数 名: PrintfLogo
*	功能说明: 打印例程名称和例程发布日期
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
static void PrintfLogo(void)
{
    uint32_t CPU_Sn0, CPU_Sn1, CPU_Sn2;

    printf("-------------------------------------------------------------\r\n");
    printf("->例程名称: %s\r\n", EXAMPLE_NAME); /* 打印例程名称 */
    printf("->例程版本: %s\r\n", DEMO_VER);     /* 打印例程版本 */
    printf("->发布日期: %s\r\n", EXAMPLE_DATE); /* 打印例程日期 */

    /* UID */
    CPU_Sn0 = *(__IO uint32_t *)(0x1FFFF7E8);
    CPU_Sn1 = *(__IO uint32_t *)(0x1FFFF7E8 + 4);
    CPU_Sn2 = *(__IO uint32_t *)(0x1FFFF7E8 + 8);

    printf("->CPU: AT32F421C8T7, LQFP48, 主频: %dMHz\r\n", SystemCoreClock / 1000000);
    printf("->FLASH: 64Kbyte, SRAM: 16Kbyte\r\n");
    printf("->UID: %08X %08X %08X\n\r", CPU_Sn2, CPU_Sn1, CPU_Sn0);

    /* 库版本 */
    printf("->库版本: V1.3.0\r\n");
    printf("-------------------------------------------------------------\r\n");
}

/*
*********************************************************************************************************
*	函 数 名: assert_failed
*	功能说明: 断言函数，宏定义在at32f4xx_conf.h文件
*	形    参: 
*	          file: 源代码文件名称（__FILE__ 表示源代码文件名）
*	          line: 代码行号（__LINE__ 表示源代码行号）
*	返 回 值: 无
*********************************************************************************************************
*/
#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t *file, uint32_t line)
{
    /* 用户可以添加自己的代码报告源代码文件名和代码行号，比如使用串口将错误文件和行号发送至外设 */
    printf("Wrong parameters value: file %s on line %d\r\n", file, line);

    /* 死循环，便于查找问题 */
    while (1)
    {
    }
}
#endif

/********************************************* End of file **********************************************/
