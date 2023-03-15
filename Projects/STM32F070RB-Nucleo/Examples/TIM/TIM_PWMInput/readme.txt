/**
  @page TIM_PWMInput TIM PWM Input example
  
  @verbatim
  ******************** (C) COPYRIGHT 2016 STMicroelectronics *******************
  * @file    TIM/TIM_PWMInput/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the TIM PWM_Input example.
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

Use of the TIM peripheral to measure the frequency and 
duty cycle of an external signal.

The TIM3CLK frequency is set to SystemCoreClock (Hz), the Prescaler is 0 so the 
counter clock is SystemCoreClock (Hz).
SystemCoreClock is set to 48 MHz for STM32F070xB Devices.

TIM3 is configured in PWM Input Mode: the external signal is connected to 
TIM3 Channel2 used as input pin.
To measure the frequency and the duty cycle we use the TIM3 CC2 interrupt request,
so in the TIM3_IRQHandler routine, the frequency and the duty cycle of the external 
signal are computed.

"uwFrequency" variable contains the external signal frequency:
TIM3 counter clock = SystemCoreClock,
Frequency = TIM3 counter clock / TIM3_CCR2 in Hz, 

"DutyCycle" variable contains the external signal duty cycle:
DutyCycle = (TIM3_CCR1*100)/(TIM3_CCR2) in %.

The minimum frequency value to measure is (TIM3 counter clock / CCR MAX)
                                         = (48 MHz)/ 65535
                                         = 732 Hz

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Directory contents 

  - TIM/TIM_PWMInput/Inc/stm32f0xx_hal_conf.h    HAL configuration file
  - TIM/TIM_PWMInput/Inc/stm32f0xx_it.h          Interrupt handlers header file
  - TIM/TIM_PWMInput/Inc/main.h                  Header for main.c module  
  - TIM/TIM_PWMInput/Src/stm32f0xx_it.c          Interrupt handlers
  - TIM/TIM_PWMInput/Src/main.c                  Main program
  - TIM/TIM_PWMInput/Src/stm32f0xx_hal_msp.c     HAL MSP file
  - TIM/TIM_PWMInput/Src/system_stm32f0xx.c      STM32F0xx system source file


@par Hardware and Software environment

  - This example runs on STM32F070xB devices.
    
  - This example has been tested with STMicroelectronics STM32F070RB-Nucleo RevC 
    board and can be easily tailored to any other supported device 
    and development board.

  - STM32F070RB-Nucleo RevC Set-up
    - Connect the external signal to measure to the TIM3 CH2 pin (PB.05: connected to pin 29 in CN10 connector).

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example


 */
