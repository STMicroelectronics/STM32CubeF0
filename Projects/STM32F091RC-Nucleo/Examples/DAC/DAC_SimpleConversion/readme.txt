/**
  @page DAC_SimpleConversion DAC Simple Conversion example
  
  @verbatim
  ******************** (C) COPYRIGHT 2016 STMicroelectronics *******************
  * @file    DAC/DAC_SimpleConversion/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the DAC Simple Conversion example.
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

How to use the DAC peripheral to do a simple conversion.

The result of conversion can be seen by connecting PA4(DAC channel1) to an oscilloscope.
The observed value is 3.3V.

STM32F091RC-Nucleo RevC board's LED can be used to monitor the process status:
  - LED2 is slowly blinking (1 sec. period) and example is stopped (using infinite loop)
  when there is an error during process.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Directory contents 

  - DAC/DAC_Simple_Conversion/Inc/stm32f0xx_hal_conf.h    HAL configuration file
  - DAC/DAC_Simple_Conversion/Inc/stm32f0xx_it.h          DMA interrupt handlers header file
  - DAC/DAC_Simple_Conversion/Inc/main.h                  Header for main.c module  
  - DAC/DAC_Simple_Conversion/Src/stm32f0xx_it.c          DMA interrupt handlers
  - DAC/DAC_Simple_Conversion/Src/main.c                  Main program
  - DAC/DAC_Simple_Conversion/Src/stm32f0xx_hal_msp.c     HAL MSP file
  - DAC/DAC_Simple_Conversion/Src/system_stm32f0xx.c      STM32F0xx system source file
  

@par Hardware and Software environment  
  - This example runs on STM32F0xx devices.
    
  - This example has been tested with STM32F091RC-Nucleo RevC board and can be
    easily tailored to any other supported device and development board.

  - STM32F091RC-Nucleo RevC Set-up 	
     - Connect PA4 (DAC Channel1)  - connected to A2 on Arduino Connector (pin 32 in CN7) -  to an oscilloscope.

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example
  

 */
