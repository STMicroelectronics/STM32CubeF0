/**
  ******************************************************************************
  * @file    UART/UART_HyperTerminal_IT/Src/main.c
  * @author  MCD Application Team
  * @brief   This sample code shows how to use UART HAL and LL APIs to transmit
  *          and receive a data buffer with a communication process based on IT;
  *          The communication is done with the Hyperterminal PC application;
  *          HAL driver is used to perform UART configuration, 
  *          then TX/RX transfers procedures are based on LL APIs use
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

/** @addtogroup STM32F0xx_HAL_LL_MIX_Examples
  * @{
  */

/** @addtogroup UART_Hyperterminal_IT
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* UART handler declaration */
UART_HandleTypeDef UartHandle;
__IO uint8_t  ubTxComplete = 0;
__IO uint8_t  ubRxComplete = 0;

/* Buffer used for transmission */
uint8_t aTxStartMessage[] = "\n\r ****UART-Hyperterminal communication based on IT (Mixed HAL/LL usage) ****\n\r Enter 10 characters using keyboard :\n\r";
__IO uint32_t uwTxIndex = 0;
uint8_t ubSizeToSend = sizeof(aTxStartMessage);
uint8_t aTxEndMessage[] = "\n\r Example Finished\n\r";

/* Buffer used for reception */
uint8_t aRxBuffer[RXBUFFERSIZE];
__IO uint32_t uwRxIndex = 0;

/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
static void Error_Handler(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
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
  
  /* Configure leds */
  BSP_LED_Init(LED2);
  
  /*##-1- Configure the UART peripheral using HAL services ###################*/
  /* Put the USART peripheral in the Asynchronous mode (UART Mode) */
  /* UART configured as follows:
      - Word Length = 8 Bits (7 data bit + 1 parity bit) : 
	                  BE CAREFUL : Program 7 data bits + 1 parity bit in PC HyperTerminal
      - Stop Bit    = One Stop bit
      - Parity      = ODD parity
      - BaudRate    = 9600 baud
      - Hardware flow control disabled (RTS and CTS signals) */
  UartHandle.Instance        = USARTx;

  UartHandle.Init.BaudRate   = 9600;
  UartHandle.Init.WordLength = UART_WORDLENGTH_8B;
  UartHandle.Init.StopBits   = UART_STOPBITS_1;
  UartHandle.Init.Parity     = UART_PARITY_ODD;
  UartHandle.Init.HwFlowCtl  = UART_HWCONTROL_NONE;
  UartHandle.Init.Mode       = UART_MODE_TX_RX;

  if(HAL_UART_Init(&UartHandle) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }

  /*##-2- Configure UART peripheral for reception process (using LL) ##########*/  
  /* Any data received will be stored "aRxBuffer" buffer : the number max of 
     data received is RXBUFFERSIZE */
  /* Enable RXNE and Error interrupts */  
  LL_USART_EnableIT_RXNE(USARTx);
  LL_USART_EnableIT_ERROR(USARTx);

  /*##-3- Start the transmission process (using LL) *##########################*/  
  /* While the UART in reception process, user can transmit data from 
     "aTxStartMessage" buffer */
  /* Start USART transmission : Will initiate TXE interrupt after TDR register is empty */
  LL_USART_TransmitData8(USARTx, aTxStartMessage[uwTxIndex++]); 

  /* Enable TXE interrupt */
  LL_USART_EnableIT_TXE(USARTx); 

  /*##-4- Wait for the end of the transfer ###################################*/
  /* USART IRQ handler is not anymore routed to HAL_UART_IRQHandler() function 
     and is now based on LL API functions use. 
     Therefore, use of HAL IT based services is no more possible. */
  /*  Once TX transfer is completed, LED2 will toggle.
      Then, when RX transfer is completed, LED2 will turn On. */
  while (ubTxComplete == 0)
  {
  }

  while (ubRxComplete == 0)
  {
    BSP_LED_Toggle(LED2);
    HAL_Delay(250);
  }

  BSP_LED_On(LED2); /* stop blink and keeps ON */

  /*##-5- Send the received Buffer ###########################################*/
  /* Even if use of HAL IT based services is no more possible, use of HAL Polling based services
     (as Transmit in polling mode) is still possible. */
  if(HAL_UART_Transmit(&UartHandle, (uint8_t*)aRxBuffer, RXBUFFERSIZE, 1000)!= HAL_OK)
  {
    /* Transfer error in transmission process */
    Error_Handler();
  }
  
  /*##-6- Send the End Message ###############################################*/  
  if(HAL_UART_Transmit(&UartHandle, (uint8_t*)aTxEndMessage, TXENDMESSAGESIZE, 1000)!= HAL_OK)
  {
    /* Transfer error in transmission process */
    Error_Handler();
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
  * @retval None
  */
static void Error_Handler(void)
{
  /* Turn LED2 to off for error */
  BSP_LED_Off(LED2); 
  while(1)
  {
  }
}

/**
  * @brief  Rx Transfer completed callback
  * @note   This example shows a simple way to report end of IT Rx transfer, and 
  *         you can add your own implementation.
  * @retval None
  */
void UART_CharReception_Callback(void)
{
  /* Read Received character. RXNE flag is cleared by reading of RDR register */
  aRxBuffer[uwRxIndex++] = LL_USART_ReceiveData8(USARTx);

  /* Check if reception is completed (expected nb of bytes has been received) */
  if (uwRxIndex == RXBUFFERSIZE)
  {
    /* Set Reception complete boolean to 1 */
    ubRxComplete = 1;
  }
}

/**
  * @brief  Function called for achieving next TX Byte sending
  * @retval None
  */
void UART_TXEmpty_Callback(void)
{
  if(uwTxIndex == (ubSizeToSend - 1))
  {
    /* Disable TXE interrupt */
    LL_USART_DisableIT_TXE(USARTx);
    
    /* Enable TC interrupt */
    LL_USART_EnableIT_TC(USARTx);
  }

  /* Fill TDR with a new char */
  LL_USART_TransmitData8(USARTx, aTxStartMessage[uwTxIndex++]);
}

/**
  * @brief  Function called at completion of last byte transmission
  * @retval None
  */
void UART_CharTransmitComplete_Callback(void)
{
  if(uwTxIndex == sizeof(aTxStartMessage))
  {
    uwTxIndex = 0;
    
    /* Disable TC interrupt */
    LL_USART_DisableIT_TC(USARTx);
    
    /* Set Tx complete boolean to 1 */
    ubTxComplete = 1;
  }
}

/**
  * @brief  UART error callbacks
  * @note   This example shows a simple way to report transfer error, and you can
  *         add your own implementation.
  * @retval None
  */
void UART_Error_Callback(void)
{
  __IO uint32_t isr_reg;

  /* Disable USARTx_IRQn */
  NVIC_DisableIRQ(USARTx_IRQn);
  
  /* Error handling example :
    - Read USART ISR register to identify flag that leads to IT raising
    - Perform corresponding error handling treatment according to flag
  */
  isr_reg = LL_USART_ReadReg(USARTx, ISR);
  if (isr_reg & LL_USART_ISR_NE)
  {
    /* Turn LED2 off: Transfer error in reception/transmission process */
    BSP_LED_Off(LED2);
  }
  else
  {
    /* Turn LED2 off: Transfer error in reception/transmission process */
    BSP_LED_Off(LED2);
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
