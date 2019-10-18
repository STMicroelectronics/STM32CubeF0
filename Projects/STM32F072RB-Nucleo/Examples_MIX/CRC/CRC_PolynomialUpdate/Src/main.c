/**
  ******************************************************************************
  * @file    CRC/CRC_PolynomialUpdate/Src/main.c
  * @author  MCD Application Team
  * @brief   This sample code shows how to use the STM32F0xx CRC HAL API
  *          to compute a CRC code of a given buffer of data words (32-bit),
  *          based on a user-defined generating polynomial.
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

#include "stm32f0xx_ll_crc.h"


/** @addtogroup STM32F0xx_HAL_Examples
  * @{
  */

/** @addtogroup CRC_PolynomialUpdate
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define BUFFER_SIZE    2


/* The user defined polynomial*/
#define CRC_POLYNOMIAL_8B  0x9B   /* X^8 + X^7 + X^4 + X^3 + X + 1 */
#define CRC_POLYNOMIAL_16B 0x1021 /* X^16 + X^12 + X^5 + 1 */

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* CRC handler declaration */
CRC_HandleTypeDef   CrcHandle;

/* Used for storing CRC Value */
__IO uint32_t uwCRCValue = 0;

/* Buffer containing the data on which the CRC will be calculated */
static const uint32_t aDataBuffer[BUFFER_SIZE] =
{
  0x12345678, 0x12213883
};

/* Expected CRC Value */
uint32_t uwExpectedCRCValue1 = 0xAB;
uint32_t uwExpectedCRCValue2 = 0x9357;

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
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
  
  /* Configure the system clock to 48 MHz */
  SystemClock_Config();

  /* Configure LED2 */
  BSP_LED_Init(LED2);

  /*##-1- Configure the CRC peripheral #######################################*/
  CrcHandle.Instance = CRC;

  /* The default polynomial is not used. It is required to defined it in CrcHandle.Init.GeneratingPolynomial*/  
  CrcHandle.Init.DefaultPolynomialUse    = DEFAULT_POLYNOMIAL_DISABLE;
  
  /* Set the value of the polynomial */
  CrcHandle.Init.GeneratingPolynomial    = CRC_POLYNOMIAL_8B;
  
  /* The user-defined generating polynomial generates a
     8-bit long CRC */
  CrcHandle.Init.CRCLength               = CRC_POLYLENGTH_8B;

  /* The default init value is used */
  CrcHandle.Init.DefaultInitValueUse     = DEFAULT_INIT_VALUE_ENABLE;

  /* The input data are not inverted */
  CrcHandle.Init.InputDataInversionMode  = CRC_INPUTDATA_INVERSION_NONE;

  /* The output data are not inverted */
  CrcHandle.Init.OutputDataInversionMode = CRC_OUTPUTDATA_INVERSION_DISABLE;

  /* The input data are 32- bit long */
  CrcHandle.InputDataFormat              = CRC_INPUTDATA_FORMAT_WORDS;

  if (HAL_CRC_Init(&CrcHandle) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }

  /*##-2- Compute the CRC of "aDataBuffer" ###################################*/
  /* First computation is done using 8-bit generating Polynomial (CRC_POLYNOMIAL_8B) 
     as configured in CRC handle, wtih HAL_CRC_Init */
  uwCRCValue = HAL_CRC_Calculate(&CrcHandle, (uint32_t *)&aDataBuffer, BUFFER_SIZE);

  /*##-3- Compare the CRC value to the Expected one ##########################*/
  if (uwCRCValue != uwExpectedCRCValue1)
  {
    /* Wrong CRC value: enter Error_Handler */
    Error_Handler();
  }
  else
  {
    /* Update CRC generating polynomial length and value using LL services.
       In this case, using LL services allows to avoid calling HAL_CRC_Init()/HAL_CRC_DeInit */
    LL_CRC_SetPolynomialSize(CRC, LL_CRC_POLYLENGTH_16B);
    LL_CRC_SetPolynomialCoef(CRC, CRC_POLYNOMIAL_16B);

    /* Following code sequence is needed in order to keep CRC handle structure 
       content in line with IP configuration */
    /* Set the value of the polynomial */
//    CrcHandle.Init.GeneratingPolynomial    = CRC_POLYNOMIAL_16B;
    /* The user-defined generating polynomial generates a 16-bit long CRC */
//    CrcHandle.Init.CRCLength               = CRC_POLYLENGTH_16B;
    
    /*##-4- Compute the CRC of "aDataBuffer" ###################################*/
    /* Second computation is done using 16-bit generating Polynomial (CRC_POLYNOMIAL_16B) 
       as configured in IP using LL APIs */
    uwCRCValue = HAL_CRC_Calculate(&CrcHandle, (uint32_t *)&aDataBuffer, BUFFER_SIZE);

    /*##-5- Compare the CRC value to the Expected one ##########################*/
    if (uwCRCValue != uwExpectedCRCValue2)
    {
      /* Wrong CRC value: enter Error_Handler */
      Error_Handler();
    }
    else
    {
      /* Right CRC value: Turn LED2 on */
      BSP_LED_On(LED2);
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
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
static void Error_Handler(void)
{
  while (1)
  {
    /* Error if LED2 is slowly blinking (1 sec. period) */
    BSP_LED_Toggle(LED2); 
    HAL_Delay(1000);   
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
