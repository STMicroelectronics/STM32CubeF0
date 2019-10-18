/**
  @page HID_Standalone_BCD USB Device Humain Interface (HID + BCD) example

  @verbatim
  ******************************************************************************
  * @file    USB_Device/HID_Standalone_BCD/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the USB HID BCD example.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  @endverbatim

@par Application Description

Use the BCD feature based on the USB HID device application on the STM32F0xx devices.

The STM32F0xx device integrated battery charger detection circuitry supports USB-IF Battery
Charger Detection, BCD. The hpcd.Init.battery_charging_enable in the usbd_conf.c
should be set to 1 to enable the support for BCD.

This example is a part of the USB Device Library package using STM32Cube firmware. It describes how to
use the BCD feature based on the USB HID device application.
This is a typical example on how to use the USB peripheral in Device mode with HID class
following the "Device Class Definition for Human Interface Devices (HID) Version 1.11
Jun 27, 2001". The example is built around the USB device library and emulate the User Button by moving
the host mouse pointer horizontally.

Battery charger detection will begin automatically when VBUS is detected. The STM32F0xx includes the
capability to detect various USB battery chargers and can detect a range of USB battery chargers including a Standard Downstream
Port (SDP), a Charging Downstream Port (CDP), and a Dedicated Charging Port (DCP). For more information
on USB battery charger detection, please see the USB Battery Charging Specification, Revision 1.2 (Note 1).

If a DCP is connected (D+/D- are shorted or connected through 200 ohm resistor)
In CDP case we used a HP dock Station: charging port

@note LED 5 (green led) is ON if SDP is detected.
      LED 5 (green led) toggles with a frequency of 10 Hz if CDP is detected
      LED 5 (green led) toggles with a frequency of 2 Hz if DCP is detected.

At the beginning of the main program the HAL_Init() function is called to reset all the peripherals,
initialize the Flash interface and the systick. The user is provided with the SystemClock_Config()
function to configure the system clock (SYSCLK) to run at 80 MHz. The Full Speed (FS) USB module uses
internally a 48-MHz clock HSI48 USB Clock.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL configuration.

@note To reduce the example footprint, the toolchain dynamic allocation is replaced by a static allocation
      by returning the address of a pre-defined static buffer with the HID class structure size.

For more details about the STM32Cube USB Device library, please refer to UM1734
"STM32Cube USB Device library".

@par Directory contents

  - USB_Device/HID_Standalone_BCD/Src/main.c                  Main program
  - USB_Device/HID_Standalone_BCD/Src/system_stm32f0xx.c      STM32F0xx system clock configuration file
  - USB_Device/HID_Standalone_BCD/Src/stm32f0xx_it.c          Interrupt handlers
  - USB_Device/HID_Standalone_BCD/Src/usbd_conf.c             General low level driver configuration
  - USB_Device/HID_Standalone_BCD/Src/usbd_desc.c             USB device HID descriptor
  - USB_Device/HID_Standalone_BCD/Inc/main.h                  Main program header file
  - USB_Device/HID_Standalone_BCD/Inc/stm32f0xx_it.h          Interrupt handlers header file
  - USB_Device/HID_Standalone_BCD/Inc/stm32f0xx_hal_conf.h    HAL configuration file
  - USB_Device/HID_Standalone_BCD/Inc/usbd_conf.h             USB device driver Configuration file
  - USB_Device/HID_Standalone_BCD/Inc/usbd_desc.h             USB device MSC descriptor header file


@par Hardware and Software environment

  - This example runs on STM32F072RB devices.

  - This example has been tested with STM32F072B-Discovery RevC board and can be
    easily tailored to any other supported device and development board.

  - STM32F072B-Discovery Rev C Set-up
    - Connect the STM32F072B-Discovery board to the PC through 'USB Mini B-Receptacle'
      cable to the USB User connector.
    - Connect the PA9 (VBUS pin) pin to the 5V pin.

@par How to use it ?

In order to make the program work, you must do the following:
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
