/**
  ******************************************************************************
  * @file    DAC/DAC_SignalsGeneration/Src/main.c
  * @author  MCD Application Team
  * @brief   This example provides a short description of how to use the DAC
  *          peripheral to generate several signals.
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

/** @addtogroup STM32F0xx_HAL_Examples
  * @{
  */

/** @addtogroup DAC_SignalsGeneration
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
DAC_HandleTypeDef    DacHandle;
static DAC_ChannelConfTypeDef sConfig;
const uint8_t aEscalator8bit[6] = {0x0, 0x33, 0x66, 0x99, 0xCC, 0xFF};
__IO uint8_t ubSelectedWavesForm = 1;
__IO uint8_t ubKeyPressed = SET;

/* Private function prototypes -----------------------------------------------*/
static void DAC_Ch1_TriangleConfig(void);
static void DAC_Ch1_EscalatorConfig(void);
static void TIM6_Config(void);
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

  /* Configure LED5 */
  BSP_LED_Init(LED5);

  /* Configure the system clock to 48 MHz */
  SystemClock_Config();

  /* Configures User push-button */
  BSP_PB_Init(BUTTON_USER, BUTTON_MODE_EXTI);

  /*##-1- Configure the DAC peripheral #######################################*/
  DacHandle.Instance = DACx;

  /*##-2- Configure the TIM peripheral #######################################*/
  TIM6_Config();

  /* Infinite loop */
  while (1)
  {
    /* If the Key is pressed */
    if (ubKeyPressed != RESET)
    {
      HAL_DAC_DeInit(&DacHandle);

      /* select waves forms according to the User push-button status */
      if (ubSelectedWavesForm == 1)
      {
        /* The triangle wave has been selected */

        /* Triangle Wave generator -------------------------------------------*/
        DAC_Ch1_TriangleConfig();
      }
      else
      {
        /* The escalator wave has been selected */

        /* Escalator Wave generator -------------------------------------------*/
        DAC_Ch1_EscalatorConfig();
      }

      ubKeyPressed = RESET;
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
  /* Turn LED5 on */
  BSP_LED_On(LED5);
  while (1)
  {
  }
}

static void DAC_Ch1_EscalatorConfig(void)
{
  /*##-1- Initialize the DAC peripheral ######################################*/
  if (HAL_DAC_Init(&DacHandle) != HAL_OK)
  {
    /* Initiliazation Error */
    Error_Handler();
  }

  /*##-1- DAC channel1 Configuration #########################################*/
  sConfig.DAC_Trigger = DAC_TRIGGER_T6_TRGO;
  sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;

  if (HAL_DAC_ConfigChannel(&DacHandle, &sConfig, DACx_CHANNEL) != HAL_OK)
  {
    /* Channel configuration Error */
    Error_Handler();
  }

  /*##-2- Enable DAC Channel1 and associated DMA #############################*/
  if (HAL_DAC_Start_DMA(&DacHandle, DACx_CHANNEL, (uint32_t *)aEscalator8bit, 6, DAC_ALIGN_8B_R) != HAL_OK)
  {
    /* Start DMA Error */
    Error_Handler();
  }
}

/**
  * @brief  DAC Channel1 Triangle Configuration
  * @param  None
  * @retval None
  */
static void DAC_Ch1_TriangleConfig(void)
{
  /*##-1- Initialize the DAC peripheral ######################################*/
  if (HAL_DAC_Init(&DacHandle) != HAL_OK)
  {
    /* Initiliazation Error */
    Error_Handler();
  }

  /*##-2- DAC channel2 Configuration #########################################*/
  sConfig.DAC_Trigger = DAC_TRIGGER_T6_TRGO;
  sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;

  if (HAL_DAC_ConfigChannel(&DacHandle, &sConfig, DACx_CHANNEL) != HAL_OK)
  {
    /* Channel configuration Error */
    Error_Handler();
  }

  /*##-3- DAC channel2 Triangle Wave generation configuration ################*/
  if (HAL_DACEx_TriangleWaveGenerate(&DacHandle, DACx_CHANNEL, DAC_TRIANGLEAMPLITUDE_1023) != HAL_OK)
  {
    /* Triangle wave generation Error */
    Error_Handler();
  }

  /*##-4- Enable DAC Channel1 ################################################*/
  if (HAL_DAC_Start(&DacHandle, DACx_CHANNEL) != HAL_OK)
  {
    /* Start Error */
    Error_Handler();
  }

  /*##-5- Set DAC channel1 DHR12RD register ################################################*/
  if (HAL_DAC_SetValue(&DacHandle, DACx_CHANNEL, DAC_ALIGN_12B_R, 0x100) != HAL_OK)
  {
    /* Setting value Error */
    Error_Handler();
  }
}

/**
  * @brief EXTI line detection callbacks
  * @param GPIO_Pin: Specifies the pins connected EXTI line
  * @retval None
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  /* Change the wave */
  ubKeyPressed = 1;

  /* Change the selected waves forms */
  ubSelectedWavesForm = !ubSelectedWavesForm;
}

/**
  * @brief  TIM6 Configuration
  * @note   TIM6 configuration is based on APB1 frequency
  * @note   TIM6 Update event occurs each TIM6CLK/256
  * @param  None
  * @retval None
  */
void TIM6_Config(void)
{
  static TIM_HandleTypeDef  htim;
  TIM_MasterConfigTypeDef sMasterConfig;

  /*##-1- Configure the TIM peripheral #######################################*/
  /* Time base configuration */
  htim.Instance = TIM6;

  htim.Init.Period            = 0x7FF;
  htim.Init.Prescaler         = 0;
  htim.Init.ClockDivision     = 0;
  htim.Init.CounterMode       = TIM_COUNTERMODE_UP;
  htim.Init.RepetitionCounter = 0;
  htim.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  HAL_TIM_Base_Init(&htim);

  /* TIM6 TRGO selection */
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;

  HAL_TIMEx_MasterConfigSynchronization(&htim, &sMasterConfig);

  /*##-2- Enable TIM peripheral counter ######################################*/
  HAL_TIM_Base_Start(&htim);
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
