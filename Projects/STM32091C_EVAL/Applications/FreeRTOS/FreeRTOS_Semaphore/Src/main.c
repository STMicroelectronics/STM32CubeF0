/**
  ******************************************************************************
  * @file    FreeRTOS/FreeRTOS_Semaphore/Src/main.c
  * @author  MCD Application Team
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2016 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
#define semtstSTACK_SIZE      configMINIMAL_STACK_SIZE

/* Private variables ---------------------------------------------------------*/
osThreadId SemThread1Handle, SemThread2Handle;
osSemaphoreId osSemaphore;

/* Private function prototypes -----------------------------------------------*/
static void SemaphoreThread1(void const *argument);
static void SemaphoreThread2(void const *argument);
void SystemClock_Config(void);

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

  /* Configure the System clock to 48 MHz */
  SystemClock_Config();

  /* Initialize LEDs */
  BSP_LED_Init(LED1);
  BSP_LED_Init(LED2);

  /* Define used semaphore */
  osSemaphoreDef(SEM);

  /* Create the semaphore used by the two threads */
  osSemaphore = osSemaphoreCreate(osSemaphore(SEM) , 1);

  /* Create the first Thread */
  osThreadDef(SEM_Thread1, SemaphoreThread1, osPriorityLow, 0, semtstSTACK_SIZE);
  SemThread1Handle = osThreadCreate(osThread(SEM_Thread1), (void *) osSemaphore);

  /* Create the second Thread */
  osThreadDef(SEM_Thread2, SemaphoreThread2, osPriorityIdle, 0, semtstSTACK_SIZE);
  SemThread2Handle = osThreadCreate(osThread(SEM_Thread2), (void *) osSemaphore);

  /* Start scheduler */
  osKernelStart();

  /* We should never get here as control is now taken by the scheduler */
  for (;;);

}

/**
  * @brief  Semaphore Thread 1 function
  * @param  argument: shared semaphore
  * @retval None
  */
static void SemaphoreThread1(void const *argument)
{
  uint32_t count = 0;
  osSemaphoreId semaphore = (osSemaphoreId) argument;

  for (;;)
  {

    if (semaphore != NULL)
    {
      /* Try to obtain the semaphore */
      if (osSemaphoreWait(semaphore , 100) == osOK)
      {
        count = osKernelSysTick() + 5000;

        /* Toggle LED1 every 200 ms for 5 seconds */
        while (count > osKernelSysTick())
        {
          /* Toggle LED1 */
          BSP_LED_Toggle(LED1);

          /* Delay 200 ms */
          osDelay(200);
        }

        /* Turn off LED1*/
        BSP_LED_Off(LED1);
        /* Release the semaphore */
        osSemaphoreRelease(semaphore);

        /* Suspend ourseleves to execute thread 2 (lower priority)  */
        osThreadSuspend(NULL);
      }
    }
  }
}

/**
  * @brief  Semaphore Thread 2 function
  * @param  argument: shared semaphore
  * @retval None
  */
static void SemaphoreThread2(void const *argument)
{
  uint32_t count = 0;
  osSemaphoreId semaphore = (osSemaphoreId) argument;

  for (;;)
  {
    if (semaphore != NULL)
    {
      /* Try to obtain the semaphore */
      if (osSemaphoreWait(semaphore , 0) == osOK)
      {
        /* Resume Thread 1 (higher priority)*/
        osThreadResume(SemThread1Handle);

        count = osKernelSysTick() + 5000;

        /* Toggle LED2 every 200 ms for 5 seconds*/
        while (count > osKernelSysTick())
        {
          BSP_LED_Toggle(LED2);

          osDelay(200);
        }

        /* Turn off LED2 */
        BSP_LED_Off(LED2);

        /* Release the semaphore to unblock Thread 1 (higher priority)  */
        osSemaphoreRelease(semaphore);
      }
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
void SystemClock_Config(void)
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
  *   where the assert_param error has occurred.
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
  {}
}
#endif
