 /**
  ******************************************************************************
  * @file    TMR/Input_Capture/main.c
  * @author  ArteryTek Technology
  * @version V1.0.4
  * @date    2019-01-04
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, ARTERYTEK SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2018 ArteryTek</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "at32f4xx.h"
#include <stdio.h>
#include "at32_board.h"

/** @addtogroup AT32F413_StdPeriph_Examples
  * @{
  */

/** @addtogroup TMR_Input_Capture
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
TMR_ICInitType  TMR_ICInitStructure;
DMA_InitType DMA_InitStructure = {0};
TMR_TimerBaseInitType  TMR_TMReBaseStructure;
TMR_OCInitType  TMR_OCInitStructure;

#define TIM3_CCR1_Address 0x40000434

__IO uint32_t TMR3Freq = 0;
__IO uint32_t Sys_Counter = 0;
__IO uint16_t CaptureNumber = 0;
__IO uint16_t SRC_Buffer[1000] = {0};
/* Private function prototypes -----------------------------------------------*/
void RCC_Configuration(void);
void GPIO_Configuration(void);
void NVIC_Configuration(void);
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  /* System Clocks Configuration */
  RCC_Configuration();

  /* NVIC configuration */
  NVIC_Configuration();


  /* Configure the GPIO ports */
  GPIO_Configuration();

  /* Board.c init */
  AT32_Board_Init();
  UART_Print_Init(115200);

  /* TMR3 configuration: Input Capture mode ---------------------
     The external signal is connected to TMR3 CH2 pin (PA.07)
     The Rising edge is used as active edge,
     The TMR3 CCR2 is used to compute the frequency value
  ------------------------------------------------------------ */

  /* DMA1 Channel4 Config */
  DMA_Reset(DMA1_Channel4);
  DMA_DefaultInitParaConfig(&DMA_InitStructure);

  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)TIM3_CCR1_Address;
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)SRC_Buffer;
  DMA_InitStructure.DMA_Direction = DMA_DIR_PERIPHERALSRC;
  DMA_InitStructure.DMA_BufferSize = 1000;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PERIPHERALINC_DISABLE;
  DMA_InitStructure.DMA_MemoryInc = DMA_MEMORYINC_ENABLE;
  DMA_InitStructure.DMA_PeripheralDataWidth = DMA_PERIPHERALDATAWIDTH_HALFWORD;
  DMA_InitStructure.DMA_MemoryDataWidth = DMA_MEMORYDATAWIDTH_HALFWORD;
  DMA_InitStructure.DMA_Mode = DMA_MODE_CIRCULAR;
  DMA_InitStructure.DMA_Priority = DMA_PRIORITY_HIGH;
  DMA_InitStructure.DMA_MTOM = DMA_MEMTOMEM_DISABLE;

  DMA_Init(DMA1_Channel4, &DMA_InitStructure);
  DMA_INTConfig(DMA1_Channel4,DMA_INT_TC,ENABLE);
  /* DMA1 Channel4 enable */
  DMA_ChannelEnable(DMA1_Channel4, ENABLE);

  /* TMRe base configuration */
  TMR_TimeBaseStructInit(&TMR_TMReBaseStructure);
  TMR_TMReBaseStructure.TMR_Period = SystemCoreClock/1000000/4-1;
  TMR_TMReBaseStructure.TMR_DIV = 0;
  TMR_TMReBaseStructure.TMR_ClockDivision = 0;
  TMR_TMReBaseStructure.TMR_CounterMode = TMR_CounterDIR_Up;

  TMR_TimeBaseInit(TMR1, &TMR_TMReBaseStructure);

  /* Output Compare TMRing Mode configuration: Channel1 */
  TMR_OCStructInit(&TMR_OCInitStructure);
  TMR_OCInitStructure.TMR_OCMode = TMR_OCMode_PWM1;
  TMR_OCInitStructure.TMR_OutputState = TMR_OutputState_Enable;
  TMR_OCInitStructure.TMR_Pulse = (SystemCoreClock/1000000)/2/4-1;
  TMR_OCInitStructure.TMR_OCPolarity = TMR_OCPolarity_High;

  TMR_OC1Init(TMR1, &TMR_OCInitStructure);

  TMR_OC1PreloadConfig(TMR1, TMR_OCPreload_Disable);
  TMR_CtrlPWMOutputs(TMR1,ENABLE);
  /* TMR1 enable counter */
  TMR_Cmd(TMR1, ENABLE);


  TMR_ICStructInit(&TMR_ICInitStructure);
  TMR_ICInitStructure.TMR_Channel = TMR_Channel_1;
  TMR_ICInitStructure.TMR_ICPolarity = TMR_ICPolarity_Rising;
  TMR_ICInitStructure.TMR_ICSelection = TMR_ICSelection_DirectTI;
  TMR_ICInitStructure.TMR_ICDIV = TMR_ICDIV_DIV1;
  TMR_ICInitStructure.TMR_ICFilter = 0x0;

  TMR_ICInit(TMR3, &TMR_ICInitStructure);

  /* Enable the CC1 Interrupt Request */
  TMR_INTConfig(TMR3, TMR_INT_CC1, ENABLE);
  /* Enable the CC1 DMA Request */
  TMR_DMACmd(TMR3,TMR_DMA_CC1,ENABLE);
  /* TMR enable counter */
  TMR_Cmd(TMR3, ENABLE);
  while (1)
  {
  }
}

void DMA1_Channel7_4_IRQHandler(void)
{
  if(DMA_GetFlagStatus(DMA1_FLAG_TC4) == SET)
  {
    TMR3Freq = SystemCoreClock/(SRC_Buffer[500] - SRC_Buffer[499]);
    printf("The external signal frequece is : %d\r\n",TMR3Freq);
    DMA_ClearITPendingBit(DMA1_FLAG_TC4);
  }
}
/**
  * @brief  Configures the different system clocks.
  * @param  None
  * @retval None
  */
void RCC_Configuration(void)
{
  /* TMR3/TMR1 clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1PERIPH_TMR3, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2PERIPH_TMR1, ENABLE);

  /* DMA1/GPIOA clock enable */
  RCC_AHBPeriphClockCmd(RCC_AHBPERIPH_DMA1|RCC_AHBPERIPH_GPIOA,ENABLE);
}
/**
  * @brief  Configure the GPIO Pins.
  * @param  None
  * @retval None
  */
void GPIO_Configuration(void)
{
  GPIO_InitType GPIO_InitStructure = {0};

  GPIO_PinAFConfig(GPIOA, GPIO_PinsSource8, GPIO_AF_2);
  GPIO_PinAFConfig(GPIOA, GPIO_PinsSource6, GPIO_AF_1);

  /* GPIOA Configuration: PA.08(TMR1 CH1) as alternate function push-pull */
  GPIO_StructInit(&GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pins = GPIO_Pins_8 ;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_MaxSpeed = GPIO_MaxSpeed_50MHz;

  GPIO_Init(GPIOA, &GPIO_InitStructure);

  /* GPIOA Configuration: PA.09(TMR1 CH2) */
  GPIO_InitStructure.GPIO_Pins = GPIO_Pins_6;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;

  GPIO_Init(GPIOA, &GPIO_InitStructure);

}

void NVIC_Configuration(void)
{
  NVIC_InitType NVIC_InitStructure;

  /* Enable the DMA1_CH6 TC Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel7_4_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  while (1)
  {}
}

#endif

/**
  * @}
  */

/**
  * @}
  */

 /******************* (C) COPYRIGHT 2018 ArteryTek *****END OF FILE****/
