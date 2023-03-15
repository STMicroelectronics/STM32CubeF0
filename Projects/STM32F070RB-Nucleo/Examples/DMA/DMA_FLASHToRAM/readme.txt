/**
  @page DMA_FLASHToRAM DMA FLASH To RAM Example
  
  @verbatim
  ******************** (C) COPYRIGHT 2016 STMicroelectronics *******************
  * @file    DMA/DMA_FLASHToRAM/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the DMA FLASH to RAM example.
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

How to use a DMA to transfer a word data buffer from Flash memory to embedded 
SRAM through the HAL API

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system
clock (SYSCLK) to run at 48 MHz.

DMA1_Channel1 is configured to transfer the contents of a 32-word data 
buffer stored in Flash memory to the reception buffer declared in RAM.

The start of transfer is triggered by software. DMA1_Channel1 memory-to-memory
transfer is enabled. Source and destination addresses incrementing is also enabled.
The transfer is started by setting the channel enable bit for DMA1_Channel1.
At the end of the transfer a Transfer Complete interrupt is generated since it
is enabled and the callback function (customized by user) is called.
 

Board's LEDs can be used to monitor the transfer status:
 - LED2 is ON when the transfer is complete (into the Transfer Complete interrupt
   routine).
 - LED2 is Toggled with a period of 200 ms when there is a transfer error 
 - LED2 is Toggled with a period of 1000 ms when when Error_Handler is called
 
It is possible to select a different channel for the DMA transfer
example by modifying defines values in the file main.h.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Directory contents 

  - DMA/DMA_FLASHToRAM/Src/system_stm32f0xx.c    STM32F0xx system clock configuration file
  - DMA/DMA_FLASHToRAM/Src/stm32f0xx_it.c        Interrupt handlers
  - DMA/DMA_FLASHToRAM/Src/main.c                Main program
  - DMA/DMA_FLASHToRAM/Inc/stm32f0xx_hal_conf.h  HAL Configuration file
  - DMA/DMA_FLASHToRAM/Inc/stm32f0xx_it.h        Interrupt handlers header file
  - DMA/DMA_FLASHToRAM/Inc/main.h                Main program header file

     
@par Hardware and Software environment

  - This example runs on STM32F070xB Devices.

  - This example has been tested with STMicroelectronics STM32F070RB-Nucleo RevC 
    board and can be easily tailored to any other supported device 
    and development board.    

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example

   

 */
