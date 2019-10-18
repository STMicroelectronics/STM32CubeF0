/**
  ******************************************************************************
  * @file    COMP/COMP_AnalogWatchdog/Src/main.c
  * @author  MCD Application Team
  * @brief   This example provides a short description of how to use the COMP 
  *          peripheral to make an analog watchdog.
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

/** @addtogroup COMP_AnalogWatchdog
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
COMP_HandleTypeDef   Comp1Handle;
COMP_HandleTypeDef   Comp2Handle;
__IO uint32_t State = 0;
/* Variable to indicate that MCU entered in Stop mode */
__IO uint32_t EnterInStopMode = 0;

/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
static void Error_Handler(void);
static void COMP_Config(void);
static void StopSequence_Config(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
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

  /*******************************************************************************
  *                          Common Configuration Routines                       *
  *******************************************************************************/  
 
  /******* Initialize LEDs available on STM32F091RC-Nucleo RevC board ******************/
  
  BSP_LED_Init(LED2);

  /* Configure the system clock to 48 MHz */
  SystemClock_Config();

  /* configure COMP1 and COMP2 with interrupts enabled */
  COMP_Config();
  
  /* Check input voltage level: within the thresholds, above the upper threshold
     or under the lower threshold */
  InputVoltageLevel_Check();
  
  /* Infinite loop */
  while (1)
  {
    if (State == STATE_OVER_THRESHOLD)
    {
      /* Restoration done only in case of MCU was in stop mode */
      if (EnterInStopMode == 1)
      {
        /* Restore config: clock, GPIO... */
        SystemClock_Config();
        
        /* Restore GPIO configuration */
          BSP_LED_Init(LED2);
        
        EnterInStopMode = 0;
      }

      /* Turn on LED1 and LED3 and turn off LED2 and LED4 */


     
      while(State == STATE_OVER_THRESHOLD)
      {
        /* add your code here */
        
        /* Toggle LED2 */
        BSP_LED_Toggle(LED2);
        HAL_Delay(50);
      }
    }
    else if (State == STATE_WITHIN_THRESHOLD)
    {
      

      /* Turn off LD2 */
      BSP_LED_Off(LED2);

      
      /* Input voltage is within the thresholds: higher and lower thresholds */
      /* Enter STOP mode with regulator in low power */
      StopSequence_Config();
    }
    else /* (State == STATE_UNDER_THRESHOLD) */
    {
      /* Restoration done only in case of MCU was in stop mode */
      if (EnterInStopMode == 1)
      {
        /* Restore config: clock, GPIO... */
        SystemClock_Config();
        
        /* Restore GPIO configuration */
        BSP_LED_Init(LED2);
        EnterInStopMode = 0;
      }
      
      /* Turn on LED2 and LED4 and turn off LED1 and LED3 */

        
      while(State == STATE_UNDER_THRESHOLD)
      {

        /* add your code here */
        /* Toggle LED2 */
        BSP_LED_Toggle(LED2);
        HAL_Delay(1000);
      }
    }
  }
}

/**
  * @brief  Configure COMP1 and COMP2 with interrupt
  * @param  None
  * @retval None
  */
