/**
  ******************************************************************************
  * @file    I2C/I2C_TwoBoards_AdvComIT/Src/main.c
  * @author  MCD Application Team
  * @brief   This sample code shows how to use STM32F0xx I2C HAL API to transmit
  *          and receive a data buffer with a communication process based on
  *          IT transfer.
  *          The communication is done using 2 Boards.
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

/** @addtogroup I2C_TwoBoards_AdvComIT
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Uncomment this line to use the board as master, if not it is used as slave */
//#define MASTER_BOARD
#define I2C_ADDRESS        0x3E
#define MASTER_REQ_READ    0x12
#define MASTER_REQ_WRITE   0x34

/* I2C TIMING Register define when I2C clock source is SYSCLK */
/* I2C TIMING is calculated in case of the I2C Clock source is the SYSCLK = 48 MHz */
/* This example use TIMING to 0x00A51314 to reach 1 MHz speed (Rise time = 100 ns, Fall time = 100 ns) */
#define I2C_TIMING      0x00A51314

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* I2C handler declaration */
/* I2C handler declared in "stm32f072b_discovery.c" file */
extern I2C_HandleTypeDef I2cHandle;


/* Buffer used for transmission */
uint8_t aTxBuffer[] = " ****I2C_TwoBoards communication based on IT****  ****I2C_TwoBoards communication based on IT****  ****I2C_TwoBoards communication based on IT**** ";

/* Buffer used for reception */
uint8_t aRxBuffer[RXBUFFERSIZE];
__IO uint16_t hTxNumData = 0;
__IO uint16_t hRxNumData = 0;
uint8_t bTransferRequest = 0;

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

  /* Configure LED_GREEN and LED_RED */
  BSP_LED_Init(LED_GREEN);
  BSP_LED_Init(LED_RED);

  /*##-1- Configure the I2C peripheral ######################################*/
  I2cHandle.Instance             = I2Cx;
  I2cHandle.Init.Timing          = I2C_TIMING;
  I2cHandle.Init.OwnAddress1     = I2C_ADDRESS;
  I2cHandle.Init.AddressingMode  = I2C_ADDRESSINGMODE_7BIT;
  I2cHandle.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  I2cHandle.Init.OwnAddress2     = 0xFF;
  I2cHandle.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  I2cHandle.Init.NoStretchMode   = I2C_NOSTRETCH_DISABLE;  

  if(HAL_I2C_Init(&I2cHandle) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }

  /* Enable the Analog I2C Filter */
  HAL_I2CEx_ConfigAnalogFilter(&I2cHandle,I2C_ANALOGFILTER_ENABLE);

