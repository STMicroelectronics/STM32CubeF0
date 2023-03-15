/**
  ******************************************************************************
  * @file    CRC/CRC_Data_Reversing_16bit_CRC/Src/main.c
  * @author  MCD Application Team
  * @brief   This sample code shows how to use the STM32F0xx CRC HAL API
  *          to compute a 16-bit CRC code from a data bytes (8-bit data) buffer,
  *          based on a user-defined generating polynomial and initialization 
  *          value, with input and output data reversing features enabled.
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

/** @addtogroup CRC_Data_Reversing_16bit_CRC
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define BUFFER_SIZE  9  /* Input buffer CRC16_DATA8[] is 9-byte long */ 

/* User-defined polynomial */
#define CRC_POLYNOMIAL_16B  0x1021  /* X^16 + X^12 + X^5 + 1, CRC-CCITT generating polynomial */

/* User-defined CRC init value */
/* As the CRC is 16-bit long, the init value is 16-bit long as well */
#define CRC_INIT_VALUE      0x5ABE

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* CRC handler declaration */
CRC_HandleTypeDef   CrcHandle;

/* Used for storing CRC Value */
__IO uint32_t uwCRCValue = 0;

/* Input data buffer */
static const uint8_t CRC16_DATA8[BUFFER_SIZE] = {0x4D, 0x3C, 0x2B, 0x1A,
                                                 0xBE, 0x71, 0xC9, 0x8A,
                                                 0x5E};
                                                 
/* Check input data buffer, used to verify data reversal options */
static const uint8_t CRC16_DATA8_CHECK[BUFFER_SIZE] = {0x58, 0xD4, 0x3C, 0xB2, 
                                                       0x51, 0x93, 0x8E, 0x7D, 
                                                       0x7A};                                                 

/* Expected CRC value yielded by CRC16_DATA8[] input buffer 
   with output data reversal feature disabled (default)
   The 16 LSB bits are the 16-bit long CRC. */
uint32_t uwExpectedCRCValue = 0x0000C20A;
/* Expected CRC value yielded by CRC16_DATA8[] input buffer 
   with output data reversal feature enabled.
   The 16 LSB bits are the 16-bit long CRC. */
