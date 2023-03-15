/**
  @page TIM_TimeBase TIM example
  
  @verbatim
  ******************** (C) COPYRIGHT 2016 STMicroelectronics *******************
  * @file    Examples_LL/TIM/TIM_TimeBase/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the TIM_TimeBase example.
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

Configuration of the TIM peripheral to generate a timebase. This 
example using the STM32F0xx TIM LL API.The peripheral initialization
uses LL unitary services functions for optimization purposes (performance and size).

In this example TIM2 input clock (TIM2CLK)  is set to APB1 clock (PCLK1),
since APB1 prescaler is equal to 1.
   TIM2CLK = PCLK1
   PCLK1 = HCLK
   => TIM2CLK = HCLK = SystemCoreClock (48 MHz)

To set the TIM2 counter clock frequency to 10 KHz, the pre-scaler (PSC) is calculated as follows:
PSC = (TIM2CLK / TIM2 counter clock) - 1
PSC = (SystemCoreClock /10 KHz) - 1

SystemCoreClock is set to 48 MHz for STM32F0xx Devices.

The auto-reload (ARR) is calculated to get a timebase period of 100ms,
meaning that initial timebase frequency is 10 Hz.
ARR = (TIM2 counter clock / timebase frequency) - 1
ARR = (TIM2 counter clock / 10) - 1

Update interrupts are enabled. Within the update interrupt service routine pin PA.05
(connected to LED2 on board STM32F072RB-Nucleo RevC) is toggled.

User push-button can be used to modify the timebase period from 100 ms
to 1 s in 100 ms steps. To do so, every time User push-button is pressed, the
autoreload register (ARR) is updated. In up-counting update event is generated 
at each counter overflow (when the counter reaches the auto-reload value). 

Finally the timebase frequency is calculated as follows:
timebase frequency = TIM2 counter clock /((PSC + 1)*(ARR + 1)*(RCR + 1))

@par Directory contents 

  - TIM/TIM_TimeBase/Inc/stm32f0xx_it.h          Interrupt handlers header file
  - TIM/TIM_TimeBase/Inc/main.h                  Header for main.c module
  - TIM/TIM_TimeBase/Inc/stm32_assert.h          Template file to include assert_failed function
  - TIM/TIM_TimeBase/Src/stm32f0xx_it.c          Interrupt handlers
  - TIM/TIM_TimeBase/Src/main.c                  Main program
  - TIM/TIM_TimeBase/Src/system_stm32f0xx.c      STM32F0xx system source file


@par Hardware and Software environment

  - This example runs on STM32F072xB devices.
    
  - This example has been tested with STM32F072RB-Nucleo RevC board and can be
    easily tailored to any other supported device and development board.


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example


 */
