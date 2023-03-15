/**
  @page I2C_TwoBoards_AdvComIT I2C Two Boards Advanced Communication IT example
  
  @verbatim
  ******************** (C) COPYRIGHT 2016 STMicroelectronics *******************
  * @file    I2C/I2C_TwoBoards_AdvComIT/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the I2C Two Boards Advanced Communication with 
  *          Interrupt example.
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

How to handle I2C data buffer transmission/reception between two boards, 
using an interrupt.

Board: STM32F0308-Discovery RevA (embeds a STM32F030R8T6 device)
SCL Pin: PB6 (P2)
SDA Pin: PB7 (P2)

   _________________________                       _________________________ 
  |           ______________|                     |______________           |
  |          |I2C1          |                     |          I2C1|          |
  |          |              |                     |              |          |
  |          |          SCL |_____________________| SCL          |          |
  |          |              |                     |              |          |
  |          |              |                     |              |          |
  |          |              |                     |              |          |
  |          |          SDA |_____________________| SDA          |          |
  |          |              |                     |              |          |
  |          |______________|                     |______________|          |
  |                         |                     |                         |
  |                      GND|_____________________|GND                      |
  |_STM32_Board 1___________|                     |_STM32_Board 2___________|

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system
clock (SYSCLK) to run at 48 MHz.

The I2C peripheral configuration is ensured by the HAL_I2C_Init() function.
This later is calling the HAL_I2C_MspInit()function which core is implementing
the configuration of the needed I2C resources according to the used hardware (CLOCK, 
GPIO and NVIC). You may update this function to change I2C configuration.

The I2C communication is then initiated.
The project is split in two parts: the Master Board and the Slave Board
- Master Board
  The HAL_I2C_Master_Receive_IT() and the HAL_I2C_Master_Transmit_IT() functions 
  allow respectively the reception and the transmission of a predefined data buffer
  in Master mode using interrupt.
- Slave Board
  The HAL_I2C_Slave_Receive_IT() and the HAL_I2C_Slave_Transmit_IT() functions 
  allow respectively the reception and the transmission of a predefined data buffer
  in Slave mode using interrupt.
The user can choose between Master and Slave through "#define MASTER_BOARD"
in the "main.c" file:
- Uncomment "#define MASTER_BOARD" to select Master board.
- Comment "#define MASTER_BOARD" to select Slave board.

For this example two buffers are used 
- aTxBuffer buffer contains the data to be transmitted 
- aRxBuffer buffer is used to save the received data
Note that both buffers have same size
                       
On Master board side:
 - Wait User push-button to be pressed (used for only synchronization at startup)
 - Request write operation by sending specific command "MASTER_REQ_WRITE" to Slave
 - Send the number of data to be written
 - Transmit aTxBuffer buffer to slave
 - Request read operation by sending specific command "MASTER_REQ_READ" to Slave
 - Send the number of data to be read
 - Receive data from Slave in aRxBuffer
 - Check the correctness of data and Toggle LED3 when data is received correctly,
   otherwise LED4 is turned ON and communication is stopped (using infinite loop)


On Slave board side:
 - Receive request from Master
 - Receive the request operation from Master and depending on the operation type (write or read):
   - If Master requests write operation:
      - Receive number of data to be written by Master
      - Receive data from master in aRxBuffer
      - Check the correctness of data and Toggle LED3 when data is received correctly,
        otherwise LED4 is turned ON and communication is stopped (using infinite loop)
   - If Master request read operation:
      - Receive number of data to be written to Master
      - Transmit aTxBuffer buffer to master
   
These operations are repeated periodically and the start of communication is triggered 
by pushing the key button of the Master board.  

 @note In Master side, only Acknowledge failure error is handled. When this error
       occurs Master restart the current operation until Slave acknowledges it's
       address.
        
 @note I2Cx instance used and associated resources can be updated in "main.h"
       file depending hardware configuration used.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Directory contents 

  - I2C/I2C_TwoBoards_AdvComIT/Inc/stm32f0xx_hal_conf.h    HAL configuration file
  - I2C/I2C_TwoBoards_AdvComIT/Inc/stm32f0xx_it.h          I2C interrupt handlers header file
  - I2C/I2C_TwoBoards_AdvComIT/Inc/main.h                  Header for main.c module  
  - I2C/I2C_TwoBoards_AdvComIT/Src/stm32f0xx_it.c          I2C interrupt handlers
  - I2C/I2C_TwoBoards_AdvComIT/Src/main.c                  Main program
  - I2C/I2C_TwoBoards_AdvComIT/Src/system_stm32f0xx.c      STM32F0xx system source file
  - I2C/I2C_TwoBoards_AdvComIT/Src/stm32f0xx_hal_msp.c     HAL MSP file    


@par Hardware and Software environment

  - This example runs on STM32F030R8 devices.
    
  - This example has been tested with STM32F0308-Discovery RevA board and can be
    easily tailored to any other supported device and development board.    

  - STM32F0308-Discovery RevA Set-up
    - Connect I2C_SCL line of Master board (PB6, P2) to I2C_SCL line of Slave Board (PB6, P2).
    - Connect I2C_SDA line of Master board (PB7, P2) to I2C_SDA line of Slave Board (PB7, P2).
    - Connect GND of Master board to GND of Slave Board.

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
    o Uncomment "#define MASTER_BOARD" and load the project in Master Board
    o Comment "#define MASTER_BOARD" and load the project in Slave Board
 - Run the example


 */
