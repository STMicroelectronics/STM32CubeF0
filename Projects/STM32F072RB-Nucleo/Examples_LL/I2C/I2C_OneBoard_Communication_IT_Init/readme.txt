/**
  @page I2C_OneBoard_Communication_IT_Init I2C example (IT Mode)
  
  @verbatim
  ******************** (C) COPYRIGHT 2016 STMicroelectronics *******************
  * @file    Examples_LL/I2C/I2C_OneBoard_Communication_IT_Init/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the I2C_OneBoard_Communication_IT_Init I2C example (IT Mode).
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

How to handle the reception of one data byte from an I2C slave device 
by an I2C master device. Both devices operate in interrupt mode. The peripheral is initialized 
with LL initialization function to demonstrate LL init usage.

This example guides you through the different configuration steps by mean of LL API
to configure GPIO and I2C peripherals using only one STM32F072RB-Nucleo RevC.

I2C1 Peripheral is configured in Slave mode with EXTI (Clock 400Khz, Own address 7-bit enabled).
I2C2 Peripheral is configured in Master mode with EXTI (Clock 400Khz).
GPIO associated to User push-button is linked with EXTI. 

LED2 blinks quickly to wait for user-button press. 

Example execution:
Press the User push-button to initiate a read request by Master.
This action will generate an I2C start condition with the Slave address and a read bit condition.
When address Slave match code is received on I2C1, an ADDR interrupt occurs.
I2C1 IRQ Handler routine is then checking Address Match Code and direction Read.
This will allow Slave to enter in transmitter mode and then send a byte when TXIS interrupt occurs.
When byte is received on I2C2, an RXNE interrupt occurs.
When RXDR register is read, Master auto-generate a NACK and STOP condition
to inform the Slave that the transfer is finished.
The NACK condition generate a NACK interrupt in Slave side treated in the I2C1 IRQ handler routine by a clear of NACK flag.
The STOP condition generate a STOP interrupt in both side (Slave and Master). I2C1 and I2C2 IRQ handler routine are then
clearing the STOP flag in both side.

LED2 is On if data is well received.

In case of errors, LED2 is blinking.

@par Directory contents 

  - I2C/I2C_OneBoard_Communication_IT_Init/Inc/stm32f0xx_it.h          Interrupt handlers header file
  - I2C/I2C_OneBoard_Communication_IT_Init/Inc/main.h                  Header for main.c module
  - I2C/I2C_OneBoard_Communication_IT_Init/Inc/stm32_assert.h          Template file to include assert_failed function
  - I2C/I2C_OneBoard_Communication_IT_Init/Src/stm32f0xx_it.c          Interrupt handlers
  - I2C/I2C_OneBoard_Communication_IT_Init/Src/main.c                  Main program
  - I2C/I2C_OneBoard_Communication_IT_Init/Src/system_stm32f0xx.c      STM32F0xx system source file

@par Hardware and Software environment

  - This example runs on STM32F072xB devices.
    
  - This example has been tested with STM32F072RB-Nucleo RevC board and can be
    easily tailored to any other supported device and development board.

  - STM32F072RB-Nucleo RevC Set-up
    - Connect GPIOs connected to I2C1 SCL/SDA (PB.6 and PB.7)
    to respectively SCL and SDA pins of I2C2 (PB.13 and PB.14).
      - I2C1_SCL  PB.6 (CN10, pin 17) : connected to I2C2_SCL PB.13 (CN10, pin 30) 
      - I2C1_SDA  PB.7 (CN7, pin 21) : connected to I2C2_SDA PB.14 (CN10, pin 28)

  - Launch the program. Press User push-button to initiate a read request by Master 
      then Slave send a byte.

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example


 */
