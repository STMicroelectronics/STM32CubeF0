/**
  ******************************************************************************
  * @file    PWR/PWR_CurrentConsumption/Src/main.c
  * @author  MCD Application Team
  * @brief   This sample code shows how to use STM32F0xx PWR HAL API to enter
  *          and exit the sleep/stop/standby modes, to measure power consumption 
  *          in these different low power modes.  
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
__IO uint32_t UserButtonStatus = 0;  /* set to 1 after User Button interrupt  */
__IO uint32_t uwWakeUpIntFlag  = 0;  /* set to 1 after RTC alarm interrupt    */
__IO uint32_t uwStandByOutFlag = 0;  /* set to 1 after exit from standby mode */
/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
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

  /* Configure LEDs */
  BSP_LED_Init(LED3); 
  BSP_LED_Init(LED4);

  /* Configure the system clock to 48 MHz */
  SystemClock_Config();

  /* Enable Power Clock */
  __HAL_RCC_PWR_CLK_ENABLE();

  /* Check and handle if the system was resumed from StandBy mode */
  if (__HAL_PWR_GET_FLAG(PWR_FLAG_SB) != RESET)
  {
    __HAL_PWR_CLEAR_FLAG(PWR_FLAG_SB);

    /* Configure User push-button */
    BSP_PB_Init(BUTTON_USER, BUTTON_MODE_GPIO);

    /* Turn on the LED4 */
    BSP_LED_On(LED4);

    uwStandByOutFlag = 1;

    /* Wait that user release the User push-button */
    while(BSP_PB_GetState(BUTTON_USER) == GPIO_PIN_RESET){}
  }

  /* Infinite loop */
  while(1)
  {
    /* Configure User push-button as external interrupt generator */
    BSP_PB_Init(BUTTON_USER, BUTTON_MODE_EXTI);
    UserButtonStatus = 0;
    
    /* Wait until User push-button is pressed to enter the Low Power mode.
       In the meantime, LED3 is blinks */
    while (UserButtonStatus == 0)
    {
      /* Toggle LED3 */
      BSP_LED_Toggle(LED3); 
      HAL_Delay(100);

      /* If exiting from stand-by mode, */
      /*   keep LED4 ON for about 5 sec. */
      if (uwStandByOutFlag > 0)
      {
        uwStandByOutFlag++;
        if (uwStandByOutFlag == 50)
        {
          BSP_LED_Off(LED4);
          uwStandByOutFlag = 0; 
        }
      }
      
    }

    /* Make sure LED3 is turned off to 
       reduce low power mode consumption */
    BSP_LED_Off(LED3);

#if defined (SLEEP_MODE)
    /* Sleep Mode Entry
        - System Running at PLL (48 MHz)
        - Flash 2 wait state
        - Instruction and Data caches ON
        - Prefetch ON
        - Code running from Internal FLASH
        - All peripherals disabled.
        - Wakeup using EXTI Line (User push-button PA.00)
    */
    SleepMode_Measure();
#elif defined (STOP_MODE)
    /* STOP Mode Entry 
      - RTC Clocked by LSE or LSI
      - Regulator in LP mode
      - HSI, HSE OFF and LSI OFF if not used as RTC Clock source
      - No IWDG
      - Wakeup using EXTI Line (User push-button PA.00)
    */
    StopMode_Measure();
#elif defined (STANDBY_MODE)
    /* STANDBY Mode Entry
        - RTC OFF
        - IWDG and LSI OFF
        - Wakeup using WakeUp Pin PWR_WAKEUP_PIN1 connected to PA.00
    */
    StandbyMode_Measure();
#endif /* SLEEP_MODE */ 
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
  
  /* Select HSE Oscillator as PLL source */
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
void Error_Handler(void)
{
   /* Error if LED4 is slowly blinking (1 sec. period) */
  BSP_LED_Init(LED4);
  BSP_LED_On(LED4);
  while (1)
  {
      BSP_LED_Toggle(LED4); 
      HAL_Delay(1000);  
  }
}

/**
  * @brief SYSTICK callback
  * @param None
  * @retval None
  */
void HAL_SYSTICK_Callback(void)
{
  HAL_IncTick();
}


/**
  * @brief EXTI line detection callbacks
  * @param GPIO_Pin: Specifies the pins connected EXTI line
  * @retval None
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if (GPIO_Pin == USER_BUTTON_PIN)
  {
    /* Configure leds */
    BSP_LED_Init(LED3);
    UserButtonStatus = 1;
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
