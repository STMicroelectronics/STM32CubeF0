/**
  ******************************************************************************
  * @file    UART/UART_OneBoards_8UART/Src/main.c
  * @author  MCD Application Team
  * @brief   This sample code shows how to use UART HAL API to transmit
  *          and receive a data buffer with a communication process based on
  *          IT transfer. 
  *          The communication is done using 2 Boards.
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

#define  __MAIN_C__    
    
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stdio.h"    
#include "string.h"    

/** @addtogroup STM32F0xx_HAL_Examples
  * @{
  */

/** @addtogroup UART_TwoBoards_ComIT
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
typedef struct 
{
  uint8_t rx_counter;
  uint8_t tx_counter;
}DEMO_UART_TransfertSate;

/* Private define ------------------------------------------------------------*/
#define USARTx(index) USART##index

enum {
      FALSE,
      TRUE = !FALSE
};
    
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* UART handler declaration */
__IO uint32_t uart_tx_transfert = 0;
/* flag used to know if the transfer is complete */
uint32_t TransferComplete = FALSE;
/* Buffer used for transmission */
uint8_t aTxBuffer[BUFFER_SIZE] = "UART1->UART2->UART3->UART4->UART5->UART6->UART7->UART8->UART1->UART2->UART3->UART4->UART5->UART6->UART7->UART8->UART1->UART2->UART3->UART4->UART5->UART6->UART7->UART8->UART1->UART";
/* Buffer used for reception */
uint8_t aRxBuffer[USART__INDEX_MAX][BUFFER_SIZE];
/* counter for transfert loop */
uint32_t count = 0;
/* Rx Tx transfer status for all the UART */
DEMO_UART_TransfertSate UartTransfertState[USART__INDEX_MAX];

/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
static void Error_Handler(void);
static uint16_t Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength);
static uint8_t GetUartIndex(UART_HandleTypeDef *UartHandle);
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  uint8_t index;
  uint8_t *ptr;
  uint32_t retr;

  
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
  
  /* Configure LED3, LED4, LED5 and LED6 */
  BSP_LED_Init(LED1);
  BSP_LED_Init(LED2);
  BSP_LED_Init(LED3);
  BSP_LED_Init(LED4);
  
  /* Configure the system clock to 48 Mhz */
  SystemClock_Config();
  
  /* Set the uart instance */
  UartHandle[USART1_INDEX].Instance        = USARTx(1);
  UartHandle[USART2_INDEX].Instance        = USARTx(2);
  UartHandle[USART3_INDEX].Instance        = USARTx(3);
  UartHandle[USART4_INDEX].Instance        = USARTx(4);
  UartHandle[USART5_INDEX].Instance        = USARTx(5);
  UartHandle[USART6_INDEX].Instance        = USARTx(6);
  UartHandle[USART7_INDEX].Instance        = USARTx(7);
  UartHandle[USART8_INDEX].Instance        = USARTx(8);
  
  for (index = 0; index < USART__INDEX_MAX; index++)
  {
    /*##-1- Configure the UARTs peripheral ######################################*/
    /* Put the USART peripheral in the Asynchronous mode (UART Mode) */
    /* UART configured as follows:
    - Word Length = 8 Bits
    - Stop Bit = One Stop bit
    - Parity = None
    - BaudRate = 9600 baud
    - Hardware flow control disabled (RTS and CTS signals) */
    UartHandle[index].Init.BaudRate   = 9600;
    UartHandle[index].Init.WordLength = UART_WORDLENGTH_8B;
    UartHandle[index].Init.StopBits   = UART_STOPBITS_1;
    UartHandle[index].Init.Parity     = UART_PARITY_NONE;
    UartHandle[index].Init.HwFlowCtl  = UART_HWCONTROL_NONE;
    UartHandle[index].Init.Mode       = UART_MODE_TX_RX;
    UartHandle[index].AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT; 
    if(HAL_UART_DeInit(&UartHandle[index]) != HAL_OK)
    {
      Error_Handler();
    }  
    if(HAL_UART_Init(&UartHandle[index]) != HAL_OK)
    {
      Error_Handler();
    }
  }
  
  do {
    
    /* counter for the number of transfert complete */
    count++;
    
    /* initialize the global variable to handle the data transfert */
    TransferComplete = FALSE;
    uart_tx_transfert = 0;
    memset(aRxBuffer, 0x0, BUFFER_SIZE * USART__INDEX_MAX);
    memset(UartTransfertState, 0x00, sizeof(DEMO_UART_TransfertSate)*USART__INDEX_MAX);
    
    /* Prepare all uart to receive a data packet */
    for (index = 0; index < USART__INDEX_MAX; index++)
    {  
      if(HAL_UART_Receive_IT(&UartHandle[index], (uint8_t*)aRxBuffer[index], PACKET_SIZE)!= HAL_OK)
      {
        Error_Handler();
      }
    }
    
    /* Send the first packet from UART1 to initiate the loop transfer */
    if(HAL_UART_Transmit(&UartHandle[USART1_INDEX], (uint8_t*)aTxBuffer, PACKET_SIZE, 2000)!= HAL_OK)
    {
      Error_Handler();
    }
    UartTransfertState[USART1_INDEX].tx_counter++;
    
    /* Loop tranfert handling, when a data has been transfert start the next transfert */ 
    do 
    {
      /* Delay can be added for the demo : to let time to see counter incrementation on the live watch */
      /*HAL_Delay(200);  */

      /* check uart_tx_transfert bit field to know wich tranfert must be restart */
      for(index = 0; index < USART__INDEX_MAX; index++)
      {
        if((uart_tx_transfert & (1 << index)) != 0 )
        {
            /* Protection done to avoid conflict for the variable update */
            HAL_NVIC_DisableIRQ(USART3_8_IRQn);
            HAL_NVIC_DisableIRQ(USART2_IRQn);
            HAL_NVIC_DisableIRQ(USART1_IRQn);
            uart_tx_transfert &= ~(1 << index);
            HAL_NVIC_EnableIRQ(USART1_IRQn);
            HAL_NVIC_EnableIRQ(USART2_IRQn);
            HAL_NVIC_EnableIRQ(USART3_8_IRQn);    
            
            /* Control if we already send all the data packet */
            if (UartTransfertState[index].tx_counter < NB_PACKET)
            {
              /* Set the data ptr to use for the transfer */
              if ( index == 0)
              {
                ptr = aTxBuffer;
              }
              else
              {
                ptr = aRxBuffer[index];
              } 
              
              retr = HAL_UART_Transmit(&UartHandle[index], (uint8_t*)(ptr+UartTransfertState[index].tx_counter*PACKET_SIZE), PACKET_SIZE, 2000);
              if(  retr != HAL_OK)
              {
                Error_Handler();
              }
              UartTransfertState[index].tx_counter++;
            }
        }
      }
      
      BSP_LED_Toggle(LED1);
      BSP_LED_Toggle(LED4);
      if (UartTransfertState[USART1_INDEX].rx_counter == NB_PACKET)
      {
        /* Transfer status complete because the last packet has been received on USART1 */
        TransferComplete = TRUE;
      }
    } while(TransferComplete == FALSE);
    
    /* Compare the sent buffer sent on UART1 with the buffer receive on UART1 */
    if(Buffercmp((uint8_t*)aTxBuffer,(uint8_t*)aRxBuffer[USART1_INDEX],BUFFER_SIZE))
    {
      Error_Handler();
    }
  } while (1);
}

