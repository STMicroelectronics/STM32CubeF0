/**
  ******************************************************************************
  * @file    Examples_MIX/ADC/ADC_SingleConversion_TriggerSW_IT/Inc/stm32f0xx_it.h
  * @author  MCD Application Team
  * @brief   This file contains the headers of the interrupt handlers.
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
#ifndef __STM32F0xx_IT_H
#define __STM32F0xx_IT_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

void NMI_Handler(void);
void HardFault_Handler(void);
void SVC_Handler(void);
void PendSV_Handler(void);
void SysTick_Handler(void);

void EXTI4_15_IRQHandler(void);

/* Note: Lines of code commented below correspond to the example using        */
/*       HAL driver only.                                                     */
/*       This example demonstrating a mix of HAL and LL drivers has replaced  */
/*       these lines using LL driver.                                         */
// void ADCx_IRQHandler(void);

void ADC1_COMP_IRQHandler(void);
#if defined(WAVEFORM_GENERATION)
void TIM6_DAC_IRQHandler(void);

#endif /* WAVEFORM_GENERATION */

#ifdef __cplusplus
}
#endif

#endif /* __STM32F0xx_IT_H */
