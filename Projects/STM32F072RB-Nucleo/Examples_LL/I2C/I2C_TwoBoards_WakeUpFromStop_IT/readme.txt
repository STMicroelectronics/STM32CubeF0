/**
  @page I2C_TwoBoards_WakeUpFromStop_IT I2C example (IT Mode)
  
  @verbatim
  ******************** (C) COPYRIGHT 2016 STMicroelectronics *******************
  * @file    Examples_LL/I2C/I2C_TwoBoards_WakeUpFromStop_IT/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the I2C_TwoBoards_WakeUpFromStop_IT I2C example (IT Mode).
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2016 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  @endverbatim

@par Example Description
  
How to handle the reception of a data byte from an I2C slave device in 
Stop mode using IT mode by an I2C master device, both using interrupt mode. The 
peripheral is initialized with LL unitary service functions to optimize for 
performance and size.

This example guides you through the different configuration steps by mean of LL API 
to configure GPIO and I2C peripherals using two STM32F072RB-Nucleo RevC.

Boards: STM32F072RB-Nucleo RevC (embeds a STM32F072RBT6 device)
SCL Pin: PB.6 (CN10, pin 17)
SDA Pin: PB.7 (CN7, pin 21)

   ______BOARD SLAVE_____                       _____BOARD MASTER_____
  |        ______________|                     |______________        |
  |       |I2C1          |                     |          I2C1|       |
  |       |              |                     |              |       |
  |       |          SCL |_____________________| SCL          |       |
  |       |              |                     |              |       |
  |       |              |                     |              |       |
  |       |          SDA |_____________________| SDA          |       |
  |       |______________|                     |______________|       |
  |         __           |                     |             __       |
  |        |__|          |                     |            |__|      |
  |        USER       GND|_____________________|GND         USER      |
  |___STM32F0xx-Nucleo___|                     |___STM32F0xx-Nucleo___|

The project is splitted in two parts the Master Board and the Slave Board
- Master Board
  I2C1 Peripheral is configured in Master mode with EXTI (Clock 100Khz).
And GPIO associated to User push-button is linked with EXTI. 
- Slave Board
  I2C1 Peripheral is configured in Slave mode with EXTI (Clock 100Khz,
  Own address 7-bit enabled, WakeUpFromStop).

The user can choose between Master and Slave through "#define SLAVE_BOARD"
in the "main.h" file:
- Comment "#define SLAVE_BOARD" to select Master board.
- Uncomment "#define SLAVE_BOARD" to select Slave board.

A first program launch, BOARD SLAVE is in Stop mode and BOARD MASTER is waiting User action on User button.
LED2 blinks quickly on BOARD MASTER to wait for user-button press. 

Example execution:
Press the User push-button on BOARD MASTER to initiate a read request by Master.
This action will generate an I2C start condition with the Slave address and a read bit condition.
When address Slave match code is received on I2C1 of BOARD SLAVE, a wake up is initiate and an ADDR interrupt occurs.
I2C1 Slave IRQ Handler routine is then checking Address Match Code and direction Read.
This will allow Slave to enter in transmitter mode and then send a byte when TXIS interrupt occurs.
When byte is received on I2C1 of BOARD MASTER, an RXNE interrupt occurs.
When RXDR register is read, Master auto-generate a NACK and STOP condition
to inform the Slave that the transfer is finished.
The NACK condition generate a NACK interrupt in Slave side treated in the I2C1 Slave IRQ handler routine by a clear of NACK flag.
The STOP condition generate a STOP interrupt in both side (Slave and Master). Both I2C1 IRQ handler routine are then
clearing the STOP flag in both side.

LED2 is On if data is well received.

In case of errors, LED2 is blinking.

@par Directory contents 

  - I2C/I2C_TwoBoards_WakeUpFromStop_IT/Inc/stm32f0xx_it.h          Interrupt handlers header file
  - I2C/I2C_TwoBoards_WakeUpFromStop_IT/Inc/main.h                  Header for main.c module
  - I2C/I2C_TwoBoards_WakeUpFromStop_IT/Inc/stm32_assert.h          Template file to include assert_failed function
  - I2C/I2C_TwoBoards_WakeUpFromStop_IT/Src/stm32f0xx_it.c          Interrupt handlers
  - I2C/I2C_TwoBoards_WakeUpFromStop_IT/Src/main.c                  Main program
  - I2C/I2C_TwoBoards_WakeUpFromStop_IT/Src/system_stm32f0xx.c      STM32F0xx system source file

@par Hardware and Software environment

  - This example runs on STM32F072xB devices.
    
  - This example has been tested with STM32F072RB-Nucleo RevC board and can be
    easily tailored to any other supported device and development board.

  - STM32F072RB-Nucleo RevC Set-up
    - Connect GPIOs connected to Board Slave I2C1 SCL/SDA (PB.6 and PB.7)
    to respectively Board Master SCL and SDA pins of I2C1 (PB.6 and PB.7).
      - I2C1_SCL  PB.6 (CN10, pin 17) : connected to I2C1_SCL PB.6 (CN10, pin 17) 
      - I2C1_SDA  PB.7 (CN7, pin 21) : connected to I2C1_SDA PB.7 (CN7, pin 21)
    - Connect Master board GND to Slave Board GND

  - Launch the program. Press User push-button to initiate a read request by Master 
      then Slave after wake up from Stop mode send a byte.

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory (The user can choose between Master 
   and Slave target through "#define SLAVE_BOARD" in the "main.h" file)
    o Comment "#define SLAVE_BOARD" and load the project in Master Board
    o Uncomment "#define SLAVE_BOARD" and load the project in Slave Board
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
