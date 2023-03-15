/**
  @page COMP_Interrupt COMP Interrupt example

  @verbatim
  ******************** (C) COPYRIGHT 2016 STMicroelectronics *******************
  * @file    COMP/COMP_Interrupt/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the COMP Interrupt example.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2016 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  @endverbatim

@par Example Description 

How to use a comparator peripheral to compare a voltage level applied on a GPIO
pin to the the internal voltage reference (VREFINT), in interrupt mode.

When the comparator input crosses (either rising or falling edges) the internal reference 
voltage VREFINT (1.22V), the comparator generates an interrupt.

In this example, the comparator is connected on pin PA1. 
Each time comparator input crosses VREFINT, LED1 toggles.

If LED2 is powering on: This indicates that the system generates an 
error.

@note Care must be taken when using HAL_Delay(), this function provides 
      accurate delay (in milliseconds) based on variable incremented in SysTick ISR. 
      This implies that if HAL_Delay() is called from a peripheral ISR process, then 
      the SysTick interrupt must have higher priority (numerically lower) than the 
      peripheral interrupt. Otherwise the caller ISR process will be blocked. 
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application need to ensure that the SysTick time base is always set 
      to 1 millisecond to have correct HAL operation.

@par Directory contents 

  - COMP/COMP_Interrupt/Inc/stm32f0xx_hal_conf.h    HAL configuration file
  - COMP/COMP_Interrupt/Inc/stm32f0xx_it.h          Interrupt handlers header file
  - COMP/COMP_Interrupt/Inc/main.h                  Header for main.c module  
  - COMP/COMP_Interrupt/Src/stm32f0xx_it.c          Interrupt handlers
  - COMP/COMP_Interrupt/Src/main.c                  Main program
  - COMP/COMP_Interrupt/Src/stm32f0xx_hal_msp.c     HAL MSP file
  - COMP/COMP_Interrupt/Src/system_stm32f0xx.c      STM32F0xx system source file


@par Hardware and Software environment 

  - This example runs on STM32F072VB devices.

  - This example has been tested with STMicroelectronics STM32072B-EVAL RevB board and can be
    easily tailored to any other supported device and development board.

  - STM32072B-EVAL Set-up
    
       - Use LED1 connected to PD.08 pin
       - Apply an external variable voltage on PA1 with average voltage 1.22V.

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example


 */
