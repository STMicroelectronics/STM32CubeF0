/**
  ******************************************************************************
  * @file    TouchSensing\TouchSensing_Linear\Src\stm32f0xx_hal_msp.c
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

/** @defgroup HAL_MSP
  * @brief HAL MSP module.
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/** @defgroup HAL_MSP_Private_Functions
  * @{
  */

/**
  * @brief TSC MSP Initialization
  *        This function configures the hardware resources used in this example:
  *           - Peripheral's clock enable
  *           - Peripheral's GPIO Configuration
  * @param htsc: TSC handle pointer
  * @retval None
  */
void HAL_TSC_MspInit(TSC_HandleTypeDef* htsc)
{
  GPIO_InitTypeDef GPIO_InitStruct;

  /*##-1- Enable TSC and GPIO clocks #########################################*/
  TSCx_CLK_ENABLE();
  TSCx_TS1_IO_GPIO_CLK_ENABLE();
  TSCx_TS2_IO_GPIO_CLK_ENABLE();
  TSCx_TS3_IO_GPIO_CLK_ENABLE();

  /*##-2- Configure Sampling Capacitor IOs (Alternate-Function Open-Drain) ###*/
  GPIO_InitStruct.Mode      = GPIO_MODE_AF_OD;
  GPIO_InitStruct.Pull      = GPIO_NOPULL;
  GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_LOW;
  
  /* Channel TS1 Sampling Capacitor IO = TSC_GROUP1_IO4 */
  GPIO_InitStruct.Pin       = TSCx_TS1_SAMPLING_PIN;
  GPIO_InitStruct.Alternate = TSCx_TS1_SAMPLING_AF;
  HAL_GPIO_Init(TSCx_TS1_SAMPLING_GPIO_PORT, &GPIO_InitStruct);
  
  /* Channel TS2 Sampling Capacitor IO = TSC_GROUP2_IO4 */
  GPIO_InitStruct.Pin       = TSCx_TS2_SAMPLING_PIN;
  GPIO_InitStruct.Alternate = TSCx_TS2_SAMPLING_AF;
  HAL_GPIO_Init(TSCx_TS2_SAMPLING_GPIO_PORT, &GPIO_InitStruct);

  /* Channel TS3 Sampling Capacitor IO = TSC_GROUP3_IO3 */
  GPIO_InitStruct.Pin       = TSCx_TS3_SAMPLING_PIN;
  GPIO_InitStruct.Alternate = TSCx_TS3_SAMPLING_AF;
  HAL_GPIO_Init(TSCx_TS3_SAMPLING_GPIO_PORT, &GPIO_InitStruct);

  /*##-3- Configure Channel IOs (Alternate-Function Output PP) ###############*/
  GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull      = GPIO_NOPULL;
  GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_LOW;

  /* Channel IO (TS1) = TSC_GROUP1_IO3 */
  GPIO_InitStruct.Pin       = TSCx_TS1_IO_PIN;
  GPIO_InitStruct.Alternate = TSCx_TS1_IO_AF;
  HAL_GPIO_Init(TSCx_TS1_IO_GPIO_PORT, &GPIO_InitStruct);

  /* Channel IO (TS2) = TSC_GROUP2_IO3 */
  GPIO_InitStruct.Pin       = TSCx_TS2_IO_PIN;
  GPIO_InitStruct.Alternate = TSCx_TS2_IO_AF;
  HAL_GPIO_Init(TSCx_TS2_IO_GPIO_PORT, &GPIO_InitStruct);
  
  /* Channel IO (TS3) = TSC_GROUP3_IO2 */
  GPIO_InitStruct.Pin       = TSCx_TS3_IO_PIN;
  GPIO_InitStruct.Alternate = TSCx_TS3_IO_AF;
  HAL_GPIO_Init(TSCx_TS3_IO_GPIO_PORT, &GPIO_InitStruct);
}

/**
  * @brief TSC MSP De-Initialization
  *        This function frees the hardware resources used in this example:
  *          - Disable the Peripheral's clock
  *          - Revert GPIO to their default state
  * @param htsc: TSC handle pointer
  * @retval None
  */
void HAL_TSC_MspDeInit(TSC_HandleTypeDef* htsc)
{
  /*##-1- Reset peripherals ##################################################*/
  TSCx_FORCE_RESET();
  TSCx_RELEASE_RESET();

  /*##-2- Disable peripherals and GPIO Clocks ################################*/
  /* De-initialize the TSC GPIO pins */
  HAL_GPIO_DeInit(TSCx_TS1_IO_GPIO_PORT, TSCx_TS1_IO_PIN);
  HAL_GPIO_DeInit(TSCx_TS1_SAMPLING_GPIO_PORT, TSCx_TS1_SAMPLING_PIN);
  HAL_GPIO_DeInit(TSCx_TS2_IO_GPIO_PORT, TSCx_TS2_IO_PIN);
  HAL_GPIO_DeInit(TSCx_TS2_SAMPLING_GPIO_PORT, TSCx_TS2_SAMPLING_PIN);
  HAL_GPIO_DeInit(TSCx_TS3_IO_GPIO_PORT, TSCx_TS3_IO_PIN);
  HAL_GPIO_DeInit(TSCx_TS3_SAMPLING_GPIO_PORT, TSCx_TS3_SAMPLING_PIN);
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
