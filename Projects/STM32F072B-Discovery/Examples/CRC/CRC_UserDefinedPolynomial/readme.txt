/**
  @page CRC_UserDefinedPolynomial Cyclic Redundancy Check Example
  
  @verbatim
  ******************** (C) COPYRIGHT 2016 STMicroelectronics *******************
  * @file    CRC/CRC_UserDefinedPolynomial/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of Cyclic Redundancy Check Example.
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

How to configure the CRC using the HAL API. The CRC (cyclic
redundancy check) calculation unit computes the 8-bit CRC code for a given
buffer of 32-bit data words, based on a user-defined generating polynomial.
In this example, the polynomial is set manually to 0x9B, that is,
X^8 + X^7 + X^4 + X^3 + X + 1.

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system
clock (SYSCLK).

The CRC peripheral configuration is ensured by the HAL_CRC_Init() function.
This later is calling the HAL_CRC_MspInit()function which core is implementing
the configuration of the needed CRC resources according to the used hardware (CLOCK, 
GPIO, DMA and NVIC). You may update this function to change CRC configuration.

The calculated CRC code is saved in uwCRCValue variable.
Once calculated, the CRC value (uwCRCValue) is compared to the CRC expected value (uwExpectedCRCValue).

STM32 Discovery board's LEDs can be used to monitor the example status:
  - LED5 (GREEN) is ON when the correct CRC value is calculated
  - LED3 (RED) is ON when there is an error in initialization or if an incorrect CRC value is calculated.


@par Directory contents 
  
  - CRC/CRC_UserDefinedPolynomial/Inc/stm32f0xx_hal_conf.h    HAL configuration file
  - CRC/CRC_UserDefinedPolynomial/Inc/stm32f0xx_it.h          Interrupt handlers header file
  - CRC/CRC_UserDefinedPolynomial/Inc/main.h                  Header for main.c module
  - CRC/CRC_UserDefinedPolynomial/Src/stm32f0xx_it.c          Interrupt handlers
  - CRC/CRC_UserDefinedPolynomial/Src/main.c                  Main program
  - CRC/CRC_UserDefinedPolynomial/Src/stm32f0xx_hal_msp.c     HAL MSP module 
  - CRC/CRC_UserDefinedPolynomial/Src/system_stm32f0xx.c      STM32F0xx system source file

     
@par Hardware and Software environment

  - This example runs on STM32F072RB devices.
  - This example has been tested with STM32F072B-Discovery RevC board and can be
    easily tailored to any other supported device and development board.

@par How to use it ? 

In order to make the program work, you must do the following:
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example
 
 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
 