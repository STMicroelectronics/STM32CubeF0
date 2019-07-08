/**
  ******************************************************************************
  * @file    stm32f0xx_it.c
  * @author  MCD Application Team
  * @brief   This file includes the interrupt handlers for the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2016 STMicroelectronics International N.V. 
  * All rights reserved.</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "k_config.h"    
#include "k_module.h"    
#include "8uart_app.h"    

/** @addtogroup CORE
  * @{
  */

/** @defgroup 
  * @brief  
  * @{
  */
  
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*             Cortex-M Processor Exceptions Handlers                         */
/******************************************************************************/
/**
  * @brief  NMI_Handler
  *         This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  HardFault_Handler
  *         This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  MemManage_Handler
  *         This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  BusFault_Handler
  *         This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  UsageFault_Handler
  *         This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  DebugMon_Handler
  *         This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/******************************************************************************/
/*                 STM32F0xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32fxxx.s).                                                */
/******************************************************************************/
void SysTick_Handler (void)
{
  HAL_IncTick(); 
}

/**
  * @brief  This function handles External lines 4_15 interrupt request.
  * @param  None
  * @retval None
  */

void EXTI4_15_IRQHandler(void)
{
  HAL_GPIO_EXTI_IRQHandler(SD_DETECT_PIN);
  HAL_GPIO_EXTI_IRQHandler(DOWN_JOY_PIN);
  HAL_GPIO_EXTI_IRQHandler(UP_JOY_PIN);
  HAL_GPIO_EXTI_IRQHandler(TAMPER_BUTTON_PIN);
}

/**
  * @brief  This function handles External lines 2_3 interrupt request.
  * @param  None
  * @retval None
  */
void EXTI2_3_IRQHandler(void)
{
  HAL_GPIO_EXTI_IRQHandler(LEFT_JOY_PIN);
  HAL_GPIO_EXTI_IRQHandler(RIGHT_JOY_PIN);
}

/**
  * @brief  This function handles External lines 0_1 interrupt request.
  * @param  None
  * @retval None
  */
void EXTI0_1_IRQHandler(void)
{
  HAL_GPIO_EXTI_IRQHandler(SEL_JOY_PIN);
}
    
/**
  * @brief  This function handles UART1 interrupt request.  
  * @param  None
  * @retval None
  * @Note   This function is redefined in "main.h" and related to DMA 
  *         used for USART data transmission     
  */
void USART1_IRQHandler(void)
{
  HAL_UART_IRQHandler(&UartHandle[USART1_INDEX]);
}

/**
  * @brief  This function handles UART2 interrupt request.  
  * @param  None
  * @retval None
  * @Note   This function is redefined in "main.h" and related to DMA 
  *         used for USART data transmission     
  */
void USART2_IRQHandler(void)
{
  HAL_UART_IRQHandler(&UartHandle[USART2_INDEX]);
}

/**
  * @brief  This function handles UART3_8 interrupt request.  
  * @param  None
  * @retval None
  * @Note   This function is redefined in "main.h" and related to DMA 
  *         used for USART data transmission     
  */  
void USART3_8_IRQHandler(void)
{
  uint32_t pendingIT = SYSCFG->IT_LINE_SR[USART3_8_IRQn];
  
  if((pendingIT & SYSCFG_ITLINE29_SR_USART3_GLB)!= RESET)
  {
    HAL_UART_IRQHandler(&UartHandle[USART3_INDEX]);
  }
  if((pendingIT & SYSCFG_ITLINE29_SR_USART4_GLB)!= RESET)
  {
    HAL_UART_IRQHandler(&UartHandle[USART4_INDEX]);
  }
  if((pendingIT & SYSCFG_ITLINE29_SR_USART5_GLB)!= RESET)
  {
    HAL_UART_IRQHandler(&UartHandle[USART5_INDEX]);
  }
  if((pendingIT & SYSCFG_ITLINE29_SR_USART6_GLB)!= RESET)
  {
    HAL_UART_IRQHandler(&UartHandle[USART6_INDEX]);
  }
  if((pendingIT & SYSCFG_ITLINE29_SR_USART7_GLB)!= RESET)
  {
    HAL_UART_IRQHandler(&UartHandle[USART7_INDEX]);
  }
  if((pendingIT & SYSCFG_ITLINE29_SR_USART8_GLB)!= RESET)
  {
    HAL_UART_IRQHandler(&UartHandle[USART8_INDEX]);
  }
}

/**
  * @}
  */

/**
  * @}
  */
  
  
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
