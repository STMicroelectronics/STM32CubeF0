/**
  @page ADC_LowPower ADC conversion example using related peripherals (GPIO, Timer),
   user control by push button and LED.

  @verbatim
  ******************** (C) COPYRIGHT 2016 STMicroelectronics *******************
  * @file    ADC/ADC_LowPower/readme.txt 
  * @author  MCD Application Team
  * @brief   brief   Description of the ADC Low Power modes example.
  ******************************************************************************
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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
