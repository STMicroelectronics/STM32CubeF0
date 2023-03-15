/**
  @page I2C_TwoBoards_ComDMA I2C Two Boards Communication DMA example
  
  @verbatim
  ******************** (C) COPYRIGHT 2016 STMicroelectronics *******************
  * @file    I2C/I2C_TwoBoards_ComDMA/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the I2C Two Boards Communication DMA example.
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
via DMA.

Board: STM32F030R8-Nucleo Rev C (embeds a STM32F030R8T6 device)
SCL Pin: PB6 (CN10, pin17)
SDA Pin: PB7 (CN7, pin21)

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
GPIO, DMA and NVIC). You may update this function to change I2C configuration.

The I2C communication is then initiated.
The project is split in two parts: the Master Board and the Slave Board
- Master Board
  The HAL_I2C_Master_Receive_DMA() and the HAL_I2C_Master_Transmit_DMA() functions 
  allow respectively the reception and the transmission of a predefined data buffer
  in Master mode.
- Slave Board
  The HAL_I2C_Slave_Receive_DMA() and the HAL_I2C_Slave_Transmit_DMA() functions 
  allow respectively the reception and the transmission of a predefined data buffer
  in Slave mode.
The user can choose between Master and Slave through "#define MASTER_BOARD"
in the "main.c" file:
If the Master board is used, the "#define MASTER_BOARD" must be uncommented.
If the Slave board is used the "#define MASTER_BOARD" must be commented.

For this example the aTxBuffer is predefined and the aRxBuffer size is same as aTxBuffer.

In a first step after the user press the User push-button on the Master Board,
I2C Master starts the communication by sending aTxBuffer through HAL_I2C_Master_Transmit_DMA()
to I2C Slave which receives aRxBuffer through HAL_I2C_Slave_Receive_DMA(). 
The second step starts when the user press the User push-button on the Master Board,
the I2C Slave sends aTxBuffer through HAL_I2C_Slave_Transmit_DMA()
to the I2C Master which receives aRxBuffer through HAL_I2C_Master_Receive_DMA().
The end of this two steps are monitored through the HAL_I2C_GetState() function
result.
Finally, aTxBuffer and aRxBuffer are compared through Buffercmp() in order to 
check buffers correctness.  

STM32F030R8-Nucleo Rev C's LED can be used to monitor the transfer status:
 - LED2 is ON when the transmission process is complete.
 - LED2 is OFF when the reception process is complete.
 - LED2 is slowly blinking (1 sec. period) in case of error in initialization or 
transmission/reception process

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

  - I2C/I2C_TwoBoards_ComDMA/Inc/stm32f0xx_hal_conf.h    HAL configuration file
  - I2C/I2C_TwoBoards_ComDMA/Inc/stm32f0xx_it.h          DMA and I2C interrupt handlers header file
  - I2C/I2C_TwoBoards_ComDMA/Inc/main.h                  Header for main.c module  
  - I2C/I2C_TwoBoards_ComDMA/Src/stm32f0xx_it.c          DMA and I2C interrupt handlers
  - I2C/I2C_TwoBoards_ComDMA/Src/main.c                  Main program
  - I2C/I2C_TwoBoards_ComDMA/Src/system_stm32f0xx.c      STM32F0xx system source file
  - I2C/I2C_TwoBoards_ComDMA/Src/stm32f0xx_hal_msp.c     HAL MSP file    


@par Hardware and Software environment

  - This example runs on STM32F030R8 devices.
    
  - This example has been tested with STM32F030R8-Nucleo Rev C board and can be
    easily tailored to any other supported device and development board.    

  - STM32F030R8-Nucleo Rev C Set-up
    - Connect I2C_SCL line of Master board (PB6, CN10, pin17) to I2C_SCL line of Slave Board (PB6, CN10, pin17).
    - Connect I2C_SDA line of Master board (PB7, CN7, pin21) to I2C_SDA line of Slave Board (PB7, CN7, pin21).
    - Connect GND of Master board to GND of Slave Board.

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
    o Uncomment "#define MASTER_BOARD" and load the project in Master Board
    o Comment "#define MASTER_BOARD" and load the project in Slave Board
 - Run the example


 */
