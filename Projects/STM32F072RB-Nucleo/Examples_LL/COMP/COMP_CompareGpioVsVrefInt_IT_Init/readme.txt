/**
  @page COMP_CompareGpioVsVrefInt_IT_Init COMP example
  
  @verbatim
  ******************** (C) COPYRIGHT 2016 STMicroelectronics *******************
  * @file    Examples_LL/COMP/COMP_CompareGpioVsVrefInt_IT_Init/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the COMP_CompareGpioVsVrefInt_IT_Init example.
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

How to use a comparator peripheral to compare a voltage level applied on a GPIO
pin to the the internal voltage reference (VREFINT), in interrupt mode. This example
is based on the STM32F0xx COMP LL API. The peripheral initialization
uses the LL initialization function to demonstrate LL init usage.

Example configuration:
Comparator instance COMP1 is configured to compare:
 - input plus set to a GPIO pin (cf pin below) in analog mode.
 - input minus set to internal voltage reference VrefInt (voltage level 1.2V,
   refer to device datasheet for min/typ/max values)
Comparator interruption is enabled through EXTI line 21 with trigger edge
set to both edges rising and falling.
Comparator is set in power mode ultra low power.

Example execution:
From the main program execution, comparator is enabled.
Each time the voltage level applied on GPIO pin (comparator input plus) 
is crossing VrefInt voltage threshold (comparator input minus),
the comparator generates an interruption.
LED is turned on or off depending on comparator output state:
turned on if comparator voltage level on input plus is higher than input minus,
turned-off otherwise.

Connection needed:
A voltage must be supplied to the analog input pin (cf pin below),
between Vdda=3.3V and 0V. Otherwise this pin can be let floating (but in this
case COMP comparison level will be undetermined).

To check comparator threshold, you can:
 - Either use a wire to connect the GPIO pin of comparator input (cf pin below)
   to other connectors on board:
    - below comparator threshold: to ground
    - above comparator threshold: to voltage of power supply Vdda
      (on board Nucleo: Vdda=3.3V, available on CN6 pin 4 or CN7 pin 5)
 - Either use an external power supply

Other peripheral used:
  1 GPIO for LED
  1 GPIO for analog input of comparator: PA.01 (Arduino connector CN8 pin A1, Morpho connector CN7 pin 30)
  EXTI line 21 (connected to COMP1 output)


@par Directory contents 

  - COMP/COMP_CompareGpioVsVrefInt_IT_Init/Inc/stm32f0xx_it.h          Interrupt handlers header file
  - COMP/COMP_CompareGpioVsVrefInt_IT_Init/Inc/main.h                  Header for main.c module
  - COMP/COMP_CompareGpioVsVrefInt_IT_Init/Inc/stm32_assert.h          Template file to include assert_failed function
  - COMP/COMP_CompareGpioVsVrefInt_IT_Init/Src/stm32f0xx_it.c          Interrupt handlers
  - COMP/COMP_CompareGpioVsVrefInt_IT_Init/Src/main.c                  Main program
  - COMP/COMP_CompareGpioVsVrefInt_IT_Init/Src/system_stm32f0xx.c      STM32F0xx system source file


@par Hardware and Software environment

  - This example runs on STM32F072xx devices.
    
  - This example has been tested with STM32F072RB-Nucleo RevC board and can be
    easily tailored to any other supported device and development board.


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
