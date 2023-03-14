/**
  ******************************************************************************
  * @file    CEC/CEC_ListenMode/Src/stm32f0xx_hal_msp.c 
  * @author  MCD Application Team
  * @brief   HAL MSP module.
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
#include "main.h"

/** @addtogroup STM32F0xx_HAL_Examples
  * @{
  */

/** @defgroup CEC_ListenMode
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
  * @brief CEC MSP Initialization 
  *        This function configures the hardware resources used in this example: 
  *           - Peripheral's clock enable
  *           - Peripheral's GPIO Configuration  
  * @param hcec: CEC handle pointer
  * @retval None
  */  
void HAL_CEC_MspInit(CEC_HandleTypeDef *hcec)
{
  GPIO_InitTypeDef  GPIO_InitStruct;
  
  /* Enable CEC clock */
  __HAL_RCC_CEC_CLK_ENABLE();  
    
  /* Enable GPIO clock and initialize GPIO */
  HDMI_CEC_LINE_CLK_ENABLE();
  GPIO_InitStruct.Pin = HDMI_CEC_LINE_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.Alternate = HDMI_CEC_LINE_AF;
  HAL_GPIO_Init(HDMI_CEC_LINE_GPIO_PORT, &GPIO_InitStruct);
  
  /* Enable and set CEC Interrupt to the lowest priority */
  HAL_NVIC_SetPriority(HDMI_CEC_IRQn, 0x0F, 0);
  HAL_NVIC_EnableIRQ(HDMI_CEC_IRQn);

}

/**
  * @brief CEC MSP De-Initialization 
  *        This function disable the Peripheral's clock
  * @param hcec: CEC handle pointer
  * @retval None
  */
void HAL_CEC_MspDeInit(CEC_HandleTypeDef *hcec)
{ 
  /* Disable CEC clock */ 
  __HAL_RCC_CEC_CLK_DISABLE();
  /* Disable GPIO TX/RX clock */
  HDMI_CEC_LINE_CLK_DISABLE();
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
