/**
  ******************************************************************************
  * @file    PWR/PWR_CurrentConsumption/stm32f0xx_lp_modes.c
  * @author  MCD Application Team
  * @brief   This file provides firmware functions to manage the following
  *          functionalities of the STM32F0xx Low Power Modes:
  *           - Sleep Mode
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
static void SYSCLKConfig_STOP(void);

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
  *            - Wakeup using EXTI Line (User push-button PA.00 pin)
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

  /* Initialize led */
  BSP_LED_Init(LED3);
}

/**
  * @brief  This function configures the system to enter Stop mode with RTC 
  *         clocked by LSE or LSI for current consumption measurement purpose.
  *         STOP Mode with RTC clocked by LSE/LSI
  *         =====================================   
  *           - RTC Clocked by LSE or LSI
  *           - Regulator in LP mode
  *           - HSI, HSE OFF and LSI OFF if not used as RTC Clock source
  *           - No IWDG
  *           - Wakeup using EXTI Line (User push-button PA.00)
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



    /* Configure User Button */
  BSP_PB_Init(BUTTON_USER, BUTTON_MODE_EXTI);

  /* Enter Stop Mode */
  HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFI);

  /* Configures system clock after wake-up from STOP: enable HSI and PLL with HSI as source*/
  SYSCLKConfig_STOP();
  
  /* Initialize LED3 on the board */
  BSP_LED_Init(LED3);
  
  /* Turn LED3 On */
  BSP_LED_On(LED3);
  
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
  *           - Wakeup using WakeUp Pin PWR_WAKEUP_PIN1 connected to PA.00
  * @param  None
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
  * @brief  Configures system clock after wake-up from STOP: enable HSE, PLL
  *         and select PLL as system clock source.
  * @param  None
  * @retval None
  */
static void SYSCLKConfig_STOP(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;
  uint32_t pFLatency = 0;

  /* Get the Oscillators configuration according to the internal RCC registers */
  HAL_RCC_GetOscConfig(&RCC_OscInitStruct);

  /* Activate PLL with HSI as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_NONE;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV1;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL6;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /* Get the Clocks configuration according to the internal RCC registers */
  HAL_RCC_GetClockConfig(&RCC_ClkInitStruct, &pFLatency);

  /* Select PLL as system clock source and configure the HCLK and PCLK1 clocks dividers */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_SYSCLK;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, pFLatency) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
