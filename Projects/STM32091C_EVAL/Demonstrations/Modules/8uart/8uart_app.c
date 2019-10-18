/**
  ******************************************************************************
  * @file    8uart_app.c
  * @author  MCD Application Team
  * @brief   System information functions
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2016 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */

#define __8UART_APP_C

/* Includes ------------------------------------------------------------------*/
#include "stdlib.h"
#include "string.h"
#include "stdio.h"
#include "k_config.h"
#include "k_module.h"
#include "k_menu.h"
#include "k_tools.h"
#include "k_window.h"
#include "8uart_app.h"

/** @addtogroup 8UART_MODULE
  * @{
  */

/** @defgroup SYSTEM_INFO
  * @brief system info routines 
  * @{
  */

/* Private typedef ----------------------------------------------------------*/    
/* structure to handle the tranfert status */
typedef struct 
{
  uint8_t rx_counter;
  uint8_t tx_counter;
}DEMO_UART_TransfertSate;

/* Private constants ----------------------------------------------------------*/
enum
{
  __ENABLE,
  __DISABLE,
}; 

/* Define to size the Data transfert */
#define PACKET_SIZE   18
#define BUFFER_SIZE   180
#define NB_PACKET     (BUFFER_SIZE / PACKET_SIZE)


/* Private function prototypes -----------------------------------------------*/
KMODULE_RETURN _HuitUartExec(void);
KMODULE_RETURN _HuitUartConfig(void);
KMODULE_RETURN _HuitUartUnConfig(void);

void HuitUartDemo(void);
void HuitUartUserAction(uint8_t sel);
void HuitUartUpdateRxTxInformation(void);
void HuitUartUpdateLoopInformation(uint16_t loop);
static void IRQ_Managment(int32_t mode, int32_t IrqName);
static void HuitUartUpdateUserHeader(void);
static uint8_t GetUartIndex(UART_HandleTypeDef *UartHandle);

/* Private Variable Menu -----------------------------------------------------*/
const tMenuItem HuitUartMenuItems[] =
{
    {"Press down to exit", 0, 0, TYPE_EXEC, MODULE_NONE, HuitUartDemo, HuitUartUserAction, NULL, NULL },
};

const tMenu HuitUartMenu = {
  "8Uarts application", HuitUartMenuItems, countof(HuitUartMenuItems), TYPE_EXEC, 1, 1 };

/* Private Variable ----------------------------------------------------------*/
/* tx status used to start a tx transaction */
static __IO uint32_t uart_tx_transfert = 0;
/* variable to check if allpacket are sent */
static uint32_t TransferComplete = 0; 
/* used to check if user want stop the application */
static uint8_t user_stop = 0;

/* Rx Tx transfer status for all the UART */
static DEMO_UART_TransfertSate UartTransfertState[USART__INDEX_MAX];

/* Buffer used for transmission */
const uint8_t aTxBuffer[BUFFER_SIZE] = "UART1->UART2->UART3->UART4->UART5->UART6->UART7->UART8->UART1->UART2->UART3->UART4->UART5->UART6->UART7->UART8->UART1->UART2->UART3->UART4->UART5->UART6->UART7->UART8->UART1->UART2";
/* Buffer used for reception */
uint8_t *aRxBuffer[USART__INDEX_MAX];

/* Private typedef -----------------------------------------------------------*/
/* External variables --------------------------------------------------------*/
const K_ModuleItem_Typedef Module8Uart =
{
  MODULE_8UART,
  _HuitUartConfig,
  _HuitUartExec,
  _HuitUartUnConfig,
  NULL
};  

/**
  * @brief  setup the HW/Memory for the 8 uart application 
  * @param  None.
  * @note   set the memeory + Hw initialisation.  
  * @retval None.
  */
