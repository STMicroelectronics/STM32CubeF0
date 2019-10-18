/**
  @page PWR_CurrentConsumption PWR Current Consumption example
  
  @verbatim
  ******************** (C) COPYRIGHT 2016 STMicroelectronics *******************
  * @file    PWR/PWR_CurrentConsumption/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the PWR Current Consumption example.
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

How to configure the system to measure the current consumption in different 
low-power modes.

The Low Power modes are:
  - SLEEP mode
  - STOP mode
  - STANDBY mode
  
To run this example, the user has to follow the following steps:
 1. Select the low power mode to be measured by uncommenting the corresponding
    line inside the stm32f0xx_lp_modes.h file.
    @code
       /* #define SLEEP_MODE */
       #define STOP_MODE
       /* #define STANDBY_MODE */
    @endcode       

 2. Use an external amperemeter to measure the IDD current. 

 3. This example can not be used in DEBUG mode due to the fact that the 
    Cortex-M0 core is no longer clocked during low power mode and so debugging 
    features are not available.

Here below a detailed description of the example code:

  @verbatim

 1. After reset, the program waits for User push-button connected to the PC.13 pin 
    to be pressed to enter the selected low power mode.
    Before that key press, LED2 is blinking.
    
     - Press again the User push-button to exit the low power mode.
     
  2. After exit from low power mode, LED2 is blinking again.
    In the case of exit from stand-by mode, LED2 is first ON for two seconds before
    starting blinking.
    
  3. The sequence can be repeated from step 1 in following the same steps (pressing
     the USER button to enter the desired low power mode)

  In case of error, LED2 transmits a sequence of three dots, three dashes, three dots.

   Low power modes description:

    - Sleep Mode
    ============  
            - System Running at PLL (48 MHz)
            - Flash 2 wait state
            - Instruction and Data caches ON
            - Prefetch OFF       
            - Code running from Internal FLASH
            - All peripherals disabled.
            - Wakeup using EXTI Line (User push-button PC.13)

    - STOP Mode
    ===========
            - RTC Clocked by LSE or LSI
            - Regulator in LP mode
            - HSI and LSI OFF
            - No IWDG
            - Wakeup using EXTI Line (User push-button PC.13)            


    - STANDBY Mode
    ==============
            - RTC OFF
            - IWDG and LSI OFF 
            - Wakeup using WakeUp Pin PWR_WAKEUP_PIN2 connected to PC.13

   @endverbatim

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

  - PWR/PWR_CurrentConsumption/Inc/stm32f0xx_conf.h         HAL Configuration file
  - PWR/PWR_CurrentConsumption/Inc/stm32f0xx_it.h           Header for stm32f0xx_it.c
  - PWR/PWR_CurrentConsumption/Inc/main.h                   Header file for main.c
  - PWR/PWR_CurrentConsumption/Inc/stm32f0xx_lp_modes.h     STM32F0xx Low Power Modes header file
  - PWR/PWR_CurrentConsumption/Src/stm32f0xx_it.c           Interrupt handlers
  - PWR/PWR_CurrentConsumption/Src/main.c                   Main program
  - PWR/PWR_CurrentConsumption/Src/stm32f0xx_hal_msp.c      HAL MSP module
  - PWR/PWR_CurrentConsumption/Src/stm32f0xx_lp_modes.c     STM32F0xx Low Power Modes source file
  - PWR/PWR_CurrentConsumption/Src/system_stm32f0xx.c       stm32f0xx system source file

@par Hardware and Software environment

  - This example runs on STM32F030R8 devices.
    
  - This example has been tested with STM32F030R8-Nucleo Rev C board embedding 
    a STM32F030R8T6 device and can be
    easily tailored to any other supported device and development board.    
      
  - STM32F030R8-Nucleo Rev C Set-up
    - Use LED2 connected to PA.05 pin.
      * LED2 toggles while waiting for the USER push-button to be pressed to enter the low power mode.
    - Remove JP6 jumper and connect an amperemeter to JP6 to measure IDD current.

@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example


 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
