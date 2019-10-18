/**
  @page IAP/IAP_Main AN4657 STM32F0xx In-Application Programming using the USART Readme file
  
  @verbatim
  ******************** (C) COPYRIGHT 2016 STMicroelectronics *******************
  * @file    IAP/IAP_Main/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of implementation of the AN4657 (in-application programming
  *          using the USART (IAP)) on STM32F0xx devices.
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
  @endverbatim

@par Application Description

This directory contains a set of sources files and pre-configured projects that 
describes how to build an application to be loaded into Flash memory using
In-Application Programming (IAP) through USART.

@par Directory contents

 - "IAP/IAP_Main/Inc": contains the IAP firmware header files 
    - IAP/IAP_Main/Inc/main.h              The main include file of the project.
    - IAP/IAP_Main/Inc/common.h            This file provides all the headers of the common functions.
    - IAP/IAP_Main/Inc/flash_if.h          This file provides all the firmware 
                                                    function headers of the flash_if.c file.
    - IAP/IAP_Main/Inc/menu.h              This file provides all the firmware
                                                    function headers of the menu.c file.
    - IAP/IAP_Main/Inc/ymodem.h            This file provides all the firmware
                                                    function headers of the ymodem.c file.
    - IAP/IAP_Main/Inc/stm32f0xx_hal_conf.h  Library Configuration file
    - IAP/IAP_Main/Inc/stm32f0xx_it.h      Header for stm32f0xx_it.c

 - "IAP/IAP_Main/MDK-ARM": contains pre-configured project for MDK-ARM toolchain

 - "IAP/IAP_Main/EWARM": contains pre-configured project for EWARM toolchain

 - "IAP/IAP_Main/SW4STM32": contains pre-configured project for SW4STM32 toolchain

 - "IAP/IAP_Main/Src": contains the IAP firmware source files
    - IAP/IAP_Main/Src/main.c              Main program
    - IAP/IAP_Main/Src/stm32f0xx_it.c      Interrupt handlers
    - IAP/IAP_Main/Src/stm32f0xx_hal_msp.c Microcontroller specific packages
                                                     initialization file.
    - IAP/IAP_Main/Src/flash_if.c          The file contains write, erase and disable
                                                    write protection of the internal Flash
                                                    memory.
    - IAP/IAP_Main/Src/menu.c              This file contains the menu to select
                                                    downloading a new binary file, uploading
                                                    internal Flash memory, executing the binary
                                                    and disabling the write protection of
                                                    write-protected pages
    - IAP/IAP_Main/Src/common.c            This file provides functions related to
                                                    read/write from/to USART peripheral
    - IAP/IAP_Main/Src/ymodem.c            This file provides all the firmware functions
                                                    related to the ymodem protocol.
    - IAP/IAP_Main/Src/system_stm32f0xx.c  STM32F0xx system source file

@par Hardware and Software environment

  - This example runs on STM32F0xx Devices.
  - This example has been tested with STMicroelectronics STM32091C-EVAL evaluation board
    and can be easily tailored to any other supported device and development board.
  
Table 1. IAP implementation on STM32091C-EVAL
/*** Platform ***|************* Implementation **************************|***** Configuration *****\
****************************************************************************************************
|    Firmware    | The IAP program is located at 0x08000000. The Flash   |                         |
|                | routines (program/erase) are executed from the Flash  |                         |
|                | memory.                                               |                         |
|                | The maximum size of this program (wo optimization)    |                         |
|                | is about 13 Kbytes and                                |                         |
|                | programmed on:                                        | Page 0 - Page 7         |  
|                | ------------------------------------------------------|-------------------------|
|                | The user application (image to be downloaded with the |                         | 
|                | IAP) will be programmed starting from address         |                         |
|                | (uint32_t)0x08004000(1).                              | (Page 8 - Page 127)     | 
|                | The maximum size of the image to be loaded is:        | (240 Kbytes             | 
|                | ------------------------------------------------------|-------------------------|
|                | The image is uploaded with the IAP from the STM32F0xx | 12 Kbytes               | 
|                | internal Flash.                                       | (Page 8 - Page 13)      |
|                | The size of the image to be uploaded is:              |                         |
|----------------|-------------------------------------------------------|-------------------------|
|    Hardware    | Push-button (active level: high)                      | Tamper push-button      |                                                                     
|                |                                                       | connected to pin PC13   |
|                | ------------------------------------------------------|-------------------------| 
|                | USART used                                            |  USART1   (CN9)         |
\**************************************************************************************************/ 
(1) User application location address is defined in the flash_if.h file as: 
#define APPLICATION_ADDRESS           ((uint32_t)0x08004000)
To modify it, change the default value to the desired one. Note that the application must be linked
relatively to the new address too.

Following picture illustrates the situation in program memory:
Figure 1. Flash memory usage

 Top Flash Memory address /-------------------------------------------\  0x0803FFFF
                          |                                           |
                          |                                           |
                          |                                           |
                          |                                           |
                          |                                           |
                          |          Page 8 - Page 127                |
                          |                                           |
                          |                                           |
                          |                                           |
                          |                                           |
                          |                                           |
                          |                                           |
                          |                                           |
                          |          User code                        |
                          |                                           |
                          |- - - - - - - - - - - - - - - - - - - - - -|
                          |          Vector table                     |
                          |-------------------------------------------|  0x08004000
                          |          IAP code                         |
                          |- - - - - - - - - - - - - - - - - - - - - -|
                          |          Vector table                     |
                          \-------------------------------------------/	 0x08000000					  
   
  - STM32091C-EVAL Set-up
    - Connect a null-modem female/female RS232 cable between the boards DB9 connector 
      and PC serial port.
      (make sure that jumpers JP1/JP13 are set to enable RS232 communication).
    - Hold the Tamper push-button during reset to enter the IAP.    

  - Terminal configuration: 
    - Word Length = 8 Bits
    - One Stop Bit
    - No parity
    - BaudRate = 115200 baud
    - flow control: None 
    - Ymodem protocol is using CRC16 by default. To switch to checksum, comment #define CRC16_F
      in ymodem.c

@par How to use it? 

In order to make the program work, you must do the following:

  1. Generate a binary image for the program provided in the 
     "IAP/IAP_Main/IAP_Binary_Template" project directory. 
  2. Program the internal Flash with the IAP (see below) 
  3. Open HyperTerminal window using the settings already defined in section
     "Hardware and Software environment" 
  4. To run the IAP driver, keep the Tamper push-button pressed at Reset. 
     The IAP main menu is then displayed on the HyperTerminal window.
  5. To download an application, press 1 and use the Ymodem protocol

Then
- Open your preferred toolchain
- Rebuild all files and load your image into target memory
- Run the example
    
 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
