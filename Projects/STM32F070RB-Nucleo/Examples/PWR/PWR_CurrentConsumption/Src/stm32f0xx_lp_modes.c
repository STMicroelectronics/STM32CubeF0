/**
  ******************************************************************************
  * @file    PWR/PWR_CurrentConsumption/stm32f0xx_lp_modes.c
  * @author  MCD Application Team
  * @brief   This file provides firmware functions to manage the following
  *          functionalities of the STM32F0xx Low Power Modes:
  *           - Sleep Mode
  *           - STOP mode without RTC
  *           - STOP mode with RTC
  *           - STANDBY mode without RTC
  *           - STANDBY mode with RTC 
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

/** @addtogroup PWR_CurrentConsumption
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* RTC handler declaration */
RTC_HandleTypeDef RTCHandle;
/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  This function configures the system to enter Sleep mode for
  *         current consumption measurement purpose.
  *         Sleep Mode
  *         ==========
  *            - System Running at PLL (48 MHz)
  *            - Flash 1 wait state
  *            - Prefetch ON
  *            - Code running from Internal FLASH
  *            - All peripherals disabled.
  *            - Wakeup using EXTI Line (User push-button PC.13 pin)
  * @param  None
  * @retval None
  */
void SleepMode_Measure(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;

  /* Configure all GPIO as analog to reduce current consumption on non used IOs */
  /* Enable GPIOs clock */
  /* Warning : Reconfiguring all GPIO will close the connexion with the debugger */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();

  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Pin = GPIO_PIN_All;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
  HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

  /* Disable GPIOs clock */
  __HAL_RCC_GPIOA_CLK_DISABLE();
  __HAL_RCC_GPIOB_CLK_DISABLE();
  __HAL_RCC_GPIOC_CLK_DISABLE();
  __HAL_RCC_GPIOD_CLK_DISABLE();
  __HAL_RCC_GPIOF_CLK_DISABLE();

  /* Configure User push-button as external interrupt generator */
  BSP_PB_Init(BUTTON_USER, BUTTON_MODE_EXTI);

  /*Suspend Tick increment to prevent wakeup by Systick interrupt. 
    Otherwise the Systick interrupt will wake up the device within 1ms (HAL time base)*/
  HAL_SuspendTick();

  /* Request to enter SLEEP mode */
  HAL_PWR_EnterSLEEPMode(PWR_MAINREGULATOR_ON, PWR_SLEEPENTRY_WFI);

  /* Resume Tick interrupt if disabled prior to sleep mode entry*/
  HAL_ResumeTick();

  /* Initialize LED2 on the board */
  BSP_LED_Init(LED2);
  
  /* Turn LED2 On */
  BSP_LED_On(LED2);
  
  /* Inserted Delay */
  HAL_Delay(200);
  
}

/**
  * @brief  This function configures the system to enter Stop mode with RTC 
  *         clocked by LSI for current consumption measurement purpose.
  *         STOP Mode with RTC clocked by LSI
  *         =====================================   
  *           - RTC Clocked by LSE or LSI
  *           - Regulator in LP mode
  *           - HSI, HSE OFF and LSI OFF if not used as RTC Clock source
  *           - No IWDG
  *           - Wakeup using EXTI Line (User push-button PC.13)
  * @param  None
  * @retval None
  */
void StopMode_Measure(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  
  /* Configure all GPIO as analog to reduce current consumption on non used IOs */
  /* Warning : Reconfiguring all GPIO will close the connexion with the debugger */
  /* Enable GPIOs clock */

  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();

  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Pin = GPIO_PIN_All;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
  HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

  /* Disable GPIOs clock */
  __HAL_RCC_GPIOA_CLK_DISABLE();
  __HAL_RCC_GPIOB_CLK_DISABLE();
  __HAL_RCC_GPIOC_CLK_DISABLE();
  __HAL_RCC_GPIOD_CLK_DISABLE();
  __HAL_RCC_GPIOF_CLK_DISABLE();



  /* Configure User push-button as external interrupt generator */
  BSP_PB_Init(BUTTON_USER, BUTTON_MODE_EXTI);

  /* Enter Stop Mode */
  HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFI);

  /* Configures system clock after wake-up from STOP: enable HSI and PLL with HSI as source*/
  SystemClock_Config();
  
  /* Initialize LED2 on the board */
  BSP_LED_Init(LED2);
  
  /* Turn LED2 On */
  BSP_LED_On(LED2);
  
  /* Inserted Delay */
  HAL_Delay(200);

  
}

/**
  * @brief  This function configures the system to enter Standby mode for
  *         current consumption measurement purpose.
  *         STANDBY Mode
  *         ============
  *           - RTC OFF
  *           - IWDG and LSI OFF
  *           - Wakeup using WakeUp Pin (wire Vdd to PA.00)
  * @retval None
  */
void StandbyMode_Measure(void)
{
  /* Enable Power Clock*/
  __HAL_RCC_PWR_CLK_ENABLE();

  /* Allow access to Backup */
  HAL_PWR_EnableBkUpAccess();

  /* Reset RTC Domain */
  __HAL_RCC_BACKUPRESET_FORCE();
  __HAL_RCC_BACKUPRESET_RELEASE();
  
  /* The Following Wakeup sequence is highly recommended prior to each Standby mode entry
     mainly  when using more than one wakeup source this is to not miss any wakeup event.
       - Disable all used wakeup sources,
       - Clear all related wakeup flags, 
       - Re-enable all used wakeup sources,
       - Enter the Standby mode.
  */
  
  /*#### Disable all used wakeup sources: WKUP pin ###########################*/
  HAL_PWR_DisableWakeUpPin(PWR_WAKEUP_PIN1);
  
  /*#### Clear all related wakeup flags ######################################*/
  /* Clear PWR wake up Flag */
  __HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);
  
  /* Enable WKUP pin */
  HAL_PWR_EnableWakeUpPin(PWR_WAKEUP_PIN1);

  /* Request to enter STANDBY mode */
  HAL_PWR_EnterSTANDBYMode();
}



/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
