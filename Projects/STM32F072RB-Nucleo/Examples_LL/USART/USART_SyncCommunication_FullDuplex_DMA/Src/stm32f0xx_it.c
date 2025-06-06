/**
  ******************************************************************************
  * @file    Examples_LL/USART/USART_SyncCommunication_FullDuplex_DMA/Src/stm32f0xx_it.c
  * @author  MCD Application Team
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2016 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx_it.h"

/** @addtogroup STM32F0xx_LL_Examples
  * @{
  */

/** @addtogroup USART_SyncCommunication_FullDuplex_DMA
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M0 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
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
}

/******************************************************************************/
/*                 STM32F0xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s), for the        */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f0xx.s).                                               */
/******************************************************************************/

/**
  * @brief  This function handles external line 4_15 interrupt request.
  * @param  None
  * @retval None
  */
void USER_BUTTON_IRQHANDLER(void)
{
  /* Manage Flags */
  if(LL_EXTI_IsActiveFlag_0_31(USER_BUTTON_EXTI_LINE) != RESET)
  {
    LL_EXTI_ClearFlag_0_31(USER_BUTTON_EXTI_LINE);

    /* Handle user button press in dedicated function */
    UserButton_Callback(); 
  }
}

/**
  * @brief  This function handles DMA1_Channel4_5_6_7 interrupt request.
  * @param  None
  * @retval None
  */
void DMA1_Channel4_5_6_7_IRQHandler(void)
{

  if(LL_DMA_IsActiveFlag_TC4(DMA1))
  {
    LL_DMA_ClearFlag_TC4(DMA1);
    /* Call function Transmission complete Callback */
    USART1_DMA1_TransmitComplete_Callback();
  }
  else if(LL_DMA_IsActiveFlag_TE4(DMA1))
  {
    /* Call Error function */
    USART_TransferError_Callback();
  }
  if(LL_DMA_IsActiveFlag_TC5(DMA1))
  {
    LL_DMA_ClearFlag_TC5(DMA1);
    /* Call function Reception complete Callback */
    USART1_DMA1_ReceiveComplete_Callback();
  }
  else if(LL_DMA_IsActiveFlag_TE5(DMA1))
  {
    /* Call Error function */
    USART_TransferError_Callback();
  }
}


/**
  * @brief  This function handles DMA1_Channel2_3 interrupt request.
  * @param  None
  * @retval None
  */
void DMA1_Channel2_3_IRQHandler(void)
{

  if(LL_DMA_IsActiveFlag_TC3(DMA1))
  {
    LL_DMA_ClearFlag_TC3(DMA1);
    /* Call function Transmission complete Callback */
    SPI1_DMA1_TransmitComplete_Callback();
  }
  else if(LL_DMA_IsActiveFlag_TE3(DMA1))
  {
    /* Call Error function */
    SPI_TransferError_Callback();
  }
  if(LL_DMA_IsActiveFlag_TC2(DMA1))
  {
    LL_DMA_ClearFlag_TC2(DMA1);
    /* Call function Reception complete Callback */
    SPI1_DMA1_ReceiveComplete_Callback();
  }
  else if(LL_DMA_IsActiveFlag_TE2(DMA1))
  {
    /* Call Error function */
    SPI_TransferError_Callback();
  }
}


/**
  * @}
  */

/**
  * @}
  */
