/**
  @page RCC_OutputSystemClockOnMCO RCC example
  
  @verbatim
  ******************** (C) COPYRIGHT 2016 STMicroelectronics *******************
  * @file    Examples_LL/RCC/RCC_OutputSystemClockOnMCO/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the RCC example.
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

Configuration of MCO pin (PA8) to output the system clock.

At start-up, USER button and MCO pin are configured. The program configures SYSCLK
to the max frequency using the PLL with HSI48 as clock source.

The signal on PA8 (or pin 23 of CN10 connector) can be monitored with an oscilloscope 
to check the different MCO configuration set at each button press.
Different configuration will be observed
 - SYSCLK frequency with frequency value around @48MHz.
 - HSI48 frequency value divided by 2, hence around @24MHz.
 - PLLCLK frequency value divided by 4, hence around @12MHz.

When user press User push-button, a LED toggle is done to indicate a change in MCO config.

@par Directory contents 

  - RCC/RCC_OutputSystemClockOnMCO/Inc/stm32f0xx_it.h          Interrupt handlers header file
  - RCC/RCC_OutputSystemClockOnMCO/Inc/main.h                  Header for main.c module
  - RCC/RCC_OutputSystemClockOnMCO/Inc/stm32_assert.h          Template file to include assert_failed function
  - RCC/RCC_OutputSystemClockOnMCO/Src/stm32f0xx_it.c          Interrupt handlers
  - RCC/RCC_OutputSystemClockOnMCO/Src/main.c                  Main program
  - RCC/RCC_OutputSystemClockOnMCO/Src/system_stm32f0xx.c      STM32F0xx system source file


@par Hardware and Software environment

  - This example runs on STM32F072xB devices.
    
  - This example has been tested with STM32F072RB-Nucleo RevC board and can be
    easily tailored to any other supported device and development board.

  - STM32F072RB-Nucleo RevC Set-up
    - Connect the MCO pin to an oscilloscope to monitor the different waveforms:
      - PA.08: connected to pin 23 of CN10 connector  

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example


 */
