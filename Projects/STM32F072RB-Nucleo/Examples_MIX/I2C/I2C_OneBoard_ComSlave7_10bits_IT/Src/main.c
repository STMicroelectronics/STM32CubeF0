/**
  ******************************************************************************
  * @file    Examples_MIX/I2C/I2C_OneBoard_ComSlave7_10bits_IT/Src/main.c
  * @author  MCD Application Team
  * @brief   This sample code shows how to use STM32F0xx I2C HAL and LL API
  *          to transmit and receive a data buffer with a communication process
  *          based on IT transfer.
  *          The communication is done using 1 Board.
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

/** @addtogroup STM32F0xx_MIX_Examples
  * @{
  */

/** @addtogroup I2C_OneBoard_ComSlave7_10bits_IT
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define I2C_SLAVE_ADDRESS1 0x13E /* 10-Bit address */
#define I2C_SLAVE_ADDRESS2 0x64  /* 7-Bit address  */

/* Timing register value is computed with the STM32CubeMX Tool,
  * Fast Mode Plus @1MHz with I2CCLK = 48 MHz,
  * rise time = 120ns, fall time = 0ns
  * Timing Value = (uint32_t)0x20200207
  */
#define I2C_TIMING      0x20200207

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* I2C handler declaration */
I2C_HandleTypeDef I2cMasterHandle;
I2C_HandleTypeDef I2cSlaveHandle;

/* Buffer used for transmission */
uint8_t aTxBuffer[] = " ****I2C_OneBoard communication based on IT****  ****I2C_OneBoard communication based on IT****  ****I2C_OneBoard communication based on IT**** ";

