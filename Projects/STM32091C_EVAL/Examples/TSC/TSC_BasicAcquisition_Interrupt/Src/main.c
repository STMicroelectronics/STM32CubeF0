/**
  ******************************************************************************
  * @file    TSC/TSC_BasicAcquisition_Interrupt/Src/main.c
  * @author  MCD Application Team
  * @brief   This example describes how to use the TSC to perform an acquisition
  *          of two channels in interrupt mode.
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

/** @addtogroup TSC_BasicAcquisition_Interrupt
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* These values can be modified, board layout dependency */
#define TSCx_TS1_MAXTHRESHOLD 525
#define TSCx_TS2_MAXTHRESHOLD 525
#define TSCx_TS1_MINTHRESHOLD 500 
#define TSCx_TS2_MINTHRESHOLD 500 
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* TSC handler declaration */
TSC_HandleTypeDef TscHandle;

/* Array used to store the two acquisition value (one per channel) */
__IO uint32_t uhTSCAcquisitionValue[2];

uint8_t IdxBank = 0;
TSC_IOConfigTypeDef IoConfig;
  
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
  BSP_LED_Init(LED1);
  BSP_LED_Init(LED2);
  BSP_LED_Init(LED4);
  
  /*##-1- Configure the TSC peripheral #######################################*/
  TscHandle.Instance                     = TSCx;
  TscHandle.Init.AcquisitionMode         = TSC_ACQ_MODE_NORMAL;
  TscHandle.Init.CTPulseHighLength       = TSC_CTPH_1CYCLE;
  TscHandle.Init.CTPulseLowLength        = TSC_CTPL_1CYCLE;
  TscHandle.Init.IODefaultMode           = TSC_IODEF_OUT_PP_LOW;
  TscHandle.Init.MaxCountInterrupt       = ENABLE;
  TscHandle.Init.MaxCountValue           = TSC_MCV_16383;
  TscHandle.Init.PulseGeneratorPrescaler = TSC_PG_PRESC_DIV64;
  TscHandle.Init.SpreadSpectrum          = DISABLE;
  TscHandle.Init.SpreadSpectrumDeviation = 127;
  TscHandle.Init.SpreadSpectrumPrescaler = TSC_SS_PRESC_DIV1;
  TscHandle.Init.SynchroPinPolarity      = TSC_SYNC_POLARITY_FALLING;
  TscHandle.Init.ChannelIOs              = 0; /* Not needed yet. Will be set with HAL_TSC_IOConfig() */
  TscHandle.Init.SamplingIOs             = 0; /* Not needed yet. Will be set with HAL_TSC_IOConfig() */
  TscHandle.Init.ShieldIOs               = 0; /* Not needed yet. Will be set with HAL_TSC_IOConfig() */

  if (HAL_TSC_Init(&TscHandle) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }

  /*##-2- Configure the touch-sensing IOs ####################################*/
  IoConfig.ChannelIOs  = TSC_GROUP8_IO2; /* Start with the first channel */
  IoConfig.SamplingIOs = (TSC_GROUP8_IO3 | TSC_GROUP8_IO3 | TSC_GROUP7_IO4);
  IoConfig.ShieldIOs   = TSC_GROUP7_IO3;
  
  if (HAL_TSC_IOConfig(&TscHandle, &IoConfig) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }

  /*##-3- Discharge the touch-sensing IOs ####################################*/
  HAL_TSC_IODischarge(&TscHandle, ENABLE);
  HAL_Delay(1); /* 1 ms is more than enough to discharge all capacitors */

  /*##-4- Start the acquisition process ######################################*/
  if (HAL_TSC_Start_IT(&TscHandle) != HAL_OK)
  {
    /* Acquisition Error */
    Error_Handler();
  }
  
  /* Infinite loop */
  while (1)
  {
	  /*Suspend Tick increment to prevent wakeup by Systick interrupt. 
      Otherwise the Systick interrupt will wake up the device within 1ms (HAL time base)*/
    HAL_SuspendTick();
  	
    /* The acquisition process is now performed in the HAL_TSC_ConvCpltCallback() function */
    HAL_PWR_EnterSLEEPMode(PWR_MAINREGULATOR_ON, PWR_SLEEPENTRY_WFI);
    
    /* Resume Tick interrupt if disabled prior to sleep mode entry*/
    HAL_ResumeTick();
  }
}

/**
  * @brief  Acquisition completed callback in non blocking mode 
  * @param  htsc: pointer to a TSC_HandleTypeDef structure that contains
  *         the configuration information for the specified TSC.
  * @retval None
  */
void HAL_TSC_ConvCpltCallback(TSC_HandleTypeDef* htsc)
{  
  /*##-5- Discharge the touch-sensing IOs ####################################*/
  HAL_TSC_IODischarge(&TscHandle, ENABLE);
  /* Note: a delay can be added here */

  /*##-6- Check if the acquisition is correct (no max count) #################*/
  if (HAL_TSC_GroupGetStatus(&TscHandle, TSC_GROUP8_IDX) == TSC_GROUP_COMPLETED)
  {
    /*##-7- Read the acquisition value #######################################*/
    uhTSCAcquisitionValue[IdxBank] = HAL_TSC_GroupGetValue(&TscHandle, TSC_GROUP8_IDX);  
    /* Note: Show the touch activity on LEDs.
       The threshold values are indicative and may need to be adjusted */
    if ((uhTSCAcquisitionValue[0] > TSCx_TS1_MINTHRESHOLD) && (uhTSCAcquisitionValue[0] < TSCx_TS1_MAXTHRESHOLD)) // Channel 1
    {
      BSP_LED_On(LED1);
    }
    else
    {
      BSP_LED_Off(LED1);
    }

    if ((uhTSCAcquisitionValue[1] > TSCx_TS2_MINTHRESHOLD) && (uhTSCAcquisitionValue[1] < TSCx_TS2_MAXTHRESHOLD)) // Channel 2
    {
      BSP_LED_On(LED2);
    }
    else
    {
      BSP_LED_Off(LED2);
    }
  }

  /*##-8- Configure the next channels to be acquired #########################*/
  if (IdxBank == 0)
  {
    IoConfig.ChannelIOs = TSC_GROUP8_IO1; /* TS2 touchkey */
    IdxBank = 1;
  }
  else
  {
    IoConfig.ChannelIOs = TSC_GROUP8_IO2; /* TS1 touchkey */
    IdxBank = 0;
  }
  
  if (HAL_TSC_IOConfig(&TscHandle, &IoConfig) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
 
  /*##-8- Re-start the acquisition process ###################################*/
  if (HAL_TSC_Start_IT(&TscHandle) != HAL_OK)
  {
    /* Acquisition Error */
    Error_Handler();
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

  /* Select PLL as system clock source and configure the HCLK, PCLK1 clocks dividers */
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
    BSP_LED_Toggle(LED4);
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
    BSP_LED_Toggle(LED4);
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