KMODULE_RETURN _HuitUartConfig(void)
{
  uint8_t index;
  
  /* Memory allocation for the example */
  for(index = 0; index < USART__INDEX_MAX; index++)
  {
      aRxBuffer[index] = malloc(sizeof(uint8_t)*BUFFER_SIZE);
      if(aRxBuffer[index] == NULL)
      {
        index--;
        while(index!=0)
        {
          free(aRxBuffer[index]);
          index--;
        }
        return KMODULE_ERROR_PRE;
      }
  }
  
  /* Allocation for the UART handle */  
  UartHandle = malloc(sizeof(UART_HandleTypeDef)*USART__INDEX_MAX);
  if(UartHandle == NULL)
  {
    return KMODULE_ERROR_PRE;
  }
  /* reset all the handle */
  memset(UartHandle, 0, sizeof(UART_HandleTypeDef)*USART__INDEX_MAX);
  
  /* Hw initialization */
  /* Set the uart instance */
  UartHandle[USART1_INDEX].Instance        = USART1;
  UartHandle[USART2_INDEX].Instance        = USART2;
  UartHandle[USART3_INDEX].Instance        = USART3;
  UartHandle[USART4_INDEX].Instance        = USART4;
  UartHandle[USART5_INDEX].Instance        = USART5;
  UartHandle[USART6_INDEX].Instance        = USART6;
  UartHandle[USART7_INDEX].Instance        = USART7;
  UartHandle[USART8_INDEX].Instance        = USART8;
  
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
       kWindow_Error("8Uart Application\nUart Deinit Error\n");
       while(1);
    }  
    if(HAL_UART_Init(&UartHandle[index]) != HAL_OK)
    {
       kWindow_Error("8Uart Application\nUart Init Error\n");
       while(1);
    }
  }
  return KMODULE_OK;
}

/**
  * @brief  unsetup the HW for the 8 uart application 
  * @param  None.
  * @note   reset the memeory + Hw initialisation.  
  * @retval None.
  */
KMODULE_RETURN _HuitUartUnConfig(void)
{
  uint8_t index;
  
  /* free the memory allocated */
  /* Memory allocation for the example */
  for(index = 0; index < USART__INDEX_MAX; index++)
  {
      free(aRxBuffer[index]);
  }  
  
  /* Hw uninitialization */
  for (index = 0; index < USART__INDEX_MAX; index++)
  {
    if(HAL_UART_DeInit(&UartHandle[index]) != HAL_OK)
    {
       kWindow_Error("8Uart Application\nUart Deinit Error\n");
       while(1);
    }  
  }
  
  /* free UartHandle memory */
  free(UartHandle);
  
  return KMODULE_OK;
}

/**
  * @brief  Run the 8 uart application 
  * @param  None.
  * @note   run and display information about the uart transaction.  
  * @retval None.
  */
KMODULE_RETURN _HuitUartExec(void)
{
  /* Prepare the main MMI */
  BSP_LCD_Clear(LCD_COLOR_WHITE);
  kMenu_Execute(HuitUartMenu);
  
  /* Execute the app 8uart */
  /* App initialization    */
  return KMODULE_OK;
}


/**
  * @brief  main function of the 8 uart application 
  * @param  None.
  * @note   loop on the 8uart circular transfer
  * @retval None.
  */