#ifdef MASTER_BOARD

  /* Configure User push-button */
  BSP_PB_Init(BUTTON_USER,BUTTON_MODE_GPIO);

  /* Wait for User push-button press before starting the Communication */
  while (BSP_PB_GetState(BUTTON_USER) != GPIO_PIN_RESET)
  {
  }

  /* Wait for User push-button release before starting the Communication */
  while (BSP_PB_GetState(BUTTON_USER) != GPIO_PIN_SET)
  {
  }

  while(1)
  {
    /* Initialize number of data variables */
    hTxNumData = TXBUFFERSIZE;
    hRxNumData = RXBUFFERSIZE;

    /* Update bTransferRequest to send buffer write request for Slave */
    bTransferRequest = MASTER_REQ_WRITE;

    /*##-2- Master sends write request for slave #############################*/
    do
    {
      if(HAL_I2C_Master_Transmit_IT(&I2cHandle, (uint16_t)I2C_ADDRESS, (uint8_t*)&bTransferRequest, 1)!= HAL_OK)
      {
        /* Error_Handler() function is called when error occurs. */
        Error_Handler();
      }

      /*  Before starting a new communication transfer, you need to check the current   
          state of the peripheral; if it�s busy you need to wait for the end of current
          transfer before starting a new one.
          For simplicity reasons, this example is just waiting till the end of the 
          transfer, but application may perform other tasks while transfer operation
          is ongoing. */  
      while (HAL_I2C_GetState(&I2cHandle) != HAL_I2C_STATE_READY)
      {
      } 

      /* When Acknowledge failure occurs (Slave don't acknowledge it's address)
         Master restarts communication */
    }
    while(HAL_I2C_GetError(&I2cHandle) == HAL_I2C_ERROR_AF);

    /*##-3- Master sends number of data to be written ########################*/
    do
    {
      if(HAL_I2C_Master_Transmit_IT(&I2cHandle, (uint16_t)I2C_ADDRESS, (uint8_t*)&hTxNumData, 2)!= HAL_OK)
      {
        /* Error_Handler() function is called when error occurs. */
        Error_Handler();
      }

      /*  Before starting a new communication transfer, you need to check the current   
          state of the peripheral; if it�s busy you need to wait for the end of current
          transfer before starting a new one.
          For simplicity reasons, this example is just waiting till the end of the 
          transfer, but application may perform other tasks while transfer operation
          is ongoing. */  
      while (HAL_I2C_GetState(&I2cHandle) != HAL_I2C_STATE_READY)
      {
      } 

      /* When Acknowledge failure occurs (Slave don't acknowledge it's address)
         Master restarts communication */
    }
    while(HAL_I2C_GetError(&I2cHandle) == HAL_I2C_ERROR_AF);

    /*##-4- Master sends aTxBuffer to slave ##################################*/
    do
    {
      if(HAL_I2C_Master_Transmit_IT(&I2cHandle, (uint16_t)I2C_ADDRESS, (uint8_t*)aTxBuffer, TXBUFFERSIZE)!= HAL_OK)
      {
        /* Error_Handler() function is called when error occurs. */
        Error_Handler();
      }

      /*  Before starting a new communication transfer, you need to check the current   
          state of the peripheral; if it�s busy you need to wait for the end of current
          transfer before starting a new one.
          For simplicity reasons, this example is just waiting till the end of the 
          transfer, but application may perform other tasks while transfer operation
          is ongoing. */  
      while (HAL_I2C_GetState(&I2cHandle) != HAL_I2C_STATE_READY)
      {
      } 

      /* When Acknowledge failure occurs (Slave don't acknowledge it's address)
         Master restarts communication */
    }
    while(HAL_I2C_GetError(&I2cHandle) == HAL_I2C_ERROR_AF);

    /* Update bTransferRequest to send buffer read request for Slave */
    bTransferRequest = MASTER_REQ_READ;

    /*##-5- Master sends read request for slave ##############################*/
    do
    {
      if(HAL_I2C_Master_Transmit_IT(&I2cHandle, (uint16_t)I2C_ADDRESS, (uint8_t*)&bTransferRequest, 1)!= HAL_OK)
      {
        /* Error_Handler() function is called when error occurs. */
        Error_Handler();
      }

      /*  Before starting a new communication transfer, you need to check the current   
          state of the peripheral; if it�s busy you need to wait for the end of current
          transfer before starting a new one.
          For simplicity reasons, this example is just waiting till the end of the 
          transfer, but application may perform other tasks while transfer operation
          is ongoing. */  
      while (HAL_I2C_GetState(&I2cHandle) != HAL_I2C_STATE_READY)
      {
      } 

      /* When Acknowledge failure occurs (Slave don't acknowledge it's address)
         Master restarts communication */
    }
    while(HAL_I2C_GetError(&I2cHandle) == HAL_I2C_ERROR_AF);

    /*##-6- Master sends number of data to be read ###########################*/
    do
    {
      if(HAL_I2C_Master_Transmit_IT(&I2cHandle, (uint16_t)I2C_ADDRESS, (uint8_t*)&hRxNumData, 2)!= HAL_OK)
      {
        /* Error_Handler() function is called when error occurs. */
        Error_Handler();
      }

      /*  Before starting a new communication transfer, you need to check the current   
          state of the peripheral; if it�s busy you need to wait for the end of current
          transfer before starting a new one.
          For simplicity reasons, this example is just waiting till the end of the 
          transfer, but application may perform other tasks while transfer operation
          is ongoing. */  
      while (HAL_I2C_GetState(&I2cHandle) != HAL_I2C_STATE_READY)
      {
      } 

      /* When Acknowledge failure occurs (Slave don't acknowledge it's address)
         Master restarts communication */
    }
    while(HAL_I2C_GetError(&I2cHandle) == HAL_I2C_ERROR_AF);

    /*##-7- Master receives aRxBuffer from slave #############################*/
    do
    {
      if(HAL_I2C_Master_Receive_IT(&I2cHandle, (uint16_t)I2C_ADDRESS, (uint8_t*)aRxBuffer, RXBUFFERSIZE)!= HAL_OK)
      {
        /* Error_Handler() function is called when error occurs. */
        Error_Handler();
      }

      /*  Before starting a new communication transfer, you need to check the current   
          state of the peripheral; if it�s busy you need to wait for the end of current
          transfer before starting a new one.
          For simplicity reasons, this example is just waiting till the end of the 
          transfer, but application may perform other tasks while transfer operation
          is ongoing. */  
      while (HAL_I2C_GetState(&I2cHandle) != HAL_I2C_STATE_READY)
      {
      } 

      /* When Acknowledge failure occurs (Slave don't acknowledge it's address)
         Master restarts communication */
    }
    while(HAL_I2C_GetError(&I2cHandle) == HAL_I2C_ERROR_AF);

    /* Check correctness of received buffer ##################################*/
    if(Buffercmp((uint8_t*)aTxBuffer,(uint8_t*)aRxBuffer,hRxNumData))
    {
      /* Processing Error */
      Error_Handler();
    }

    /* Flush Rx buffers */
    Flush_Buffer((uint8_t*)aRxBuffer,RXBUFFERSIZE);

    /* Toggle LED_GREEN */
    BSP_LED_Toggle(LED_GREEN);

    /* This delay permits to see LED_GREEN toggling */
    HAL_Delay(25);
  }
