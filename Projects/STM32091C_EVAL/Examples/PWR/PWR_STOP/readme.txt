/**
  @page PWR_STOP Power Stop Mode Example
  
  @verbatim
  ******************** (C) COPYRIGHT 2016 STMicroelectronics *******************
  * @file    PWR/PWR_STOP/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the Power Stop Mode example.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2016 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  @endverbatim

@par Example Description 
 
How to enter the Stop mode and wake up from this mode by using the RTC wakeup 
timer event or an interrupt.

In the associated software
  - the system clock is set to 48 MHz
  - the EXTI4_15 is configured to generate interrupt on falling edge
  - the EXTI_Line22 connected internally to the RTC Wakeup event is configured
    to generate an interrupt on rising edge each 4s
  - the SysTick is programmed to generate an interrupt each 1 ms 
In the SysTick interrupt handler, LED2 is toggled, this is used to indicate whether
the MCU is in STOP or RUN mode.

The system enters STOP mode and will wait for the RTC Wakeup event to be 
generated each 4s, or Key push button is pressed. 
 - If the RTC WakeUp event (EXTI_Line20) is the source of wakeup from STOP, LED1 is toggled.
 - If the Tamper push-button (EXTI4_15) is the source of wakeup from STOP, LED4 is toggled.

This behavior is repeated in an infinite loop.

LEDs are used to monitor the system state as following:
 - LED1 toggled: system woken up from STOP using RTC WakeUp Interrupt
 - LED2 toggling: system in RUN mode
 - LED3 Initialization error (RTC, RCC,...)
 - LED4 toggled: system woken up from STOP using EXTI4_15 (Tamper push-button)

 @note To measure the current consumption in STOP mode, please refer to 
      @subpage PWR_CurrentConsumption example.

@note This example can not be used in DEBUG mode, this is due to the fact 
      that the Cortex-M0 core is no longer clocked during low power mode 
      so debugging features are disbaled

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.
      
@note  Care must be taken when HAL_RCCEx_PeriphCLKConfig() is used to select the RTC clock source; in this 
       case the Backup domain will be reset in order to modify the RTC Clock source, as consequence RTC  
       registers (including the backup registers) and RCC_BDCR register are set to their reset values.

@par Directory contents 

  - PWR/PWR_STOP/Inc/stm32f0xx_conf.h         HAL Configuration file
  - PWR/PWR_STOP/Inc/stm32f0xx_it.h           Header for stm32f0xx_it.c
  - PWR/PWR_STOP/Inc/main.h                   Header file for main.c
  - PWR/PWR_STOP/Src/system_stm32f0xx.c       STM32F0xx system clock configuration file
  - PWR/PWR_STOP/Src/stm32f0xx_it.c           Interrupt handlers
  - PWR/PWR_STOP/Src/main.c                   Main program
  - PWR/PWR_STOP/Src/stm32f0xx_hal_msp.c      HAL MSP module

@par Hardware and Software environment

  - This example runs on STM32F0xx devices
    
      
  - This example has been tested with STMicroelectronics STM32091C-EVAL
    evaluation board and can be easily tailored to any other supported device 
    and development board.

  - STM32091C-EVAL Set-up
    - Use LED1, LED2, LED3 and LED4 connected respectively to PE.00, PE.01, PE.07 and PE.08 pins
    - Use the Tamper push-button connected to pin PC.13 (EXTI4_15)

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example


 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
