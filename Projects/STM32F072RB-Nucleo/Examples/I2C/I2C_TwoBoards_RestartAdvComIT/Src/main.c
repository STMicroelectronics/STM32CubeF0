/**
  ******************************************************************************
  * @file    I2C/I2C_TwoBoards_RestartAdvComIT/Src/main.c
  * @author  MCD Application Team
  * @brief   This sample code shows how to use STM32F0xx I2C HAL API to transmit
  *          and receive a data buffer with a communication process based on
  *          IT transfer and with a repeated start condition between the transmit
  *          and receive process.. 
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

#if defined(__GNUC__)
#include <stdio.h>	/*rtt*/
#include <stdlib.h>	/*rtt*/
#endif

#include "main.h"

/** @addtogroup STM32F0xx_HAL_Examples
  * @{
  */

/** @addtogroup I2C_TwoBoards_RestartAdvComIT
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Uncomment this line to use the board as master, if not it is used as slave */
//#define MASTER_BOARD
#define I2C_ADDRESS        0x3C

/**
  * @brief Defines related to Slave process
  */
#define SLAVE_CHIP_NAME     0
#define SLAVE_CHIP_REVISION 1
#define SLAVE_LAST_INFO     SLAVE_CHIP_REVISION

/**
  * @brief Defines related to Timeout to keep Leds status
  */
#define LED_STATUS_TIMEOUT  1000 /* 1 Second */

#if (USE_VCP_CONNECTION == 1)
/**
  * @brief Defines related to Timeout to uart Transmission
  */
#define UART_TIMEOUT_VALUE  1000 /* 1 Second */
#endif

/**
  * @brief Defines related to I2C clock speed
  */
/* I2C TIMING Register define when I2C clock source is SYSCLK */
/* I2C TIMING is calculated in case of the I2C Clock source is the SYSCLK = 72 MHz */
/* This example use TIMING to 0x00C4092A to reach 1 MHz speed (Rise time = 26ns, Fall time = 2ns) */
#define I2C_TIMING      0x00C4092A

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
#if (USE_VCP_CONNECTION == 1)
/* UART handler declaration */
UART_HandleTypeDef UartHandle;
#endif
/* I2C handler declaration */
I2C_HandleTypeDef I2cHandle;

/**
  * @brief Variables related to Master process
  */
/* aCommandCode declaration array    */
/* [CommandCode][RequestSlaveAnswer] */
/* {CODE, YES/NO}                    */
const char* aCommandCode[4][4] = {
  {"CHIP_NAME", "YES"},
  {"CHIP_REVISION", "YES"},
  {"LOW_POWER", "NO"},
  {"WAKE_UP", "NO"}};
                  
uint8_t*     pMasterTransmitBuffer     = (uint8_t*)(&aCommandCode[0]);
uint8_t      ubMasterNbCommandCode     = sizeof(aCommandCode[0][0]);
uint8_t      aMasterReceiveBuffer[0xF] = {0};
__IO uint8_t ubMasterNbDataToReceive   = sizeof(aMasterReceiveBuffer);
__IO uint8_t ubMasterNbDataToTransmit  = 0;
uint8_t      ubMasterCommandIndex      = 0;
__IO uint8_t ubMasterReceiveIndex      = 0;

/**
  * @brief Variables related to Slave process
  */
const char* aSlaveInfo[]      = {
                  "STM32F072RB",
                  "1.2.3"};

uint8_t       aSlaveReceiveBuffer[0xF]  = {0};
uint8_t*      pSlaveTransmitBuffer      = 0;
__IO uint8_t  ubSlaveNbDataToTransmit   = 0;
uint8_t       ubSlaveInfoIndex          = 0xFF;
__IO uint8_t  ubSlaveReceiveIndex       = 0;
uint32_t      uwTransferDirection       = 0;
__IO uint32_t uwTransferInitiated       = 0;
__IO uint32_t uwTransferEnded           = 0;

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void FlushBuffer8(uint8_t* pBuffer1, uint16_t BufferLength);
static void Error_Handler(void);

#if defined(__GNUC__) && defined(MASTER_BOARD) && defined(SW4STM32)
extern void initialise_monitor_handles(void);	/*rtt*/
#endif

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
#if defined(__GNUC__) && defined(MASTER_BOARD) && defined(SW4STM32)
  initialise_monitor_handles();	/*rtt*/

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

  /* Configure LED2 */
  BSP_LED_Init(LED2);

