/**
  @page STM32F072B-Discovery_Demonstration   STM32F072B-Discovery demonstration firmware
  
  @verbatim
  ******************** (C) COPYRIGHT 2016 STMicroelectronics *******************
  * @file    Demonstration/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the STM32F072B-Discovery demonstration firmware
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2016 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  @endverbatim

@par Demo Description 
 
Demonstration of firmware based on STM32Cube. This example uses the MEMS motion
sensor to blink the four LEDs according to the motion direction,the linear
touch sensor blink the four LEDs according to the finger position.
Below you find the sequence to launch the discover demonstration :

  - Four LEDs blink
       
  - The gyroscope MEMS sensor (L3GD20):  move the board and observe LEDs blinking
    according to motion direction.
    Right : LED5 is ON.
    Left  : LED4 is ON.
    UP    : LED3 is ON.
    DOWN  : LED6 is ON.
     
  - Touch Sensor Linear (TSL) blink the four LEDs according to the finger position
    it can perform in both direction.
                              
                
Note: - Press user button to swich from sequence to the second one. 
 

@par Directory contents 

  - Demonstrations/Inc/main.c               Main program header file
  - Demonstrations/Inc/stm32f0xx_hal_conf.h HAL Configuration file
  - Demonstrations/Inc/stm32f0xx_it.h       Interrupt handlers header file
  - Demonstrations/Inc/tsl_conf.h           TSC configuration file
  - Demonstrations/Inc/tsl_user.h           TSC user configuration and api header file
  - Demonstrations/Src/main.c               Main program
  - Demonstrations/Src/stm32f0xx_hal_msp.c  HAL MSP module file
  - Demonstrations/Src/stm32f0xx_it.c       Interrupt handlers
  - Demonstrations/Src/system_stm32f0xx.c   STM32F0xx system source file
  - Demonstrations/Src/tsl_user.c           TSC user configuration and api file


@par Hardware and Software environment

  - This Demonstration has been tested with STMicroelectronics STM32F072B-DISCO board
    can be easily tailored to any other supported device and development board.      
  - This Demonstration runs on STM32F072RB devices.

@par How to use it ? 

In order to make the program work, you must do the following :
- Open your preferred toolchain 
- Rebuild all files and load your image into target memory
- Run the example

    
 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
