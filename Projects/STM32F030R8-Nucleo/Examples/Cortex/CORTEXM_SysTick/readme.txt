    /**
  @page CORTEXM_SysTick CORTEXM SysTick example

  @verbatim
  ******************** (C) COPYRIGHT 2016 STMicroelectronics *******************
  * @file    CORTEXM/CORTEXM_SysTick/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the CortexM SysTick example.
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

How to use the default SysTick configuration with a 1 ms timebase to toggle LEDs.

  The system clock is set to 48 MHz for STM32F0xx devices, the SysTick is 
  clocked by the AHB clock(HCLK).

  A "HAL_Delay" function is implemented based on the SysTick end-of-count event.

@note Care must be taken when using HAL_Delay(), this function provides accurate
      delay (in milliseconds) based on variable incremented in SysTick ISR. This
      implies that if HAL_Delay() is called from a peripheral ISR process, then 
      the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Directory contents 

  - CortexM/CORTEXM_SysTick/Inc/stm32f0xx_hal_conf.h    HAL configuration file
  - CortexM/CORTEXM_SysTick/Inc/stm32f0xx_it.h          Interrupt handlers header file
  - CortexM/CORTEXM_SysTick/Inc/main.h                  Header for main.c module  
  - CortexM/CORTEXM_SysTick/Src/stm32f0xx_it.c          Interrupt handlers
  - CortexM/CORTEXM_SysTick/Src/main.c                  Main program
  - CortexM/CORTEXM_SysTick/Src/system_stm32f0xx.c      STM32F0xx system source file

@par Hardware and Software environment 

  - This example runs on STM32F030R8 devices.
    
  - This example has been tested with STM32F030R8-Nucleo Rev C board and can be
    easily tailored to any other supported device and development board.

@par How to use it ? 
In order to make the program work, you must do the following :
- Open your preferred toolchain 
- Rebuild all files and load your image into target memory
- Run the example


 */
