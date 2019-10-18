/**
  @page Demo   Demo STM32F031K6-Nucleo(revC)
 
  @verbatim
  ******************************************************************************
  * @file    readme.txt 
  * @author  MCD Application Team
  * @brief   Description of STM32F031K6-Nucleo(revC) Demo
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

@par Demo Description

How to use the Gravitech 7 segment 4 digits shield with a Nucleo 32 Board.

This demonstration is a basic counter which is displayed on the digits of the 
shield.
The demonstration stops on 3232 value.
The displayed counter on the screen at the end is refreshed each 5 ms.
In case of error, LED3 toggles with a period of one second.

@note Care must be taken when using HAL_Delay(), this function provides accurate
      delay (in milliseconds) based on variable incremented in SysTick ISR. 
      This implies that if HAL_Delay() is called from a peripheral ISR process, 
      then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application need to ensure that the SysTick time base is always set to
      1 millisecond to have correct HAL operation.

@par Directory contents
 
  - Demonstrations/Inc/stm32f0xx_hal_conf.h    HAL configuration file
  - Demonstrations/Inc/stm32f0xx_it.h          Interrupt handlers header file
  - Demonstrations/Inc/main.h                  Header for main.c module
  - Demonstrations/Src/stm32f0xx_it.c          Interrupt handlers
  - Demonstrations/Src/main.c                  Main program
  - Demonstrations/Src/system_stm32f0xx.c      STM32F0xx system source file


@par Hardware and Software environment

  - This example runs on STM32F031x6 devices.
  - This demo has been tested with STM32F031K6-Nucleo(revC) board and can be
    easily tailored to any other supported device and development board.
  - Gravitech 4 digits 7 segment nano shield is connected on Arduino nano connectors.

@par How to use it ? 

In order to make the program work, you must do the following :
  - Open your preferred toolchain 
  - Rebuild all files and load your image into target memory
  - Run the example
 
 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
