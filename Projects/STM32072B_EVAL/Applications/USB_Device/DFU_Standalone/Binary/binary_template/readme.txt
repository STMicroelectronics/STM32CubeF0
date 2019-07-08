    /**
  @page DFU_Standalone Binary Template

  @verbatim
  ******************************************************************************
  * @file    DFU_Standalone/Binary/binary_template/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the binary template example.
  ******************************************************************************
  *
  * Copyright (c) 2016 STMicroelectronics International N.V. All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  @endverbatim

@par Application Description 

This directory contains a set of sources files that build the application to be
loaded into Flash memory using device firmware upgrade (DFU).

To build such application, some special configuration has to be preformed: 
1. Set the program load address at 0x08007000, using your toolchain linker file
2. Relocate the vector table at address 0x08007000. To perform this operation,copy the vector
table from the Flash (mapped at the base of the application load address 0x08007000) to the base 
address of the SRAM at 0x20000000.
    
This example configures the SysTick to generate a time base equal to 1 ms.
The system clock is set to 48 MHz the SysTick is clocked by the AHB clock (HCLK)

A "Delay" function is implemented based on the SysTick end-of-count event.
Four LEDs are toggled with a timing defined by the Delay function.

The 48 MHz clock for the system can be derived from one of the two following sources:
  – PLL clock(clocked by the HSE)
  – RC48 Clock (HSI48): The HSI48 clock is generated from an internal 48 MHz RC oscillator, coupled with
    the clock recovery system (CRS). 
    The CRS allows automatic trimming of the internal HSI48.    
User can select system clock from HSI48 or PLL through macro defined in main.h
(USE_CLKSOURCE_CRSHSI48 and USE_CLKSOURCE_PLL).

@par Directory contents 

  - DFU_Standalone/Binary/binary_template/Inc/stm32f0xx_hal_conf.h    HAL configuration file
  - DFU_Standalone/Binary/binary_template/stm32f0xx_it.h              Interrupt handlers header file
  - DFU_Standalone/Binary/binary_template/Inc/main.h                  Header for main.c module  
  - DFU_Standalone/Binary/binary_template/Src/stm32f0xx_it.c          Interrupt handlers
  - DFU_Standalone/Binary/binary_template/Src/main.c                  Main program
  - DFU_Standalone/Binary/binary_template/Src/system_stm32f0xx.c      STM32F0xx system source file

@par Hardware and Software environment 

  - This example runs on STM32F072VB devices.
    
  - This example has been tested with STMicroelectronics STM32072B-EVAL RevB board and can be
    easily tailored to any other supported device and development board.      

@par How to use it ? 
In order to make the program work, you must do the following :
- Open your preferred toolchain 
- Rebuild all files and load your image into target memory
- Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
