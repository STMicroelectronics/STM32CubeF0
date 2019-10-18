/**
  ******************************************************************************
  * @file    TSC/TSC_BasicAcquisition_Polling/Inc/main.h
  * @author  MCD Application Team
  * @brief   Header for main.c module
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx_hal.h"
#include "stm32f072b_discovery.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* User can use this section to tailor TSCx/TSCx instance used and associated
   resources */
/* Definition for TSCx clock resources */
#define TSCx                                        TSC
#define TSCx_CLK_ENABLE()                           __HAL_RCC_TSC_CLK_ENABLE()
#define TSCx_TS1_IO_GPIO_CLK_ENABLE()               __HAL_RCC_GPIOA_CLK_ENABLE()
#define TSCx_TS2_IO_GPIO_CLK_ENABLE()               __HAL_RCC_GPIOA_CLK_ENABLE()
#define TSCx_TS3_IO_GPIO_CLK_ENABLE()               __HAL_RCC_GPIOB_CLK_ENABLE()
#define TSCx_TS1_CAPACITOR_GPIO_CLK_ENABLE()        __HAL_RCC_GPIOA_CLK_ENABLE()
#define TSCx_TS2_CAPACITOR_GPIO_CLK_ENABLE()        __HAL_RCC_GPIOA_CLK_ENABLE()
#define TSCx_TS3_CAPACITOR_GPIO_CLK_ENABLE()        __HAL_RCC_GPIOB_CLK_ENABLE()

#define TSCx_FORCE_RESET()                          __HAL_RCC_TSC_FORCE_RESET()
#define TSCx_RELEASE_RESET()                        __HAL_RCC_TSC_RELEASE_RESET()

/* Definition for TSCx IO Pins */
#define TSCx_TS1_IO_PIN                             GPIO_PIN_2
#define TSCx_TS1_IO_GPIO_PORT                       GPIOA
#define TSCx_TS1_IO_AF                              GPIO_AF3_TSC

#define TSCx_TS2_IO_PIN                             GPIO_PIN_6
#define TSCx_TS2_IO_GPIO_PORT                       GPIOA
#define TSCx_TS2_IO_AF                              GPIO_AF3_TSC

#define TSCx_TS3_IO_PIN                             GPIO_PIN_0
#define TSCx_TS3_IO_GPIO_PORT                       GPIOB
#define TSCx_TS3_IO_AF                              GPIO_AF3_TSC

/* Definition for TSCx Sampling Capacitor Pins */
#define TSCx_TS1_SAMPLING_PIN                       GPIO_PIN_3
#define TSCx_TS1_SAMPLING_GPIO_PORT                 GPIOA
#define TSCx_TS1_SAMPLING_AF                        GPIO_AF3_TSC

#define TSCx_TS2_SAMPLING_PIN                       GPIO_PIN_7
#define TSCx_TS2_SAMPLING_GPIO_PORT                 GPIOA
#define TSCx_TS2_SAMPLING_AF                        GPIO_AF3_TSC

#define TSCx_TS3_SAMPLING_PIN                       GPIO_PIN_1
#define TSCx_TS3_SAMPLING_GPIO_PORT                 GPIOB
#define TSCx_TS3_SAMPLING_AF                        GPIO_AF3_TSC

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
