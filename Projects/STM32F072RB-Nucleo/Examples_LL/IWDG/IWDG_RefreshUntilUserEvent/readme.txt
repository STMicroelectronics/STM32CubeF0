/**
  @page IWDG_RefreshUntilUserEvent IWDG example
  
  @verbatim
  ******************** (C) COPYRIGHT 2016 STMicroelectronics *******************
  * @file    Examples_LL/IWDG/IWDG_RefreshUntilUserEvent/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the IWDG_RefreshUntilUserEvent example.
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

How to configure the IWDG peripheral to ensure periodical counter update and 
generate an MCU IWDG reset when a user button is pressed. The peripheral 
is initialized with LL unitary service functions to optimize 
for performance and size.

Example Configuration:
Configure the IWDG (prescaler, counter) and enable it.
Infinite refresh of the IWDG down-counter done in the main loop.
Led is blinking fast & continuously

Example Execution:
When User Button is pressed, the down-counter automatic refresh mechanism is 
disable and thus, reset will occur. After a reset, when re-entering in the main, 
RCC IWDG Reset Flag will be checked and if we are back from a IWDG reset the led
will be switch ON.

Waiting a new user button pressed to re-activate the IWDG

@par Directory contents 

  - IWDG/IWDG_RefreshUntilUserEvent/Inc/stm32f0xx_it.h          Interrupt handlers header file
  - IWDG/IWDG_RefreshUntilUserEvent/Inc/main.h                  Header for main.c module
  - IWDG/IWDG_RefreshUntilUserEvent/Inc/stm32_assert.h          Template file to include assert_failed function
  - IWDG/IWDG_RefreshUntilUserEvent/Src/stm32f0xx_it.c          Interrupt handlers
  - IWDG/IWDG_RefreshUntilUserEvent/Src/main.c                  Main program
  - IWDG/IWDG_RefreshUntilUserEvent/Src/system_stm32f0xx.c      STM32F0xx system source file


@par Hardware and Software environment

  - This example runs on STM32F072xB devices.
    
  - This example has been tested with STM32F072RB-Nucleo RevC board and can be
    easily tailored to any other supported device and development board.


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
