/**
  ******************************************************************************
  * @file    UART/UART_TwoBoards_ComIT/Src/main.c 
  * @author  MCD Application Team
  * @brief   This sample code shows how to use UART HAL API to transmit
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

/** @addtogroup UART_TwoBoards_ComIT
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define TRANSMITTER_BOARD

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* UART handler declaration */
UART_HandleTypeDef UartHandle;
__IO ITStatus UartReady = RESET;
__IO uint32_t VirtualUserButtonStatus = 0;  /* set to 1 after User set a button  */

/* Buffer used for transmission */
uint8_t aTxBuffer[] = " ****UART_TwoBoards_ComIT****  ****UART_TwoBoards_ComIT****  ****UART_TwoBoards_ComIT**** ";

/* Buffer used for reception */
uint8_t aRxBuffer[RXBUFFERSIZE];

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void Error_Handler(void);
static uint16_t Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
#ifdef TRANSMITTER_BOARD
  GPIO_InitTypeDef  GPIO_InitStruct;
#endif
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
  
  /* Configure LED3 */
  BSP_LED_Init(LED3);

  /*##-1- Configure the UART peripheral ######################################*/
  /* Put the USART peripheral in the Asynchronous mode (UART Mode) */
  /* UART configured as follows:
      - Word Length = 8 Bits
      - Stop Bit = One Stop bit
      - Parity = None
      - BaudRate = 9600 baud
      - Hardware flow control disabled (RTS and CTS signals) */
  UartHandle.Instance        = USARTx;

  UartHandle.Init.BaudRate   = 9600;
  UartHandle.Init.WordLength = UART_WORDLENGTH_8B;
  UartHandle.Init.StopBits   = UART_STOPBITS_1;
  UartHandle.Init.Parity     = UART_PARITY_NONE;
  UartHandle.Init.HwFlowCtl  = UART_HWCONTROL_NONE;
  UartHandle.Init.Mode       = UART_MODE_TX_RX;
  UartHandle.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT; 
  if(HAL_UART_DeInit(&UartHandle) != HAL_OK)
  {
    Error_Handler();
  }  
  if(HAL_UART_Init(&UartHandle) != HAL_OK)
  {
    Error_Handler();
  }
  
#ifdef TRANSMITTER_BOARD

  /* Configure PA.12 (Arduino D2) as input with External interrupt */
  GPIO_InitStruct.Pin = GPIO_PIN_12;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;

  /* Enable GPIOA clock */
  __HAL_RCC_GPIOA_CLK_ENABLE();

  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* Enable and set PA.12 (Arduino D2) EXTI Interrupt to the lowest priority */
  NVIC_SetPriority((IRQn_Type)(EXTI4_15_IRQn), 0x03);
  HAL_NVIC_EnableIRQ((IRQn_Type)(EXTI4_15_IRQn));
  /* Wait for the user to set GPIOA to GND before starting the Communication.
     In the meantime, LED3 is blinking */
  while(VirtualUserButtonStatus == 0)
  {
      /* Toggle LED3*/
      BSP_LED_Toggle(LED3);
      HAL_Delay(100);
  }

  BSP_LED_Off(LED3);
  
  /* The board sends the message and expects to receive it back */
  
  /*##-2- Start the transmission process #####################################*/  
  /* While the UART in reception process, user can transmit data through 
     "aTxBuffer" buffer */
  if(HAL_UART_Transmit_IT(&UartHandle, (uint8_t*)aTxBuffer, TXBUFFERSIZE)!= HAL_OK)
  {
    Error_Handler();
  }
  
  /*##-3- Wait for the end of the transfer ###################################*/   
  while (UartReady != SET)
  {
  }
  
  /* Reset transmission flag */
  UartReady = RESET;
  
  /*##-4- Put UART peripheral in reception process ###########################*/  
  if(HAL_UART_Receive_IT(&UartHandle, (uint8_t *)aRxBuffer, RXBUFFERSIZE) != HAL_OK)
  {
    Error_Handler();
  }

