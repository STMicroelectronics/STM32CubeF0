/**
  ******************************************************************************
  * @file    ADC/ADC_LowPower/Inc/main.h
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
#include <stdio.h>
#include "stm32f0xx_hal.h"
#include "stm32072b_eval.h"
#include "stm32072b_eval_lcd.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* User can use this section to tailor ADCx instance under use and associated
   resources */

/* ## Definition of ADC related resources ################################### */
/* Definition of ADCx clock resources */
#define ADCx                            ADC1
#define ADCx_CLK_ENABLE()               __HAL_RCC_ADC1_CLK_ENABLE()

#define ADCx_FORCE_RESET()              __HAL_RCC_ADC1_FORCE_RESET()
#define ADCx_RELEASE_RESET()            __HAL_RCC_ADC1_RELEASE_RESET()

/* Definition of ADCx channels */
#define ADCx_CHANNELa                   ADC_CHANNEL_10

/* Definition of ADCx channels pins */
#define ADCx_CHANNELa_GPIO_CLK_ENABLE() __HAL_RCC_GPIOC_CLK_ENABLE()
#define ADCx_CHANNELa_GPIO_PORT         GPIOC
#define ADCx_CHANNELa_PIN               GPIO_PIN_0

/* Definition of ADCx NVIC resources */
#define ADCx_IRQn                       ADC1_COMP_IRQn
#define ADCx_IRQHandler                 ADC1_COMP_IRQHandler

/* ## Definition of TIM related resources ################################### */
/* Definition of TIMx clock resources */
#define TIMx                            TIM3
#define TIMx_CLK_ENABLE()               __HAL_RCC_TIM3_CLK_ENABLE()

#define TIMx_FORCE_RESET()              __HAL_RCC_TIM3_FORCE_RESET()
#define TIMx_RELEASE_RESET()            __HAL_RCC_TIM3_RELEASE_RESET()

#define ADC_EXTERNALTRIGCONV_Tx_TRGO    ADC_EXTERNALTRIGCONV_T3_TRGO

/* Exported macro ------------------------------------------------------------*/

/* Exported functions ------------------------------------------------------- */

#endif /* __MAIN_H */
