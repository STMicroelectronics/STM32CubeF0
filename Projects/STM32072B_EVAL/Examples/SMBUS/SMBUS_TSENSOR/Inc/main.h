/**
  ******************************************************************************
  * @file    SMBUS/SMBUS_TSENSOR/Inc/main.h 
  * @author  MCD Application Team
  * @brief   Header for main.c module
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
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx_hal.h"
#include "stm32f0xx_tsensor.h"
#include "stm32072b_eval.h"
#include "stm32072b_eval_lcd.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* User can use this section to tailor SMBUSx/SMBUSx instance used and associated
   resources */
/* Definition for SMBUSx clock resources */
#define SMBUSx_RCC_PERIPHCLK              RCC_PERIPHCLK_I2C1
#define SMBUSx_CLKSOURCE_SYSCLK           RCC_I2C1CLKSOURCE_SYSCLK
#define SMBUSx_CLK_ENABLE()               __HAL_RCC_I2C1_CLK_ENABLE()
#define SMBUSx_SDA_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOB_CLK_ENABLE()
#define SMBUSx_SCL_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOB_CLK_ENABLE() 

#define SMBUSx_FORCE_RESET()              __HAL_RCC_I2C1_FORCE_RESET()
#define SMBUSx_RELEASE_RESET()            __HAL_RCC_I2C1_RELEASE_RESET()

/* Definition for SMBUSx Pins */
#define SMBUSx_SCL_PIN                    GPIO_PIN_6
#define SMBUSx_SCL_GPIO_PORT              GPIOB
#define SMBUSx_SDA_PIN                    GPIO_PIN_7
#define SMBUSx_SDA_GPIO_PORT              GPIOB
#define SMBUSx_SCL_SDA_AF                 GPIO_AF1_I2C1
#define SMBUSx_ALERT_PIN                  GPIO_PIN_5
#define SMBUSx_ALERT_GPIO_PORT            GPIOB
#define SMBUSx_ALERT_AF                   GPIO_AF3_I2C1

/* Definition for SMBUSx's NVIC */
#define SMBUSx_IRQn                       I2C1_IRQn
#define SMBUSx_IRQHandler                 I2C1_IRQHandler
#endif /* __MAIN_H */