void HuitUartDemo(void)
{
  uint8_t index, *ptr;
  uint16_t loop=0;
  
  /* Set stop OFF before to start a new test */
  user_stop = 0; 
  
  /* Display user information */
  HuitUartUpdateUserHeader();
  
  /* Test Loop */
  do 
  {  
    /* initialize the global variable to handle the data transfert */
    TransferComplete = 0;
    uart_tx_transfert = 0;
    for(index =0; index <USART__INDEX_MAX; index++)
    {
      memset(aRxBuffer[index], 0x0, BUFFER_SIZE);
    }
    memset(UartTransfertState, 0x00, sizeof(DEMO_UART_TransfertSate)*USART__INDEX_MAX);
    
    /* Initialise the user interface */
    HuitUartUpdateLoopInformation(loop);
    HuitUartUpdateRxTxInformation();

    /* Prepare all uart to receive a data packet */
    for (index = 0; index < USART__INDEX_MAX; index++)
    {  
      if(HAL_UART_Receive_IT(&UartHandle[index], (uint8_t*)aRxBuffer[index], PACKET_SIZE)!= HAL_OK)
      {
         kWindow_Error("8Uart Application\nReception Error\n");
         while(1);
      }
    }
        
    /* Send the first packet from UART1 to initiate the loop transfer */
    if(HAL_UART_Transmit_IT(&UartHandle[USART1_INDEX], (uint8_t*)aTxBuffer, PACKET_SIZE)!= HAL_OK)
    {
      kWindow_Error("8Uart Application\nTransmitt Error\n");
      while(1);
    }
    
    /* Loop tranfert handling, when a data has been transfert start the next transfert */ 
    do 
    {
      /* Handle the user ky press to exit application */
      if (user_stop == 1 )
      {
        return;
      }
     
      /* check uart_tx_transfert bit field to know wich tranfert must be restart */
      for(index = 0; index < USART__INDEX_MAX; index++)
      {  
        HAL_NVIC_DisableIRQ(USART3_8_IRQn);
        HAL_NVIC_DisableIRQ(USART2_IRQn);
        HAL_NVIC_DisableIRQ(USART1_IRQn);
        if((uart_tx_transfert & (1 << index)) != 0 )
        {
          /* Protection done to avoid conflict the variable update */
         
          /* reset the status bit */
          uart_tx_transfert &= ~(1 << index);
          
          HAL_NVIC_EnableIRQ(USART1_IRQn);
          HAL_NVIC_EnableIRQ(USART2_IRQn);
          HAL_NVIC_EnableIRQ(USART3_8_IRQn);
        
          HuitUartUpdateRxTxInformation();
          /* Control if we already send all the data packet */
          if (UartTransfertState[index].tx_counter < NB_PACKET)
          {

            /* Set the data ptr to use for the transfer */
            if ( index == 0)
            {
              ptr = (uint8_t *)aTxBuffer;
            }
            else
            {
              ptr = aRxBuffer[index];
            }
            
            if(  HAL_UART_Transmit_IT(&UartHandle[index], (uint8_t*)(ptr+UartTransfertState[index].tx_counter*PACKET_SIZE), PACKET_SIZE) != HAL_OK)
            {
              kWindow_Error("8Uart Application\nTransmission Error\n");
              while(1);
            }
          }
        }
        else
        {
          HAL_NVIC_EnableIRQ(USART1_IRQn);
          HAL_NVIC_EnableIRQ(USART2_IRQn);
          HAL_NVIC_EnableIRQ(USART3_8_IRQn);
        }
    
        /* Update user information */
         
      }
      
      if (UartTransfertState[USART1_INDEX].rx_counter == NB_PACKET)
      {
        /* Transfer status complete because the last packet has been received on USART1 */
        TransferComplete = 1;
      }
    } while(TransferComplete == 0);
    
    /* Compare the sent buffer sent on UART1 with the buffer receive on UART1 */
    if(kTools_Buffercmp((uint8_t*)aTxBuffer,(uint8_t*)aRxBuffer[USART1_INDEX],BUFFER_SIZE))
    {
       kWindow_Error("8Uart Application\nBuffercmp Error\n");
       while(1);
    }
    
    /* count the number of test loop */
    loop++;
    
  }while (1);
}

/**
  * @brief  Display uart transaction state 
  * @param  None.
  * @note   This display information about the uart transaction.  
  * @retval None.
  */
void HuitUartUpdateUserHeader(void)
{
#define HEADER_POSITION  88 
  
  /* Tx info */
  BSP_LCD_SetFont(&Font16);
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
  BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
  /* CN1 & JP1 information */
  
  BSP_LCD_DisplayStringAt(0,  HEADER_POSITION - 3*Font16.Height, (uint8_t *)"All jumpers closed on CN1" , CENTER_MODE);
  BSP_LCD_DisplayStringAt(0,  HEADER_POSITION - 2*Font16.Height, (uint8_t *)"JP1 fitted in pos EXT_RX" , CENTER_MODE);
  
  BSP_LCD_SetBackColor(LCD_COLOR_BLUE);   
  BSP_LCD_FillRect(0, HEADER_POSITION, BSP_LCD_GetXSize(), Font16.Height);
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
  BSP_LCD_DisplayStringAt(0,  HEADER_POSITION, (uint8_t *)"Uart 1  2  3  4  5  6  7  8" , CENTER_MODE);
    
  BSP_LCD_SetFont(&Font24);  
}  


