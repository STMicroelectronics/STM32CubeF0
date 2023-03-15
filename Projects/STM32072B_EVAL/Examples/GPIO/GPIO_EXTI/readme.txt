/**
  @page GPIO_EXTI GPIO EXTI example
  
  @verbatim
  ******************** (C) COPYRIGHT 2016 STMicroelectronics *******************
  * @file    GPIO/GPIO_EXTI/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the GPIO EXTI example.
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

How to configure external interrupt lines.

In this example, 2 EXTI lines (EXTI4_15 and EXTI0_1) are configured to generate
an interrupt on each rising and falling edge, respectively. 
In the interrupt routine a led connected to a specific GPIO pin is toggled.

In this example:
    - EXTI4_15 is connected to PC.13 pin
      - when falling edge is detected on EXTI4_15 by pressing Tamper push-button, LED1 toggles

    - EXTI0_1 is connected to PA.00 pin
      - when falling edge is detected on EXTI0_1, LED2 toggles
      
On STM32072B-EVAL:
    - EXTI4_15 is connected to Tamper push-button
    - EXTI0_1 is connected to Joystick Sel push-button

In this example, HCLK is configured at 48 MHz.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Directory contents 

  - GPIO/GPIO_EXTI/Inc/stm32f0xx_hal_conf.h    HAL configuration file
  - GPIO/GPIO_EXTI/Inc/stm32f0xx_it.h          Interrupt handlers header file
  - GPIO/GPIO_EXTI/Inc/main.h                  Header for main.c module  
  - GPIO/GPIO_EXTI/Src/stm32f0xx_it.c          Interrupt handlers
  - GPIO/GPIO_EXTI/Src/main.c                  Main program
  - GPIO/GPIO_EXTI/Src/system_stm32f0xx.c      STM32F0xx system source file

@par Hardware and Software environment

  - This example runs on STM32F072VB devices.
    
  - This example has been tested with STMicroelectronics STM32072B-EVAL RevB board and can be
    easily tailored to any other supported device and development board.

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example



 */
