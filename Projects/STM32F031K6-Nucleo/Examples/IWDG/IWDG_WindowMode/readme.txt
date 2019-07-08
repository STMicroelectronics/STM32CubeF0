/**
  @page IWDG_WindowMode IWDG Reset with window mode
  
  @verbatim
  ********************* COPYRIGHT(c) 2016 STMicroelectronics *******************
  * @file    IWDG/IWDG_WindowMode/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the IWDG Reset with window mode.
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

How to periodically update the IWDG reload counter and simulate a software fault that generates 
an MCU IWDG reset after a preset laps of time.

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system
clock (SYSCLK) to run at 48 MHz.

The IWDG time-out is set to 762 ms (the time-out may varies due to LSI frequency 
dispersion).

The Window option is enabled with a window register value set to 400 ms.
To prevent a reset, the down-counter must be reloaded when its value is:
 -lower than the window register value (400ms)
 -greater than 0x0
The IWDG counter is therefore refreshed each 450 ms in the main program infinite loop to 
prevent a IWDG reset (762 - 450 = 312 within the interval).
LED3 is also toggled each 450 ms indicating that the program is running. 
LED3 will turn off if any error occurs.

An EXTI Line is connected to a GPIO pin, configured to generate an interrupt
when putting or removing a jumper between PA.12 (Arduino D2) and GND.

Once the EXTI Line event occurs by putting or removing a jumper between PA.12 (Arduino D2) and GND,  
the refresh period is set to 200 ms.
That will make refresh being outside window value. As a result, when the IWDG counter is reloaded, 
the IWDG reset occurs.

If the IWDG reset is generated, after the system resumes from reset, LED3 turns on for 4 seconds.

If the EXTI Line event does not occur, the IWDG counter is indefinitely refreshed
in the main program infinite loop, and there is no IWDG reset.

@note Care must be taken when using HAL_Delay(), this function provides accurate
      delay (in milliseconds) based on variable incremented in SysTick ISR. This
      implies that if HAL_Delay() is called from a peripheral ISR process, then 
      the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Directory contents 

  - IWDG/IWDG_WindowMode/Inc/stm32f0xx_hal_conf.h    HAL configuration file
  - IWDG/IWDG_WindowMode/Inc/stm32f0xx_it.h          Interrupt handlers header file
  - IWDG/IWDG_WindowMode/Inc/main.h                  Header for main.c module  
  - IWDG/IWDG_WindowMode/Src/stm32f0xx_it.c          Interrupt handlers
  - IWDG/IWDG_WindowMode/Src/main.c                  Main program
  - IWDG/IWDG_WindowMode/Src/system_stm32f0xx.c      STM32F0xx system source file


@par Hardware and Software environment

  - This example runs on STM32F0xx devices.
    
  - This example has been tested with STM32F031K6-Nucleo(revC) board and can be
    easily tailored to any other supported device and development board.


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files: Project->Rebuild all
 - Load project image: Project->Download and Debug
 - Run program: Debug->Go(F5) 

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
