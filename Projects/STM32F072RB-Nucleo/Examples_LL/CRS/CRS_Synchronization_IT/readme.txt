/**
  @page CRS_Synchronization_IT CRS example
  
  @verbatim
  ******************** (C) COPYRIGHT 2016 STMicroelectronics *******************
  * @file    Examples_LL/CRS/CRS_Synchronization_IT/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the CRS_Synchronization_IT example.
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

How to configure the clock recovery service in IT mode through the
STM32F0xx CRS LL API. The peripheral initialization uses LL unitary
service functions for optimization purposes (performance and size).

At the beginning of the main program the SystemClock_Config() function is used to 
configure the system clock (SYSCLK) to run at 48 MHz.

Then, HSI48 and LSE oscillators are enabled. 
In this example, after pressing the user button, as a first step a synchronization miss is raised 
when using a wrong synchronization source i.e. GPIO with nothing connected (instead of LSE). 
In a 2nd step, a reconfiguration is done to use LSE source and reach the correct synchronization.
HSI48 synchronize to LSE (32768Hz), adjusting the trimmer such to get the counting to 48000000/32768. 

In this test, this is the scenario:
 - set the source to GPIO and then enable IT
 - Wait for User push-button press to start the test (Fast toggle LED2)
 - to provoke a SYNC event, call the software event generation function
 - Interrupt is raised to indicate a SYNC MISS and LED2 remains off
 - in main program, wait for 1 sec and reconfigure source to LSE
 - Interrupt is raised to indicate a SYNC OK and then power on LED2
 - Toggle quickly LED2 when SYNC WARN is detected
 - Increase tolerance (FELIM value) when SYNC WARN is detected
 - At the end of the test LED2 should stay ON

@par Directory contents 

  - CRS/CRS_Synchronization_IT/Inc/stm32f0xx_it.h          Interrupt handlers header file
  - CRS/CRS_Synchronization_IT/Inc/main.h                  Header for main.c module
  - CRS/CRS_Synchronization_IT/Inc/stm32_assert.h          Template file to include assert_failed function
  - CRS/CRS_Synchronization_IT/Src/stm32f0xx_it.c          Interrupt handlers
  - CRS/CRS_Synchronization_IT/Src/main.c                  Main program
  - CRS/CRS_Synchronization_IT/Src/system_stm32f0xx.c      STM32F0xx system source file


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


 */
