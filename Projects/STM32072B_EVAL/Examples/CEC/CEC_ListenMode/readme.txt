/**
  @page CEC CEC_ListenMode example
  
  @verbatim
  ******************** (C) COPYRIGHT 2016 STMicroelectronics *******************
  * @file    CEC/CEC_ListenMode/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the CEC Listen Mode example.
  ******************************************************************************
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  @endverbatim

@par Example Description 

How to configure and use the CEC peripheral to receive and transmit messages 
between two boards, while a third board (the spy device) listens but doesn't 
acknowledge the received messages.

- Hardware Description

To use this example, three STM32072B-EVAL RevB boards (called Device_1, Device_2 and
Device_3) are loaded with the same software then connected through CEC lines
(PB8 or HDMI connectors) and GND.
 /|\  In the firmware file main.h, uncomment the dedicated line to use
/_!_\ the CEC peripheral as STM32 device_1, STM32 device_2 or STM32 device_3.

@verbatim
*------------------------------------------------------------------------------*
|    STM32072B-EVAL RevB           STM32072B-EVAL RevB           STM32072B-EVAL RevB          |
|  Device Address :0x01      Device Address :0x03     Device Address :0x04     |
|  ____________________     ____________________     ____________________      |
| |                    |   |                    |   |                    |     |
| |           _________|___|______CECLine_______|___|__________          |     | 
| |     _____|____     |   |     ____|_____     |   |     _____|____     |     |
| |    |   CEC    |    |   |    |   CEC    |    |   |    |   CEC    |    |     |
| |    | Device_1 |    |   |    | Device_2 |    |   |    | Device_3 |    |     |
| |    |__________|    |   |    |__________|    |   |    |__________|    |     |
| |                    |   |                    |   |                    |     |
| |  O LD1             |   |  O LD1             |   |  O LD1             |     |
| |  O LD2    Joystick |   |  O LD2    Joystick |   |  O LD2    Joystick |     |
| |  O LD3        _    |   |  O LD3        _    |   |  O LD3        _    |     |
| |  O LD4       |_|   |   |  O LD4       |_|   |   |  O LD4       |_|   |     |
| |                    |   |                    |   |                    |     |
| |             GND O--|---|--O GND      GND O--|---|--O GND             |     |
| |____________________|   |____________________|   |____________________|     |
|                                                                              |
|                                                                              |
*------------------------------------------------------------------------------*
*
@endverbatim


- Software Description

The test unrolls as follows.

On TX side, four possible messages can be transmitted and are indicated as
below on the transmitting board:
 - when Tamper push-button is pressed, LED1 toggles
 - when Joystick Sel push-button is pressed, LED2 toggles
 - when Joystick UP push-button is pressed, LED3 toggles
 - when Joystick DOWN push-button is pressed, LED4 toggles


Device_1 sends messages addressed to Device_2,
Device_2 sends messages addressed to Device_1.
Both Device_1 and Device_2 acknowledge the received messages.


Accordingly, the following happens on the RX side (i.e. on both receiving boards,
one which acknowledges the message and the other one which doesn't) in case of successful
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


In case Device_3 (the spy device) transmits, only Device_1 reacts
to the received messages. 


Practically, 2 EXTI lines (EXTI0_1 and EXTI4_15) are configured to 
generate an interrupt on each falling or rising edge. 
A specific message is then transmitted by the CEC IP
and a LED connected to a specific GPIO pin is toggled.
    - EXTI0_1 is mapped to PA.00
    - EXTI4_15 is mapped to PC.13, PF.09 and PF.10 

Then, on TX side,
  - when rising edge is detected on EXTI0_1, LED2 toggles
  - when falling edge is detected on EXTI4_15 and EXTI line interrupt is detected
    on PC.13, LED1 toggles
  - when falling edge is detected on EXTI4_15 and EXTI line interrupt is detected
    on PF.09, LED3 toggles
  - when falling edge is detected on EXTI4_15 and EXTI line interrupt is detected
    on PF.10, LED4 toggles
   

In this example, HCLK is configured at 48 MHz.

@Known Limitation: Currently there is an issue when pressing the joystick/button on Device_1,
  just after Device_3 has been used (button pressed): there will be a transmission error.
  It is necessary to use (press joystick/button) on Device_2, then Device_1 becomes operative again.


@par Directory contents 

  - CEC/CEC_ListenMode/Inc/stm32f0xx_hal_conf.h    HAL configuration file
  - CEC/CEC_ListenMode/Inc/stm32f0xx_it.h          Interrupt handlers header file
  - CEC/CEC_ListenMode/Inc/main.h                        Header for main.c module  
  - CEC/CEC_ListenMode/Src/stm32f0xx_it.c          Interrupt handlers
  - CEC/CEC_ListenMode/Src/system_stm32f0xx.c      STM32F0xx system source file
  - CEC/CEC_ListenMode/Src/main.c                        Main program
  - CEC/CEC_ListenMode/Src/stm32f0xx_hal_msp.c     IP hardware resources initialization
  
@par Hardware and Software environment

  - This example runs on STM32F072VB Devices.
    
  - This example has been tested with STM32072B-EVAL RevB board and can be
    easily tailored to any other supported device and development board.      


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - in main.h: uncomment DEVICE_1 for first board, uncomment DEVICE_2 for second board, uncomment DEVICE_3 for third board
 - Rebuild all files and load your image into target memory
 - With a wire, connect HDMI_CEC_LINE_GPIO_PORT-HDMI_CEC_LINE_PIN between the 3 boards
 - Connect the ground of the 2 boards
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
