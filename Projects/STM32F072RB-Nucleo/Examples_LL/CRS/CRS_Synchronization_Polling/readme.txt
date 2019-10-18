/**
  @page CRS_Synchronization_Polling CRS example
  
  @verbatim
  ******************** (C) COPYRIGHT 2016 STMicroelectronics *******************
  * @file    Examples_LL/CRS/CRS_Synchronization_Polling/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the CRS_Synchronization_Polling example.
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

How to configure the clock recovery service in polling mode through the
STM32F0xx CRS LL API. The peripheral initialization uses LL unitary
service functions for optimization purposes (performance and size).

At the beginning of the main program the SystemClock_Config() function is used to 
configure the system clock (SYSCLK) to run at 48 MHz.

Then, HSI48 and LSE oscillators are enabled. In this example CRS will use LSE 
oscillator to calibrate HSI48 frequency.
When HSI48 and LSE are started, automatic calibration starts and program call the
polling function to wait for synchronization status.

In this test, there are results which could occur:
 - Wait for User push-button press to start the test (Fast toggle LED2)
 - few loops occurs up to SYNC OK event (if SYNC WARN, HSI48 trimming value will be
 automatically modified)
 - in case of SYNC MISS or SYNC ERROR, there is an issue with synchronization input
 parameters. In this case, user need to apply new parameters after checking synchronization
 information and restart the synchronization.
 - LED2 is slowly blinking (1 sec. period) in case of timeout during polling 
 procedure. It may due to an issue with LSE synchronization frequency.
 - LED2 will finally power on if SYNC OK is returned by CRS.
 
@par Directory contents 

  - CRS/CRS_Synchronization_Polling/Inc/stm32f0xx_it.h          Interrupt handlers header file
  - CRS/CRS_Synchronization_Polling/Inc/main.h                  Header for main.c module
  - CRS/CRS_Synchronization_Polling/Inc/stm32_assert.h          Template file to include assert_failed function
  - CRS/CRS_Synchronization_Polling/Src/stm32f0xx_it.c          Interrupt handlers
  - CRS/CRS_Synchronization_Polling/Src/main.c                  Main program
  - CRS/CRS_Synchronization_Polling/Src/system_stm32f0xx.c      STM32F0xx system source file


@par Hardware and Software environment

  - This example runs on STM32F072xB devices.
    
  - This example has been tested with STM32F072RB-Nucleo RevC board and can be
    easily tailored to any other supported device and development board.

  - STM32F072RB-Nucleo RevC Set-up
    - Connect the MCO pin to an oscilloscope to monitor HSI48 frequency:
      - PA.08: connected to pin 23 of CN10 connector  


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
