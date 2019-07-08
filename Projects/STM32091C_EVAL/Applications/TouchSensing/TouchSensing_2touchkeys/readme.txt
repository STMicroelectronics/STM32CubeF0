/**
  @page TouchSensing 2 touchkey sensors example
 
  @verbatim
  ******************** (C) COPYRIGHT 2016 STMicroelectronics *******************
  * @file    TouchSensing\TouchSensing_2touchkeys\readme.txt
  * @author  MCD Application Team
  * @brief   Description of the TouchSensing 2 touchkey sensors example.
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

@par Description

This firmware is a basic example on how to use the STMTouch driver with 2 touchkey
sensors. The ECS and DTO are also used.

Observed behaviour:

- The LED2 is ON when the TS1 sensor is touched.
- The LED1 is ON when the TS2 sensor is touched.
- The LED4 blinks each time the ECS process is finished.
- The LED3 blinks continuously in case of error.

- On the LCD are also displayed:
  * The ECS state (ON/OFF).
  * The TS1 and TS2 touchkey sensors state (RELEASE, DETECT, ...) and their delta value.

- The ECS is ON when no touch is detected (all sensors are in the RELEASE state).
  The ECS is always present but its behavior can be modified using some parameters in
  the tsl_conf.h file.

- You can experiment the DTO by touching a sensor for few seconds. The RELEASE state
  is automatically entered and a re-calibration is performed after the timeout is reached.
  This timeout is programmable by the TSLPRM_DTO parameter in the tsl_conf.h file.

@par Project Directory contents

    - TouchSensing_2touchkeys\Inc\main.h                Header for main.c file
    - TouchSensing_2touchkeys\Inc\stm32f0xx_hal_conf.h  HAL Library configuration file
    - TouchSensing_2touchkeys\Inc\stm32f0xx_it.h        Header for stm32f0xx_it.c file
    - TouchSensing_2touchkeys\Src\stmCriticalSection.h  Header for stmCriticalSection.c file
    - TouchSensing_2touchkeys\Inc\tsl_conf.h            STMTouch driver configuration file
    - TouchSensing_2touchkeys\Inc\tsl_user.h            Header for tsl_user.c file

    - TouchSensing_2touchkeys\Src\main.c                Main program file
    - TouchSensing_2touchkeys\Src\stm32f0xx_hal_msp.c   HAL MSP module file
    - TouchSensing_2touchkeys\Src\stm32f0xx_it.c        Interrupt handlers file
    - TouchSensing_2touchkeys\Src\stmCriticalSection.c  STMStudio lock/unlock mechanism file
    - TouchSensing_2touchkeys\Src\system_stm32f0xx.c    System initialization file
    - TouchSensing_2touchkeys\Src\tsl_user.c            Touchsensing channels/banks description file
    
@par Hardware and Software environment

  - This example runs on STM32F091xx devices.
    
  - This example has been tested with STM32091C-EVAL board. It can be
    easily tailored to any other devices that embed the TSC peripheral and to
    any other development board supporting touchsensing sensor.
     
@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example
 
 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
