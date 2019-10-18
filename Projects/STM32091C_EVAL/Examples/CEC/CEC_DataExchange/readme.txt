/**
  @page CEC CEC_DataExchange example
  
  @verbatim
  ******************** (C) COPYRIGHT 2016 STMicroelectronics *******************
  * @file    CEC/CEC_DataExchange/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the CEC Data Exchangeexample.
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

How to configure and use the CEC peripheral to receive and transmit messages.

- Hardware Description

To use this example, two STM32091C-EVAL boards (called Device_1 and 
Device_2) are loaded with the same software then connected through CEC lines
(PB10 or HDMI connectors) and GND.

 /|\  In the firmware file main.h, uncomment the dedicated line to use
/_!_\ the CEC peripheral as STM32 device_1 or STM32 device_2.

@verbatim
*------------------------------------------------------------------------------*
|           STM32091C_EVAL                         STM32091C_EVAL              |
|         Device Address :0x01                    Device Address :0x03         |
|         ____________________                   ____________________          |
|        |                    |                 |                    |         |
|        |                    |                 |                    |         | 
|        |     __________     |                 |     __________     |         |
|        |    |   CEC    |____|____CECLine______|____|   CEC    |    |         |
|        |    | Device_1 |    |                 |    | Device_2 |    |         |
|        |    |__________|    |                 |    |__________|    |         |
|        |                    |                 |                    |         |
|        |  O LD1             |                 |  O LD1             |         |
|        |  O LD2    Joystick |                 |  O LD2    Joystick |         |
|        |  O LD3        _    |                 |  O LD3        _    |         |
|        |  O LD4       |_|   |                 |  O LD4       |_|   |         |
|        |                    |                 |                    |         |
|        |             GND O--|-----------------|--O GND             |         |
|        |____________________|                 |____________________|         |
|                                                                              |
|                                                                              |
*------------------------------------------------------------------------------**
@endverbatim


- Software Description

The test unrolls as follows.

On TX side, four possible messages can be transmitted and are indicated as
below on the transmitting board:
 - when Tamper push-button is pressed, LED1 toggles
 - when Joystick Sel push-button is pressed, LED2 toggles
 - when Joystick UP push-button is pressed, LED3 toggles
 - when Joystick DOWN push-button is pressed, LED4 toggles

Accordingly, the following happens on the RX side in case of successful
reception:
 - when Tamper push-button is pressed on TX side, 
     * all RX side LEDs are turned off 
 - when Joystick Sel push-button is pressed on TX side, on RX side
     *  LED1 and LED2 are turned on
     *  LED3 and LED4 are turned off 
 - when Joystick UP push-button is pressed on TX side, 
     *  all RX side LEDs are turned on
 - when Joystick DOWN push-button is pressed on TX side, on RX side 
     * LED1 and LED2 are turned off
     * LED3 and LED4 are turned on    
In case of unsuccessful reception, LED3 is turned on.

Practically, 2 EXTI lines (EXTI0_1 and EXTI4_15) are configured to 
generate an interrupt on each falling or rising edge. 
A specific message is then transmitted by the CEC IP
and a LED connected to a specific GPIO pin is toggled.
    - EXTI0_1 is mapped to PA.00
    - EXTI4_15 is mapped to PC.13, PE.04 and PE.05 

Then, on TX side,
  - when rising edge is detected on EXTI0_1, LED2 toggles
  - when falling edge is detected on EXTI4_15 and EXTI line interrupt is detected
    on PC.13, LED1 toggles
  - when falling edge is detected on EXTI4_15 and EXTI line interrupt is detected
    on PE.04, LED3 toggles
  - when falling edge is detected on EXTI4_15 and EXTI line interrupt is detected
    on PE.05, LED4 toggles
   

In this example, HCLK is configured at 48 MHz.

@par Directory contents 

  - CEC/CEC_DataExchange/Inc/stm32f0xx_hal_conf.h    HAL configuration file
  - CEC/CEC_DataExchange/Inc/stm32f0xx_it.h          Interrupt handlers header file
  - CEC/CEC_DataExchange/Inc/main.h                  Header for main.c module  
  - CEC/CEC_DataExchange/Src/stm32f0xx_it.c          Interrupt handlers
  - CEC/CEC_DataExchange/Src/system_stm32f0xx.c      STM32F0xx system source file
  - CEC/CEC_DataExchange/Src/main.c                  Main program
  - CEC/CEC_DataExchange/Src/stm32f0xx_hal_msp.c     IP hardware resources initialization
  
@par Hardware and Software environment

  - This example runs on STM32091C  Devices.
    
  - This example has been tested with STM32091C-EVAL board and can be
    easily tailored to any other supported device and development board.      


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - in main.h, uncomment DEVICE_1 for first board, uncomment DEVICE_2 for second board
 - Rebuild all files and load your image into target memory
 - With a wire, connect HDMI_CEC_LINE_GPIO_PORT-HDMI_CEC_LINE_PIN between the 2 boards
 - Connect the ground of the 2 boards
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
