/**
  @page ADC_DMA_Transfer ADC DMA Transfer example

  @verbatim
  ******************** (C) COPYRIGHT 2016 STMicroelectronics *******************
  * @file    ADC/ADC_DMA_Transfer/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the ADC DMA Transfer example.
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

How to configure and use the ADC to convert an external analog input and get 
the result using a DMA transfer through the HAL API.

The ADC is configured to continuously convert ADC_CHANNEL_0, resolution is set to 12 bits right aligned, conversion is software-triggered..

DMA1_Channel1 is configured in Circular mode to transfer continuously
the content of ADC_DR (Data Register) to "aADCxConvertedData" variable which 
stores the conversion result.

User may watch variable array "aADCxConvertedData" and check that its values are modified
according to the voltage applied to pin PA.00 (pin 28 in CN7 connector).

STM32 board LED can be used to monitor the conversion:
  - LED2 is ON when there is an error in initialization.


@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Directory contents 

  - ADC/ADC_DMA_Transfer/Inc/stm32f0xx_hal_conf.h    HAL configuration file
  - ADC/ADC_DMA_Transfer/Inc/stm32f0xx_it.h          HAL interrupt handlers header file
  - ADC/ADC_DMA_Transfer/Inc/main.h                  Header for main.c module  
  - ADC/ADC_DMA_Transfer/Src/stm32f0xx_it.c          HAL interrupt handlers
  - ADC/ADC_DMA_Transfer/Src/main.c                  Main program
  - ADC/ADC_DMA_Transfer/Src/stm32f0xx_hal_msp.c     HAL MSP file 
  - ADC/ADC_DMA_Transfer/Src/system_stm32f0xx.c      STM32F0xx system source file


@par Hardware and Software environment

  - This example runs on STM32F030x8 devices.

  - This example has been tested with STM32F030R8-Nucleo Rev C board and can be
    easily tailored to any other supported device and development board.

  - STM32F030R8-Nucleo Rev C Set-up
    Use an external power supply and adjust supply voltage and connect it on 

  - To monitor the conversion result, put the "aADCxConvertedData" variable in the debugger live watch.

@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example


 */
