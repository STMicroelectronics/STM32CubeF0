/**
  ******************************************************************************
  * @file    RTC/RTC_Tamper/Src/main.c
  * @author  MCD Application Team
  * @brief   This sample code shows how to use STM32F0xx RTC HAL API to write/read
  *          data to/from RTC Backup data registers and demonstrates the Tamper
  *          detection feature.
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

/** @addtogroup RTC_Tamper
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* RTC handler declaration */
RTC_HandleTypeDef RtcHandle;

__IO FlagStatus TamperStatus;

/* Backup registers table */
uint32_t aBKPDataReg[BACKUP_COUNT] =
{
  RTC_BKP_DR0, RTC_BKP_DR1, RTC_BKP_DR2,
  RTC_BKP_DR3, RTC_BKP_DR4
};


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
  uint32_t index = 0;
  RTC_TamperTypeDef  stamperstructure;
  
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

  /* Configure LED5, LED4, LED3 and LED6 */
  BSP_LED_Init(LED5);
  BSP_LED_Init(LED4);
  BSP_LED_Init(LED3);
  BSP_LED_Init(LED6);

  /* Configure User push-button button */
  BSP_PB_Init(BUTTON_USER, BUTTON_MODE_GPIO);
  
  /* Configure the system clock to 48 MHz */
  SystemClock_Config();

  /*##-1- Configure the RTC peripheral #######################################*/
  /* Configure RTC prescaler and RTC data registers */
  /* RTC configured as follows:
      - Hour Format    = Format 24
      - Asynch Prediv  = Value according to source clock
      - Synch Prediv   = Value according to source clock
      - OutPut         = Output Disable
      - OutPutPolarity = High Polarity
      - OutPutType     = Open Drain */
  RtcHandle.Init.HourFormat     = RTC_HOURFORMAT_24;
  RtcHandle.Init.AsynchPrediv   = RTC_ASYNCH_PREDIV;
  RtcHandle.Init.SynchPrediv    = RTC_SYNCH_PREDIV;
  RtcHandle.Init.OutPut         = RTC_OUTPUT_DISABLE;
  RtcHandle.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  RtcHandle.Init.OutPutType     = RTC_OUTPUT_TYPE_OPENDRAIN;
  RtcHandle.Instance            = RTC;
  
  if (HAL_RTC_Init(&RtcHandle) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }

  /*##-2- Configure RTC Tamper ###############################################*/
  stamperstructure.Tamper                       = RTC_TAMPER_2;
  stamperstructure.Trigger                      = RTC_TAMPERTRIGGER_FALLINGEDGE;
  stamperstructure.Filter                       = RTC_TAMPERFILTER_DISABLE;
  stamperstructure.SamplingFrequency            = RTC_TAMPERSAMPLINGFREQ_RTCCLK_DIV32768;
  stamperstructure.PrechargeDuration            = RTC_TAMPERPRECHARGEDURATION_1RTCCLK;
  stamperstructure.TamperPullUp                 = RTC_TAMPER_PULLUP_ENABLE;
  stamperstructure.TimeStampOnTamperDetection   = RTC_TIMESTAMPONTAMPERDETECTION_DISABLE;
  
  if (HAL_RTCEx_SetTamper_IT(&RtcHandle, &stamperstructure) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }

  /* Clear the Tamper interrupt pending bit */
  __HAL_RTC_TAMPER_CLEAR_FLAG(&RtcHandle,RTC_FLAG_TAMP2F);

  /*##-3- Write Data on the Back Up registers ################################*/
  for (index = 0; index < BACKUP_COUNT; index++)
  {
    HAL_RTCEx_BKUPWrite(&RtcHandle, aBKPDataReg[index], 0xDF59 + (index * 0x5A));
  }

  /*##-4- Check Data is stored on the Back Up registers ######################*/
  for (index = 0; index < BACKUP_COUNT; index++)
  {
    if (HAL_RTCEx_BKUPRead(&RtcHandle, aBKPDataReg[index]) != (0xDF59 + (index * 0x5A)))
    {
      Error_Handler();
    }
  }

  /* Turn LED4 on */
  BSP_LED_On(LED4);

  /* Reset flag after writing of backup register in order to wait for new button press */
  TamperStatus = RESET;

  /*##-5- Wait for the tamper button is pressed ##############################*/
  while (TamperStatus != SET);

  /*##-6- Deactivate the tamper                 ##############################*/
  HAL_RTCEx_DeactivateTamper(&RtcHandle, RTC_TAMPER_2);
    
  /*##-7- Check Data is cleared on the Back Up registers #####################*/
  for (index = 0; index < BACKUP_COUNT; index++)
  {
    if (HAL_RTCEx_BKUPRead(&RtcHandle, aBKPDataReg[index]) != 0x00)
    {
      Error_Handler();
    }
  }

  /* Infinite loop */
  while (1)
  {
    /* Turn LED6 on */
    BSP_LED_Toggle(LED6);

    /* Delay */
    HAL_Delay(100);
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
void Error_Handler(void)
{
  /* Turn LED3 on */
  BSP_LED_On(LED3);
  while (1)
  {
  }
}

/**
  * @brief  Tamper event callback function
  * @param  RTC handle
  * @retval None
  */
void HAL_RTCEx_Tamper2EventCallback(RTC_HandleTypeDef *hrtc)
{
  /* LED5 On: Tamper button pressed */
  BSP_LED_On(LED5);
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
