/**
  ******************************************************************************
  * @file    TSC/TSC_BasicAcquisition_Polling/Src/main.c
  * @author  MCD Application Team
  * @brief   This example describes how to use the TSC to perform an acquisition
  *          of one channel in polling mode.
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

/** @addtogroup TSC_BasicAcquisition_Polling
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* These values can be modified, board layout dependency */
#define TSCx_MIN_THRESHOLD       1000
#define TSCx_LOW_MAXTHRESHOLD    1290
#define TSCx_MEDIUM_MAXTHRESHOLD 1270
#define TSCx_HIGH_MAXTHRESHOLD   1245
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* TSC handler declaration */
TSC_HandleTypeDef TscHandle;

/* Array used to store the acquisition value */
__IO uint32_t uhTSCAcquisitionValue;

/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
static void Error_Handler(void);

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

  /* Configure the system clock to 48 MHz */
  SystemClock_Config();

  /* Configure LEDs */
  BSP_LED_Init(LED4); /* Touch activity */
  BSP_LED_Init(LED6); /* Touch activity */
  BSP_LED_Init(LED5); /* Touch activity */
  BSP_LED_Init(LED3); /* Error */
  
  /*##-1- Configure the TSC peripheral #######################################*/
  TscHandle.Instance                     = TSCx;
  TscHandle.Init.AcquisitionMode         = TSC_ACQ_MODE_NORMAL;
  TscHandle.Init.CTPulseHighLength       = TSC_CTPH_1CYCLE;
  TscHandle.Init.CTPulseLowLength        = TSC_CTPL_1CYCLE;
  TscHandle.Init.IODefaultMode           = TSC_IODEF_IN_FLOAT; /* Because the electrodes are interlaced on this board */
  TscHandle.Init.MaxCountInterrupt       = DISABLE;
  TscHandle.Init.MaxCountValue           = TSC_MCV_16383;
  TscHandle.Init.PulseGeneratorPrescaler = TSC_PG_PRESC_DIV64;
  TscHandle.Init.SpreadSpectrum          = DISABLE;
  TscHandle.Init.SpreadSpectrumDeviation = 127;
  TscHandle.Init.SpreadSpectrumPrescaler = TSC_SS_PRESC_DIV1;
  TscHandle.Init.SynchroPinPolarity      = TSC_SYNC_POLARITY_FALLING;
  TscHandle.Init.ChannelIOs              = TSC_GROUP1_IO3; /* TS1 touchkey */
  TscHandle.Init.SamplingIOs             = (TSC_GROUP1_IO4 | TSC_GROUP2_IO4 | TSC_GROUP3_IO3);
  TscHandle.Init.ShieldIOs               = 0;

  if (HAL_TSC_Init(&TscHandle) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }

  /* Infinite loop */
  while (1)
  {  
    /*##-2- Discharge the touch-sensing IOs ##################################*/
    /* Must be done before each acquisition */
    HAL_TSC_IODischarge(&TscHandle, ENABLE);
    HAL_Delay(1); /* 1 ms is more than enough to discharge all capacitors */

    /*##-3- Start the acquisition process ####################################*/
    if (HAL_TSC_Start(&TscHandle) != HAL_OK)
    {
      /* Acquisition Error */
      Error_Handler();
    }

    /*##-4- Wait for the end of acquisition ##################################*/
    /*  Before starting a new acquisition, you need to check the current state of
         the peripheral; if it’s busy you need to wait for the end of current
         acquisition before starting a new one. */
    while (HAL_TSC_GetState(&TscHandle) == HAL_TSC_STATE_BUSY)
    {
      /* For simplicity reasons, this example is just waiting till the end of the
         acquisition, but application may perform other tasks while acquisition
         operation is ongoing. */
    }

    /*##-5- Clear flags ######################################################*/
    __HAL_TSC_CLEAR_FLAG(&TscHandle, (TSC_FLAG_EOA | TSC_FLAG_MCE));
    
    /*##-6- Check if the acquisition is correct (no max count) ###############*/
    if (HAL_TSC_GroupGetStatus(&TscHandle, TSC_GROUP1_IDX) == TSC_GROUP_COMPLETED)
    {
      /*##-7- Store the acquisition value ####################################*/
      uhTSCAcquisitionValue = HAL_TSC_GroupGetValue(&TscHandle, TSC_GROUP1_IDX);

      /* Show touch activity on LEDs */
      /* Threshold values are indicative and may need to be adjusted */
      if ((uhTSCAcquisitionValue > TSCx_MIN_THRESHOLD) && (uhTSCAcquisitionValue < TSCx_LOW_MAXTHRESHOLD))
      {
        BSP_LED_On(LED4);
        if (uhTSCAcquisitionValue < TSCx_MEDIUM_MAXTHRESHOLD)
        {
          BSP_LED_On(LED6);
          if (uhTSCAcquisitionValue < TSCx_HIGH_MAXTHRESHOLD)
          {
            BSP_LED_On(LED5);
          }
          else
          {
            BSP_LED_Off(LED5);
          }
        }
        else
        {
          BSP_LED_Off(LED6);
          BSP_LED_Off(LED5);
        }
      }
      else
      {
        BSP_LED_Off(LED4);
        BSP_LED_Off(LED6);
        BSP_LED_Off(LED5);
      }
    }
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
  while (1)
  {
    BSP_LED_Toggle(LED3);
    HAL_Delay(200);
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
    BSP_LED_Toggle(LED3);
    HAL_Delay(100);
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
