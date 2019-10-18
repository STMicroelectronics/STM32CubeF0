/**
  ******************************************************************************
  * @file    COMP/COMP_Interrupt/Src/stm32f0xx_hal_msp.c
  * @author  MCD Application Team
  * @brief   HAL MSP module.
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

/** @addtogroup STM32F0xx_HAL_Examples
  * @{
  */

/** @defgroup COMP_Interrupt
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* COMP handler declared in "main.c" file */
extern COMP_HandleTypeDef     hcomp1;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/** @defgroup HAL_MSP_Private_Functions
  * @{
  */

/**
  * @brief COMP MSP Initialization 
  *        This function configures the hardware resources used in this example:
  *           - Peripheral's clock enable
  * @param hcomp: COMP handle pointer
  * @retval None
  */
void HAL_COMP_MspInit(COMP_HandleTypeDef* hcomp)
{
  GPIO_InitTypeDef       GPIO_InitStruct;

  /*##-1- Enable peripherals and GPIO Clocks #################################*/
  /* Enable GPIO clock */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  
  /*##-2- Configure peripheral GPIO ##########################################*/ 
  /* COMP1 Analog GPIO pin configuration */
  GPIO_InitStruct.Pin   = GPIO_PIN_1;
  GPIO_InitStruct.Mode  = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull  = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*##-3- Configure the NVIC for COMP1 #######################################*/
   /* Enable the COMP1 IRQ Channel */
  HAL_NVIC_SetPriority(ADC1_COMP_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(ADC1_COMP_IRQn);
}

/**
  * @brief  DeInitializes the COMP MSP.
  * @param  hcomp: pointer to a COMP_HandleTypeDef structure that contains
  *         the configuration information for the specified COMP.  
  * @retval None
  */
void HAL_COMP_MspDeInit(COMP_HandleTypeDef* hcomp)
{
  /*##-1- Reset peripherals ##################################################*/
  /* Disable COMP1 clock */
  __HAL_RCC_SYSCFG_CLK_DISABLE();
  /* Disable GPIO clock */
  __HAL_RCC_GPIOA_CLK_DISABLE();

  /*##-2- Disable peripherals and GPIO  ######################################*/
  /* De-Initialize COMP1 */
  HAL_COMP_DeInit(&hcomp1);
  /* De-initialize the GPIO pin */
  HAL_GPIO_DeInit(GPIOA, GPIO_PIN_1);

  /*##-3- Disable the NVIC for COMP ##########################################*/
  HAL_NVIC_DisableIRQ(ADC1_COMP_IRQn);
}


/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
