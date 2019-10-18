/**
  @page Demo   Demo STM32091C_EVAL
 
  @verbatim
  ******************************************************************************
  * @file    readme.txt 
  * @author  MCD Application Team
  * @brief   Description of STM32091C_EVAL Demo
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

Demonstration firmware based on STM32Cube. This example helps you to discover
STM32 Cortex-M devices that are plugged onto your STM32 Nucleo board.

At the beginning of the main program the HAL_Init() function is called to reset
all the peripherals, initialize the Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system clock
(SYSCLK) to run at 48 MHz.
 
 
Below you find the different items of demonstration :

 - 8uarts application : is using the 8 uarts available to transfert a buffer by packet.
 - file browser : aplication to browse on the SD card.
 - image browser : displays the pictures present on the SD card (\USER).
 - low power : allows to enter in low power mode(stop and standby) 
              and the wakeup can be ordered by an EXTI or an alarm.
 - thermometer : displays the temperature (captor Stlm75).

@note Care must be taken when using HAL_Delay(), this function provides accurate
      delay (in milliseconds) based on variable incremented in SysTick ISR. 
      This implies that if HAL_Delay() is called from a peripheral ISR process, 
      then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application need to ensure that the SysTick time base is always set to
      1 millisecond to have correct HAL operation.


@par Hardware and Software environment

  - This example runs on STM32091C_EVAL devices..
    
  - This example has been tested with STM32091C_EVAL board and can be
    easily tailored to any other supported device and development board.      

  - A SDSC microSD card (capacity up to 4GB), must contain the .bmp files
    available under the FW package \SD_card folder.
    

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
