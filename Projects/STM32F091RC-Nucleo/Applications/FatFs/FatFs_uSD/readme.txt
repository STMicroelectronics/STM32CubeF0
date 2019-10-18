/**
  @page FatFs_uSD   FatFs with uSD card drive application
 
  @verbatim
  ******************************************************************************
  * @file    readme.txt
  * @author  MCD Application Team
  * @brief   Description of the FatFs with uSD card drive application
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

@par Application Description

How to use STM32Cube firmware with FatFs middleware component as a generic FAT
file system module. This application uses Fatfs features to configure a 
microSD drive.

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system clock
(SYSCLK) to run at 48 MHz.
 
The application is based on writing and reading back a text file from a drive,
and it's performed using FatFs APIs to access the FAT volume as described
in the following steps: 

 - Link the uSD disk I/O driver;
 - Register the file system object (mount) to the FatFs module for the uSD drive;
 - Create a FAT file system (format) on the uSD drive;
 - Create and Open new text file object with write access;
 - Write data to the text file;
 - Close the open text file;
 - Open text file object with read access;
 - Read back data from the text file;
 - Close the open text file;
 - Check on read data from text file;
 - Unlink the uSD disk I/O driver.

Notice that the application manages possible errors that might occur during the 
access to FAT volume, when using FatFs APIs. In addition, user can check if the
written text file is available on the uSD card.

It is possible to fine tune needed FatFs features by modifying defines values 
in FatFs configuration file “ffconf.h” available under the project includes 
directory, in a way to fit the application requirements. 

STM32F091RC-Nucleo RevC's LED can be used to monitor the transfer status:
  - LED2 is ON during example runs. (due to LED use the same physical link as µSD drive communication)
  - LED2 is slow blinking (200 ms) when the application runs successfully.
  - LED2 is fast blinking in case of error in initialization or during FatFs_uSD process.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Directory contents
 
  - FatFs/FatFs_uSD/Inc/stm32f0xx_hal_conf.h    HAL configuration file
  - FatFs/FatFs_uSD/Inc/stm32f0xx_it.h          Interrupt handlers header file
  - FatFs/FatFs_uSD/Inc/main.h                  Header for main.c module
  - FatFs/FatFs_uSD/Inc/ffconf.h                FAT file system module configuration file   
  - FatFs/FatFs_uSD/Src/stm32f0xx_it.c          Interrupt handlers
  - FatFs/FatFs_uSD/Src/main.c                  Main program
  - FatFs/FatFs_uSD/Src/system_stm32f0xx.c      STM32F0xx system source file

@par Hardware and Software environment

  - This application runs on STM32F091xC devices
    
  - This application has been tested with STM32F091RC-Nucleo RevC board and can be
    easily tailored to any other supported device and development board.        

  - Adafruit 1.8" TFT shield must be connected on CN5,CN6, CN8 and CN9 Arduino connectors, 
    for more details please refer to UM1726.

  - A uSD card must be present on Adafruit uSD connector board.
  
For more details about the adafruit 1.8" TFT shield, please visit: 
http://www.adafruit.com/blog/2012/04/26/new-product-adafruit-1-8-18-bit-color-tft-shield-wmicrosd-and-joystick/

@par How to use it ? 

In order to make the program work, you must do the following:
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the application

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
