/**
  ******************************************************************************
  * @file    COMP/COMP_AnalogWatchdog/Inc/main.h
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
#include "stm32f0xx_nucleo.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* User can use this section to tailor COMPx instance used and associated 
   resources */
/* Definition for COMPx clock resources */
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
