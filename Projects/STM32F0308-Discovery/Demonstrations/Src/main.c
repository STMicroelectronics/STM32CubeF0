/**
  ******************************************************************************
  * @file    Demonstrations/Src/main.c 
  * @author  MCD Application Team
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2016 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/** @addtogroup STM32F0xx_HAL_Demonstrations
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint8_t BlinkSpeed = 0;

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

  /* Configure the system clock to have a system clock = 48 Mhz */
  SystemClock_Config();
  
  /* Configure LED3 and LED4 on STM32F0308-Discovery */
  BSP_LED_Init(LED3);
  BSP_LED_Init(LED4);

  /* Initialize User_Button on STM32F0308-Discovery */
  BSP_PB_Init(BUTTON_USER, BUTTON_MODE_GPIO);

  /* Initiate Blink Speed variable */ 
  BlinkSpeed = 1;

  /* Infinite loop */
  while(1)
  {  
    /* Check if the user button is pressed */
    if(BSP_PB_GetState(BUTTON_USER) == GPIO_PIN_SET)
    {
      /* BlinkSpeed: 1 -> 2 -> 0, then re-cycle */
      /* Turn on LD4 Blue LED during 1s each time User button is pressed */
      BSP_LED_On(LED4);

      /* wait for 1s */
      HAL_Delay(1000);

      /* Turn off LD4 Blue LED after 1s each time User button is pressed */
      BSP_LED_Off(LED4);

      /* Increment the blink speed counter */
      BlinkSpeed++;

      /* Default value for blink speed counter */
      if(BlinkSpeed == 3)
      {  
        BlinkSpeed = 0;
      }
    }

    /* Test on blink speed */
    if(BlinkSpeed == 2)
    {
      /* LED3 toggles each 100 ms */
      BSP_LED_Toggle(LED3);
      
      /* maintain LED3 status for 100ms */
      HAL_Delay(100);
    }
    else if(BlinkSpeed == 1)
    {
      /* LED3 toggles each 200 ms */
      BSP_LED_Toggle(LED3);
      
      /* maintain LED3 status for 200ms */
      HAL_Delay(200);
    }
    else
    {  
      /* LED3 Off */
      BSP_LED_Off(LED3);
    }
  }
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 48000000
  *            HCLK(Hz)                       = 48000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 1
  *            HSE Frequency(Hz)              = 8000000
  *            PREDIV                         = 1
  *            PLLMUL                         = 6
  *            Flash Latency(WS)              = 1
  * @param  None
  * @retval None
  */
static void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;

  /* Enable HSE Oscillator and Activate PLL with HSE as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV1;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL6;
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
  /* User may add here some code to deal with this error */
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
void assert_failed(uint8_t* file, uint32_t line)
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/ 
