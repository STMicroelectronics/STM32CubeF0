/**
  @page ADC_RegularConversion_Polling conversion using Polling

  @verbatim
  ******************** (C) COPYRIGHT 2016 STMicroelectronics *******************
  * @file    ADC/ADC_RegularConversion_Polling/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the ADC RegularConversion Polling example.
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

How to use the ADC in Polling mode to convert data through the HAL API.

When the end of conversion occurs, the converted data of ADC1 DR register is 
affected to the uhADCxConvertedValue variable.

Conversion time (reference manual, Reset and Clock control part):
  The function SystemClock_Config() configures the clock divider as follows:
  1) The system clock is 48MHz.
  2) AHB Prescaler = 1 => AHB clock is 48 MHz.
  2) APB Prescaler = 1 => APB clock is 48 MHz.
  3) ADC Prescaler = 4 => ADC clock is 12 MHz. (14 Mhz max)

  Sampling time is set to ADC_SAMPLETIME_28CYCLES_5 (28.5 cycles).
  ConvTime = Sampling time + 12.5 ADC clock cycles.
           = 41 clock cycles
           = 3.42 us

User can vary the ADC_CHANNEL_10 voltage using the Eval Board potentiometer (RV3) connected to PC.00.
The converted value is monitored through debugger: uhADCxConvertedValue variable.

STM32 Eval board's LEDs can be used to monitor the transfer status:
  - LED3 is ON when there is an error in initialization.

@par Directory contents 

  - ADC/ADC_RegularConversion_Polling/Inc/stm32f0xx_hal_conf.h    HAL configuration file
  - ADC/ADC_RegularConversion_Polling/Inc/stm32f0xx_it.h          Interrupt handlers header file
  - ADC/ADC_RegularConversion_Polling/Inc/main.h                  Header for main.c module  
  - ADC/ADC_RegularConversion_Polling/Src/stm32f0xx_it.c          Interrupt handlers
  - ADC/ADC_RegularConversion_Polling/Src/main.c                  Main program
  - ADC/ADC_RegularConversion_Polling/Src/stm32f0xx_hal_msp.c     HAL MSP file 
  - ADC/ADC_RegularConversion_Polling/Src/system_stm32f0xx.c      STM32F0xx system source file

@par Hardware and Software environment 

  - This example runs on STM32F0xx devices.
  
  - This example has been tested with STMicroelectronics STM32072B-EVAL RevB board and can be
    easily tailored to any other supported device and development board.

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example


 */
