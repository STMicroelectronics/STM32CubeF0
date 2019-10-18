/**
  @page CAN_Networking CAN Networking example
  
  @verbatim
  ******************** (C) COPYRIGHT 2016 STMicroelectronics *******************
  * @file    CAN/CAN_Networking/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the CAN Networking example.
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

@par Example Description 

How to configure the CAN peripheral to send and receive CAN frames in 
normal mode.  

The sent frames are used to control LEDs by pressing Tamper push-button.
	
The CAN serial communication link is a bus to which a number of units may be
connected. This number has no theoretical limit. Practically the total number
of units will be limited by delay times and/or electrical loads on the bus line.

This program behaves as follows:
  - After reset, all LEDs are OFF on the N eval boards
  - By Pressing on Tamper push-button : LED1 turns ON and all other LEDs are OFF, on the N
    eval boards connected to the bus. 
  - Press on Tamper push-button again to send CAN Frame to command LEDn+1 ON, all other LEDs 
    are OFF on the N eval boards.
    
@note This example is tested with a bus of 3 units. The same program example is 
      loaded in all units to send and receive frames.
@note Any unit in the CAN bus may play the role of sender (by pressing Tamper push-button)
      or receiver.

  The CAN is configured as follows:
    - Bit Rate   = 1 Mbit/s  
    - CAN Clock  = external clock (HSE)
    - ID Filter  = All identifiers are allowed
    - RTR = Data
    - DLC = 1 byte
    - Data: Led number that should be turned ON

@par Directory contents 

  - CAN/CAN_Networking/Inc/stm32f0xx_hal_conf.h    HAL configuration file
  - CAN/CAN_Networking/Inc/stm32f0xx_it.h          DMA interrupt handlers header file
  - CAN/CAN_Networking/Inc/main.h                  Header for main.c module  
  - CAN/CAN_Networking/Src/stm32f0xx_it.c          DMA interrupt handlers
  - CAN/CAN_Networking/Src/main.c                  Main program
  - CAN/CAN_Networking/Src/stm32f0xx_hal_msp.c     HAL MSP file
  - CAN/CAN_Networking/Src/system_stm32f0xx.c      STM32F0xx system source file
  
@par Hardware and Software environment

  - This example runs on STM32F0xx devices.
  - This example has been tested with STM32072B-EVAL RevB board and can be
    easily tailored to any other supported device and development board.

  - STM32072B-EVAL RevB Set-up 
    - Use LED1, LED2, LED3 and LED4 connected respectively to PD.08, PD.09, PD.10
      and PD.11 pins
    - Use Tamper push-button connected to PC.13
    - Connect a male/male CAN cable between at least 2 EVAL CAN connectors (CN6)
    - You have to configure the jumpers as follows:

  @verbatim
  
       +-------------------------------+
       |   Jumper    |       CAN       |
       +-------------------------------+
       |   JP4       |       1-2       |
       |   JP3       |      fitted     |
       +-------------------------------+
  @endverbatim
  
@par How to use it ? 

In order to make the program work, you must do the following:
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example
 
 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
 