/**
  * @brief  Tx Transfer completed callback
  * @param  UartHandle: UART handle. 
  * @note   This callback is called by uart driver to forward application 
            the Tx transfert complete information. 
  * @retval None
  */
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *UartHandle)
{
  /* Never called because all tx are done in polling mode */ 
  Error_Handler();
}

/**
  * @brief  Rx Transfer completed callback
  * @param  UartHandle: UART handle
  * @note   This callback is called by uart driver to forward application 
            the Rx transfert complete information. 
  * @retval None
  */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *UartHandle)
{
  uint8_t index;

  index = GetUartIndex(UartHandle);
  
  /* This uart is now avaible to send data, set the bit field to allow the main function to perform data transfer */
  uart_tx_transfert|= (1 << index);
  /* Increment the reception counter */
  UartTransfertState[index].rx_counter++;
  /* Check if all packet are already received */
  if (UartTransfertState[index].rx_counter < NB_PACKET)
  {
    /* else prepare the next packet reception */
    if(HAL_UART_Receive_IT(UartHandle, (uint8_t*)(aRxBuffer[index]+UartTransfertState[index].rx_counter*PACKET_SIZE), PACKET_SIZE) != HAL_OK)
    { 
      Error_Handler();
    }
  }
}

/**
  * @brief  Get Uart index in the table Rx Transfer completed callback
  * @param  UartHandle: UART handle
  * @note   This example shows a simple way to report end of DMA Rx transfer, and 
  *         you can add your own implementation.
  * @retval None
  */
uint8_t GetUartIndex(UART_HandleTypeDef *UartHandle)
{
uint8_t index = USART1_INDEX;

  switch((uint32_t)UartHandle->Instance)
  {
  case (uint32_t)USART1 :
    index = USART1_INDEX;
    break;
  case (uint32_t)USART2 :
    index = USART2_INDEX;
    break;
  case (uint32_t)USART3 :
    index = USART3_INDEX;
    break;
  case (uint32_t)USART4 :
    index = USART4_INDEX;
    break;
  case (uint32_t)USART5 :
    index = USART5_INDEX;
    break;
  case (uint32_t)USART6 :
    index = USART6_INDEX;
    break;
  case (uint32_t)USART7 :
    index = USART7_INDEX;
    break;
  case (uint32_t)USART8 :
    index = USART8_INDEX;
    break;
  }
  return index;
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
   /* Transfer error in reception/transmission process, call the Error_Handler*/
   Error_Handler();
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
  while(1)
  {
    BSP_LED_Toggle(LED1);
    BSP_LED_Toggle(LED2);
    BSP_LED_Toggle(LED3);
    BSP_LED_Toggle(LED4);
    HAL_Delay(100);
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
