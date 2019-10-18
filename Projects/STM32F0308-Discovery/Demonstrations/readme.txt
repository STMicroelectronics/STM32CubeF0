/**
  @page STM32F0308-Discovery_Demonstration   STM32F00308-Discovery demonstration firmware
  
  @verbatim
  ******************** (C) COPYRIGHT 2016 STMicroelectronics *******************
  * @file    Demonstration/readme.txt  
  * @author  MCD Application Team
  * @brief   Description of the STM32F0308-Discovery demonstration firmware
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

@par Demonstration Description 

Use the USER push button B1 connected to PA0 and the two leds LED4 and LED3
connected respectively to PC8 and PC9.

In the demonstration  the LED3 is initially blinking each 200ms.
  - For the first pressing on the USER button, the speed of the led blinking is incremented. 
    The LED3 blinks each 100ms.
  - For the second pressing on the USER button, LED3 stops blinking and the status is off.
  - A third pressing on the USER button causes the initial behavior of the LED3.

Each time the USER button is pressed, the LED4 is turned on for 1s.


@par Directory contents 

  - Demonstrations/Inc/main.h               Main program header file
  - Demonstrations/Inc/stm32f0xx_hal_conf.h HAL Configuration file
  - Demonstrations/Inc/stm32f0xx_it.h       Interrupt handlers header file
  - Demonstrations/Src/main.c               Main program
  - Demonstrations/Src/stm32f0xx_it.c       Interrupt handlers
  - Demonstrations/Src/system_stm32f0xx.c   STM32F0xx system source file
  

@par Hardware and Software environment

  
  - This Demonstration has been tested with STMicroelectronics STM32F0308-DISCO (MB1134) 
    RevA and can be easily tailored to any other supported device and development board.
  - This Demonstration runs on STM32F030R8 devices.

@par How to use it ? 

In order to make the program work, you must do the following :
- Open your preferred toolchain 
- Rebuild all files and load your image into target memory
- Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
