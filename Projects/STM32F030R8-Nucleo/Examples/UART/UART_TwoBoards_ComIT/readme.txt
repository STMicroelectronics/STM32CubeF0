/**
  @page UART_TwoBoards_ComIT UART Two Boards Communication IT example

  @verbatim
  ******************** (C) COPYRIGHT 2016 STMicroelectronics *******************
  * @file    UART/UART_TwoBoards_ComIT/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the UART Two Boards Communication IT example.
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
  @endverbatim

@par Example Description 

UART transmission (transmit/receive) in Interrupt mode
between two boards.

Board: STM32F030R8-Nucleo Rev C (embeds a STM32F030R8T6 device)
Tx Pin: PA.09
Rx Pin: PA.10
   _________________________                       _________________________ 
  |           ______________|                     |______________           |
  |          |USART         |                     |         USART|          |
  |          |              |                     |              |          |
  |          |           TX |_____________________| RX           |          |
  |          |              |                     |              |          |
  |          |              |                     |              |          |
  |          |              |                     |              |          |
  |          |           RX |_____________________| TX           |          |
  |          |              |                     |              |          |
  |          |______________|                     |______________|          |
  |                         |                     |                         |
  |                         |                     |                         |
  |                      GND|_____________________|GND                      |
  |_STM32_Board 1___________|                     |_STM32_Board 2___________|



Two identical boards are connected as shown on the picture above.
Board 1: transmitting then receiving board
Board 2: receiving then transmitting board

The user presses the User push-button on board 1.
Then, board 1 sends in interrupt mode a message to board 2 that sends it back to 
board 1 in interrupt mode as well.
Finally, board 1 and 2 compare the received message to that sent.
If the messages are the same, the test passes.


WARNING: as both boards do not behave the same way :
 The user can choose between board 1 and board 2 through "#define TRANSMITTER_BOARD"
 in the "/Src/main.c" file:
 - Uncomment "#define TRANSMITTER_BOARD" to select board 1(the transmitting then receiving board).
 - Comment "#define TRANSMITTER_BOARD" to select board 2 (The receiving then transmitting board).

STM32F030R8-Nucleo Rev C board LED is used to monitor the transfer status:
- While board 1 is waiting for the user to press the User push-button, its LED2 is
  blinking rapidly (100 ms period).
- While board 2 is waiting for the message from board 1, its LED2 is emitting
  a couple of flashes every half-second.
- When the test passes, LED2 on both boards is turned on, otherwise the test has failed. 
- If there is an initialization or transfer error, LED2 is slowly blinking (1 sec. period).

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system
clock (SYSCLK) to run at 48 MHz.


The UART is configured as follows:
    - BaudRate = 9600 baud  
    - Word Length = 8 bits (8 data bits, no parity bit)
    - One Stop Bit
    - No parity
    - Hardware flow control disabled (RTS and CTS signals)
    - Reception and transmission are enabled in the time

@note USARTx/UARTx instance used and associated resources can be updated in "main.h"
file depending hardware configuration used.

@note When the parity is enabled, the computed parity is inserted at the MSB
position of the transmitted data.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.


@par Directory contents 

  - UART/UART_TwoBoards_ComIT/Inc/stm32f0xx_hal_conf.h    HAL configuration file
  - UART/UART_TwoBoards_ComIT/Inc/stm32f0xx_it.h          IT interrupt handlers header file
  - UART/UART_TwoBoards_ComIT/Inc/main.h                  Header for main.c module  
  - UART/UART_TwoBoards_ComIT/Src/stm32f0xx_it.c          IT interrupt handlers
  - UART/UART_TwoBoards_ComIT/Src/main.c                  Main program
  - UART/UART_TwoBoards_ComIT/Src/stm32f0xx_hal_msp.c     HAL MSP module
  - UART/UART_TwoBoards_ComIT/Src/system_stm32f0xx.c      STM32F0xx system source file


@par Hardware and Software environment 

  - This example runs on STM32F030xx devices.    
  - This example has been tested with two STM32F030R8-Nucleo Rev C boards embedding
    a STM32F030R8T6 device and can be easily tailored to any other supported device 
    and development board.

  - STM32F030R8-Nucleo Rev C set-up
    - Connect a wire between 1st board PA.09 pin (Uart Tx) and 2nd board PA.10 pin (Uart Rx)
    - Connect a wire between 1st board PA.10 pin (Uart Rx) and 2nd board PA.09 pin (Uart Tx)
    - Connect 1st board GND to 2nd Board GND    

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example


 */
