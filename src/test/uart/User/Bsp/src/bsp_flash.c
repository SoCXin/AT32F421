/*
*********************************************************************************************************
*
*	程序名称: 片上FLASH驱动模块
*	文件名称: bsp_flash.c
*	版    本: v1.0.0
*   编    写: 莫利奖
*	说    明: 
*
*	版本记录: 
*	v1.0.0: 2021年4月15日，初版
*
*	Copyright (C), 2021-2031, 深圳市风巽云科技有限公司 https://fxymcu.taobao.com
*
*********************************************************************************************************
*/

#include "bsp.h"

/*
*********************************************************************************************************
*	函 数 名: bsp_FLASH_ErasePage
*	功能说明: 擦除CPU FLASH一个页 （1KB)
*	形    参: ulFlashAddr: FLASH地址
*	返 回 值:
*            FLASH_BSY = 1    忙
*            FLASH_PGRM_FLR   编程错误
*            FLASH_WRPRT_FLR  写保护错误
*            FLASH_PRC_DONE   操作结束
*            FLASH_TIMEOU     超时
*
*********************************************************************************************************
*/
uint8_t bsp_FLASH_ErasePage(uint32_t ulFlashAddr)
{
    uint8_t re;

    if (ulFlashAddr < FLASH_BASE || ulFlashAddr >= USER_FLASH_END_ADDRESS)
    {
        return FLASH_PGRM_FLR;
    }
    /* FLASH 解锁 */
    FLASH_Unlock();
    re = FLASH_ErasePage(ulFlashAddr);
    /* FLASH加锁，禁止写FLASH控制寄存器 */
    FLASH_Lock();

    return re;
}

/*
*********************************************************************************************************
*	函 数 名: bsp_FLASH_WriteBuffer
*	功能说明: 写数据到 CPU FLASH
*	形    参: 
*	         ulFlashAddr: FLASH地址
*	         pByte: 数据缓冲区
*	         ulSize: 数据大小
*	返 回 值:
*            FLASH_BSY = 1    忙
*            FLASH_PGRM_FLR   编程错误
*            FLASH_WRPRT_FLR  写保护错误
*            FLASH_PRC_DONE   操作结束
*            FLASH_TIMEOU     超时
*
*********************************************************************************************************
*/
uint8_t bsp_FLASH_WriteBuffer(uint32_t ulFlashAddr, uint8_t *pByte, uint32_t ulSize)
{
    uint32_t ulCnt;

    if (ulFlashAddr < FLASH_BASE || ulFlashAddr + ulSize >= USER_FLASH_END_ADDRESS)
    {
        return FLASH_PGRM_FLR;
    }
    if (ulSize == 0)
    {
        return FLASH_PGRM_FLR;
    }
    /* FLASH 解锁 */
    FLASH_Unlock();
    for (ulCnt = 0; ulCnt < ulSize; ulCnt++)
    {
        if (FLASH_ProgramByte(ulFlashAddr++, *pByte++) != FLASH_PRC_DONE)
        {
            return FLASH_PGRM_FLR;
        }
    }
    /* FLASH加锁，禁止写FLASH控制寄存器 */
    FLASH_Lock();

    return FLASH_PRC_DONE;
}

/*
*********************************************************************************************************
*	函 数 名: bsp_FLASH_ReadBuffer
*	功能说明: 读取CPU Flash的内容
*	形    参: 
*	         ulFlashAddr: FLASH地址
*	         pByte: 数据缓冲区
*	         ulSize: 数据大小
*	返 回 值: 0.成功 1.失败
*********************************************************************************************************
*/
uint8_t bsp_FLASH_ReadBuffer(uint32_t ulFlashAddr, uint8_t *pByte, uint32_t ulSize)
{
    uint32_t ulCnt;

    if (ulFlashAddr < FLASH_BASE || ulFlashAddr + ulSize >= USER_FLASH_END_ADDRESS)
    {
        return FLASH_PGRM_FLR;
    }
    if (ulSize == 0)
    {
        return 1;
    }
    for (ulCnt = 0; ulCnt < ulSize; ulCnt++)
    {
        *(pByte++) = *(uint8_t *)ulFlashAddr++;
    }

    return 0;
}

/********************************************* End of file **********************************************/
