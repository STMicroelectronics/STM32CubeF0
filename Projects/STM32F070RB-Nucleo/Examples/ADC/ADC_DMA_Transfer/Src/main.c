/**
  ******************************************************************************
  * @file    ADC/ADC_DMA_Transfer/Src/main.c
  * @author  MCD Application Team
  * @brief   This example describes how to use the DMA to convert continuously data.
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

/** @addtogroup ADC_DMA_Transfer
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

/* Definitions of data related to this example */
  /* Definition of ADCx conversions data table size */
  #define ADC_CONVERTED_DATA_BUFFER_SIZE   ((uint32_t)  32)   /* Size of array aADCxConvertedData[] */

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* ADC handle declaration */
ADC_HandleTypeDef             AdcHandle;

/* ADC channel configuration structure declaration */
ADC_ChannelConfTypeDef        sConfig;

/* Variable containing ADC conversions data */
static uint16_t   aADCxConvertedData[ADC_CONVERTED_DATA_BUFFER_SIZE];

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void Error_Handler(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{
 /* This sample code shows how to convert an analog input and read the converted
    data using DMA transfer.
    To proceed, 4 steps are required: */
   
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

  /* ### - 1 - Initialize ADC peripheral #################################### */
  
  AdcHandle.Instance          = ADCx;
  if (HAL_ADC_DeInit(&AdcHandle) != HAL_OK)
  {
    /* ADC de-initialization Error */
    Error_Handler();
  }

  AdcHandle.Init.ClockPrescaler        = ADC_CLOCK_SYNC_PCLK_DIV2;      /* Synchronous clock mode, input ADC clock with prscaler 2 */

  AdcHandle.Init.Resolution            = ADC_RESOLUTION_12B;            /* 12-bit resolution for converted data */
  AdcHandle.Init.DataAlign             = ADC_DATAALIGN_RIGHT;           /* Right-alignment for converted data */
  AdcHandle.Init.ScanConvMode          = ADC_SCAN_DIRECTION_FORWARD;    /* Sequencer disabled (ADC conversion on only 1 channel: channel set on rank 1) */
  AdcHandle.Init.EOCSelection          = ADC_EOC_SINGLE_CONV;           /* EOC flag picked-up to indicate conversion end */
  AdcHandle.Init.LowPowerAutoPowerOff  = DISABLE;
  AdcHandle.Init.LowPowerAutoWait      = DISABLE;                       /* Auto-delayed conversion feature disabled */
  AdcHandle.Init.ContinuousConvMode    = ENABLE;                        /* Continuous mode enabled (automatic conversion restart after each conversion) */
  AdcHandle.Init.DiscontinuousConvMode = DISABLE;                       /* Parameter discarded because sequencer is disabled */
  AdcHandle.Init.ExternalTrigConv      = ADC_SOFTWARE_START;            /* Software start to trig the 1st conversion manually, without external event */
  AdcHandle.Init.ExternalTrigConvEdge  = ADC_EXTERNALTRIGCONVEDGE_NONE; /* Parameter discarded because software trigger chosen */
  AdcHandle.Init.DMAContinuousRequests = ENABLE;                        /* ADC DMA continuous request to match with DMA circular mode */
  AdcHandle.Init.Overrun               = ADC_OVR_DATA_OVERWRITTEN;      /* DR register is overwritten with the last conversion result in case of overrun */
  AdcHandle.Init.SamplingTimeCommon    = ADC_SAMPLETIME_71CYCLES_5;

  /* Initialize ADC peripheral according to the passed parameters */
  if (HAL_ADC_Init(&AdcHandle) != HAL_OK)
  {
    Error_Handler();
  }
  
  
  /* ### - 2 - Start calibration ############################################ */
  if (HAL_ADCEx_Calibration_Start(&AdcHandle) != HAL_OK)
  {
    Error_Handler();
  }
  
  /* ### - 3 - Channel configuration ######################################## */
  sConfig.Channel      = ADCx_CHANNEL;               /* Channel to be converted */
  sConfig.Rank         = ADC_RANK_CHANNEL_NUMBER;
  if (HAL_ADC_ConfigChannel(&AdcHandle, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  
  /* ### - 4 - Start conversion in DMA mode ################################# */
  if (HAL_ADC_Start_DMA(&AdcHandle,
                        (uint32_t *)aADCxConvertedData,
                        ADC_CONVERTED_DATA_BUFFER_SIZE
                       ) != HAL_OK)
  {
    Error_Handler();
  }
  
  /* Infinite Loop */
  while (1)
  { 
  }
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
  *            System Clock source            = PLL (HSI)
  *            SYSCLK(Hz)                     = 48000000
  *            HCLK(Hz)                       = 48000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 1
  *            HSI Frequency(Hz)              = 8000000
  *            PREDIV                         = 2
  *            PLLMUL                         = 12
  *            Flash Latency(WS)              = 1
  * @param  None
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;
  
  /* No HSE Oscillator on Nucleo, Activate PLL with HSI as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_NONE;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV2;
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
static void Error_Handler(void)
{
  /* Turn LED2 on*/
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