#else
  
  /* The board receives the message and sends it back */

  /*##-2- Put UART peripheral in reception process ###########################*/  
  if(HAL_UART_Receive_IT(&UartHandle, (uint8_t *)aRxBuffer, RXBUFFERSIZE) != HAL_OK)
  {
    Error_Handler();
  }
  
  /*##-3- Wait for the end of the transfer ###################################*/   
  /* While waiting for message to come from the other board, LED3 is
     blinking according to the following pattern: a double flash every half-second */  
  while (UartReady != SET)
  {
      BSP_LED_On(LED3); 
      HAL_Delay(100);
      BSP_LED_Off(LED3); 
      HAL_Delay(100);
      BSP_LED_On(LED3); 
      HAL_Delay(100);
      BSP_LED_Off(LED3); 
      HAL_Delay(500); 
  } 
  
  /* Reset transmission flag */
  UartReady = RESET;
  BSP_LED_Off(LED3); 
  
  /*##-4- Start the transmission process #####################################*/  
  /* While the UART in reception process, user can transmit data through 
     "aTxBuffer" buffer */
  if(HAL_UART_Transmit_IT(&UartHandle, (uint8_t*)aTxBuffer, TXBUFFERSIZE)!= HAL_OK)
  {
    Error_Handler();
  }
  
#endif /* TRANSMITTER_BOARD */
  
  /*##-5- Wait for the end of the transfer ###################################*/   
  while (UartReady != SET)
  {
  } 
  
  /* Reset transmission flag */
  UartReady = RESET;

  /*##-6- Compare the sent and received buffers ##############################*/
  if(Buffercmp((uint8_t*)aTxBuffer,(uint8_t*)aRxBuffer,RXBUFFERSIZE))
  {
    Error_Handler();
  }
  
  /* Turn on LED3 if test passes then enter infinite loop */
  BSP_LED_On(LED3); 
  /* Infinite loop */
  while (1)
  {
  }
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
  *            System Clock source            = PLL (HSI/2)
  *            SYSCLK(Hz)                     = 48000000
  *            HCLK(Hz)                       = 48000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 1
  *            HSI Frequency(Hz)              = 8000000
  *            PREDIV                         = 1
  *            PLLMUL                         = 12
  *            Flash Latency(WS)              = 1
  * @param  None
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;
  
  /* No HSE Oscillator on Nucleo, Activate PLL with HSI/2 as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_NONE;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV1;
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
  * @brief  Tx Transfer completed callback
  * @param  UartHandle: UART handle. 
  * @note   This example shows a simple way to report end of IT Tx transfer, and 
  *         you can add your own implementation. 
  * @retval None
  */
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *UartHandle)
{
  /* Set transmission flag: transfer complete */
  UartReady = SET;

  
}

/**
  * @brief  Rx Transfer completed callback
  * @param  UartHandle: UART handle
  * @note   This example shows a simple way to report end of DMA Rx transfer, and 
  *         you can add your own implementation.
  * @retval None
  */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *UartHandle)
{
  /* Set transmission flag: transfer complete */
  UartReady = SET;
  
  
}

/**
  * @brief  UART error callbacks
  * @param  UartHandle: UART handle
  * @note   This example shows a simple way to report transfer error, and you can
  *         add your own implementation.
  * @retval None
  */
void HAL_UART_ErrorCallback(UART_HandleTypeDef *UartHandle)
{
    Error_Handler();
}


/**
  * @brief EXTI line detection callbacks
  * @param GPIO_Pin: Specifies the pins connected EXTI line
  * @retval None
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if(GPIO_Pin == GPIO_PIN_12)
  {
    VirtualUserButtonStatus = 1;
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
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
static void Error_Handler(void)
{
  /* Turn LED3 on */
  BSP_LED_On(LED3);
  while(1)
  {
    /* Error if LED3 is slowly blinking (1 sec. period) */
    BSP_LED_Toggle(LED3); 
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
