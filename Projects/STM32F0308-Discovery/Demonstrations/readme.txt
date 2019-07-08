/**
  @page STM32F0308-Discovery_Demonstration   STM32F00308-Discovery demonstration firmware
  
  @verbatim
  ******************** (C) COPYRIGHT 2016 STMicroelectronics *******************
  * @file    Demonstration/readme.txt  
  * @author  MCD Application Team
  * @brief   Description of the STM32F0308-Discovery demonstration firmware
  ******************************************************************************
  *
  * <h2><center>&copy; COPYRIGHT(c) 2016 STMicroelectronics</center></h2>
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

@par Demonstration Description 

This demonstration consists on using the USER push button B1 connected to PA0 and 
the two leds LED4 and LED3 connected respectively to PC8 and PC9.

In the demonstration  the LED3 is initially blinking each 200ms.
  - For the first pressing on the USER button, the speed of the led blinking is incremented. 
    The LED3 blinks each 100ms.
  - For the second pressing on the USER button, LED3 stops blinking and the status is off.
  - A third pressing on the USER button causes the initial behavior of the LED3.

Each time the USER button is pressed, the LED4 is turned on for 1s.


@par Directory contents 

  - Demonstrations/Inc/main.h               Main program header file
  - Demonstrations/Inc/stm32f0xx_hal_conf.h HAL Configuration file
  - Demonstrations/Inc/stm32f0xx_it.h       Interrupt handlers header file
  - Demonstrations/Src/main.c               Main program
  - Demonstrations/Src/stm32f0xx_it.c       Interrupt handlers
  - Demonstrations/Src/system_stm32f0xx.c   STM32F0xx system source file
  

@par Hardware and Software environment

  
  - This Demonstration has been tested with STMicroelectronics STM32F0308-DISCO (MB1134) 
    RevA and can be easily tailored to any other supported device and development board.
  - This Demonstration runs on STM32F030R8 devices.

@par How to use it ? 

In order to make the program work, you must do the following :
- Open your preferred toolchain 
- Rebuild all files and load your image into target memory
- Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
