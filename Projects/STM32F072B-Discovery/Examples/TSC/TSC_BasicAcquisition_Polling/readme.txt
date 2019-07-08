/**
  @page TSC_BasicAcquisition_Polling Touch-Sensing basic acquisition using polling

  @verbatim
  ******************** (C) COPYRIGHT 2016 STMicroelectronics *******************
  * @file    TSC/TSC_BasicAcquisition_Polling/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the TSC basic acquisition polling example.
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

Use of the TSC to perform continuous acquisitions 
of one channel in Polling mode.

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system
clock (SYSCLK) to run at 48 MHz.

The TSC peripheral configuration is ensured by the HAL_TSC_Init() function.
This later is calling the HAL_TSC_MspInit()function which core is implementing
the configuration of the needed TSC resources according to the used hardware (CLOCK, 
GPIO). You may update this function to change TSC configuration.

The TSC acquisition is then initiated.
When the end of acquisition occurs, the acquired data are affected to the
uhTSCAcquisitionValue[] variable.
Depending of value of uhTSCAcquisitionValue[] under the max threshold values,
the corresponding LEDs are set ON.
These max threshold values can be modified in top of main.c, due to disparity link to board layout quality.

STM32F072B-Discovery RevC's LEDs are used as follows:
  - LED4, LED6 and LED5 show the touch activity (low to high pressure)
  - LED3 toggles when an error occurs (TSC initialization, TSC start or assert).
 
@note TSCx instance used and associated resources can be updated in "main.h"
file depending hardware configuration used.

@note TSCx_LOW_THRESHOLD can be modify if expected result is not satisfied.
This values is link to board layout quality.
To calibrate this value, need to check result of uhTSCAcquisitionValue[] when your digits is near the sensor.
The new LOW threshold is around 1% under this value.

@note TSCx_MEDIUM_THRESHOLD can be modify if expected result is not satisfied.
This values is link to board layout quality.
To calibrate this value, need to check result of uhTSCAcquisitionValue[] when your digits just touch the sensor.
The new MEDIUM threshold is around 1% under this value.

@note TSCx_MAX_THRESHOLD can be modify if expected result is not satisfied.
This values is link to board layout quality.
To calibrate this value, need to check result of uhTSCAcquisitionValue[] when your digits hard pressed the sensor.
The new MAX threshold is around 1% under this value.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Directory contents 

  - TSC/TSC_BasicAcquisition_Polling/Inc/stm32f0xx_hal_conf.h    HAL configuration file
  - TSC/TSC_BasicAcquisition_Polling/Inc/stm32f0xx_it.h          Interrupt handlers header file
  - TSC/TSC_BasicAcquisition_Polling/Inc/main.h                  Header for main.c module  
  - TSC/TSC_BasicAcquisition_Polling/Src/stm32f0xx_it.c          Interrupt handlers
  - TSC/TSC_BasicAcquisition_Polling/Src/main.c                  Main program
  - TSC/TSC_BasicAcquisition_Polling/Src/stm32f0xx_hal_msp.c     HAL MSP file 
  - TSC/TSC_BasicAcquisition_Polling/Src/system_stm32f0xx.c      STM32F0xx system source file

@par Hardware and Software environment 

  - This example runs on STM32F072RB devices.
  
  - This example has been tested with STM32F072B-Discovery RevC board and can be
    easily tailored to any other supported device and development board.

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