/**
  * @brief  Display loop number
  * @param  None.
  * @note   This display information about the number of loop test.  
  * @retval None.
  */
void HuitUartUpdateLoopInformation(uint16_t loop)
{
#define LOOP_POSITION  152
  uint8_t string[30];
  /* Loop info */
  sprintf((char *)string, "test loops : %d", loop);
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
  BSP_LCD_SetBackColor(LCD_COLOR_WHITE);   
  BSP_LCD_DisplayStringAt(0,  LOOP_POSITION, string , CENTER_MODE);
}  

/**
  * @brief  Display uart transaction state 
  * @param  None.
  * @note   This display information about the uart transaction.  
  * @retval None.
  */
void HuitUartUpdateRxTxInformation(void)
{
#define TX_POSITION  104 
#define RX_POSITION  120
uint8_t string[50];  
  
  /* Tx info */
  sprintf((char *)string, "  TX %2d %2d %2d %2d %2d %2d %2d %2d ", UartTransfertState[USART1_INDEX].tx_counter,UartTransfertState[USART2_INDEX].tx_counter,
                                                                UartTransfertState[USART3_INDEX].tx_counter,UartTransfertState[USART4_INDEX].tx_counter,
                                                                UartTransfertState[USART5_INDEX].tx_counter,UartTransfertState[USART6_INDEX].tx_counter,
                                                                UartTransfertState[USART7_INDEX].tx_counter,UartTransfertState[USART8_INDEX].tx_counter);
 
  BSP_LCD_SetFont(&Font16);
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
  BSP_LCD_SetBackColor(LCD_COLOR_RED);   
  BSP_LCD_DisplayStringAt(0,  TX_POSITION, string , CENTER_MODE);

/* Tx info */
  sprintf((char *)string, "  RX %2d %2d %2d %2d %2d %2d %2d %2d ", UartTransfertState[USART1_INDEX].rx_counter,UartTransfertState[USART2_INDEX].rx_counter,
                                                                UartTransfertState[USART3_INDEX].rx_counter,UartTransfertState[USART4_INDEX].rx_counter,
                                                                UartTransfertState[USART5_INDEX].rx_counter,UartTransfertState[USART6_INDEX].rx_counter,
                                                                UartTransfertState[USART7_INDEX].rx_counter,UartTransfertState[USART8_INDEX].rx_counter);
   
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
  BSP_LCD_SetBackColor(LCD_COLOR_BLACK); 
  BSP_LCD_DisplayStringAt(0,  RX_POSITION, string , CENTER_MODE);
  BSP_LCD_SetFont(&Font24);  
}  

/**
  * @brief  Get User action 
  * @param  sel : User selection (JOY_SEL,...)
  * @note   This example is the only way to get user information.  
  * @retval None
  */
void HuitUartUserAction(uint8_t sel)
{
  switch(sel)
  {
  case JOY_DOWN : 
       user_stop = 1;
    break;
  default :
    break;
  }
}

/**
  * @brief  Get Uart index in the table Rx Transfer completed callback
  * @param  UartHandle: UART handle
  * @note   none.
  * @retval uart index 
  */
