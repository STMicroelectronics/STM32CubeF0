/**
  @page DFU_Standalone USB Device Firmware Upgrade (DFU) example
  
  @verbatim
  ******************************************************************************
  * @file    USB_Device/DFU_Standalone/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the USB DFU example.
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

Compliant implementation of the Device Firmware Upgrade (DFU) 
capability to program the embedded Flash memory through the USB peripheral.

This is a typical example on how to use the STM32F0xx USB Device peripheral to implement the DFU
capability for programming the embedded flash memory of the STM32F0xx devices.

At the beginning of the main program the HAL_Init() function is called to reset all the peripherals,
initialize the Flash interface and the systick. The user is provided with the SystemClock_Config()
function to configure the system clock (SYSCLK) to run at 48 MHz.

The 48 MHz clock for the USB can be derived from one of the two following sources:
  – PLL clock(clocked by the HSE): If the USB uses the PLL as clock source.
  – RC48 Clock (HSI48): The HSI48 clock is generated from an internal 48 MHz RC oscillator, coupled with
    the clock recovery system (CRS). 
    The CRS allows automatic trimming of the internal HSI48, synchronized with USB SOF signal at 1KHz rate, 
    to guarantee its optimal accuracy over the whole device operational range.    
User can select USB clock from HSI48 or PLL through macro defined in main.h
(USE_USB_CLKSOURCE_CRSHSI48 and USE_USB_CLKSOURCE_PLL).

The DFU transactions are based on Endpoint 0 (control endpoint) transfer. All requests and status 
control are sent/received through this endpoint.

The Internal flash memory memory is split as follows:
 - DFU area located in [0x08000000 : USBD_DFU_APP_DEFAULT_ADD-1]: Only read access
 - Application area located in [USBD_DFU_APP_DEFAULT_ADD : Device's end address]: Read, Write, and Erase
   access

In this application, two operating modes are available:
 1. DFU operating mode: 
    This mode is entered after an MCU reset in case:
     - The DFU mode is forced by the user: the user presses the key button.
     - No valid code found in the application area: a code is considered valid if the MSB of the initial
       Main Stack Pointer (MSP) value located in the first address of the application area is equal to 
       0x2000
      
 2. Run-time application mode: 
    This is the normal run-time activities. A binary which toggles LEDs on the evaluation board is 
    provided in Binary directory.

@note After each device reset, hold down the Tamper button on the evaluation board to enter the DFU mode.     

Traditionally, firmware is stored in Hex, S19 or Binary files, but these formats do not contain the 
necessary information to perform the upgrade operation, they contain only the actual data of the program
to be downloaded. However, the DFU operation requires more information, such as the product identifier, 
vendor identifier, Firmware version and the Alternate setting number (Target ID) of the target to be 
used, this information makes the upgrade targeted and more secure. To add this information, DFU file 
format is used. For more details refer to the "DfuSe File Format Specification" document (UM0391).

To generate a DFU image, download "DFUse Demonstration" tool and use DFU File Manager to convert a 
binary image into a DFU image. This tool is for download from www.st.com
To download a *.dfu image, use "DfuSe Demo" available within "DFUse Demonstration" install directory. 

Please refer to UM0412, DFuSe USB device firmware upgrade STMicroelectronics extension for more details
on the driver installation and PC host user interface.
    
@note The application needs to ensure that the SysTick time base is set to 1 millisecond
      to have correct HAL configuration.

@note To reduce the example footprint, the toolchain dynamic allocation is replaced by a static allocation
      by returning the address of a pre-defined static buffer with the DFU class structure size. 
	  
@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

@par USB Library Configuration

It is possible to fine tune needed USB Device features by modifying defines values in USBD configuration
file “usbd_conf.h” available under the project includes directory, in a way to fit the application
requirements, such as:      
 - USBD_DFU_APP_DEFAULT_ADD, specifing the address from where user's application will be downloaded.

Device's end address is the end address of the flash memory and it is dependent on the device in use.


@par Directory contents 

  - USB_Device/DFU_Standalone/Src/main.c                  Main program
  - USB_Device/DFU_Standalone/Src/system_stm32f0xx.c      STM32F0xx system clock configuration file
  - USB_Device/DFU_Standalone/Src/stm32f0xx_it.c          Interrupt handlers
  - USB_Device/DFU_Standalone/Src/usbd_conf.c             General low level driver configuration
  - USB_Device/DFU_Standalone/Src/usbd_desc.c             USB device descriptor
  - USB_Device/DFU_Standalone/Src/usbd_dfu_flash.c        Internal flash memory management
  - USB_Device/DFU_Standalone/Inc/main.h                  Main program header file
  - USB_Device/DFU_Standalone/Inc/stm32f0xx_it.h          Interrupt handlers header file
  - USB_Device/DFU_Standalone/Inc/stm32f0xx_hal_conf.h    HAL configuration file
  - USB_Device/DFU_Standalone/Inc/usbd_conf.h             USB device driver Configuration file
  - USB_Device/DFU_Standalone/Inc/usbd_desc.h             USB device descriptor header file
  - USB_Device/DFU_Standalone/Inc/usbd_dfu_flash.h        Internal flash memory management header file


@par Hardware and Software environment

  - This example runs on STM32F072VB devices.

  - This example has been tested with STMicroelectronics STM32072B-EVAL RevB board and can be easily tailored 
     to any other supported device and development board.

  - STM32072B-EVAL MB1070 Rev B Set-up:
    - Install the DFU driver available in "DfuSe Demonstrator" installation directory
    - For Windows 8.1 and later : Update STM32 DFU device driver manually from Windows Device Manager.
      The install of required device driver is available under "Program Files\STMicroelectronics\Software\
      DfuSe v3.0.5\Bin\Driver\Win8.1" directory.
    - Connect the STM32072B-EVAL board to the PC through 'USB A-Male to A-Male' cable to the CN4 connector


@par How to use it ?

In order to make the program work, you must do the following:
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example
 
 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
