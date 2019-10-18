/**
  ******************************************************************************
  * @file    IWDG/IWDG_Reset/Src/main.c
  * @author  MCD Application Team
  * @brief   This sample code shows how to use the STM32F0xx IWDG HAL API
  *          to update at regular period the IWDG counter and how to simulate
  *          a software fault generating an MCU IWDG reset on expiry of a
  *          programmed time period.
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

/** @addtogroup IWDG_Example
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* IWDG and TIM handlers declaration */
static IWDG_HandleTypeDef   IwdgHandle;
TIM_HandleTypeDef           Input_Handle;
RCC_ClkInitTypeDef          RCC_ClkInitStruct;
RCC_OscInitTypeDef          RCC_OscInitStruct;

uint16_t tmpCC4[2] = {0, 0};
__IO uint32_t uwLsiFreq = 0;
__IO uint32_t uwCaptureNumber = 0;

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void Error_Handler(void);

static uint32_t GetLSIFrequency(void);

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
  
  /* Configure the system clock to 48 MHz */
  SystemClock_Config();

  /* Configure LED2 */
  BSP_LED_Init(LED2);

  /* Configure User push-button */
  BSP_PB_Init(BUTTON_USER, BUTTON_MODE_EXTI);
  
  /*##-1- Check if the system has resumed from IWDG reset ####################*/
  if (__HAL_RCC_GET_FLAG(RCC_FLAG_IWDGRST) != RESET)
  {
    /* IWDGRST flag set: Turn LED2 on */
    BSP_LED_On(LED2);

    /* Insert 4s delay */
    HAL_Delay(4000);

    /* Notification done: Turn LED2 off */
    BSP_LED_Off(LED2);
  }

  /* Clear reset flags in any cases */
  __HAL_RCC_CLEAR_RESET_FLAGS();

  /*##-2- Get the LSI frequency: TIM14 is used to measure the LSI frequency ###*/
  uwLsiFreq = GetLSIFrequency();
  
  /*##-3- Configure & Start the IWDG peripheral #########################################*/
  /* Set counter reload value to obtain 1 sec. IWDG TimeOut.
     IWDG counter clock Frequency = uwLsiFreq
     Set Prescaler to 32 (IWDG_PRESCALER_32)
     Timeout Period = (Reload Counter Value * 32) / uwLsiFreq
     So Set Reload Counter Value = (1 * uwLsiFreq) / 32 */
  IwdgHandle.Instance = IWDG;
  IwdgHandle.Init.Prescaler = IWDG_PRESCALER_32;
  IwdgHandle.Init.Reload = (uwLsiFreq / 32);
  IwdgHandle.Init.Window = IWDG_WINDOW_DISABLE;

  if(HAL_IWDG_Init(&IwdgHandle) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }

  /* Infinite loop */
  while (1)
  {
    /* Toggle LED2 */
    BSP_LED_Toggle(LED2);

    /* Insert 990 ms delay */
    HAL_Delay(990);

    /* Refresh IWDG: reload counter */
    if(HAL_IWDG_Refresh(&IwdgHandle) != HAL_OK)
    {
      /* Refresh Error */
      Error_Handler();
    }
  }
}

/**
  * @brief  Configures TIM14 to measure the LSI oscillator frequency. 
  * @param  None
  * @retval LSI Frequency
  */
static uint32_t GetLSIFrequency(void)
{
  TIM_IC_InitTypeDef    TIMInput_Config;


  /* Configure the TIM peripheral *********************************************/ 
  /* Set TIMx instance */  
  Input_Handle.Instance = TIM14;
  
  /* TIM14 configuration: Input Capture mode ---------------------
     The LSI oscillator is connected to TIM14 CH1.
     The Rising edge is used as active edge.
     The TIM14 CCR1 is used to compute the frequency value. 
  ------------------------------------------------------------ */
  Input_Handle.Init.Prescaler         = 0;
  Input_Handle.Init.CounterMode       = TIM_COUNTERMODE_UP;
  Input_Handle.Init.Period            = 0xFFFF;
  Input_Handle.Init.ClockDivision     = 0;
  Input_Handle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if(HAL_TIM_IC_Init(&Input_Handle) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
  
  /* Connect internally the TIM14_CH1 Input Capture to the LSI clock output */
  HAL_TIMEx_RemapConfig(&Input_Handle, TIM_TIM14_MCO);

  /* Connect internally the MCO to LSI */
  HAL_RCC_MCOConfig(RCC_MCO, RCC_MCO1SOURCE_LSI, RCC_MCODIV_1);

  /* Configure the Input Capture of channel 1 */
  TIMInput_Config.ICPolarity  = TIM_ICPOLARITY_RISING;
  TIMInput_Config.ICSelection = TIM_ICSELECTION_DIRECTTI;
  TIMInput_Config.ICPrescaler = TIM_ICPSC_DIV8;
  TIMInput_Config.ICFilter    = 0;
  if(HAL_TIM_IC_ConfigChannel(&Input_Handle, &TIMInput_Config, TIM_CHANNEL_1) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }

  /* Start the TIM Input Capture measurement in interrupt mode */
  if(HAL_TIM_IC_Start_IT(&Input_Handle, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }

  /* Wait until the TIM14 get 2 LSI edges */
  while(uwCaptureNumber != 2)
  {
  }

  /* Disable TIM14 CC1 Interrupt Request */
  HAL_TIM_IC_Stop_IT(&Input_Handle, TIM_CHANNEL_1);
  
  /* Deinitialize the TIM14 peripheral registers to their default reset values */
  HAL_TIM_IC_DeInit(&Input_Handle);

  return uwLsiFreq;
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
void SystemClock_Config(void)
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
    /* Initialization Error */
    while(1); 
  }

  /* Select PLL as system clock source and configure the HCLK and PCLK1 clocks dividers */
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

/**
  * @brief  Input Capture callback in non blocking mode 
  * @param  htim : TIM IC handle
  * @retval None
*/
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
  uint32_t lsiperiod = 0;

  /* Get the Input Capture value */
  tmpCC4[uwCaptureNumber++] = HAL_TIM_ReadCapturedValue(&Input_Handle, TIM_CHANNEL_1);

  if (uwCaptureNumber >= 2)
  {
    /* Compute the period length */
    lsiperiod = (uint16_t)(0xFFFF - tmpCC4[0] + tmpCC4[1] + 1);

    /* Frequency computation */ 
    uwLsiFreq = (uint32_t) SystemCoreClock / lsiperiod;
    uwLsiFreq *= 8;
  }
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void Error_Handler(void)
{
  /* Turn LED2 off */
  BSP_LED_Off(LED2);

  while(1)
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
