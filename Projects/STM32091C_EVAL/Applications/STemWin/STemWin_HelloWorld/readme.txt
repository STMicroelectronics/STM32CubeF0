/**
  @page STemWin_HelloWorld Readme file
 
  @verbatim
  ******************************************************************************
  * @file    STemWin_HelloWorld/readme.txt 
  * @author  MCD Application Team
  * @brief
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

This application shows how to implement a simple "Hello World" example based on STemWin.

The example allows also to run the different Segger samples that can be
downloaded from here:
http://www.segger.com/emwin-samples.html
To do this, user has only to replace the file "Basic_HelloWorld.c" into the
project workspace by the downloaded one.

Note that the following user files may need to be updated:
  LCDConf_stm32091c_eval.c
  GUIConf_stm32091c_eval.c
(if for example more GUI allocated memory is needed)

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Directory contents 

  - STemWin/STemWin_HelloWorld/Inc/GUIConf.h                Header for GUIConf_stm32091c_eval.c
  - STemWin/STemWin_HelloWorld/Inc/LCDConf_stm32091c_eval.h  Header for LCDConf_stm32091c_eval.c
  - STemWin/STemWin_HelloWorld/Inc/stm32f0xx_hal_conf.h     HAL configuration file
  - STemWin/STemWin_HelloWorld/Inc/stm32f0xx_it.h           Interrupt handlers header file
  - STemWin/STemWin_HelloWorld/Inc/main.h                   Header for main.c module  
  - STemWin/STemWin_HelloWorld/Src/stm32f0xx_it.c           Interrupt handlers
  - STemWin/STemWin_HelloWorld/Src/BASIC_HelloWorld.c       Simple demo drawing "Hello world"
  - STemWin/STemWin_HelloWorld/Src/GUIConf_stm32091c_eval.c  Display controller initialization
  - STemWin/STemWin_HelloWorld/Src/LCDConf_stm32091c_eval.c  Configuration file for the GUI library
  - STemWin/STemWin_HelloWorld/Src/main.c                   Main program
  - STemWin/STemWin_HelloWorld/Src/system_stm32f0xx.c       STM32F0xx system source file


@par Hardware and Software environment

  - This example runs on STM32F091xC devices.
    
  - This example has been tested with STM32091C-EVAL board and can be
    easily tailored to any other supported device and development board.  
    

@par How to use it ? 

In order to make the program work, you must do the following :
  - Open your preferred toolchain 
  - Rebuild all files and load your image into target memory
  - Run the example
 
 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
