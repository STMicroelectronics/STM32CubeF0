/**
  @page IAP_Binary_Template AN4657 IAP Binary Template Readme file
  
  @verbatim
  ******************** (C) COPYRIGHT 2016 STMicroelectronics *******************
  * @file    IAP/IAP_Binary_Template/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the IAP_Binary_Template directory.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2016 STMicroelectronics International N.V. 
  * All rights reserved.</center></h2>
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
loaded into Flash memory using In-Application Programming (IAP) through USART.

To build such application, some special configuration has to be performed:
1. Set the application load address at 0x08004000, using your toolchain linker file
2. To be able to serve the application interrupts, you need to relocate the vector 
   table (which contains the interrupt handlers). However, unlike CortexM3 and CortexM4, 
   the CortexM0 processor do not support vector table relocation (it is fixed at 
   address 0x00000000).
   A solution will be to relocate by software the vector table to the internal SRAM:  
    - Copy the vector table from the Flash (mapped at the base of the application load
      address 0x08004000) to the base address of the SRAM at 0x20000000.
    - Remap SRAM at address 0x00000000, using __HAL_SYSCFG_REMAPMEMORY_SRAM() macro
    - Then once an interrupt occurs, the CortexM0 processor will fetch the interrupt 
      handler start address from the relocated vector table in SRAM, then it will 
      jump to execute the interrupt handler located in the Flash.
   This operation should be done at the initialization phase of the application.    

The SysTick example provided within the STM32F0xx HAL Cube examples is used as illustration.
This example configures the SysTick to generate a time base equal to 1 ms.
The system clock is set to the maximum system frequency, the SysTick is clocked by 
the AHB clock (HCLK). A "Delay" function is implemented based on the SysTick 
end-of-count event.
Four LEDs are toggled with a timing defined by the Delay function.


@par Directory contents 

 - IAP/IAP_Binary_Template/EWARM: This folder contains a IAR pre-configured project 
                                  file that produces a binary image of SysTick 
                                  example that could be loaded using IAP.
                                          
 - IAP/IAP_Binary_Template/MDK-ARM: This folder contains a Keil pre-configured project 
                                    file that produces a binary image of SysTick 
                                    example that could be loaded using IAP.

 - IAP/IAP_Binary_Template/SW4STM32: This folder contains a SW4STM32 pre-configured project 
                                     file that produces a binary image of SysTick 
                                     example that could be loaded using IAP.

 - IAP/IAP_Binary_Template/Inc: contains the binary template firmware header files 
     - IAP/IAP_Binary_Template/Inc/stm32f0xx_hal_conf.h  Library Configuration file
     - IAP/IAP_Binary_Template/Inc/stm32f0xx_it.h        Header for stm32f0xx_it.c
     - IAP/IAP_Binary_Template/Inc/main.h                Header for main.c

 - IAP/IAP_Binary_Template/Src: contains the binary template firmware source files 
     - IAP/IAP_Binary_Template/Src/main.c              Main program
     - IAP/IAP_Binary_Template/Src/stm32f0xx_it.c      Interrupt handlers
     - IAP/IAP_Binary_Template/Src/system_stm32f0xx.c  STM32F0xx system source file

 - IAP/IAP_Binary_Template/Utilities: This folder contains Axf (Keil) conversion scripts to BIN format.

@note The "system_stm32f0xx.c" is generated by an automatic clock configuration 
      system and can be easily customized to your own configuration. 
     
@par Hardware and Software environment

  - This example runs on STM32F0xx Devices.
  
  - This example has been tested with STMicroelectronics STM32091C-EVAL evaluation board
    and can be easily tailored to any other supported device and development board.

@par How to use it ?  

In order to load the SysTick example with the IAP, you must do the following:

 - EWARM:
    - Open the Project.eww workspace
    - Rebuild all files: Project->Rebuild all
    - A binary file "STM32091C_EVAL_SysTick.bin" will be generated under "STM32091C_EVAL/Exe" folder.  
    - Finally load this image with IAP application

 - MDK-ARM:
    - Open the Project.uvproj project
    - Rebuild all files: Project->Rebuild all target files
    - Go to "/IAP_Binary_Template/Utilities" directory and run "axftobin.bat"
      (Fromelf Exe path might have to be updated in "axftobin.bat" file, according to your Keil setup).
    - A binary file "STM32091C_EVAL_SysTick.bin" will be generated under "/IAP_Binary_Template/MDK-ARM/STM32091C_EVAL" folder. 
    - Finally load this image with IAP application

 - SW4STM32
    - Open the SW4STM32 toolchain.
    - Browse to the SW4STM32 workspace directory, select the project (.project file in \IAP_Binary_Template\SW4STM32\STM32091C_EVAL directory).
    - Rebuild all project files.
    - A binary file "STM32091C_EVAL.bin" will be generated under "STM32091C_EVAL\Debug" folder. 
    - Finally load this image with IAP application

 * <h2><center>&copy; COPYRIGHT 2016 STMicroelectronics</center></h2>
 */