#else
  while(1)
  {
    /* Initialize number of data variables */
    hTxNumData = 0;
    hRxNumData = 0;

    /*##-2- Slave receive request from master ################################*/
    while(HAL_I2C_Slave_Receive_IT(&I2cHandle, (uint8_t*)&bTransferRequest, 1)!= HAL_OK)
    {
    }

    /*  Before starting a new communication transfer, you need to check the current
    state of the peripheral; if it�s busy you need to wait for the end of current
    transfer before starting a new one.
    For simplicity reasons, this example is just waiting till the end of the
    transfer, but application may perform other tasks while transfer operation
    is ongoing. */
    while (HAL_I2C_GetState(&I2cHandle) != HAL_I2C_STATE_READY)
    {
    }

    /* If master request write operation #####################################*/
    if (bTransferRequest == MASTER_REQ_WRITE)
    {
      /*##-3- Slave receive number of data to be read ########################*/
      while(HAL_I2C_Slave_Receive_IT(&I2cHandle, (uint8_t*)&hRxNumData, 2)!= HAL_OK);

      /*  Before starting a new communication transfer, you need to check the current
      state of the peripheral; if it�s busy you need to wait for the end of current
      transfer before starting a new one.
      For simplicity reasons, this example is just waiting till the end of the
      transfer, but application may perform other tasks while transfer operation
      is ongoing. */
      while (HAL_I2C_GetState(&I2cHandle) != HAL_I2C_STATE_READY)
      {
      }

      /*##-4- Slave receives aRxBuffer from master ###########################*/
      while(HAL_I2C_Slave_Receive_IT(&I2cHandle, (uint8_t*)aRxBuffer, hRxNumData)!= HAL_OK);

      /*  Before starting a new communication transfer, you need to check the current
      state of the peripheral; if it�s busy you need to wait for the end of current
      transfer before starting a new one.
      For simplicity reasons, this example is just waiting till the end of the
      transfer, but application may perform other tasks while transfer operation
      is ongoing. */
      while (HAL_I2C_GetState(&I2cHandle) != HAL_I2C_STATE_READY)
      {
      }

      /* Check correctness of received buffer ################################*/
      if(Buffercmp((uint8_t*)aTxBuffer,(uint8_t*)aRxBuffer,hRxNumData))
      {
        /* Processing Error */
        Error_Handler();
      }

      /* Flush Rx buffers */
      Flush_Buffer((uint8_t*)aRxBuffer,RXBUFFERSIZE);

      /* Toggle LED_GREEN */
      BSP_LED_Toggle(LED_GREEN);
    }
    /* If master request write operation #####################################*/
    else
    {
      /*##-3- Slave receive number of data to be written #####################*/
      while(HAL_I2C_Slave_Receive_IT(&I2cHandle, (uint8_t*)&hTxNumData, 2)!= HAL_OK);

      /*  Before starting a new communication transfer, you need to check the current
      state of the peripheral; if it�s busy you need to wait for the end of current
      transfer before starting a new one.
      For simplicity reasons, this example is just waiting till the end of the
      transfer, but application may perform other tasks while transfer operation
      is ongoing. */
      while (HAL_I2C_GetState(&I2cHandle) != HAL_I2C_STATE_READY)
      {
      }

      /*##-4- Slave transmit aTxBuffer to master #############################*/
      while(HAL_I2C_Slave_Transmit_IT(&I2cHandle, (uint8_t*)aTxBuffer, RXBUFFERSIZE)!= HAL_OK);

      /*  Before starting a new communication transfer, you need to check the current
      state of the peripheral; if it�s busy you need to wait for the end of current
      transfer before starting a new one.
      For simplicity reasons, this example is just waiting till the end of the
      transfer, but application may perform other tasks while transfer operation
      is ongoing. */
      while (HAL_I2C_GetState(&I2cHandle) != HAL_I2C_STATE_READY)
      {
      }
    }
  }
#endif /* MASTER_BOARD */
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
  /** Error_Handler() function is called when error occurs.
    * 1- When Slave don't acknowledge it's address, Master restarts communication.
    * 2- When Master don't acknowledge the last data transferred, Slave don't care in this example.
    */
  if (HAL_I2C_GetError(I2cHandle) != HAL_I2C_ERROR_AF)
  {
    /* Turn Off LED_GREEN */
    BSP_LED_Off(LED_GREEN);

    /* Turn On LED_RED */
    BSP_LED_On(LED_RED);
  }
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
static void Error_Handler(void)
{
	/* Turn LED_GREEN off */
	BSP_LED_Off(LED_GREEN);
	/* Turn LED_RED on */
	BSP_LED_On(LED_RED);
  while(1)
  {
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
