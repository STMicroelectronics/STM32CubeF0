/**
  @page RCC_CRS_Synchronization_IT RCC Clock Recovery Service example
  
  @verbatim
  ******************** (C) COPYRIGHT 2016 STMicroelectronics *******************
  * @file    RCC/RCC_CRS_Synchronization_IT/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the RCC Clock Recovery Service example.
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

@par Example Description 

Configuration of the clock recovery service (CRS) in Interrupt mode, using the RCC HAL API.

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system
clock (SYSCLK) to run at 48 MHz.

Then, HSI48 and LSE oscillators are enabled. 
In this example, as a first step a synchronization miss is raised when using a wrong synchronization 
source (GPIO instead of LSE which is not configured). In a 2nd step, a reconfiguration is done to
use LSE and the correct synchronization.

In this test, this is the scenario:
 - set the source to GPIO and then enable IT
 - to provoke a SYNC event, call the software event generation function
 - Interrupt is raised to indicate a SYNC MISS and then power on LED4
 - in main program, wait for 1 sec and reconfigure source to LSE
 - Toggle quickly LED1 when SYNC WARN is detected
 - Increase tolerance (FELIM value) when SYNC WARN is detected
 - At the end of the test LED1 should stay ON

Note: HSI48 frequency can be checked with oscilloscope using MCO PIN PA.08

@par Directory contents 

  - RCC/RCC_CRS_Synchonization_IT/Inc/stm32f0xx_hal_conf.h    HAL configuration file
  - RCC/RCC_CRS_Synchonization_IT/Inc/stm32f0xx_it.h          Interrupt handlers header file
  - RCC/RCC_CRS_Synchonization_IT/Inc/main.h                  Header for main.c module  
  - RCC/RCC_CRS_Synchonization_IT/Src/stm32f0xx_it.c          Interrupt handlers
  - RCC/RCC_CRS_Synchonization_IT/Src/main.c                  Main program
  - RCC/RCC_CRS_Synchonization_IT/Src/system_stm32f0xx.c      STM32F0xx system source file

@par Hardware and Software environment

  - This example runs on STM32F042x6, STM32F071xB, STM32F072xB, STM32F078xB and STM32F091xC Devices.
    
  - This example has been tested with STM32072B-EVAL RevB board and can be
    easily tailored to any other supported device and development board.    


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example


 */
