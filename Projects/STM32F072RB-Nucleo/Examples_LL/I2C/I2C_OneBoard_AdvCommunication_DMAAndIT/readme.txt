/**
  @page I2C_OneBoard_AdvCommunication_DMAAndIT I2C (Master DMA Mode)
  
  @verbatim
  ******************** (C) COPYRIGHT 2016 STMicroelectronics *******************
  * @file    Examples_LL/I2C/I2C_OneBoard_AdvCommunication_DMAAndIT/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the I2C_OneBoard_AdvCommunication_DMAAndIT I2C example (Master DMA Mode).
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

How to exchange data between an I2C master device in DMA mode and an I2C slave 
device in interrupt mode. The peripheral is initialized with LL unitary service 
functions to optimize for performance and size.

This example guides you through the different configuration steps by mean of LL API
to configure GPIO, DMA and I2C peripherals using only one STM32F072RB-Nucleo RevC.

I2C1 Peripheral is configured in Slave mode with EXTI (Clock 400Khz, Own address 7-bit enabled).
I2C2 Peripheral is configured in Master mode with DMA (Clock 400Khz).
GPIO associated to User push-button is linked with EXTI. 

LED2 blinks quickly to wait for user-button press. 

Example execution:
Press the User push-button to initiate a write request by Master through Handle_I2C_Master_Transmit() or 
through Handle_I2C_Master_TransmitReceive() routine depends on Command Code type.

Command code type is decomposed in two categories :
1- Action Command code
    a. Type of command which need an action from Slave Device without send any specific anwser)
    b. I2C sequence is composed like that :
     _____________________________________________________________________________________
    |_START_|_Slave_Address_|_Wr_|_A_|_Command_Code_BYTE_1_|_A_|_Command_Code_BYTE_2_|_A_|....
     ________________________________
    |_Command_Code_BYTE_M_|_A_|_STOP_|

First of all, through Handle_I2C_Master_Transmit() routine, Master device generate an I2C start condition
with the Slave address and a write bit condition.
When address Slave match code is received on I2C1, an ADDR interrupt occurs.
I2C1 IRQ Handler routine is then checking Address Match Code and direction Write.
This will allow Slave to enter in receiver mode and then acknowledge Master to send the Command code bytes through DMA.
When acknowledge is received on I2C2, DMA transfer the Command code data from flash memory buffer to I2C2 TXDR register.
This will allow Master to transmit a byte to the Slave.
Each time a byte is received on I2C1 (Slave), an RXNE interrupt occurs and byte is stored into an internal buffer
until a STOP condition.
And so each time the Slave acknowledge the byte received,
DMA transfer the next data from flash memory buffer to I2C2 TXDR register until Transfer completed.
Master auto-generate a Stop condition when DMA transfer is achieved.

The STOP condition generate a STOP interrupt and initiate the end of reception on Slave side.
I2C1 IRQ handler and Handle_I2C_Master_Transmit() routine are then clearing the STOP flag in both side.

LED2 is On if data are well sent and the Command Code sent to slave is print in the Terminal I/O.

2- Request Command code :
    a. Type of command which need a specific data answer from Slave Device.
    b. I2C sequence is composed like that :
     _____________________________________________________________________________________
    |_START_|_Slave_Address_|_Wr_|_A_|_Command_Code_BYTE_1_|_A_|_Command_Code_BYTE_2_|_A_|....
     ______________________________________________________________________________
    |_Command_Code_BYTE_M_|_A_|_RESTART_|_Slave_Address_|_Rd_|_A_|_Data_BYTE_1_|_A_|...
     ___________________________________________
    |_Data_BYTE_2_|_A_|_Data_BYTE_N_|_NA_|_STOP_|

First of all, through Handle_I2C_Master_TransmitReceive() routine, Master device generate an I2C start condition
with the Slave address and a write bit condition.
When address Slave match code is received on I2C1, an ADDR interrupt occurs.
I2C1 IRQ Handler routine is then checking Address Match Code and direction Write.
This will allow Slave to enter in receiver mode and then acknowledge Master to send the Command code bytes through DMA.
When acknowledge is received on I2C2, DMA transfer the Command code data from flash memory buffer to I2C2 TXDR register.
This will allow Master to transmit a byte to the Slave.
Each time a byte is received on I2C1 (Slave), an RXNE interrupt occurs and byte is stored into an internal buffer
until a RESTART condition.
And so each time the Slave acknowledge the byte received,
DMA transfer the next data from flash memory buffer to I2C2 TXDR register until Transfer completed.
Then Master device generate a RESTART condition with Slave address and a read bit condition.
When address Slave match code is received on I2C1, an ADDR interrupt occurs.
I2C1 IRQ Handler routine is then checking Address Match Code and direction Read.
This will allow Slave to enter in transmitter mode and then send a byte when TXIS interrupt occurs.
When byte is received on I2C2, an RXNE event occurs and DMA transfer data from RXDR register to an internal buffer
until end of transfer.
Master auto-generate a NACK and STOP condition to inform the Slave that the transfer is finished.

The STOP condition generate a STOP interrupt and initiate the end of transmission on Slave side.
I2C1 IRQ handler and Handle_I2C_Master_TransmitReceive() routine are then clearing the STOP flag in both side.

LED2 is On (500 ms) if data are well sent and the Command Code sent to slave is print as follow depending of IDE :
Note that
 - When resorting to EWARM IAR IDE:
 Command Code is displayed on debugger as follows: View --> Terminal I/O

 - When resorting to MDK-ARM KEIL IDE:
   Command Code could not be displayed on debugger but user can use the Virtual Com port of the STM32F072RB-Nucleo RevC
   to display Command Code on PC side using an hyperterminal. See below, information related to this possibility.

- When resorting to AC6 SW4STM32 IDE:
 In Debug configuration window\ Startup, in addition to "monitor reset halt" add the command "monitor arm semihosting enable"
 Command Code is displayed on debugger as follows: Window--> Show View--> Console.

Other proposal to retrieve display of Command Code for all IDE is to use the Virtual Com.

In order to select use of Virtual Com port feature of STLINK for connection between STM32F072RB-Nucleo RevC and PC,
User has to set USE_VCP_CONNECTION define to 1 in main.h file.
If so, please ensure that USART communication between the target MCU and ST-LINK MCU is properly enabled 
on HW board in order to support Virtual Com Port (Default HW SB configuration allows use of VCP)

After each use cases, the LED2 blinks quickly to wait for a new user-button press to send a new Command code to the Slave device.

In all cases, if an error occurs, LED2 is blinking slowly.


@par Directory contents 

  - I2C/I2C_OneBoard_AdvCommunication_DMAAndIT/Inc/stm32f0xx_it.h          Interrupt handlers header file
  - I2C/I2C_OneBoard_AdvCommunication_DMAAndIT/Inc/main.h                  Header for main.c module
  - I2C/I2C_OneBoard_AdvCommunication_DMAAndIT/Inc/stm32_assert.h          Template file to include assert_failed function
  - I2C/I2C_OneBoard_AdvCommunication_DMAAndIT/Src/stm32f0xx_it.c          Interrupt handlers
  - I2C/I2C_OneBoard_AdvCommunication_DMAAndIT/Src/main.c                  Main program
  - I2C/I2C_OneBoard_AdvCommunication_DMAAndIT/Src/system_stm32f0xx.c      STM32F0xx system source file

@par Hardware and Software environment

  - This example runs on STM32F072xB devices.
    
  - This example has been tested with STM32F072RB-Nucleo RevC board and can be
    easily tailored to any other supported device and development board.

  - STM32F072RB-Nucleo RevC Set-up
    - Connect GPIOs connected to I2C1 SCL/SDA (PB.6 and PB.7)
    to respectively SCL and SDA pins of I2C2 (PB.13 and PB.14).
      - I2C1_SCL  PB.6 (CN10, pin 17) : connected to I2C2_SCL PB.13 (CN10, pin 30) 
      - I2C1_SDA  PB.7 (CN7, pin 21) : connected to I2C2_SDA PB.14 (CN10, pin 28)

  - Launch the program in debug mode to benefit of Terminal I/O information. Press User push-button to initiate a write request by Master 
      then Slave receive bytes.

    Or

  - Launch serial communication SW on PC (as HyperTerminal or TeraTerm) with proper configuration 
    (115200 bauds, 8 bits data, 1 stop bit, no parity, no HW flow control) connected with the com port number of STLINK.
    Then Launch the program. Press User push-button to initiate a write request by Master 
      then Slave receive bytes.

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
