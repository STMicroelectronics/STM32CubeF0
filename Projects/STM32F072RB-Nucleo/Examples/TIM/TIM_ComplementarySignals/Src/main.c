/**
  ******************************************************************************
  * @file    TIM/TIM_ComplementarySignals/Src/main.c 
  * @author  MCD Application Team
  * @brief   This sample code shows how to use STM32F0xx TIM HAL API to generate
  *          3 signals in PWM with its complementaries.
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

/** @addtogroup TIM_ComplementarySignals
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
#define  PERIOD_VALUE       (1200 - 1)  /* Period Value  */
#define  PULSE1_VALUE       600         /* Capture Compare 1 Value  */
#define  PULSE2_VALUE       300         /* Capture Compare 2 Value  */
#define  PULSE3_VALUE       150         /* Capture Compare 3 Value  */


/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Timer handler declaration */
TIM_HandleTypeDef    TimHandle;

/* Timer Output Compare Configuration Structure declaration */
TIM_OC_InitTypeDef              sPWMConfig;
/* Timer Break Configuration Structure declaration */
TIM_BreakDeadTimeConfigTypeDef sBreakConfig;

/* Counter Prescaler value */
uint32_t uwPrescalerValue = 0;

/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
static void Error_Handler(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{

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

  /* Configure LED2 */
  BSP_LED_Init(LED2);

  /* Configure the system clock to 48 MHz */
  SystemClock_Config();

  /* Compute the prescaler value to have TIM1 counter clock equal to 12MHz */
  uwPrescalerValue = (uint32_t) ((SystemCoreClock  / 12000000) - 1);
  
  
  /*##-1- Configure the TIM peripheral #######################################*/ 
  /* --------------------------------------------------------------------------- 
  1/ Generate 3 complementary PWM signals with 3 different duty cycles:
  
    TIM1 input clock (TIM1CLK) is set to APB1 clock (PCLK1), since APB1
    prescaler is 1.
    TIM1CLK = PCLK1
    PCLK1 = HCLK
    => TIM1CLK = HCLK = SystemCoreClock
  
    TIM1CLK is fixed to SystemCoreClock, the TIM1 Prescaler is set to have
    TIM1 counter clock = 12MHz.

    The objective is to generate PWM signal at 10 KHz:
    - TIM1_Period = (TIM1 counter clock / 10000) - 1

    The Three Duty cycles are computed as the following description: 

    The channel 1 duty cycle is set to 50% so channel 1N is set to 50%.
    The channel 2 duty cycle is set to 25% so channel 2N is set to 75%.
    The channel 3 duty cycle is set to 12.5% so channel 3N is set to 87.5%.
    
   The Timer pulse is calculated as follows:
     - ChannelxPulse = DutyCycle * (TIM1_Period - 1) / 100
          
  2/ Insert a dead time equal to (100/SystemCoreClock) us

  3/ Configure the break feature, active at High level, and using the automatic 
     output enable feature
       
  4/ Use the Locking parameters level1. 
  
    Note: 
     SystemCoreClock variable holds HCLK frequency and is defined in system_stm32f0xx.c file.
     Each time the core clock (HCLK) changes, user had to update SystemCoreClock 
     variable value. Otherwise, any configuration based on this variable will be incorrect.
     This variable is updated in three ways:
      1) by calling CMSIS function SystemCoreClockUpdate()
      2) by calling HAL API function HAL_RCC_GetSysClockFreq()
      3) each time HAL_RCC_ClockConfig() is called to configure the system clock frequency     
  --------------------------------------------------------------------------- */

  /* Initialize TIM peripheral as follows:
       + Prescaler = (SystemCoreClock/12000000) - 1
       + Period = (1200 - 1)  (to have an output frequency equal to 10 KHz)
       + ClockDivision = 0
       + Counter direction = Up
  */
  /* Select the Timer instance */
  TimHandle.Instance = TIM1;
  
  TimHandle.Init.Prescaler         = uwPrescalerValue;
  TimHandle.Init.Period            = PERIOD_VALUE;
  TimHandle.Init.ClockDivision     = 0;
  TimHandle.Init.CounterMode       = TIM_COUNTERMODE_UP;
  TimHandle.Init.RepetitionCounter = 0;
  TimHandle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if(HAL_TIM_PWM_Init(&TimHandle) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }

  /*##-2- Configure the PWM channels #########################################*/ 
  /* Common configuration for all channels */
  sPWMConfig.OCMode       = TIM_OCMODE_PWM1;
  sPWMConfig.OCPolarity   = TIM_OCPOLARITY_HIGH;
  sPWMConfig.OCNPolarity  = TIM_OCNPOLARITY_HIGH;
  sPWMConfig.OCIdleState  = TIM_OCIDLESTATE_SET;
  sPWMConfig.OCNIdleState = TIM_OCNIDLESTATE_RESET;  
  sPWMConfig.OCFastMode   = TIM_OCFAST_DISABLE;  

  /* Set the pulse value for channel 1 */
  sPWMConfig.Pulse = PULSE1_VALUE;  
  if(HAL_TIM_PWM_ConfigChannel(&TimHandle, &sPWMConfig, TIM_CHANNEL_1) != HAL_OK)
  {
    /* Configuration Error */
    Error_Handler();
  }
  
  /* Set the pulse value for channel 2 */
  sPWMConfig.Pulse = PULSE2_VALUE;
  if(HAL_TIM_PWM_ConfigChannel(&TimHandle, &sPWMConfig, TIM_CHANNEL_2) != HAL_OK)
  {
    /* Configuration Error */
    Error_Handler();
  }
  
  /* Set the pulse value for channel 3 */
  sPWMConfig.Pulse = PULSE3_VALUE;
  if(HAL_TIM_PWM_ConfigChannel(&TimHandle, &sPWMConfig, TIM_CHANNEL_3) != HAL_OK)
  {
    /* Configuration Error */
    Error_Handler();
  }
  
  /* Set the Break feature & Dead time */
  sBreakConfig.BreakState       = TIM_BREAK_ENABLE;
  sBreakConfig.DeadTime         = 100;
  sBreakConfig.OffStateRunMode  = TIM_OSSR_ENABLE;
  sBreakConfig.OffStateIDLEMode = TIM_OSSI_ENABLE;
  sBreakConfig.LockLevel        = TIM_LOCKLEVEL_1;  
  sBreakConfig.BreakPolarity    = TIM_BREAKPOLARITY_HIGH;
  sBreakConfig.AutomaticOutput  = TIM_AUTOMATICOUTPUT_ENABLE;
  
  if(HAL_TIMEx_ConfigBreakDeadTime(&TimHandle, &sBreakConfig) != HAL_OK)
  {
    /* Configuration Error */
    Error_Handler();
  }

  /*##-3- Start PWM signals generation #######################################*/ 
  /* Start channel 1 */
  if(HAL_TIM_PWM_Start(&TimHandle, TIM_CHANNEL_1) != HAL_OK)
  {
    /* Starting Error */
    Error_Handler();
  }
  /* Start channel 1N */
  if(HAL_TIMEx_PWMN_Start(&TimHandle, TIM_CHANNEL_1) != HAL_OK)
  {
    /* Starting Error */
    Error_Handler();
  }
  
  /* Start channel 2 */
  if(HAL_TIM_PWM_Start(&TimHandle, TIM_CHANNEL_2) != HAL_OK)
  {
    /* Starting Error */
    Error_Handler();
  }
  /* Start channel 2N */
  if(HAL_TIMEx_PWMN_Start(&TimHandle, TIM_CHANNEL_2) != HAL_OK)
  {
    /* Starting Error */
    Error_Handler();
  }
  
  /* Start channel 3 */
  if(HAL_TIM_PWM_Start(&TimHandle, TIM_CHANNEL_3) != HAL_OK)
  {
    /* Starting Error */
    Error_Handler();
  }
  /* Start channel 3N */
  if(HAL_TIMEx_PWMN_Start(&TimHandle, TIM_CHANNEL_3) != HAL_OK)
  {
    /* Starting Error */
    Error_Handler();
  }

  while (1)
  {
  }
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
  *            System Clock source            = PLL (HSI48)
  *            SYSCLK(Hz)                     = 48000000
  *            HCLK(Hz)                       = 48000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 1
  *            HSI Frequency(Hz)              = 48000000
  *            PREDIV                         = 2
  *            PLLMUL                         = 2
  *            Flash Latency(WS)              = 1
  * @param  None
  * @retval None
  */
static void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;
  
  /* Select HSI48 Oscillator as PLL source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI48;
  RCC_OscInitStruct.HSI48State = RCC_HSI48_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI48;
  RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV2;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct)!= HAL_OK)
  {
    Error_Handler();
  }

  /* Select PLL as system clock source and configure the HCLK and PCLK1 clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1)!= HAL_OK)
  {
    Error_Handler();
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
