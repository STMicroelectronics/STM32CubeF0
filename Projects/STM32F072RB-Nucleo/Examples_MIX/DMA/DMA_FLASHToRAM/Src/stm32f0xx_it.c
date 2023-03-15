/**
  ******************************************************************************
  * @file    Examples_MIX/DMA/DMA_FLASHToRAM/Src/stm32f0xx_it.c
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

/** @addtogroup STM32F0xx_MIX_Examples
  * @{
  */

/** @addtogroup DMA_FLASHToRAM
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern DMA_HandleTypeDef   DmaHandle;
extern DMA_TypeDef*        DmaInstance;

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
  HAL_IncTick();
}

/******************************************************************************/
/*                 STM32F0xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f0xx.s).                                               */
/******************************************************************************/

/**
  * @brief  This function handles DMA channel interrupt request.
  * @param  None
  * @retval None
  */
void DMA_INSTANCE_IRQHANDLER(void)
{
  /* Check the interrupts and clear flags */

  /* Customize process using LL interface to improve performance           */
  /* (exhaustive feature management not handled)                           */
  /* Using LL interface, use :                                             */
  /* - LL_DMA_IsActiveFlag_TC1() to check complete DMA1_Channel1 Interrupt */
  /* - LL_DMA_IsActiveFlag_TE1() to check error DMA1_Channel1 Interrupt    */
  /* - LL_DMA_ClearFlag_TC1() to clear specific transfer complete flag     */
  if(LL_DMA_IsActiveFlag_TC1(DmaInstance) == 1)
  {
    LL_DMA_ClearFlag_TC1(DmaInstance);
    TransferComplete(&DmaHandle);
  }
  else if(LL_DMA_IsActiveFlag_TE1(DmaInstance) == 1)
  {
    TransferError(&DmaHandle);
  }

  /* Using HAL interface, use :                                             */
  /* - HAL_DMA_IRQHandler() to handle all DMA Interrupts (complete, errors) */
}

/**
  * @}
  */

/**
  * @}
  */
