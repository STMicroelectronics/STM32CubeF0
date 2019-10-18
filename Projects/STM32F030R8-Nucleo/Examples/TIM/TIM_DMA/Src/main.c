/**
  ******************************************************************************
  * @file    TIM/TIM_DMA/Src/main.c
  * @author  MCD Application Team
  * @brief   This sample code shows how to use DMA with TIM1 Update request to
  *          transfer Data from memory to TIM1 Capture Compare Register 3 (CCR3).
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

/** @addtogroup TIM_DMA
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Timer handler declaration */
TIM_HandleTypeDef    TimHandle;

/* Timer Output Compare Configuration Structure declaration */
TIM_OC_InitTypeDef sConfig;

/* Capture Compare buffer */
uint32_t aCCValue_Buffer[3] = {0, 0, 0};

/* Timer Period*/
uint32_t uwTimerPeriod  = 0;

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void Error_Handler(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{
 /* This sample code shows how to use DMA with TIM1 Update request to transfer
  Data from memory to TIM1 Capture Compare Register 3 (CCR3), through the
  STM32F0xx HAL API. To proceed, 3 steps are required */

  /* STM32F0xx HAL library initialization:
       - Configure the Flash prefetch
       - Systick timer is configured by default as source of time base, but user 
         can eventually implement his proper time base source (a general purpose 
         timer for example or other time source), keeping in mind that Time base 
         duration should be kept 1ms since PPP_TIMEOUT_VALUEs are defined and 
         handled in milliseconds basis.
       - Low Level Initialization
     */
  HAL_Init();

  /* Configure the system clock to 48 MHz */
  SystemClock_Config();

  /* Configure LED2 */
  BSP_LED_Init(LED2);

  /* Compute the value of ARR regiter to generate signal frequency at 17.57 Khz */
  uwTimerPeriod = (uint32_t)((SystemCoreClock / 17570) - 1);
  /* Compute CCR1 value to generate a duty cycle at 75% */
  aCCValue_Buffer[0] = (uint32_t)(((uint32_t) 75 * (uwTimerPeriod - 1)) / 100);
  /* Compute CCR2 value to generate a duty cycle at 50% */
  aCCValue_Buffer[1] = (uint32_t)(((uint32_t) 50 * (uwTimerPeriod - 1)) / 100);
  /* Compute CCR3 value to generate a duty cycle at 25% */
  aCCValue_Buffer[2] = (uint32_t)(((uint32_t) 25 * (uwTimerPeriod - 1)) / 100);

  /*##-1- Configure the TIM peripheral #######################################*/
  /* ---------------------------------------------------------------------------
  TIM1 input clock (TIM1CLK) is set to APB1 clock (PCLK1), since APB1
  prescaler is 1.
    TIM1CLK = PCLK1
    PCLK1 = HCLK
    => TIM1CLK = HCLK = SystemCoreClock

  TIM1CLK = SystemCoreClock, Prescaler = 0, TIM1 counter clock = SystemCoreClock
  SystemCoreClock is set to 48 MHz for STM32F0xx devices.

  The objective is to configure TIM1 channel 3 to generate a PWM
  signal with a frequency equal to 17.57 KHz:
     - TIM1_Period = (SystemCoreClock / 17570) - 1
  and a variable duty cycle that is changed by the DMA after a specific number of
  Update DMA request.

  The number of this repetitive requests is defined by the TIM1 Repetition counter,
  each 4 Update Requests, the TIM1 Channel 3 Duty Cycle changes to the next new
  value defined by the aCCValue_Buffer.

    Note:
     SystemCoreClock variable holds HCLK frequency and is defined in system_stm32f0xx.c file.
     Each time the core clock (HCLK) changes, user had to update SystemCoreClock
     variable value. Otherwise, any configuration based on this variable will be incorrect.
     This variable is updated in three ways:
      1) by calling CMSIS function SystemCoreClockUpdate()
      2) by calling HAL API function HAL_RCC_GetSysClockFreq()
      3) each time HAL_RCC_ClockConfig() is called to configure the system clock frequency
  -----------------------------------------------------------------------------*/
  /* Initialize TIM1 peripheral as follows:
      + Period = TimerPeriod (To have an output frequency equal to 17.570 KHz)
      + Repetition Counter = 3
      + Prescaler = 0
      + ClockDivision = 0
      + Counter direction = Up
  */
  TimHandle.Instance = TIMx;

  TimHandle.Init.Period            = uwTimerPeriod;
  TimHandle.Init.RepetitionCounter = 3;
  TimHandle.Init.Prescaler         = 0;
  TimHandle.Init.ClockDivision     = 0;
  TimHandle.Init.CounterMode       = TIM_COUNTERMODE_UP;
  TimHandle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_PWM_Init(&TimHandle) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }

  /*##-2- Configure the PWM channel 3 ########################################*/
  sConfig.OCMode       = TIM_OCMODE_PWM1;
  sConfig.OCPolarity   = TIM_OCPOLARITY_HIGH;
  sConfig.Pulse        = aCCValue_Buffer[0];
  sConfig.OCNPolarity  = TIM_OCNPOLARITY_HIGH;
  sConfig.OCFastMode   = TIM_OCFAST_DISABLE;
  sConfig.OCIdleState  = TIM_OCIDLESTATE_RESET;
  sConfig.OCNIdleState = TIM_OCNIDLESTATE_RESET;
  if (HAL_TIM_PWM_ConfigChannel(&TimHandle, &sConfig, TIM_CHANNEL_3) != HAL_OK)
  {
    /* Configuration Error */
    Error_Handler();
  }

  /*##-3- Start PWM signal generation in DMA mode ############################*/
  if (HAL_TIM_PWM_Start_DMA(&TimHandle, TIM_CHANNEL_3, aCCValue_Buffer, 3) != HAL_OK)
  {
    /* Starting Error */
    Error_Handler();
  }

  while (1)
  {
  }

}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
static void Error_Handler(void)
{
  /* Turn LED2 on */
  BSP_LED_On(LED2);
  while (1)
  {
  }
}


/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
  *            System Clock source            = PLL (HSI/2)
  *            SYSCLK(Hz)                     = 48000000
  *            HCLK(Hz)                       = 48000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 1
  *            HSI Frequency(Hz)              = 8000000
  *            PREDIV                         = 1
  *            PLLMUL                         = 12
  *            Flash Latency(WS)              = 1
  * @param  None
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;
  
  /* No HSE Oscillator on Nucleo, Activate PLL with HSI/2 as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_NONE;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV1;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL12;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct)!= HAL_OK)
  {
    /* Initialization Error */
    while(1); 
  }

  /* Select PLL as system clock source and configure the HCLK, PCLK1 clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1)!= HAL_OK)
  {
    /* Initialization Error */
    while(1); 
  }
}
#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}

#endif

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
