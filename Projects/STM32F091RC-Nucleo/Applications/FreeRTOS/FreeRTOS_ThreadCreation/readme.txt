/**
  @page FreeRTOS_ThreadCreation FreeRTOS Thread Creation example
 
  @verbatim
  ******************************************************************************
  * @file    FreeRTOS/FreeRTOS_ThreadCreation/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the FreeRTOS Thread Creation example.
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

@par Application Description

How to implement thread creation using CMSIS RTOS API. 

This example creates two threads with the same priority, which execute in 
a periodic cycle of 5 seconds for Thread 1 and 10 seconds for Thread 2. 

Thread 1 toggles LED2 every 250 milliseconds for 5 seconds then resumes Thread 2
and suspends itself. Thread 2 resumes and toggles LED2 every 500 milliseconds
for 10 seconds, then Thread 2 resumes Thread 1 and suspends itself.

@note Care must be taken when using HAL_Delay(), this function provides accurate
      delay (in milliseconds) based on variable incremented in HAL time base ISR.
      This implies that if HAL_Delay() is called from a peripheral ISR process, then
      the HAL time base interrupt must have higher priority (numerically lower) than
      the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the HAL time base interrupt priority you have to use HAL_NVIC_SetPriority()
      function.
 
@note The application needs to ensure that the HAL time base is always set to 1 millisecond
      to have correct HAL operation.

@note The FreeRTOS heap size configTOTAL_HEAP_SIZE defined in FreeRTOSConfig.h is set accordingly to the 
      OS resources memory requirements of the application with +10% margin and rounded to the upper Kbyte boundary.

For more details about FreeRTOS implementation on STM32Cube, please refer to UM1722 "Developing Applications 
on STM32Cube with RTOS".


@par Directory contents
    - FreeRTOS/FreeRTOS_ThreadCreation/Src/main.c                       Main program
    - FreeRTOS/FreeRTOS_ThreadCreation/Src/stm32f0xx_hal_timebase_tim.c HAL timebase file
    - FreeRTOS/FreeRTOS_ThreadCreation/Src/stm32f0xx_it.c               Interrupt handlers
    - FreeRTOS/FreeRTOS_ThreadCreation/Src/system_stm32f0xx.c           STM32F0xx system clock configuration file
    - FreeRTOS/FreeRTOS_ThreadCreation/Inc/main.h                       Main program header file
    - FreeRTOS/FreeRTOS_ThreadCreation/Inc/stm32f0xx_hal_conf.h         HAL Library Configuration file
    - FreeRTOS/FreeRTOS_ThreadCreation/Inc/stm32f0xx_it.h               Interrupt handlers header file
    - FreeRTOS/FreeRTOS_ThreadCreation/Inc/FreeRTOSConfig.h             FreeRTOS Configuration file

@par Hardware and Software environment

  - This example runs on STM32F091xC devices.
    
  - This example has been tested with STM32F091RC-Nucleo RevC board and can be
    easily tailored to any other supported device and development board.
    

@par How to use it ?

In order to make the program work, you must do the following:
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example
  

 */
