/**
  ******************************************************************************
  * @file    UART/UART_OneBoards_8UART/Src/stm32f0xx_it.c 
  * @author  MCD Application Team
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2016 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f0xx_it.h"
#include "stdio.h"   
/** @addtogroup STM32F0xx_HAL_Examples
  * @{
  */

/** @addtogroup UART_TwoBoards_ComIT
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* UART handler declared in "main.c" file */
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M0 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief   This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
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
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
  HAL_IncTick();
}

/******************************************************************************/
/*                 STM32F0xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f0xx.s).                                               */
/******************************************************************************/
/**
  * @brief  This function handles UART interrupt request.  
  * @param  None
  * @retval None
  * @Note   This function is redefined in "main.h" and related to DMA 
  *         used for USART data transmission     
  */
void USART1_IRQHandler(void)
{
  HAL_UART_IRQHandler(&UartHandle[USART1_INDEX]);
}

void USART2_IRQHandler(void)
{
  HAL_UART_IRQHandler(&UartHandle[USART2_INDEX]);
}

void USART3_8_IRQHandler(void)
{
  if (__HAL_GET_PENDING_IT(HAL_ITLINE_USART3)!= RESET)
  {
    HAL_UART_IRQHandler(&UartHandle[USART3_INDEX]);
  }
  if (__HAL_GET_PENDING_IT(HAL_ITLINE_USART4)!= RESET)
  {
     HAL_UART_IRQHandler(&UartHandle[USART4_INDEX]);
  }
  if (__HAL_GET_PENDING_IT(HAL_ITLINE_USART5)!= RESET)
  {
     HAL_UART_IRQHandler(&UartHandle[USART5_INDEX]);
  }
  if (__HAL_GET_PENDING_IT(HAL_ITLINE_USART6)!= RESET)
  {
     HAL_UART_IRQHandler(&UartHandle[USART6_INDEX]);
  }
  if (__HAL_GET_PENDING_IT(HAL_ITLINE_USART7)!= RESET)
  {
     HAL_UART_IRQHandler(&UartHandle[USART7_INDEX]);
  }
  if (__HAL_GET_PENDING_IT(HAL_ITLINE_USART8)!= RESET)
  {
     HAL_UART_IRQHandler(&UartHandle[USART8_INDEX]);
  }
}


/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/


/**
  * @}
  */ 

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