uint8_t GetUartIndex(UART_HandleTypeDef *UartHandle)
{
uint8_t index=0;
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
  * @brief  Rx Transfer completed callback
  * @param  UartHandle: UART handle
  * @note   This example shows a simple way to report end of DMA Rx transfer, and 
  *         you can add your own implementation.
  * @retval None
  */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *UartHandle)
{
  uint8_t index = GetUartIndex(UartHandle);
  
  if(UartTransfertState[index].rx_counter == 0)
  {  
    /* this uart is now avaible to send data, set the bit to begin data transfer for this uartx */
    uart_tx_transfert|= (1 << index);
  }

  /* Increment the reception counter */
  UartTransfertState[index].rx_counter++;

  /* Chekc if all packet are already received */
  if(UartTransfertState[index].rx_counter < NB_PACKET)
  {
    /* prepare the next packet reception */
    HAL_UART_Receive_IT(UartHandle, (uint8_t*)(aRxBuffer[index]+UartTransfertState[index].rx_counter*PACKET_SIZE), PACKET_SIZE);
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
  uint8_t index = GetUartIndex(UartHandle);
  /* Increment the numbre of packet sent */ 
  UartTransfertState[index].tx_counter++;
  /* set the bit for the next transfert */
  uart_tx_transfert|= (1 << index);
}


/**
  * @brief UART MSP Initialization 
  *        This function configures the hardware resources used in this example: 
  *           - Peripheral's clock enable
  *           - Peripheral's GPIO Configuration  
  *           - NVIC configuration for UART interrupt request enable
  * @param huart: UART handle pointer
  * @retval None
  */
void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{  
  GPIO_InitTypeDef  GPIO_InitStruct;
  
  switch((uint32_t)huart->Instance)
  {
  case (uint32_t)USART1 :
    {
      /*##-1- Enable peripherals and GPIO Clocks #################################*/
      /* Enable GPIO TX/RX clock */
      __HAL_RCC_GPIOA_CLK_ENABLE();
      
      /* Enable USARTx clock */
      __HAL_RCC_USART1_CLK_ENABLE(); 
      
      /*##-2- Configure peripheral GPIO ##########################################*/  
      /* UART TX GPIO pin configuration  */
      GPIO_InitStruct.Pin       = GPIO_PIN_9 | GPIO_PIN_10;
      GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
      GPIO_InitStruct.Pull      = GPIO_PULLUP;
      GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;
      GPIO_InitStruct.Alternate = GPIO_AF1_USART1;
      
      HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
      
      /*##-3- Configure the NVIC for UART ########################################*/
      /* NVIC for USART */
      HAL_NVIC_SetPriority(USART1_IRQn, 1, 0);
      HAL_NVIC_EnableIRQ(USART1_IRQn);
    }
    break;
  case (uint32_t)USART2 :
    {
      /*##-1- Enable peripherals and GPIO Clocks #################################*/
      /* Enable GPIO TX/RX clock */
      __HAL_RCC_GPIOD_CLK_ENABLE();
      
      /* Enable USARTx clock */
      __HAL_RCC_USART2_CLK_ENABLE(); 
      
      /*##-2- Configure peripheral GPIO ##########################################*/  
      /* UART TX GPIO pin configuration  */
      GPIO_InitStruct.Pin       = GPIO_PIN_6 | GPIO_PIN_5;
      GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
      GPIO_InitStruct.Pull      = GPIO_PULLUP;
      GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;
      GPIO_InitStruct.Alternate = GPIO_AF0_USART2;
      
      HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
      
      /*##-3- Configure the NVIC for UART ########################################*/
      /* NVIC for USART */
      HAL_NVIC_SetPriority(USART2_IRQn, 1, 0);
      HAL_NVIC_EnableIRQ(USART2_IRQn);
    }
    break;
  case (uint32_t)USART3 :
    {
      /*##-1- Enable peripherals and GPIO Clocks #################################*/
      /* Enable GPIO TX/RX clock */
      __HAL_RCC_GPIOD_CLK_ENABLE();
      __HAL_RCC_SYSCFG_CLK_ENABLE();
      
      /* Enable USARTx clock */
      __HAL_RCC_USART3_CLK_ENABLE(); 
      
      /*##-2- Configure peripheral GPIO ##########################################*/  
      /* UART TX GPIO pin configuration  */
      GPIO_InitStruct.Pin       = GPIO_PIN_8 | GPIO_PIN_9;
      GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
      GPIO_InitStruct.Pull      = GPIO_PULLUP;
      GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;
      GPIO_InitStruct.Alternate = GPIO_AF0_USART3;
      
      HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
 
      /*##-3- Configure the NVIC for UART ########################################*/
      /* NVIC for USART */
      HAL_NVIC_SetPriority(USART3_8_IRQn, 0, 0);
      HAL_NVIC_EnableIRQ(USART3_8_IRQn);
    }
    break;
  case (uint32_t)USART4 :
    {
      /*##-1- Enable peripherals and GPIO Clocks #################################*/
      /* Enable GPIO TX/RX clock */
      __HAL_RCC_GPIOC_CLK_ENABLE();
      
      /* Enable USARTx clock */
      __HAL_RCC_USART4_CLK_ENABLE(); 
      
      /*##-2- Configure peripheral GPIO ##########################################*/  
      /* UART TX GPIO pin configuration  */
      GPIO_InitStruct.Pin       = GPIO_PIN_10 | GPIO_PIN_11;
      GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
      GPIO_InitStruct.Pull      = GPIO_PULLUP;
      GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;
      GPIO_InitStruct.Alternate = GPIO_AF0_USART4;
      
      HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
      
      /* To allow the IT source identification */
      __HAL_RCC_SYSCFG_CLK_ENABLE();
      /*##-3- Configure the NVIC for UART ########################################*/
      /* NVIC for USART */
      HAL_NVIC_SetPriority(USART3_8_IRQn, 0, 0);
      HAL_NVIC_EnableIRQ(USART3_8_IRQn);
    }
    break;
  case (uint32_t)USART5 :
    {
      /*##-1- Enable peripherals and GPIO Clocks #################################*/
      /* Enable GPIO TX/RX clock */
      __HAL_RCC_GPIOB_CLK_ENABLE();
      
      /* Enable USARTx clock */
      __HAL_RCC_USART5_CLK_ENABLE(); 
      
      /*##-2- Configure peripheral GPIO ##########################################*/  
      /* UART TX GPIO pin configuration  */
      GPIO_InitStruct.Pin       = GPIO_PIN_3 | GPIO_PIN_4;
      GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
      GPIO_InitStruct.Pull      = GPIO_PULLUP;
      GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;
      GPIO_InitStruct.Alternate = GPIO_AF4_USART5;
      
      HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
      
      /* To allow the IT source identification */
      __HAL_RCC_SYSCFG_CLK_ENABLE();
      /*##-3- Configure the NVIC for UART ########################################*/
      /* NVIC for USART */
      HAL_NVIC_SetPriority(USART3_8_IRQn, 0, 0);
      HAL_NVIC_EnableIRQ(USART3_8_IRQn);
    }
    break;
  case (uint32_t)USART6 :
    {
      /*##-1- Enable peripherals and GPIO Clocks #################################*/
      /* Enable GPIO TX/RX clock */
      __HAL_RCC_GPIOF_CLK_ENABLE();
      
      /* Enable USARTx clock */
      __HAL_RCC_USART6_CLK_ENABLE(); 
      
      /*##-2- Configure peripheral GPIO ##########################################*/  
      /* UART TX GPIO pin configuration  */
      GPIO_InitStruct.Pin       = GPIO_PIN_9 | GPIO_PIN_10;
      GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
      GPIO_InitStruct.Pull      = GPIO_PULLUP;
      GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;
      GPIO_InitStruct.Alternate = GPIO_AF1_USART6;
      
      HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);
      
      /* To allow the IT source identification */
      __HAL_RCC_SYSCFG_CLK_ENABLE(); 
      /*##-3- Configure the NVIC for UART ########################################*/
      /* NVIC for USART */
      HAL_NVIC_SetPriority(USART3_8_IRQn, 0, 0);
      HAL_NVIC_EnableIRQ(USART3_8_IRQn);
    }
    break;
  case (uint32_t)USART7 :
    {
      /*##-1- Enable peripherals and GPIO Clocks #################################*/
      /* Enable GPIO TX/RX clock */
      __HAL_RCC_GPIOF_CLK_ENABLE();
      
      /* Enable USARTx clock */
      __HAL_RCC_USART7_CLK_ENABLE(); 
      
      /*##-2- Configure peripheral GPIO ##########################################*/  
      /* UART TX GPIO pin configuration  */
      GPIO_InitStruct.Pin       = GPIO_PIN_3 | GPIO_PIN_2;
      GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
      GPIO_InitStruct.Pull      = GPIO_PULLUP;
      GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;
      GPIO_InitStruct.Alternate = GPIO_AF1_USART7;
      
      HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);
      
      /* To allow the IT source identification */
      __HAL_RCC_SYSCFG_CLK_ENABLE(); 
      /*##-3- Configure the NVIC for UART ########################################*/
      /* NVIC for USART */
      HAL_NVIC_SetPriority(USART3_8_IRQn, 0, 0);
      HAL_NVIC_EnableIRQ(USART3_8_IRQn);
    }
    break;
  case (uint32_t)USART8 :
    {
      /*##-1- Enable peripherals and GPIO Clocks #################################*/
      /* Enable GPIO TX/RX clock */
      __HAL_RCC_GPIOC_CLK_ENABLE();
      
      /* Enable USARTx clock */
      __HAL_RCC_USART8_CLK_ENABLE(); 
      
      /*##-2- Configure peripheral GPIO ##########################################*/  
      /* UART TX GPIO pin configuration  */
      GPIO_InitStruct.Pin       = GPIO_PIN_8 | GPIO_PIN_9;
      GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
      GPIO_InitStruct.Pull      = GPIO_PULLUP;
      GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;
      GPIO_InitStruct.Alternate = GPIO_AF1_USART8;
      
      HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
      
      /* To allow the IT source identification */
      __HAL_RCC_SYSCFG_CLK_ENABLE(); 
      /*##-3- Configure the NVIC for UART ########################################*/
      /* NVIC for USART */
      HAL_NVIC_SetPriority(USART3_8_IRQn, 1, 0);
      HAL_NVIC_EnableIRQ(USART3_8_IRQn);
    }
    break;
  }
}

