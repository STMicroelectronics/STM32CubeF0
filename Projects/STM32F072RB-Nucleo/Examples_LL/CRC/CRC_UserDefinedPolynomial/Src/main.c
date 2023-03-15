/**
  ******************************************************************************
  * @file    Examples_LL/CRC/CRC_UserDefinedPolynomial/Src/main.c
  * @author  MCD Application Team
  * @brief   This example describes how to use CRC peripheral for generating 8-bit CRC value
  *          for an input data Buffer, based on a user defined polynomial value,
  *          using the STM32F0xx CRC LL API.
  *          Peripheral initialization done using LL unitary services functions.
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

/** @addtogroup STM32F0xx_LL_Examples
  * @{
  */

/** @addtogroup CRC_UserDefinedPolynomial
  * @{
  */

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
#define BUFFER_SIZE    39   /* 9 u32 + 1 u16 + 1 u8 */

/* 8-bit long user defined Polynomial value for this example
   In this example, the polynomial is set manually to 0x9B 
   that is X^8 + X^7 + X^4 + X^3 + X + 1. */
#define  CRC8_POLYNOMIAL_VALUE    0x9B

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
/* Used for storing CRC Value */
__IO uint8_t ubCRCValue = 0;


static const uint8_t aDataBuffer[BUFFER_SIZE] =
{
  0x21, 0x10, 0x00, 0x00, 0x63, 0x30, 0x42, 0x20, 0xa5, 0x50, 0x84, 0x40,
  0xe7, 0x70, 0xc6, 0x60, 0x4a, 0xa1, 0x29, 0x91, 0x8c, 0xc1, 0x6b, 0xb1,
  0xce, 0xe1, 0xad, 0xd1, 0x31, 0x12, 0xef, 0xf1, 0x52, 0x22, 0x73, 0x32,
  0xa1, 0xb2, 0xc3
};

/* Expected CRC Value */
uint8_t ubExpectedCRCValue = 0x0C;

/* Private function prototypes -----------------------------------------------*/
void     SystemClock_Config(void);
void     Configure_CRC(void);
uint8_t  Calculate_CRC(uint32_t);
void     CheckCRCResultValue(void);
void     LED_Init(void);
void     LED_On(void);
void     LED_Blinking(uint32_t Period);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  /* Configure the system clock to 48 MHz */
  SystemClock_Config();

  /* Initialize LED2 */
  LED_Init();

  /* Configure CRC (CRC IP configuration using user-defined Polynomial value) */
  Configure_CRC();

  /* Perform CRC calculation on data contained in aDataBuffer */
  ubCRCValue = Calculate_CRC(BUFFER_SIZE);

  /* Check if CRC computed result value is equal to expected one */
  CheckCRCResultValue();

  /* Infinite loop */
  while (1)
  {
  }
}

/**
  * @brief  This function configures CRC Instance.
  * @note   This function is used to :
  *         -1- Enable peripheral clock for CRC.
  *         -2- Configure CRC functional parameters.
  * @note   Peripheral configuration is minimal configuration from reset values.
  *         Thus, some useless LL unitary functions calls below are provided as
  *         commented examples - setting is default configuration from reset.
  * @param  None
  * @retval None
  */
void Configure_CRC(void)
{

  /* (1) Enable peripheral clock for CRC                   *********************/
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_CRC);

  /* (2) Configure CRC functional parameters  ********************************/

  /* Configure CRC calculation unit with user defined polynomial value, 8-bit long */
  LL_CRC_SetPolynomialCoef(CRC, CRC8_POLYNOMIAL_VALUE);  
  LL_CRC_SetPolynomialSize(CRC, LL_CRC_POLYLENGTH_8B);
  
  /* Initialize default CRC initial value */
  /* Reset value is LL_CRC_DEFAULT_CRC_INITVALUE */
  // LL_CRC_SetInitialData(CRC, LL_CRC_DEFAULT_CRC_INITVALUE);  
  
  /* Set input data inversion mode : No inversion*/
  /* Reset value is LL_CRC_INDATA_REVERSE_NONE */
  // LL_CRC_SetInputDataReverseMode(CRC, LL_CRC_INDATA_REVERSE_NONE);
  
  /* Set output data inversion mode : No inversion */
  /* Reset value is LL_CRC_OUTDATA_REVERSE_NONE */
  // LL_CRC_SetOutputDataReverseMode(CRC, LL_CRC_OUTDATA_REVERSE_NONE);
}

/**
  * @brief  This function performs CRC calculation on BufferSize bytes from input data buffer aDataBuffer.
  * @param  BufferSize Nb of bytes to be processed for CRC calculation
  * @retval 8-bit CRC value computed on input data buffer
  */
