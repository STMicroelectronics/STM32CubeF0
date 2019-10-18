/**
  @page TIM_PWMOutput TIM PWM Output example
  
  @verbatim
  ******************** (C) COPYRIGHT 2016 STMicroelectronics *******************
  * @file    TIM/TIM_PWMOutput/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the PWM signals generation using TIM2
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

Configuration of the TIM peripheral in PWM (pulse width modulation) mode.

SystemCoreClock is set to 48 MHz for STM32F0xx Devices.

    In this example TIM2 input clock (TIM2CLK) is set to APB1 clock (PCLK1),
    since APB1 prescaler is equal to 1.
      TIM2CLK = PCLK1
      PCLK1 = HCLK
      => TIM2CLK = HCLK = SystemCoreClock

    To get TIM2 counter clock at 16 MHz, the prescaler is computed as follows:
       Prescaler = (TIM2CLK / TIM2 counter clock) - 1
       Prescaler = ((SystemCoreClock) /16 MHz) - 1

    To get TIM2 output clock at 24 KHz, the period (ARR)) is computed as follows:
       ARR = (TIM2 counter clock / TIM2 output clock) - 1
           = 665

    TIM2 Channel1 duty cycle = (TIM2_CCR1/ TIM2_ARR + 1)* 100 = 50%
    TIM2 Channel2 duty cycle = (TIM2_CCR2/ TIM2_ARR + 1)* 100 = 37.5%
    TIM2 Channel3 duty cycle = (TIM2_CCR3/ TIM2_ARR + 1)* 100 = 25%
    TIM2 Channel4 duty cycle = (TIM2_CCR4/ TIM2_ARR + 1)* 100 = 12.5%


The PWM waveforms can be displayed using an oscilloscope.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.


@par Directory contents 

  - TIM/TIM_PWMOutput/Inc/stm32f0xx_hal_conf.h    HAL configuration file
  - TIM/TIM_PWMOutput/Inc/stm32f0xx_it.h          Interrupt handlers header file
  - TIM/TIM_PWMOutput/Inc/main.h                  Header for main.c module  
  - TIM/TIM_PWMOutput/Src/stm32f0xx_it.c          Interrupt handlers
  - TIM/TIM_PWMOutput/Src/main.c                  Main program
  - TIM/TIM_PWMOutput/Src/stm32f0xx_hal_msp.c     HAL MSP file
  - TIM/TIM_PWMOutput/Src/system_stm32f0xx.c      STM32F0xx system source file


@par Hardware and Software environment

  - This example runs on STM32F072xB devices.
  - In this example, the clock is set to 48 MHz.
    
  - This example has been tested with STMicroelectronics STM32F072RB-Nucleo RevC 
    board and can be easily tailored to any other supported device 
    and development board.      

  - STM32F072RB-Nucleo RevC Set-up
   Connect the following pins to an oscilloscope to monitor the different waveforms:
        - TIM2_CH1 : PA.05 (pin 11 in CN10 connector)
        - TIM2_CH2 : PB.03 (pin 31 in CN10 connector)
        - TIM2_CH3 : PB.10 (pin 25 in CN10 connector)
        - TIM2_CH4 : PB.11 (pin 18 in CN10 connector)


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