#if (USE_VCP_CONNECTION == 1)
  /* Configure the UART peripheral                                        */
  /* Put the USART peripheral in the Asynchronous mode (UART Mode) */
  /* UART configured as follows:
      - Word Length = 8 Bits
      - Stop Bit = One Stop bit
      - Parity = None
      - BaudRate = 115200 baud
      - Hardware flow control disabled (RTS and CTS signals) */
  UartHandle.Instance        = USARTx;

  UartHandle.Init.BaudRate   = 115200;
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
#endif

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

  /* Infinite loop */
  while (1)
  {
    /* Wait for User push-button press before starting the Communication */
    while (BSP_PB_GetState(BUTTON_USER) != GPIO_PIN_RESET)
    {
    }
  
    /* Wait for User push-button release before starting the Communication */
    while (BSP_PB_GetState(BUTTON_USER) != GPIO_PIN_SET)
    {
    }

    /* The board sends the message and expects to receive it back if necessary. */

    /* If Master no request a Slave answer, Run master in transmitter mode only. */
    if(strncmp(aCommandCode[ubMasterCommandIndex][1], "NO", 2) == 0)
    {
      /*##-2- Start the transmission process #####################################*/  
      /* Master prepare and send the transmission buffer ("pMasterTransmitBuffer") 
         through a "New" communication frame. The communication will be stopped at
         the end of transmission process thanks to "I2C_FIRST_AND_LAST_FRAME" option usage. */
      pMasterTransmitBuffer    = (uint8_t*)(aCommandCode[ubMasterCommandIndex][0]);
      ubMasterNbDataToTransmit = strlen((char *)(aCommandCode[ubMasterCommandIndex][0]));

      /* Handle I2C events (Master Transmit only) */
      do
      {
        if(HAL_I2C_Master_Seq_Transmit_IT(&I2cHandle, (uint16_t)I2C_ADDRESS, pMasterTransmitBuffer, ubMasterNbDataToTransmit, I2C_FIRST_AND_LAST_FRAME)!= HAL_OK)
        {
          /* Error_Handler() function is called when error occurs. */
          Error_Handler();
        }

        /*##-3- Wait for the end of the transfer #################################*/  
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

      /*##-4- Monitor Status through Terminal I/O ##############################*/  
      /* Display through external Terminal IO the Command Code acknowledge by Slave device */
      printf("Slave goes to %s.\n\r", (char*)(aCommandCode[ubMasterCommandIndex][0]));
    }
    /* Else Master request a Slave answer, Run master in transmitter mode then receiver mode. */
    else
    {
      /*##-2- Start the transmission process #####################################*/  
      /* Master prepare and send the transmission buffer ("pMasterTransmitBuffer") 
         through a "New" communication frame. The communication will not stopped thanks
         to "I2C_FIRST_FRAME" option usage. This will allow to generate a restart condition
         after change the I2C peripheral from transmission process to reception process */
      pMasterTransmitBuffer    = (uint8_t*)(aCommandCode[ubMasterCommandIndex][0]);
      ubMasterNbDataToTransmit = strlen((char *)(aCommandCode[ubMasterCommandIndex][0]));

      /* Handle I2C events (Master Transmit only) */
      do
      {
        if(HAL_I2C_Master_Seq_Transmit_IT(&I2cHandle, (uint16_t)I2C_ADDRESS, pMasterTransmitBuffer, ubMasterNbDataToTransmit, I2C_FIRST_FRAME)!= HAL_OK)
        {
          /* Error_Handler() function is called when error occurs. */
          Error_Handler();
        }

        /*##-3- Wait for the end of the transfer #################################*/  
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

      /*##-4- Put I2C peripheral in reception process ###########################*/  
      /* Master generate a restart condition and then change the I2C peripheral 
         from transmission process to reception process, to retrieve information
         data from Slave device. */
      do
      {
        if(HAL_I2C_Master_Seq_Receive_IT(&I2cHandle, (uint16_t)I2C_ADDRESS, aMasterReceiveBuffer, strlen((char *)(aSlaveInfo[ubMasterCommandIndex])), I2C_LAST_FRAME)!= HAL_OK)
        {
          /* Error_Handler() function is called when error occurs. */
          Error_Handler();
        }

        /*##-5- Wait for the end of the transfer #################################*/  
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

      /*##-6- Monitor Status through Terminal I/O ##############################*/  
      /* Display through external Terminal IO the Slave Answer received */
      printf("%s : %s\n\r", (char*)(aCommandCode[ubMasterCommandIndex][0]), (char*)aMasterReceiveBuffer);
    }

  
    /* Prepare Index to send next command code */
    ubMasterCommandIndex++;
    if(ubMasterCommandIndex >= ubMasterNbCommandCode)
    {
      ubMasterCommandIndex = 0;
    }
    
    /* For User help, keep Leds status until timeout */
    HAL_Delay(LED_STATUS_TIMEOUT);

    /* Then Clear and Reset process variables, arrays and Leds status, for next transfer */
    FlushBuffer8(aMasterReceiveBuffer, COUNTOF(aMasterReceiveBuffer));
    ubMasterNbDataToTransmit = 0;
    ubMasterReceiveIndex     = 0;
    BSP_LED_Off(LED2);
    
#else /* SLAVE_BOARD */
    
  /* Infinite loop */
  while (1)
  {
    /*##-2- Put I2C peripheral in Listen address match code process ##########*/  
    /* This action will allow I2C periphal to able to treat Master request when
       necessary depending of transfer direction requested by Master */
    if(HAL_I2C_EnableListen_IT(&I2cHandle) != HAL_OK)
    {
      /* Transfer error in reception process */
      Error_Handler();
    }
    
    /*##-3- Wait for a new frame communication with a Master #################*/  
    /*  Before starting a transfer, you need to wait a Master request event.
        For simplicity reasons, this example is just waiting till an Address callback event,
       but application may perform other tasks while transfer operation is ongoing. */  
    while(uwTransferInitiated != 1)
    {
    }

    /*##-4- Wait for the end of the frame communication ######################*/  
    /*  Before ending a transfer, you need to wait a Master end event.
        For simplicity reasons, this example is just waiting till an Stop condition event,
        but application may perform other tasks while transfer operation is ongoing. */  
    while(uwTransferEnded != 1)
    {
    }
    
    /* For User help, keep Leds status until timeout */
    HAL_Delay(LED_STATUS_TIMEOUT);

    /*##-5- Clear, reset process variables, arrays and Leds status ###########*/  
    FlushBuffer8(aSlaveReceiveBuffer, COUNTOF(aSlaveReceiveBuffer));
    uwTransferInitiated = 0;
    uwTransferEnded = 0;
    ubSlaveReceiveIndex = 0;
    ubSlaveInfoIndex = 0xFF;
    BSP_LED_Off(LED2);
#endif /* MASTER_BOARD */
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

#if (USE_VCP_CONNECTION == 1)
/**
  * @brief  Retargets the C library printf function to the USARTx.
  * @param  ch: character to send
  * @param  f: pointer to file (not used)
  * @retval The character transmitted
  */
#ifdef __GNUC__
/* With GCC, small printf (option LD Linker->Libraries->Small printf
   set to 'Yes') calls __io_putchar() */
int __io_putchar(int ch)
#else
int fputc(int ch, FILE *f)
#endif /* __GNUC__ */
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the EVAL_COM1 and Loop until the end of transmission */
  HAL_UART_Transmit(&UartHandle, (uint8_t *)&ch, 1, UART_TIMEOUT_VALUE); 

  return ch;
}
#endif

/**
  * @brief  Tx Transfer completed callback.
  * @param  I2cHandle: I2C handle 
  * @note   This example shows a simple way to report end of IT Tx transfer, and 
  *         you can add your own implementation. 
  * @retval None
  */
#ifdef MASTER_BOARD
void HAL_I2C_MasterTxCpltCallback(I2C_HandleTypeDef *I2cHandle)
{
  /* Turn LED2 on: Transfer in transmission process is correct */
  BSP_LED_On(LED2);
}
#else
void HAL_I2C_SlaveTxCpltCallback(I2C_HandleTypeDef *I2cHandle)
{
  /* Turn LED2 off: Transfer in transmission process is correct */
  BSP_LED_Off(LED2);
}
#endif /* MASTER_BOARD */

/**
  * @brief  Rx Transfer completed callback.
  * @param  I2cHandle: I2C handle
  * @note   This example shows a simple way to report end of IT Rx transfer, and 
  *         you can add your own implementation.
  * @retval None
  */
#ifdef MASTER_BOARD
void HAL_I2C_MasterRxCpltCallback(I2C_HandleTypeDef *I2cHandle)
{
  /* Turn LED2 off: Transfer in reception process is correct */
  BSP_LED_Off(LED2);
}
#else
void HAL_I2C_SlaveRxCpltCallback(I2C_HandleTypeDef *I2cHandle)
{
  /* Turn LED2 on: Transfer in reception process is correct */
  BSP_LED_On(LED2);

  /* Check Command code receive previously */
  /* If data received match with a Internal Command Code, set the associated index */
  /* Which will use for Transmission process if requested by Master */
  if(strcmp((char *)(aSlaveReceiveBuffer), (char *)(aCommandCode[0][0])) == 0)
  {
    ubSlaveInfoIndex = SLAVE_CHIP_NAME;
  }
  else if(strcmp((char *)(aSlaveReceiveBuffer), (char *)(aCommandCode[1][0])) == 0)
  {
    ubSlaveInfoIndex = SLAVE_CHIP_REVISION;
  }
  else
  {
    if(HAL_I2C_Slave_Seq_Receive_IT(I2cHandle, &aSlaveReceiveBuffer[ubSlaveReceiveIndex], 1, I2C_FIRST_FRAME) != HAL_OK)
    {
      Error_Handler();
    }
    ubSlaveReceiveIndex++;
  }
}
#endif /* MASTER_BOARD */

#ifndef MASTER_BOARD
/**
  * @brief  Slave Address Match callback.
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *                the configuration information for the specified I2C.
  * @param  TransferDirection: Master request Transfer Direction (Write/Read), value of @ref I2C_XferOptions_definition
  * @param  AddrMatchCode: Address Match Code
  * @retval None
  */
void HAL_I2C_AddrCallback(I2C_HandleTypeDef *hi2c, uint8_t TransferDirection, uint16_t AddrMatchCode)
{
  if(AddrMatchCode == I2C_ADDRESS)
  {
    uwTransferInitiated = 1;
    uwTransferDirection = TransferDirection;

    /* First of all, check the transfer direction to call the correct Slave Interface */
    if(uwTransferDirection == I2C_DIRECTION_TRANSMIT)
    {
      if(HAL_I2C_Slave_Seq_Receive_IT(&I2cHandle, &aSlaveReceiveBuffer[ubSlaveReceiveIndex], 1, I2C_FIRST_FRAME) != HAL_OK)
      {
        Error_Handler();
      }
      ubSlaveReceiveIndex++;
    }
    else
    {
      pSlaveTransmitBuffer = (uint8_t*)(aSlaveInfo[ubSlaveInfoIndex]);
      ubSlaveNbDataToTransmit = strlen((char *)(aSlaveInfo[ubSlaveInfoIndex]));

      if(HAL_I2C_Slave_Seq_Transmit_IT(&I2cHandle, pSlaveTransmitBuffer, ubSlaveNbDataToTransmit, I2C_LAST_FRAME) != HAL_OK)
      {
        Error_Handler();
      }
    }
  }
  else
  {
    /* Call Error Handler, Wrong Address Match Code */
    Error_Handler();
  }
}

/**
  * @brief  Listen Complete callback.
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *                the configuration information for the specified I2C.
  * @retval None
  */
void HAL_I2C_ListenCpltCallback(I2C_HandleTypeDef *hi2c)
{
  uwTransferEnded = 1;
}
#endif

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
    Error_Handler();
  }
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
  * @brief  Flush 8-bit buffer.
  * @param  pBuffer1: pointer to the buffer to be flushed.
  * @param  BufferLength: buffer's length
  * @retval None
  */
static void FlushBuffer8(uint8_t* pBuffer1, uint16_t BufferLength)
{
  uint8_t Index = 0;
  
  for (Index = 0; Index < BufferLength; Index++)
  {
    pBuffer1[Index] = 0;
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