/* Buffer used for reception */
uint8_t aRxBuffer[RXBUFFERSIZE];

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static uint16_t Buffercmp(uint8_t *pBuffer1, uint8_t *pBuffer2, uint16_t BufferLength);
static void Flush_Buffer(uint8_t* pBuffer, uint16_t BufferLength);
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
  
  /*##-1- Configure the I2C MASTER peripheral to discuss with Slave1 #########*/
  /* Addressing Mode : 10-Bit */
  I2cMasterHandle.Instance              = I2Cx_MASTER;
  I2cMasterHandle.Init.Timing           = I2C_TIMING;
  I2cMasterHandle.Init.OwnAddress1      = 0x00;
  I2cMasterHandle.Init.AddressingMode   = I2C_ADDRESSINGMODE_10BIT;
  I2cMasterHandle.Init.DualAddressMode  = I2C_DUALADDRESS_DISABLE;
  I2cMasterHandle.Init.OwnAddress2      = 0x00;
  I2cMasterHandle.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  I2cMasterHandle.Init.GeneralCallMode  = I2C_GENERALCALL_DISABLE;
  I2cMasterHandle.Init.NoStretchMode    = I2C_NOSTRETCH_DISABLE;  
  
  if(HAL_I2C_Init(&I2cMasterHandle) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
  
  /* Enable the Analog I2C Filter */
  HAL_I2CEx_ConfigAnalogFilter(&I2cMasterHandle,I2C_ANALOGFILTER_ENABLE);

  /*##-2- Configure the I2C SLAVE peripheral #################################*/
  /* Own Address 1 : 10-Bit(custom) (Slave1) */
  /* Own Address 2 : 7-Bit(default) (Slave2) */
  /* Dual Mode enabled                */
  I2cSlaveHandle.Instance              = I2Cx_SLAVE;
  I2cSlaveHandle.Init.Timing           = I2C_TIMING;
  I2cSlaveHandle.Init.OwnAddress1      = I2C_SLAVE_ADDRESS1;
  I2cSlaveHandle.Init.AddressingMode   = I2C_ADDRESSINGMODE_10BIT;
  I2cSlaveHandle.Init.DualAddressMode  = I2C_DUALADDRESS_ENABLE;
  I2cSlaveHandle.Init.OwnAddress2      = I2C_SLAVE_ADDRESS2;
  I2cSlaveHandle.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  I2cSlaveHandle.Init.GeneralCallMode  = I2C_GENERALCALL_DISABLE;
  I2cSlaveHandle.Init.NoStretchMode    = I2C_NOSTRETCH_DISABLE;  
  
  if(HAL_I2C_Init(&I2cSlaveHandle) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
  
  /* Enable the Analog I2C Filter */
  HAL_I2CEx_ConfigAnalogFilter(&I2cSlaveHandle,I2C_ANALOGFILTER_ENABLE);

  /* Configure User push-button button */
  BSP_PB_Init(BUTTON_USER,BUTTON_MODE_GPIO);

  /* Wait for User push-button press before starting the Communication */
  while (BSP_PB_GetState(BUTTON_USER) != GPIO_PIN_RESET)
  {
  }
  
  /* Wait for User push-button release before starting the Communication */
  while (BSP_PB_GetState(BUTTON_USER) != GPIO_PIN_SET)
  {
  }

  /*##-3- Slave1 and Slave2 Receive process from master ####################*/
  /* Receive data through "aRxBuffer" buffer */
  while(HAL_I2C_Slave_Receive_IT(&I2cSlaveHandle, (uint8_t*)&aRxBuffer, RXBUFFERSIZE)!= HAL_OK)
  {
  }

  /*##-4- Master Transmit process for Slave1 ###############################*/
  /* Transmit data through "aTxBuffer" buffer */
  while(HAL_I2C_Master_Transmit_IT(&I2cMasterHandle, (uint16_t)I2C_SLAVE_ADDRESS1, (uint8_t*)&aTxBuffer, TXBUFFERSIZE)!= HAL_OK)
  {
    /* Error_Handler() function is called when Timeout error occurs.
       When Acknowledge failure occurs (Slave don't acknowledge its address)
       Master restarts communication */
    if (HAL_I2C_GetError(&I2cMasterHandle) != HAL_I2C_ERROR_AF)
    {
      Error_Handler();
    }
  }

  /*##-5- Wait for the end of the transfer ###################################*/
  /*  Before starting a new communication transfer, you need to check the current
      state of the peripheral; if it�s busy you need to wait for the end of current
      transfer before starting a new one.
      For simplicity reasons, this example is just waiting till the end of the
      transfer, but application may perform other tasks while transfer operation
      is ongoing. */
  while (HAL_I2C_GetState(&I2cSlaveHandle) != HAL_I2C_STATE_READY)
  {
  }

  while (HAL_I2C_GetState(&I2cMasterHandle) != HAL_I2C_STATE_READY)
  {
  }

  /*##-6- Compare the sent and received buffers ##############################*/
  if(Buffercmp((uint8_t*)aTxBuffer, (uint8_t*)aRxBuffer, TXBUFFERSIZE))
  {
    /* Processing Error */
    Error_Handler();
  }

  /* Toggle LED2: Transfer in reception Slave1 process is correct */
  BSP_LED_Toggle(LED2);

  /* Flush Rx buffers */
  Flush_Buffer((uint8_t*)aRxBuffer,RXBUFFERSIZE);

  /* Wait for User push-button press before starting the Communication with Slave2 */
  while (BSP_PB_GetState(BUTTON_USER) != GPIO_PIN_RESET)
  {
  }
  
  /* Wait for User push-button release before starting the Communication with Slave2 */
  while (BSP_PB_GetState(BUTTON_USER) != GPIO_PIN_SET)
  {
  }

  /*##-7- Slave1 and Slave2 Transmit process for master ######################*/
  while(HAL_I2C_Slave_Transmit_IT(&I2cSlaveHandle, (uint8_t*)&aTxBuffer, TXBUFFERSIZE)!= HAL_OK)
  {
  }

  /*##-8- Configure the I2C MASTER peripheral to discuss with Slave2 (7-Bit address) #*/
  /* Using LL interface, initializes directly the I2C MASTER peripheral in 7-Bit addressing mode*/
  LL_I2C_SetMasterAddressingMode(I2cMasterHandle.Instance, I2C_ADDRESSINGMODE_7BIT);

  /* Following code sequence is needed in order to keep I2C handle structure          */
  /* content in line with IP configuration                                            */
  I2cMasterHandle.Init.AddressingMode  = I2C_ADDRESSINGMODE_7BIT;

  /* Using HAL interface, following interface must be used :                          */
  /*  - HAL_I2C_DeInit() then HAL_I2C_Init() to deInitializes then Initializes        */
  /*  the I2C MASTER peripheral to perform an update of AddressingMode Init parameter */
  
  /*##-9- Put I2C peripheral in Reception process from Slave 2 ###############*/
  while(HAL_I2C_Master_Receive_IT(&I2cMasterHandle, (uint16_t)I2C_SLAVE_ADDRESS2, (uint8_t*)&aRxBuffer, TXBUFFERSIZE)!= HAL_OK)
  {
    /* Error_Handler() function is called when Timeout error occurs.
       When Acknowledge failure occurs (Slave don't acknowledge its address)
       Master restarts communication */
    if (HAL_I2C_GetError(&I2cMasterHandle) != HAL_I2C_ERROR_AF)
    {
      Error_Handler();
    }
  }

  /*##-10- Wait for the end of the transfer ##################################*/  
  /*  Before starting a new communication transfer, you need to check the current   
      state of the peripheral; if it�s busy you need to wait for the end of current
      transfer before starting a new one.
      For simplicity reasons, this example is just waiting till the end of the
      transfer, but application may perform other tasks while transfer operation
      is ongoing. */
  while (HAL_I2C_GetState(&I2cSlaveHandle) != HAL_I2C_STATE_READY)
  {
  }

  while (HAL_I2C_GetState(&I2cMasterHandle) != HAL_I2C_STATE_READY)
  {
  }

  /*##-11- Compare the sent and received buffers #############################*/
  if(Buffercmp((uint8_t*)aTxBuffer, (uint8_t*)aRxBuffer, TXBUFFERSIZE))
  {
    /* Processing Error */
    Error_Handler();
  }

  /* Toggle LED2: Transfer in Slave2 transmission process is correct */
  BSP_LED_Toggle(LED2);

  /* Infinite loop */  
  while(1)
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
  * @brief  I2C error callbacks.
  * @param  I2cHandle: I2C handle
  * @note   This example shows a simple way to report transfer error, and you can
  *         add your own implementation.
  * @retval None
  */
void HAL_I2C_ErrorCallback(I2C_HandleTypeDef *I2cHandle)
{
  Error_Handler();
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
static void Error_Handler(void)
{
  /* Error if LED2 is slowly blinking (1 sec. period) */
  while(1)
  {    
    BSP_LED_Toggle(LED2); 
    HAL_Delay(1000);
  } 
}

/**
  * @brief  Compares two buffers.
  * @param  pBuffer1, pBuffer2: buffers to be compared.
  * @param  BufferLength: buffer's length
  * @retval 0  : pBuffer1 identical to pBuffer2
  *         >0 : pBuffer1 differs from pBuffer2
  */
static uint16_t Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength)
{
  while (BufferLength--)
  {
    if ((*pBuffer1) != *pBuffer2)
    {
      return BufferLength;
    }
    pBuffer1++;
    pBuffer2++;
  }

  return 0;
}

/**
  * @brief  Flushes the buffer
  * @param  pBuffer: buffers to be flushed.
  * @param  BufferLength: buffer's length
  * @retval None
  */
static void Flush_Buffer(uint8_t* pBuffer, uint16_t BufferLength)
{
  while (BufferLength--)
  {
    *pBuffer = 0;

    pBuffer++;
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
void assert_failed(uint8_t* file, uint32_t line)
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