uint8_t Calculate_CRC(uint32_t BufferSize)
{
  register uint32_t data = 0;
  register uint32_t index = 0;

  /* Compute the CRC of Data Buffer array*/
  for (index = 0; index < (BufferSize / 4); index++)
  {
    data = (uint32_t)((aDataBuffer[4 * index] << 24) | (aDataBuffer[4 * index + 1] << 16) | (aDataBuffer[4 * index + 2] << 8) | aDataBuffer[4 * index + 3]);
    LL_CRC_FeedData32(CRC, data);
  }
  
  /* Last bytes specific handling */
  if ((BUFFER_SIZE % 4) != 0)
  {
    if  (BUFFER_SIZE % 4 == 1)
    {
      LL_CRC_FeedData8(CRC, aDataBuffer[4 * index]);
    }
    if  (BUFFER_SIZE % 4 == 2)
    {
      LL_CRC_FeedData16(CRC, (uint16_t)((aDataBuffer[4 * index] << 8) | aDataBuffer[4 * index + 1]));

    }
    if  (BUFFER_SIZE % 4 == 3)
    {
      LL_CRC_FeedData16(CRC, (uint16_t)((aDataBuffer[4 * index] << 8) | aDataBuffer[4 * index + 1]));

      LL_CRC_FeedData8(CRC, aDataBuffer[4 * index + 2]);
    }
  }

  /* Return computed CRC value */
  return(LL_CRC_ReadData8(CRC));
}

/**
  * @brief  Check CRC computation result value.
  * @param  None
  * @retval None
  */
void CheckCRCResultValue(void)
{
  /* Compare the CRC value to the Expected one */
  if (ubCRCValue != ubExpectedCRCValue)
  {
    /* Wrong CRC value: Set LED2 to Blinking mode (Error) */
    LED_Blinking(LED_BLINK_ERROR);
  }
  else
  {
    /* Right CRC value: Turn LED2 on */
    LED_On();
  }
}

/**
  * @brief  Initialize LED2.
  * @param  None
  * @retval None
  */
void LED_Init(void)
{
  /* Enable the LED2 Clock */
  LED2_GPIO_CLK_ENABLE();

  /* Configure IO in output push-pull mode to drive external LED2 */
  LL_GPIO_SetPinMode(LED2_GPIO_PORT, LED2_PIN, LL_GPIO_MODE_OUTPUT);
  /* Reset value is LL_GPIO_OUTPUT_PUSHPULL */
  //LL_GPIO_SetPinOutputType(LED2_GPIO_PORT, LED2_PIN, LL_GPIO_OUTPUT_PUSHPULL);
  /* Reset value is LL_GPIO_SPEED_FREQ_LOW */
  //LL_GPIO_SetPinSpeed(LED2_GPIO_PORT, LED2_PIN, LL_GPIO_SPEED_FREQ_LOW);
  /* Reset value is LL_GPIO_PULL_NO */
  //LL_GPIO_SetPinPull(LED2_GPIO_PORT, LED2_PIN, LL_GPIO_PULL_NO);
}

/**
  * @brief  Turn-on LED2.
  * @param  None
  * @retval None
  */
void LED_On(void)
{
  /* Turn LED2 on */
  LL_GPIO_SetOutputPin(LED2_GPIO_PORT, LED2_PIN);
}

/**
  * @brief  Set LED2 to Blinking mode for an infinite loop (toggle period based on value provided as input parameter).
  * @param  Period : Period of time (in ms) between each toggling of LED
  *   This parameter can be user defined values. Pre-defined values used in that example are :
  *     @arg LED_BLINK_FAST : Fast Blinking
  *     @arg LED_BLINK_SLOW : Slow Blinking
  *     @arg LED_BLINK_ERROR : Error specific Blinking
  * @retval None
  */
void LED_Blinking(uint32_t Period)
{
  /* Toggle IO in an infinite loop */
  while (1)
  {
    LL_GPIO_TogglePin(LED2_GPIO_PORT, LED2_PIN);  
    LL_mDelay(Period);
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
  /* Set FLASH latency */ 
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_1);

  /* Enable HSI48 and wait for activation*/
  LL_RCC_HSI48_Enable(); 
  while(LL_RCC_HSI48_IsReady() != 1) 
  {
  };
  
  /* Main PLL configuration and activation */
  LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSI48, LL_RCC_PLL_MUL_2, LL_RCC_PREDIV_DIV_2);
  
  LL_RCC_PLL_Enable();
  while(LL_RCC_PLL_IsReady() != 1)
  {
  };
  
  /* Sysclk activation on the main PLL */
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL)
  {
  };
  
  /* Set APB1 prescaler */
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
  
  /* Set systick to 1ms in using frequency set to 48MHz */
  /* This frequency can be calculated through LL RCC macro */
  /* ex: __LL_RCC_CALC_PLLCLK_FREQ (HSI48_VALUE, LL_RCC_PLL_MUL_2, LL_RCC_PREDIV_DIV_2) */
  LL_Init1msTick(48000000);
  
  /* Update CMSIS variable (which can be updated also through SystemCoreClockUpdate function) */
  LL_SetSystemCoreClock(48000000);
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
     ex: printf("Wrong parameters value: file %s on line %d", file, line) */

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