static void COMP_Config(void)
{
 
  /*##-1- Configure the COMPx peripheral ###################################*/
  /* COMP1 Init: the higher threshold is set to VREFINT ~ 1.22V
     but can be changed to other available possibilities */
  Comp1Handle.Instance                = COMP1;

  Comp1Handle.Init.InvertingInput     = COMP_INVERTINGINPUT_VREFINT;
  Comp1Handle.Init.Output             = COMP_OUTPUT_NONE;
  Comp1Handle.Init.OutputPol          = COMP_OUTPUTPOL_NONINVERTED;
  Comp1Handle.Init.Hysteresis         = COMP_HYSTERESIS_HIGH;
  Comp1Handle.Init.Mode               = COMP_MODE_LOWPOWER;
  Comp1Handle.Init.WindowMode         = COMP_WINDOWMODE_DISABLE;
  Comp1Handle.Init.TriggerMode        = COMP_TRIGGERMODE_IT_RISING_FALLING;
  if(HAL_COMP_Init(&Comp1Handle) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler(); 
  }
  
  
  /* COMP2 Init: the lower threshold is set to VREFINT/4 ~ 1.22 / 4 ~ 0.305 V 
     but can be changed to other available possibilities */
  Comp2Handle.Instance                = COMP2;

  Comp2Handle.Init.InvertingInput     = COMP_INVERTINGINPUT_1_4VREFINT;
  Comp2Handle.Init.Output             = COMP_OUTPUT_NONE;
  Comp2Handle.Init.OutputPol          = COMP_OUTPUTPOL_NONINVERTED;
  Comp2Handle.Init.Hysteresis         = COMP_HYSTERESIS_HIGH;
  Comp2Handle.Init.Mode               = COMP_MODE_LOWPOWER;
  Comp2Handle.Init.WindowMode         = COMP_WINDOWMODE_ENABLE;
  Comp2Handle.Init.TriggerMode        = COMP_TRIGGERMODE_IT_RISING_FALLING;
  if(HAL_COMP_Init(&Comp2Handle) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler(); 
  }
  

  /* Start COMP1 */
  if(HAL_COMP_Start_IT(&Comp1Handle) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler(); 
  }
  
  /* Start COMP2 */
  if(HAL_COMP_Start_IT(&Comp2Handle) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler(); 
  } 
}

/**
  * @brief  Prepare the system to enter STOP mode.
  * @param  None
  * @retval None
  */
static void StopSequence_Config(void)
{
  GPIO_InitTypeDef      GPIO_InitStruct;
  
  /* PWR Peripheral clock enable */
  __HAL_RCC_PWR_CLK_ENABLE();
  
  /* Enable GPIOs clock */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();

  /* Configure all GPIO port pins in Analog mode */
  GPIO_InitStruct.Pin = GPIO_PIN_All;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
  HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);
  
  /* Request to enter STOP mode with regulator in low power */
  HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFI);
  EnterInStopMode = 1;
}

/**
  * @brief  check input voltage level: within the thresholds, above the upper 
  *         threshold or under the lower threshold
  * @param  None
  * @retval None
  */
void InputVoltageLevel_Check(void)
{
  /*  Check if COMP1 and COMP2 output level is high */
  if (((HAL_COMP_GetOutputLevel(&Comp1Handle)) == COMP_OUTPUTLEVEL_HIGH) 
   && ((HAL_COMP_GetOutputLevel(&Comp2Handle)) == COMP_OUTPUTLEVEL_HIGH))
  {
    /* A rising edge is detected so the input voltage is higher than VREFINT */
    State = STATE_OVER_THRESHOLD;
  }
  else if (((HAL_COMP_GetOutputLevel(&Comp1Handle)) == COMP_OUTPUTLEVEL_LOW)
       && ((HAL_COMP_GetOutputLevel(&Comp2Handle)) == COMP_OUTPUTLEVEL_HIGH))
  {
    /* A falling edge is detected so the input voltage is lower than VREFINT */
    State = STATE_WITHIN_THRESHOLD;
  }
  else if (((HAL_COMP_GetOutputLevel(&Comp1Handle)) == COMP_OUTPUTLEVEL_LOW)
       && ((HAL_COMP_GetOutputLevel(&Comp2Handle)) == COMP_OUTPUTLEVEL_LOW))
  {
    State = STATE_UNDER_THRESHOLD;
  }
}

/**
  * @brief  Comparator callback.
  * @param  hcomp: COMP handle
  * @retval None
  */
void HAL_COMP_TriggerCallback(COMP_HandleTypeDef *hcomp)
{
  /* Check input voltage level: within the thresholds, */
  /* above the upper threshold or under the lower threshold */
  InputVoltageLevel_Check();
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
  /* No left */
  
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
