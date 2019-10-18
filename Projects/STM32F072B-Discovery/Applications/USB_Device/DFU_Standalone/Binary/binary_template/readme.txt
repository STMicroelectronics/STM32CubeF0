    /**
  @page DFU_Standalone Binary Template

  @verbatim
  ******************************************************************************
  * @file    DFU_Standalone/Binary/binary_template/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the binary template example.
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

  - This example runs on STM32F072RB devices.
    
  - This example has been tested with STM32F072B-Discovery board and can be
    easily tailored to any other supported device and development board.      

@par How to use it ? 
In order to make the program work, you must do the following :
- Open your preferred toolchain 
- Rebuild all files and load your image into target memory
- Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
