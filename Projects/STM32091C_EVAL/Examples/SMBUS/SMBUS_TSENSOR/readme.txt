/**
  @page SMBUS_TSENSOR SMBUS Temperature Sensor example
  
  @verbatim
  ******************** (C) COPYRIGHT 2016 STMicroelectronics *******************
  * @file    SMBUS/SMBUS_TSENSOR/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the SMBUS Temperature Sensor example.
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

MBUS data buffer transmission/reception using an interrupt. The STM32 microcontroller communicates with an SMBUS temperature sensor.

   __________________________________________________________________________                        
  |           ______________                        ______________           |
  |          | I2C1         |                      |   TSENSOR    |          |
  |          |              |                      |              |          |
  |          |      SCL(PB6)|______________________|CLOCK         |          |
  |          |              |                      |              |          |
  |          |              |                      |              |          |
  |          |              |                      |              |          |
  |          |      SDA(PB7)|______________________|DATA          |          |
  |          |              |                      |              |          |
  |          |SMB alert(PB5)|______________________|EVENT         |          |
  |          |______________|                      |______________|          |
  |      __                                                                  |
  |     |__|                                                                 |
  |     Tamper push-button                                                                  |
  |                                                                          |
  |__________________________________________________________________________|
      STM32091C-EVAL

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system
clock (SYSCLK) to run at 48 MHz.

The temperature sensor configuration is ensured by TSENSOR_Init() which verify that
temperature sensor is ready to communicate by calling TSENSOR_IsReady(), then
set the configuration using HAL_SMBUS_Master_Transmit_IT() and HAL_SMBUS_EnableAlert_IT()

The SMBUS peripheral configuration is ensured by the HAL_SMBUS_Init()
and HAL_SMBUS_IsDeviceReady() called respectively in TSENSOR_Init() function.
This later is calling the HAL_SMBUS_MspInit()function which core is implementing
the configuration of the needed SMBUS resources according to the used hardware (CLOCK, 
GPIO and NVIC). You may update this function to change SMBUS configuration.

The SMBUS/Temperature Sensor communication is then initiated.

Then at each loop in main program a read and a display on LCD of the current temperature
is done each second.
An specific warning message is display on LCD in case of an alert from temperature sensor
is detected.

This management is perform using the TSENSOR_ReadTemp()and TSENSOR_ReadStatus() functions 
allow respectively the read of the current temperature and get the temperature sensor status.

STM32091C-EVAL's Messages and LEDs can be used to monitor the temperature sensor status:
 - LED3 is ON when there is an error during communication with the temperature sensor.
 - Please refer to the Figure 5. "OS output temperature response diagram" in the following document :
 http://www.st.com/web/en/resource/technical/document/datasheet/CD00153511.pdf
   - In case of the Temperature high limit interrupt is raised, this warning message is displayed on the LCD :
                                    Temperature Limit High
                                       has been exceeded

   - In case of the low Temperature interrupt is raised, a warning message is displayed on the LCD :
                                      Temperature is at or
                                       below the Low Limit

@note TSENSOR_SMBUS instance used and associated resources can be updated in "stm32f0xx_tsensor.c"
and "main.h" files depending hardware configuration used.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Directory contents 

  - SMBUS/SMBUS_TSENSOR/Inc/stm32f0xx_hal_conf.h    HAL configuration file
  - SMBUS/SMBUS_TSENSOR/Inc/stm32f0xx_it.h          Interrupt handlers header file
  - SMBUS/SMBUS_TSENSOR/Inc/main.h                  Header for main.c module  
  - SMBUS/SMBUS_TSENSOR/Inc/stm32f0xx_tsensor.h     Temperature Sensor driver header file
  - SMBUS/SMBUS_TSENSOR/Src/stm32f0xx_it.c          Interrupt handlers
  - SMBUS/SMBUS_TSENSOR/Src/main.c                  Main program
  - SMBUS/SMBUS_TSENSOR/Src/system_stm32f0xx.c      STM32F0xx system source file
  - SMBUS/SMBUS_TSENSOR/Src/stm32f0xx_hal_msp.c     HAL MSP file    
  - SMBUS/SMBUS_TSENSOR/Src/stm32f0xx_tsensor.c     Temperature Sensor driver file

@par Hardware and Software environment

  - This example runs on STM32F0xx devices.
    
  - This example has been tested with STM32091C-EVAL board and can be
    easily tailored to any other supported device and development board.    

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example


 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
