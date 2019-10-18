/**
  @page ADC_LowPower ADC conversion example using related peripherals (GPIO, Timer),
   user control by push button and LED.

  @verbatim
  ******************** (C) COPYRIGHT 2016 STMicroelectronics *******************
  * @file    ADC/ADC_LowPower/readme.txt 
  * @author  MCD Application Team
  * @brief   brief   Description of the ADC Low Power modes example.
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

How to use the ADC peripheral to perform conversions with ADC low-power modes: 
auto-wait and auto-power off.

Other peripherals related to ADC are used:
Mandatory:
 - GPIO peripheral is used in analog mode to drive signal from device pin to
   ADC input.
Optionally:
 - Timer peripheral is used to trig ADC conversions.

ADC settings:
  Trig of conversion start done by external event (timer at 1kHz).
  Continuous mode disabled to have only 1 conversion at each conversion trig.
ADC conversion results:
 - ADC conversions results are transferred manually (by software), into variable
   array "uhADCxConvertedValue".
Board settings:
 - ADC is configured to convert ADC_CHANNEL_10 (pin PC.00).
 - The voltage input on ADC channel is provided from potentiometer RV3 (which is connected to
ADC_CHANNEL_10 (pin PC.00) on STM32072B-EVAL).
   Turning this potentiometer will make the voltage vary into full range: from 0 to Vdda (3.3V).
   ==> Therefore, there is no external connection needed to run this example.

How to use this example:
ADC overrun feature is used to watch the ADC low power modes effectiveness:
 - If definition label "#define ADC_LOWPOWER" is commented, then ADC does not use
   low power features.
   Trigger events are occurring continuously (from timer trigger out), ADC performs
   a conversion at each trigger event.
   ADC conversion results are not fetch after each conversion, therefore some results
   are overwritten and ADC overrun triggers.
   => Press Tamper push-button (fetch an ADC conversion result and refresh LED state),
   then LED1 should turn on.
   
 - If definition label "#define ADC_LOWPOWER" is uncommented, then ADC does use
   low power features.
   Trigger events are occurring continuously (from timer trigger out), but ADC does not
   perform a conversion at each trigger event: after the first ADC conversion, ADC does
   not start a new conversion until the previous conversion result has been fetched
   (done by function "HAL_ADC_GetValue(&AdcHandle)" into the example: this is ADC feature
   "LowPowerAutoWait".
   Moreover, ADC is powered off during this idle phase: this is ADC feature
   "LowPowerAutoPowerOff".
   When user press Tamper push-button, ADC result is fetched and ADC is freed from idle phase:
   a new conversion starts at the next trigger event.
   ADC conversion results are never overwritten, ADC overrun does not trigger
   => Press Tamper push-button (fetch an ADC conversion result and refresh LED state),
      then LED1 should remain turned off.


@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Directory contents 

  - ADC/ADC_LowPower/Inc/stm32f0xx_hal_conf.h    HAL configuration file
  - ADC/ADC_LowPower/Inc/stm32f0xx_it.h          HAL interrupt handlers header file
  - ADC/ADC_LowPower/Inc/main.h                  Header for main.c module  
  - ADC/ADC_LowPower/Src/stm32f0xx_it.c          HAL interrupt handlers
  - ADC/ADC_LowPower/Src/main.c                  Main program
  - ADC/ADC_LowPower/Src/stm32f0xx_hal_msp.c     HAL MSP file
  - ADC/ADC_LowPower/Src/system_stm32f0xx.c      STM32F0xx system source file


@par Hardware and Software environment 

  - This example runs on STM32F0xx devices.

  - This example has been tested with STM32072B-EVAL RevB board and can be
    easily tailored to any other supported device and development board. 

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
