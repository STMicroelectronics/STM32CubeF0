/**
  @page FreeRTOS_LowPower FreeRTOS Low Power example
 
  @verbatim
  ******************************************************************************
  * @file    FreeRTOS/FreeRTOS_LowPower/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the FreeRTOS low power example.
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

How to enter and exit low-power mode with CMSIS RTOS API.

This example creates two threads.

   + An Rx thread that blocks on a queue to wait for data, blinking the LED1 each 
     time data is received (turning it on and then off again) before returning 
     to block on the queue once more.

   + A Tx thread that repeatedly enters blocked state for 500ms.  
     On exiting the blocked state the Tx thread sends a value through the queue 
     to the Rx thread (causing the Rx thread to exit the blocked state and blink 
     the LED1).

Blocking for a finite period allows the kernel to stop tick interrupt
and place STM32 into Sleep mode - the lowest-power mode possible
that allows the CPU registers and RAM to retain their state.

In this example, non-used GPIOs are configured to analog, thus helping to reduce 
the power consumption of the device.

Observed behaviour:

Every 500ms the MCU will come out of the low power state to turn the LED1 on,
then return to the low power state for 20ms before leaving the low power
state again to turn the LED1 off.  This will be observed as a fast blinking
on the LED1.

The RTOS tick is suppressed while the MCU is in its low power state.

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
    - FreeRTOS/FreeRTOS_LowPower/Src/main.c                       Main program
    - FreeRTOS/FreeRTOS_LowPower/Src/stm32f0xx_hal_timebase_tim.c HAL timebase file
    - FreeRTOS/FreeRTOS_LowPower/Src/stm32f0xx_it.c               Interrupt handlers
    - FreeRTOS/FreeRTOS_LowPower/Src/system_stm32f0xx.c           STM32F0xx system clock configuration file
    - FreeRTOS/FreeRTOS_LowPower/Inc/main.h                       Main program header file
    - FreeRTOS/FreeRTOS_LowPower/Inc/stm32f0xx_hal_conf.h         HAL Library Configuration file
    - FreeRTOS/FreeRTOS_LowPower/Inc/stm32f0xx_it.h               Interrupt handlers header file
    - FreeRTOS/FreeRTOS_LowPower/Inc/FreeRTOSConfig.h             FreeRTOS Configuration file

@par Hardware and Software environment

  - This example runs on STM32072xB devices.
    
  - This example has been tested with STM32072B-EVAL RevB board and can be
    easily tailored to any other supported device and development board.
    

@par How to use it ?

In order to make the program work, you must do the following:
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example
  

 */
