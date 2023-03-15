/**
  @page USART_Communication_Tx USART Transmitter example (Polling mode)
  
  @verbatim
  ******************** (C) COPYRIGHT 2016 STMicroelectronics *******************
  * @file    Examples_LL/USART/USART_Communication_Tx/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the USART_Communication_Tx example.
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

Configuration of GPIO and USART peripherals to send characters 
asynchronously to an HyperTerminal (PC) in Polling mode. If the transfer could not 
be complete within the allocated time, a timeout allows to exit from the sequence 
with timeout error. This example is based on STM32F0xx USART LL API. 
The peripheral initialization uses LL unitary service functions for optimization purposes (performance and size).

USART Peripheral is configured in asynchronous mode (115200 bauds, 8 data bit, 1 start bit, 1 stop bit, no parity).
No HW flow control is used.
GPIO associated to User push-button is linked with EXTI. 
Virtual Com port feature of STLINK could be used for UART communication between board and PC.

Example execution:
LED is then turned Off.
On press on push button , first character of buffer to be transmitted is written into USART Transmit Data Register (TDR) 
Program then starts polling on USART TXE flag before sending next character.
On last character, program polls on TC flag to ensure transmit of last character is completed.
At completion of Tx Buffer sending, LED is turned On.
In case of errors, LED is blinking (1sec period).

Program is written so that, any new press on User push-button will lead to new transmission of complete buffer.

@par Directory contents 

  - USART/USART_Communication_Tx/Inc/stm32f0xx_it.h          Interrupt handlers header file
  - USART/USART_Communication_Tx/Inc/main.h                  Header for main.c module
  - USART/USART_Communication_Tx/Inc/stm32_assert.h          Template file to include assert_failed function
  - USART/USART_Communication_Tx/Src/stm32f0xx_it.c          Interrupt handlers
  - USART/USART_Communication_Tx/Src/main.c                  Main program
  - USART/USART_Communication_Tx/Src/system_stm32f0xx.c      STM32F0xx system source file


@par Hardware and Software environment

  - This example runs on STM32F072xB devices.
    
  - This example has been tested with STM32F072RB-Nucleo RevC board and can be
    easily tailored to any other supported device and development board.

  - STM32F072RB-Nucleo RevC Set-up
    In order to select use of Virtual Com port feature of STLINK for connection between STM32F072RB-Nucleo RevC and PC,
    User has to set USE_VCP_CONNECTION define to 1 in main.h file.
    If so, please ensure that USART communication between the target MCU and ST-LINK MCU is properly enabled 
    on HW board in order to support Virtual Com Port (Default HW SB configuration allows use of VCP)

    If VCP is not used (USE_VCP_CONNECTION define set to 0 in main.h file), GPIOs connected
    to USART1 TX/RX (PA.09 and PA.10) should be wired
    to respectively RX and TX pins of PC UART (could be done through a USB to UART adapter).
    Connect GND between STM32 board and PC UART.

  - Launch serial communication SW on PC (as HyperTerminal or TeraTerm) with proper configuration 
    (115200 bauds, 8 bits data, 1 stop bit, no parity, no HW flow control). 
    Port should be selected according to USE_VCP_CONNECTION value.

  - Launch the program. Press on User push button on board to initiate data transfer.

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example


 */
