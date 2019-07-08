/**
  @page PWR_CurrentConsumption PWR Current Consumption example
  
  @verbatim
  ******************** (C) COPYRIGHT 2016 STMicroelectronics *******************
  * @file    PWR/PWR_CurrentConsumption/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the PWR Current Consumption example.
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

How to configure the system to measure the current consumption in different 
low-power modes.

The Low Power modes are:
  - SLEEP mode
  - STOP mode
  - STANDBY mode
  
To run this example, user has to follow the following steps:
 1. Select the low power modes to be measured by uncommenting the corresponding
    line inside the stm32f0xx_lp_modes.h file.
    @code
       /* #define SLEEP_MODE   */
       /* #define STOP_MODE    */
       /* #define STANDBY_MODE */
    @endcode       

 2. Use an external amperemeter to measure the IDD current. 

 3. This example can not be used in DEBUG mode, this is due to the fact that the 
    Cortex-M0 core is no longer clocked during low power mode so debugging 
    features are disabled.

 4. Initial condition, wire GND to PA0 (connector CN7 : PIN28), for specific standby mode test case.
 
Here below a detailed description of the example code:

  @verbatim

 1. After reset, the program waits for User push-button connected to PC.13 pin 
    to be pressed - LED2 is blinking - to enter the selected low power mode.
     - When the RTC is not used in the low power mode configuration, press
       again the User push-button to exit the low power mode or, in standby mode wire VDD
       to PA.00
    
     - Press again the User push-button to exit the low power mode.
     
  2. After exit from low power mode, LED2 is blinking again.
    In the case of exit from stand-by mode, LED2 is first ON for two seconds before
    starting blinking.
    
  3. The sequence can be repeated from step 1 in following the same steps (pressing
     the USER button to enter the desired low power mode)

  In case of error, LED2 is slowly blinking (1 sec. period).

   Low power modes description:

    - Sleep Mode
    ============  
            - System Running at PLL (48 MHz)
            - Flash 1 wait state
            - Instruction and Data caches ON
            - Prefetch OFF       
            - Code running from Internal FLASH
            - All peripherals disabled.
            - Wakeup using EXTI Line (User push-button PC.13)

    - STOP Mode
    ===========
            - RTC Clocked by LSI
            - Regulator in LP mode
            - HSI and LSI OFF
            - No IWDG
            - Wakeup using EXTI Line (User push-button PC.13)            

    - STANDBY Mode
    ==============
            - RTC OFF
            - IWDG and LSI OFF 
            - Wakeup using wakeup pin (wire Vdd to PA.00)

   @endverbatim

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.
      
@par Directory contents 

  - PWR/PWR_CurrentConsumption/Inc/stm32f0xx_hal_conf.h     HAL Configuration file
  - PWR/PWR_CurrentConsumption/Inc/stm32f0xx_it.h           Interrupt handlers header file
  - PWR/PWR_CurrentConsumption/Inc/main.h                   Header for main.c module
  - PWR/PWR_CurrentConsumption/Inc/stm32f0xx_lp_modes.h     STM32F0xx Low Power Modes header file
  - PWR/PWR_CurrentConsumption/Src/stm32f0xx_it.c           Interrupt handlers
  - PWR/PWR_CurrentConsumption/Src/main.c                   Main program
  - PWR/PWR_CurrentConsumption/Src/stm32f0xx_hal_msp.c      HAL MSP module
  - PWR/PWR_CurrentConsumption/Src/stm32f0xx_lp_modes.c     STM32F0xx Low Power Modes source file
  - PWR/PWR_CurrentConsumption/Src/system_stm32f0xx.c       stm32f0xx system source file

@par Hardware and Software environment

  - This example runs on STM32F070xx devices
    
  - This example has been tested with STM32F070RB-Nucleo RevC board embedding 
    a STM32F070RBT6 device and can be
    easily tailored to any other supported device and development board.    
      
  - STM32F070RB-Nucleo RevC Set-up
    - Wire GND to PA0 (CN7, PIN28)
    - Use LED2 connected to PA.05 pin.
      * LED2 toggles while waiting for the USER push-button to be pressed to enter the low power mode.
    - Connect an amperemeter to JP6 to measure the IDD current

@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example


 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