/**
* @brief UART MSP De-Initialization 
*        This function frees the hardware resources used in this example:
*          - Disable the Peripheral's clock
*          - Revert GPIO and NVIC configuration to their default state
* @param huart: UART handle pointer
* @retval None
*/
void HAL_UART_MspDeInit(UART_HandleTypeDef *huart)
{
  switch((uint32_t)huart->Instance)
  {
  case (uint32_t)USART1 :
    {
      /*##-1- Reset peripherals ##################################################*/
      __HAL_RCC_USART1_FORCE_RESET();
      __HAL_RCC_USART1_RELEASE_RESET();
      __HAL_RCC_USART1_CLK_DISABLE();
      
      /*##-2- Disable peripherals and GPIO Clocks #################################*/
      /* Configure UART Tx/Rx as alternate function  */
      HAL_GPIO_DeInit(GPIOA, GPIO_PIN_9 | GPIO_PIN_10);
      
      /*##-3- Disable the NVIC for UART ##########################################*/
      IRQ_Managment(__DISABLE, USART1_IRQn);
    }
    break;
  case (uint32_t)USART2 :
    {
      /*##-1- Reset peripherals ##################################################*/
      __HAL_RCC_USART2_FORCE_RESET();
      __HAL_RCC_USART2_RELEASE_RESET();
      __HAL_RCC_USART2_CLK_DISABLE();
      
      /*##-2- Disable peripherals and GPIO Clocks #################################*/
      /* Configure UART Tx/Rx as alternate function  */
      HAL_GPIO_DeInit(GPIOD, GPIO_PIN_5 | GPIO_PIN_6);
      
      /*##-3- Disable the NVIC for UART ##########################################*/
      IRQ_Managment(__DISABLE, USART2_IRQn);
    }
    break;
  case (uint32_t)USART3 :
    {
      /*##-1- Reset peripherals ##################################################*/
      __HAL_RCC_USART3_FORCE_RESET();
      __HAL_RCC_USART3_RELEASE_RESET();
      __HAL_RCC_USART3_CLK_DISABLE();
      
      /*##-2- Disable peripherals and GPIO Clocks #################################*/
      /* Configure UART Tx/Rx as alternate function  */
      HAL_GPIO_DeInit(GPIOD, GPIO_PIN_9 | GPIO_PIN_10);
      
      /*##-3- Disable the NVIC for UART ##########################################*/
      IRQ_Managment(__DISABLE, USART3_8_IRQn);
      
    }
    break;
  case (uint32_t)USART4 :
    {
      /*##-1- Reset peripherals ##################################################*/
      __HAL_RCC_USART4_FORCE_RESET();
      __HAL_RCC_USART4_RELEASE_RESET();
      __HAL_RCC_USART4_CLK_DISABLE();
      
      /*##-2- Disable peripherals and GPIO Clocks #################################*/
      /* Configure UART Tx/Rx as alternate function  */
      HAL_GPIO_DeInit(GPIOC, GPIO_PIN_10 | GPIO_PIN_11);
      
      /*##-3- Disable the NVIC for UART ##########################################*/
      IRQ_Managment(__DISABLE, USART3_8_IRQn);
    }
    break;
  case (uint32_t)USART5 :
    {
      /*##-1- Reset peripherals ##################################################*/
      __HAL_RCC_USART5_FORCE_RESET();
      __HAL_RCC_USART5_RELEASE_RESET();
      __HAL_RCC_USART5_CLK_DISABLE();
      
      /*##-2- Disable peripherals and GPIO Clocks #################################*/
      /* Configure UART Tx/Rx as alternate function  */
      HAL_GPIO_DeInit(GPIOB, GPIO_PIN_3 | GPIO_PIN_4);
      
      /*##-3- Disable the NVIC for UART ##########################################*/
      IRQ_Managment(__DISABLE, USART3_8_IRQn);
    }
    break;
  case (uint32_t)USART6 :
    {
      /*##-1- Reset peripherals ##################################################*/
      __HAL_RCC_USART6_FORCE_RESET();
      __HAL_RCC_USART6_RELEASE_RESET();
      __HAL_RCC_USART6_CLK_DISABLE();
      
      /*##-2- Disable peripherals and GPIO Clocks #################################*/
      /* Configure UART Tx/Rx as alternate function  */
      HAL_GPIO_DeInit(GPIOF, GPIO_PIN_9 | GPIO_PIN_10);
      
      /*##-3- Disable the NVIC for UART ##########################################*/
      IRQ_Managment(__DISABLE, USART3_8_IRQn);
    }
    break;
  case (uint32_t)USART7 :
    {
      /*##-1- Reset peripherals ##################################################*/
      __HAL_RCC_USART7_FORCE_RESET();
      __HAL_RCC_USART7_RELEASE_RESET();
      __HAL_RCC_USART7_CLK_DISABLE();
      
      /*##-2- Disable peripherals and GPIO Clocks #################################*/
      /* Configure UART Tx/Rx as alternate function  */
      HAL_GPIO_DeInit(GPIOF, GPIO_PIN_2 | GPIO_PIN_3);
      
      /*##-3- Disable the NVIC for UART ##########################################*/
      IRQ_Managment(__DISABLE, USART3_8_IRQn);
    }
    break;
  case (uint32_t)USART8 :
    {
      /*##-1- Reset peripherals ##################################################*/
      __HAL_RCC_USART8_FORCE_RESET();
      __HAL_RCC_USART8_RELEASE_RESET();
      __HAL_RCC_USART8_CLK_DISABLE();
      
      /*##-2- Disable peripherals and GPIO Clocks #################################*/
      /* Configure UART Tx/Rx as alternate function  */
      HAL_GPIO_DeInit(GPIOC, GPIO_PIN_8 | GPIO_PIN_9);
      
      /*##-3- Disable the NVIC for UART ##########################################*/
      IRQ_Managment(__DISABLE, USART3_8_IRQn);
    }
    break;
  }
}


/**
  * @brief IRQ_Managment 
  *        This function is used to avoid ENABLE or DISABLE when the IRQ has been already update for another peripheral :
  *          - Disable/Enable the Peripheral's clock
  *
* * @param huart: UART handle pointer
  * @retval None
  */
static void IRQ_Managment(int32_t mode, int32_t IrqName)
{
  static int8_t IRQ_state[3] = { 0, 0, 0};
  uint8_t IRQ_index;
  
  switch(IrqName)
  {
  case USART1_IRQn   : IRQ_index=0; break;
  case USART2_IRQn   : IRQ_index=1; break;
  case USART3_8_IRQn : IRQ_index=3; break;
  default: return; 
  }

  switch(mode)
  {
  case __ENABLE :
    IRQ_state[IRQ_index]++;
    if(IRQ_state[IRQ_index] == 1)
    {
      HAL_NVIC_DisableIRQ((IRQn_Type)IrqName);
    }
    break;
  case __DISABLE :
    if(IRQ_state[IRQ_index] != 0)
    {
      IRQ_state[IRQ_index]--;
      if(IRQ_state[IRQ_index] == 0)
      {
        HAL_NVIC_DisableIRQ((IRQn_Type)IrqName);
      }
    }
  break;
  }
}
/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

