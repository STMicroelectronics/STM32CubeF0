/**
  @page UART_OneBoards_8UART 

  @verbatim
  ******************** (C) COPYRIGHT 2016 STMicroelectronics *******************
  * @file    UART/UART_OneBoards_8UART/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the 8 UART Communication IT example.
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
  @endverbatim

@par Examples' Hardware configuration
 Board: STM3291C_EVAL with specific CN1 header for all UART Tx/Rx
 
 1. Chain all UARTs thanks to jumpers that can be easily plugged on UART dedicated connector CN1.
  __________________________________
 |																	|
 | ¤	 ¤		¤		¤		¤		¤		¤		¤		|
 | |	 |		|		|		|		|		|		|		| CN1
 | ¤	 ¤		¤		¤		¤		¤		¤		¤		|
  ----------------------------------
 2. Set jumper JP1 to 2<->3 in order to route (UART1Rx) PA10 to CN1.
 
@par Example Description

This example guides you through the different configuration steps by mean of HAL API 
to ensure Data buffer transmission and reception

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system
clock (SYSCLK) to run at 48 MHz.

The UART peripheral configuration is ensured by the HAL_UART_Init() function.
This later is calling the HAL_UART_MspInit()function which core is implementing
the configuration of the needed UART resources according to the used hardware (CLOCK, 
GPIO, NVIC). You may update this function to change UART configuration.

For this example aTxBuffer is predefined with a 180 chars string, that will be 
transmitted/received in loop by packet of 18 data through all 8 UARTs. The packet
size has been defined to 18 datas, it can be changed but  the result of 
BUFFER_SIZE/PACKET_SIZE must be an integer.

This can be monitored, on your favourite IDE, by watching in live specific variables:
"UartTransfertState" will show the number of packet transmitted/received on each UART.
"aRxBuffer" will show in real time the buffer state for each usart.
"count" will show how many full transfert loop done.

STM32 Eval board's LEDs can be used to monitor the transfer status:
 - LED1 & LED4 : Toggled after each transfer.
 - LED1, LED2, LED3 & LED4 : Toggled in case of error. 

@note 
In case of SW break during the tranfer, Uart HW processing is not stopped and 
there is a big chance to get an overun if you restart the SW

@note
In case one of the jumpers on CN1 is removed during the loopback test, 
the packet tarnsmission/reception will be stopped.

@note
RX under IT & Tx in polling mode.

@par Directory contents 

  - UART/UART_OneBoards_8UART/Inc/stm32f0xx_hal_conf.h    HAL configuration file
  - UART/UART_OneBoards_8UART/Inc/stm32f0xx_it.h          DMA interrupt handlers header file
  - UART/UART_OneBoards_8UART/Inc/main.h                  Header for main.c module  
  - UART/UART_OneBoards_8UART/Src/stm32f0xx_it.c          DMA interrupt handlers
  - UART/UART_OneBoards_8UART/Src/main.c                  Main program
  - UART/UART_OneBoards_8UART/Src/stm32f0xx_hal_msp.c     HAL MSP module
  - UART/UART_OneBoards_8UART/Src/system_stm32f0xx.c      STM32F0xx system source file
   
@par Hardware and Software environment
  - This example runs on STM32F091xC devices.
    
  - This example has been tested with STM32091C-EVAL board.

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example
 
 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
