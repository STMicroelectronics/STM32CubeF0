/**
  ******************************************************************************
  * @file    Examples_MIX/I2C/I2C_OneBoard_ComSlave7_10bits_IT/Inc/main.h
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
#include "stm32f0xx_nucleo.h"
#include "stm32f0xx_ll_i2c.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* User can use this section to tailor I2Cx/I2Cx instance used and associated
   resources */
/* Definition for I2Cx_MASTER clock resources */
#define I2Cx_MASTER                             I2C2
#define I2Cx_MASTER_CLK_ENABLE()                __HAL_RCC_I2C2_CLK_ENABLE()
#define I2Cx_MASTER_SDA_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOB_CLK_ENABLE()
#define I2Cx_MASTER_SCL_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOB_CLK_ENABLE() 

#define I2Cx_MASTER_FORCE_RESET()               __HAL_RCC_I2C2_FORCE_RESET()
#define I2Cx_MASTER_RELEASE_RESET()             __HAL_RCC_I2C2_RELEASE_RESET()

/* Definition for I2Cx_MASTER Pins */
#define I2Cx_MASTER_SCL_PIN                     GPIO_PIN_13
#define I2Cx_MASTER_SCL_GPIO_PORT               GPIOB
#define I2Cx_MASTER_SDA_PIN                     GPIO_PIN_14
#define I2Cx_MASTER_SDA_GPIO_PORT               GPIOB
#define I2Cx_MASTER_SCL_SDA_AF                  GPIO_AF5_I2C2

/* Definition for I2Cx_MASTER's NVIC */
#define I2Cx_MASTER_IRQn                        I2C2_IRQn
#define I2Cx_MASTER_IRQHandler                  I2C2_IRQHandler

/* Definition for I2Cx_SLAVE clock resources */
#define I2Cx_SLAVE                              I2C1
#define RCC_PERIPHCLK_I2Cx_SLAVE                RCC_PERIPHCLK_I2C1
#define RCC_I2Cx_SLAVECLKSOURCE_SYSCLK          RCC_I2C1CLKSOURCE_SYSCLK
#define I2Cx_SLAVE_CLK_ENABLE()                 __HAL_RCC_I2C1_CLK_ENABLE()
#define I2Cx_SLAVE_SDA_GPIO_CLK_ENABLE()        __HAL_RCC_GPIOB_CLK_ENABLE()
#define I2Cx_SLAVE_SCL_GPIO_CLK_ENABLE()        __HAL_RCC_GPIOB_CLK_ENABLE() 

#define I2Cx_SLAVE_FORCE_RESET()                __HAL_RCC_I2C1_FORCE_RESET()
#define I2Cx_SLAVE_RELEASE_RESET()              __HAL_RCC_I2C1_RELEASE_RESET()

/* Definition for I2Cx_SLAVE Pins */
#define I2Cx_SLAVE_SCL_PIN                      GPIO_PIN_6
#define I2Cx_SLAVE_SCL_GPIO_PORT                GPIOB
#define I2Cx_SLAVE_SDA_PIN                      GPIO_PIN_7
#define I2Cx_SLAVE_SDA_GPIO_PORT                GPIOB
#define I2Cx_SLAVE_SCL_SDA_AF                   GPIO_AF1_I2C1

/* Definition for I2Cx_SLAVE's NVIC */
#define I2Cx_SLAVE_IRQn                         I2C1_IRQn
#define I2Cx_SLAVE_IRQHandler                   I2C1_IRQHandler

/* Size of Transmission buffer */
#define TXBUFFERSIZE                            (COUNTOF(aTxBuffer) - 1)
/* Size of Reception buffer */
#define RXBUFFERSIZE                            TXBUFFERSIZE

/* Exported macro ------------------------------------------------------------*/
#define COUNTOF(__BUFFER__)   (sizeof(__BUFFER__) / sizeof(*(__BUFFER__)))
/* Exported functions ------------------------------------------------------- */

#endif /* __MAIN_H */
