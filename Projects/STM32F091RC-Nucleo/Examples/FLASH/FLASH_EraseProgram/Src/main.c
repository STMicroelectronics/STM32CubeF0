/**
  ******************************************************************************
  * @file    FLASH/FLASH_EraseProgram/Src/main.c
  * @author  MCD Application Team
  * @brief   This example provides a description of how to erase and program the
  *          STM32F0xx FLASH.
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

/** @addtogroup FLASH_EraseProgram
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define FLASH_USER_START_ADDR   ADDR_FLASH_PAGE_16   /* Start @ of user Flash area */
#define FLASH_USER_END_ADDR     ADDR_FLASH_PAGE_127 + FLASH_PAGE_SIZE   /* End @ of user Flash area */

#define DATA_32                 ((uint32_t)0x12345678)

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint32_t Address = 0, PageError = 0;
__IO uint32_t data32 = 0 , MemoryProgramStatus = 0;

/*Variable used for Erase procedure*/
static FLASH_EraseInitTypeDef EraseInitStruct;

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

  /* Initialize LED2 */
  BSP_LED_Init(LED2);

  /* Configure the system clock to 48 MHz */
  SystemClock_Config();

  /* Unlock the Flash to enable the flash control register access *************/
  HAL_FLASH_Unlock();

  /* Erase the user Flash area
    (area defined by FLASH_USER_START_ADDR and FLASH_USER_END_ADDR) ***********/

  /* Fill EraseInit structure*/
  EraseInitStruct.TypeErase = FLASH_TYPEERASE_PAGES;
  EraseInitStruct.PageAddress = FLASH_USER_START_ADDR;
  EraseInitStruct.NbPages = (FLASH_USER_END_ADDR - FLASH_USER_START_ADDR) / FLASH_PAGE_SIZE;

  if (HAL_FLASHEx_Erase(&EraseInitStruct, &PageError) != HAL_OK)
  {
    /*
      Error occurred while page erase.
      User can add here some code to deal with this error.
      PageError will contain the faulty page and then to know the code error on this page,
      user can call function 'HAL_FLASH_GetError()'
    */
    /* Infinite loop */
    while (1)
    {
      /* Make LED2 blink (100ms on, 2s off) to indicate error in Erase operation */
      BSP_LED_On(LED2);
      HAL_Delay(100);
      BSP_LED_Off(LED2);
      HAL_Delay(2000);
    }
  }

  /* Program the user Flash area word by word
    (area defined by FLASH_USER_START_ADDR and FLASH_USER_END_ADDR) ***********/

  Address = FLASH_USER_START_ADDR;

  while (Address < FLASH_USER_END_ADDR)
  {
    if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, Address, DATA_32) == HAL_OK)
    {
      Address = Address + 4;
    }
    else
    {
      /* Error occurred while writing data in Flash memory.
         User can add here some code to deal with this error */
      while (1)
      {
        /* Make LED2 blink (100ms on, 2s off) to indicate error in Write operation */
        BSP_LED_On(LED2);
        HAL_Delay(100);
        BSP_LED_Off(LED2);
        HAL_Delay(2000);
      }
    }
  }

  /* Lock the Flash to disable the flash control register access (recommended
     to protect the FLASH memory against possible unwanted operation) *********/
  HAL_FLASH_Lock();

  /* Check if the programmed data is OK
      MemoryProgramStatus = 0: data programmed correctly
      MemoryProgramStatus != 0: number of words not programmed correctly ******/
  Address = FLASH_USER_START_ADDR;
  MemoryProgramStatus = 0x0;

  while (Address < FLASH_USER_END_ADDR)
  {
    data32 = *(__IO uint32_t *)Address;

    if (data32 != DATA_32)
    {
      MemoryProgramStatus++;
    }
    Address = Address + 4;
  }

  /*Check if there is an issue to program data*/
  if (MemoryProgramStatus == 0)
  {
    /* No error detected. Switch on LED2*/
    BSP_LED_On(LED2);
  }
  else
  {
    /* Error detected. LED2 will blink with 1s period */
    while (1)
    {
      BSP_LED_On(LED2);
      HAL_Delay(1000);
      BSP_LED_Off(LED2);
      HAL_Delay(1000);
    }
  }

  /* Infinite loop */
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
