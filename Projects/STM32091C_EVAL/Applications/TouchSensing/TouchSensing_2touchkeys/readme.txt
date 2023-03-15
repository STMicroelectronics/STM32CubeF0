/**
  @page TouchSensing 2 touchkey sensors example
 
  @verbatim
  ******************** (C) COPYRIGHT 2016 STMicroelectronics *******************
  * @file    TouchSensing\TouchSensing_2touchkeys\readme.txt
  * @author  MCD Application Team
  * @brief   Description of the TouchSensing 2 touchkey sensors example.
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
 

 */
