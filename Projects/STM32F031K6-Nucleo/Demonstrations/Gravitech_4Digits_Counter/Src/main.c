/**
  ******************************************************************************
  * @file    Demonstrations/Gravitech_4Digits_Counter/Src/main.c 
  * @author  MCD Application Team
  * @brief   This demo describes how to display 4 digits using the Gravitech 
             7 Segment Display for Arduino Nano shield mounted on the 
             STM32 Nucleo board.
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

/** @addtogroup Demo
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void Error_Handler(void);
/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  uint32_t counter = 0;
  
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
  
  /* Configure the system clock = 48 MHz */
  SystemClock_Config();

  /* Initialize the LED present on the Nucleo 32 Board */
  BSP_LED_Init(LED3);
  
  /* Initialize the Gravitech 4 digits 7 segments nano shield */
  if(BSP_DIGIT4_SEG7_Init() != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
  
  /* Infinite loop */
  while(1)
  {
    /* Stop the demonstration on 3232! */
    if(counter == 3232)
    {
      while(1)
      {
        /* Display the counter on the screen */
        if(BSP_DIGIT4_SEG7_Display(3232) != HAL_OK)
        {
          /* Communication Error */
          Error_Handler();
        }
        
        /* Add some delay */
        HAL_Delay(200);
        
        /* Display the counter on the screen */
        if(BSP_DIGIT4_SEG7_Display(DIGIT4_SEG7_RESET) != HAL_OK)
        {
          /* Communication Error */
          Error_Handler();
        }       
        
        /* Add some delay */
        HAL_Delay(50); 
      }
    }
    
    /* Display the counter on the screen */
    if(BSP_DIGIT4_SEG7_Display(counter++ % 10000) != HAL_OK)
    {
      /* Communication Error */
      Error_Handler();
    }
    
    /* Add some delay */
    HAL_Delay(5);
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
/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void Error_Handler(void)
{
  /* Configure LED3 */
  BSP_LED_Init(LED3);

  while (1)
  {
    /* Toggle LED3 with a period of one second */
    BSP_LED_Toggle(LED3);
    HAL_Delay(1000);
  }
}

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
