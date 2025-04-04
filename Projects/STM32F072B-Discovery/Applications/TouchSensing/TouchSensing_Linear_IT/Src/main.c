/**
  ******************************************************************************
  * @file    TouchSensing\TouchSensing_Linear_IT\Src\main.c 
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

/** @addtogroup STM32F0xx_HAL_Examples
  * @{
  */

/** @addtogroup TouchSensing_Linear_IT
  * @{
  */

extern uint32_t idx_bank;

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* define for the linear */
#define LINEAR_DETECT ((MyLinRots[0].p_Data->StateId == TSL_STATEID_DETECT) || \
                       (MyLinRots[0].p_Data->StateId == TSL_STATEID_DEB_RELEASE_DETECT))
#define LINEAR_POSITION (MyLinRots[0].p_Data->Position)

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* TSC handler declaration */
TSC_HandleTypeDef TscHandle;

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void Error_Handler(void);
static void Process_Sensors(void);

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
  BSP_LED_Init(LED4); // Linear sensor touched
  BSP_LED_Init(LED5); // Linear sensor position
  BSP_LED_Init(LED3); // Error
  BSP_LED_Init(LED6); // ECS
  
  /* Configure the TSC peripheral */
  TscHandle.Instance                     = TSCx;
  TscHandle.Init.AcquisitionMode         = TSC_ACQ_MODE_NORMAL;
  TscHandle.Init.CTPulseHighLength       = TSC_CTPH_2CYCLES;
  TscHandle.Init.CTPulseLowLength        = TSC_CTPL_2CYCLES;
  TscHandle.Init.IODefaultMode           = TSC_IODEF_OUT_PP_LOW;
  TscHandle.Init.MaxCountInterrupt       = ENABLE;
  TscHandle.Init.MaxCountValue           = TSC_MCV_8191;
  TscHandle.Init.PulseGeneratorPrescaler = TSC_PG_PRESC_DIV64;
  TscHandle.Init.SpreadSpectrum          = DISABLE;
  TscHandle.Init.SpreadSpectrumDeviation = 127;
  TscHandle.Init.SpreadSpectrumPrescaler = TSC_SS_PRESC_DIV1;
  TscHandle.Init.SynchroPinPolarity      = TSC_SYNC_POLARITY_FALLING;
  /* All channel, shield and sampling IOs must be declared below */
  TscHandle.Init.ChannelIOs              = (TSC_GROUP1_IO3 | TSC_GROUP2_IO3 | TSC_GROUP3_IO2);
  TscHandle.Init.SamplingIOs             = (TSC_GROUP1_IO4 | TSC_GROUP2_IO4 | TSC_GROUP3_IO3);
  TscHandle.Init.ShieldIOs               = 0;
  if (HAL_TSC_Init(&TscHandle) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
  
  /* Initialize the STMTouch driver  and start acquisition */
  tsl_user_Init();
  
  /* Infinite loop */
  while (1)
  {
    Process_Sensors();
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
void SystemClock_Config(void)
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
    /* Initialization Error */
    while(1); 
  }

  /* Select PLL as system clock source and configure the HCLK and PCLK1 clocks dividers */
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
  * @brief  Display sensors information on LEDs
  * @param  None
  * @retval None
  */
void Process_Sensors(void)
{
  if (LINEAR_DETECT)
  {
    BSP_LED_On(LED4);
  }
  else
  {
    BSP_LED_Off(LED4);
  }
}

/**
  * @brief  This function handles a tick.
  * @param  None
  * @retval None
  */
void User_Tick_Management(void)
{
  static uint32_t tempo_50ms=0;
  static uint32_t tempo_100ms=0;
  static uint32_t tempo_200ms=0;
  static uint32_t tempo_500ms=0;
  
  tempo_50ms++;
  tempo_50ms%=50;
  tempo_100ms++;
  tempo_100ms%=100;
  tempo_200ms++;
  tempo_200ms%=200;
  tempo_500ms++;
  tempo_500ms%=500;
  
  if (LINEAR_POSITION < 4)
  {
    if (tempo_500ms==0)
    {
      BSP_LED_Toggle(LED5);
    }
  }
  else if (LINEAR_POSITION < 8)
  {
    if (tempo_200ms==0)
    {
      BSP_LED_Toggle(LED5);
    }
  }
  else if (LINEAR_POSITION < 12)
  {
    if (tempo_100ms==0)
    {
      BSP_LED_Toggle(LED5);
    }
  }
  else
  {
    if (tempo_50ms==0)
    {
      BSP_LED_Toggle(LED5);
    }
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
    BSP_LED_Toggle(LED3);
    HAL_Delay(200);
  }
}

#ifdef USE_FULL_ASSERT

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
    BSP_LED_Toggle(LED3);
    HAL_Delay(100);
  }
}
#endif

/**
  * @brief  Acquisition completed callback in non blocking mode 
  * @param  htsc: pointer to a TSC_HandleTypeDef structure that contains
  *         the configuration information for the specified TSC.
  * @retval None
  */
void HAL_TSC_ConvCpltCallback(TSC_HandleTypeDef* htsc)
{
  static __IO TSL_tTick_ms_T ECSLastTick; /* Hold the last time value for ECS */
  
  /* Discharge all capacitors */
#if TSLPRM_IODEF > 0
  __HAL_TSC_SET_IODEF_OUTPPLOW(&TscHandle);
#endif
                         
  TSL_acq_BankGetResult(idx_bank++, 0, 0); /* Read current bank result */
    
  /* Check if we have acquired all the banks */
  if (idx_bank > TSLPRM_TOTAL_BANKS - 1)
  {
    /* Process the objects state machine, DxS and ECS */
    TSL_obj_GroupProcess(&MyObjGroup);
    TSL_dxs_FirstObj(&MyObjGroup);
    if (TSL_tim_CheckDelay_ms(100, &ECSLastTick) == TSL_STATUS_OK)
    {
      if(TSL_ecs_Process(&MyObjGroup) == TSL_STATUS_OK)
      {
        BSP_LED_Toggle(LED6);
      }
      else
      {
        BSP_LED_Off(LED6);
      }
    }
    /* Start again with the first bank */
    idx_bank = 0;    
  }

  /* Configure bank */
  TSL_acq_BankConfig(idx_bank);
  
  /* Start bank acquisition in Interrupt mode */
  TSL_acq_BankStartAcq_IT();
}

/**
  * @brief  Error callback in non blocking mode
  * @param  htsc: pointer to a TSC_HandleTypeDef structure that contains
  *         the configuration information for the specified TSC.
  * @retval None
  */
void HAL_TSC_ErrorCallback(TSC_HandleTypeDef* htsc)
{
  /* Infinite loop */
  while (1)
  {
    BSP_LED_Toggle(LED3);
    HAL_Delay(300);
  }
}

/**
  * @}
  */

/**
  * @}
  */