uint32_t uwExpectedCRCValue_reversed;

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void Error_Handler(void);
static uint32_t Reverse(register uint32_t x);

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

  /* Configure LED3 and LED5 */
  BSP_LED_Init(LED3);
  BSP_LED_Init(LED5);


  /****************************************************************************/
  /*                                                                          */
  /*                     CRC peripheral initialization                        */
  /*                                                                          */    
  /****************************************************************************/
    
  CrcHandle.Instance = CRC;

  /* The default polynomial is not used. The one to be used must be defined 
     in CrcHandle.Init.GeneratingPolynomial */  
  CrcHandle.Init.DefaultPolynomialUse    = DEFAULT_POLYNOMIAL_DISABLE;
  
  /* Set the value of the generating polynomial. 
    The one used in that example is the 16-bit long CRC generating
    polynomial X^16 + X^12 + X^5 + 1 */
  CrcHandle.Init.GeneratingPolynomial    = CRC_POLYNOMIAL_16B;
  
  /* The user-defined generating polynomial yields a 16-bit long CRC */
  CrcHandle.Init.CRCLength               = CRC_POLYLENGTH_16B;

  /* The default init value is not used */
  CrcHandle.Init.DefaultInitValueUse     = DEFAULT_INIT_VALUE_DISABLE;
  
  /* User init value is used instead */
  CrcHandle.Init.InitValue               = CRC_INIT_VALUE;

  /* The input data are inverted by word */
  CrcHandle.Init.InputDataInversionMode  = CRC_INPUTDATA_INVERSION_WORD;
  /* The bit reversal is done on a full word basis.
     
     The input stream yielded by CRC16_DATA8[] is the sequence of
     bytes 0x4D, 0x3C, 0x2B, 0x1A, 0xBE, 0x71, ...
     meaning that the first word written in the CRC DR register is
      0x4D3C2B1A.
     
     Bit reversal done by the hardware on the full word leads to the actual 
     CRC processing of
      0x58D43CB2.
     
     Similarly, the second word written in the IP is 0xBE71C98A, leading to
     the processing of 0x51938E7D after hardware input data reversal.
     
     Note that when the software writes less than a word in the IP (to minimize the 
     number of write accesses for a given number of bytes), the bit-reversal operation 
     is carried out only on the inputted data.
     Therefore, the last written data is the last byte 0x5E, processed as 0x7A
     by the hardware after bit-reversal to wrap-up the CRC computation.
     
     This means that the field InputDataInversionMode set to CRC_INPUTDATA_INVERSION_WORD
     applied to {0x4D, 0x3C, 0x2B, 0x1A, 0xBE, 0x71, 0xC9, 0x8A, 0x5E}
     yields the same result as InputDataInversionMode set to CRC_INPUTDATA_INVERSION_NONE
     applied to {0x58, 0xD4, 0x3C, 0xB2, 0x51, 0x93, 0x8E, 0x7D, 0x7A}.
     
  */       
  
  /* The output data are inverted */
  CrcHandle.Init.OutputDataInversionMode = CRC_OUTPUTDATA_INVERSION_ENABLE;
  /* Output data reversal can only be done at bit level. 
     The expected CRC is 0x5043 after output data reversal, meaning 
     that the CRC before the reversal operation is 0xC20A. */

  /* The input data are bytes (8-bit long data) */
  CrcHandle.InputDataFormat = CRC_INPUTDATA_FORMAT_BYTES;

  /* De-initialize the CRC peripheral */
  if (HAL_CRC_DeInit(&CrcHandle) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }  

  /* Then, initialize the CRC handle */
  if (HAL_CRC_Init(&CrcHandle) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }


  /****************************************************************************/
  /*                                                                          */
  /*                      CRC computation                                     */
  /*                                                                          */    
  /****************************************************************************/

  /* The 16-bit long CRC of the 9-byte buffer is computed. After IP initialization, 
     the CRC calculator is initialized with the user-defined value that is 0x5ABE.
    
    The computed CRC is stored in uint32_t uwCRCValue. The 16-bit long CRC is made of
    uwCRCValue 16 LSB bits. */

  uwCRCValue = HAL_CRC_Accumulate(&CrcHandle, (uint32_t *)&CRC16_DATA8, BUFFER_SIZE);

  /* Compare the CRC value to the expected one (reversed) */
  uwExpectedCRCValue_reversed = (Reverse(uwExpectedCRCValue) >> 16);
  if (uwCRCValue != uwExpectedCRCValue_reversed)
  {
    /* Wrong CRC value: Turn LED3 on */
    Error_Handler();
  }
             
  
  
  /****************************************************************************/
  /*                                                                          */
  /*         CRC computation with reversal options disabled                   */
  /*                                                                          */    
  /****************************************************************************/

  /* Next, the input and output data inversion features are disabled and
     it is verified that the input data CRC16_DATA8_CHECK[] which is defined with 
     a different endianness scheme yields the same CRC.
     
     As explained above, CRC16_DATA8_CHECK is obtained from CRC16_DATA8
     by a bit-reversal operation carried out on full word and vice versa. */
     
    
  /* Reversal options are disabled */  
  CrcHandle.Init.InputDataInversionMode  = CRC_INPUTDATA_INVERSION_NONE; 
  CrcHandle.Init.OutputDataInversionMode = CRC_OUTPUTDATA_INVERSION_DISABLE; 
  
   /* IP re-initialization. The CRC calculator is initialized again with 
     the user-defined value that is 0x5ABE. */
  if (HAL_CRC_Init(&CrcHandle) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }


  /*  The newly computed CRC is stored in uint32_t uwCRCValue. */
  uwCRCValue = HAL_CRC_Accumulate(&CrcHandle, (uint32_t *)&CRC16_DATA8_CHECK, BUFFER_SIZE);


  /* Compare the new CRC value to the expected one, taking into account the
     output data reversal feature is disabled. */
  if (uwCRCValue != uwExpectedCRCValue)
  {
    /* Wrong CRC value: Turn LED3 on */
    Error_Handler();
  }
  else
  {
    /* Right CRC value: Turn LED5 on */
    BSP_LED_On(LED5);
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
  /* Turn LED3 on */
  BSP_LED_On(LED3);
  while (1)
  {
  }  

}

/**
  * @brief  Reverse bit order in a word (equivalent to __RBIT instruction)
  * @param  x: input word to reverse its bits
  * @retval reversed bits of the input word
  */
static uint32_t Reverse(register uint32_t x)
{
    x = (((x & 0xaaaaaaaa) >> 1) | ((x & 0x55555555) << 1));
    x = (((x & 0xcccccccc) >> 2) | ((x & 0x33333333) << 2));
    x = (((x & 0xf0f0f0f0) >> 4) | ((x & 0x0f0f0f0f) << 4));
    x = (((x & 0xff00ff00) >> 8) | ((x & 0x00ff00ff) << 8));
    return((x >> 16) | (x << 16));

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
