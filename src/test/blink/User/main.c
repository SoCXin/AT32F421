/*
*********************************************************************************************************
*
*	程序名称: LED灯闪烁
*	文件名称: main.c
*   MCU型号 : AT32F413RBT7 FLASH:128KB SRAM:64KB
*   运行平台: 风巽云AT32F421开发板
*	版    本: v1.0.2
*   编    写: 莫利奖
*	说    明: 演示GPIO的输出设置
*
*	版本记录:
*	v1.0.0: 2020年11月25日，初版
*	v1.0.1: 2021年3月3日，升级标准库至1.2.9
*   v1.0.2: 2021年4月15日，升级标准库至1.3.0，规范示例
*
*	Copyright (C), 2021-2031, 深圳市风巽云科技有限公司 https://fxymcu.taobao.com
*
*********************************************************************************************************
*/

#include "bsp.h"

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
    /* 系统初始化 */
    System_Init();

    while (1)
    {
		bsp_LED_Toggle();
		bsp_DelayMs(300);
    }
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
