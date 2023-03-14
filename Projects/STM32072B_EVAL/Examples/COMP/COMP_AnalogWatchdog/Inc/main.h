/**
  ******************************************************************************
  * @file    COMP/COMP_AnalogWatchdog/Inc/main.h
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
#include "stm32072b_eval.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* User can use this section to tailor COMPx instance used and associated 
   resources */
/* Definition for COMPx clock resources */
#define COMPx_CLK_ENABLE()              __HAL_RCC_SYSCFG_CLK_ENABLE()
#define COMPx_GPIO_CLK_ENABLE()         __HAL_RCC_GPIOA_CLK_ENABLE()

/* Definition for COMPx Channel Pin */
#define COMPx_PIN                        GPIO_PIN_1
#define COMPx_GPIO_PORT                  GPIOA 

/* Definition for COMPx's NVIC */
#define COMPx_IRQn                    ADC1_COMP_IRQn
#define COMPx_IRQHandler              ADC1_COMP_IRQHandler


/* Exported macro ------------------------------------------------------------*/
#define STATE_OVER_THRESHOLD    0x00000001
#define STATE_WITHIN_THRESHOLD  0x00000002
#define STATE_UNDER_THRESHOLD   0x00000003

/* Exported functions ------------------------------------------------------- */
void InputVoltageLevel_Check(void);

#endif /* __MAIN_H */
