/**
  @page SPI_TwoBoards_FullDuplex_DMA SPI example
  
  @verbatim
  ******************** (C) COPYRIGHT 2016 STMicroelectronics *******************
  * @file    Examples_LL/SPI/SPI_TwoBoards_FullDuplex_DMA/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the SPI_TwoBoards_FullDuplex_DMA example.
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

Data buffer transmission and reception via SPI using DMA mode. This example is 
based on the STM32F0xx SPI LL API. The peripheral initialization uses 
LL unitary service functions for optimization purposes (performance and size).

The communication is done with 2 boards through SPI.
   _________________________                        _________________________
  |       ___________ ______|                      |__________________       |
  |      |SPI1              |                      |             SPI1 |      |
  |      |                  |                      |                  |      |
  |      |         CLK(PB3) |______________________|(PB3)CLK          |      |
  |      |                  |                      |                  |      |
  |      |         MISO(PB4)|______________________|(PB4)MISO         |      |
  |      |                  |                      |                  |      |
  |      |         MOSI(PB5)|______________________|(PB5)MOSI         |      |
  |      |                  |                      |                  |      |
  |      |__________________|                      |__________________|      |
  |      __                 |                      |                         |
  |     |__|                |                      |                         |
  |     USER                |                      |                         |
  |                      GND|______________________|GND                      |
  |                         |                      |                         |
  |_STM32F0xx Master _______|                      |_STM32F0xx Slave ________|

This example shows how to configure GPIO and SPI peripherals
to use a Full-Duplex communication using DMA Transfer mode through the STM32F0xx SPI LL API.

The project is split in two parts the Master Board and the Slave Board:

- Master Board
  SPI1 Peripheral is configured in Master mode.
  DMA1_Channel2_3 and DMA1_Channel2_3 configured to transfer Data via SPI peripheral
  GPIO associated to User push-button is linked with EXTI. 
  
- Slave Board
  SPI1 Peripheral is configured in Slave mode.
  DMA1_Channel2_3 and DMA1_Channel2_3 configured to transfer Data via SPI peripheral

The user can choose between Master and Slave through "#define MASTER_BOARD"
in the "main.c" file:
- Uncomment "#define MASTER_BOARD" to select Master board.
- Comment "#define MASTER_BOARD" to select Slave board.

Example execution:
On BOARD MASTER, LED2 is blinking Fast (200ms) and wait User push-button action.
Press User push-button on BOARD MASTER start a Full-Duplex communication through DMA.
On MASTER side, Clock will be generated on SCK line, Transmission(MOSI Line) and reception (MISO Line) 
will be done at the same time. 
SLAVE SPI will received  the Clock (SCK Line), so Transmission(MISO Line) and reception (MOSI Line) will be done also.

LED2 is On on both boards if data is well received.

In case of errors, LED2 is blinking Slowly (1s).

@note You need to perform a reset on Master board, then perform it on Slave board
      to have the correct behaviour of this example.


@par Directory contents 

  - SPI/SPI_TwoBoards_FullDuplex_DMA/Inc/stm32f0xx_it.h          Interrupt handlers header file
  - SPI/SPI_TwoBoards_FullDuplex_DMA/Inc/main.h                  Header for main.c module
  - SPI/SPI_TwoBoards_FullDuplex_DMA/Inc/stm32_assert.h          Template file to include assert_failed function
  - SPI/SPI_TwoBoards_FullDuplex_DMA/Src/stm32f0xx_it.c          Interrupt handlers
  - SPI/SPI_TwoBoards_FullDuplex_DMA/Src/main.c                  Main program
  - SPI/SPI_TwoBoards_FullDuplex_DMA/Src/system_stm32f0xx.c      STM32F0xx system source file

@par Hardware and Software environment

  - This example runs on STM32F072xB devices.

  - This example has been tested with STM32F072RB-Nucleo RevC board and can be
    easily tailored to any other supported device and development board.

  - STM32F072RB-Nucleo RevC Set-up
    - Connect Master board PB3 to Slave Board PB3 (connected to pin 31 of CN10 connector)
    - Connect Master board PB4 to Slave Board PB4 (connected to pin 27 of CN10 connector)
    - Connect Master board PB5 to Slave Board PB5 (connected to pin 29 of CN10 connector)
    - Connect Master board GND to Slave Board GND

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
    o Uncomment "#define MASTER_BOARD" and load the project in Master Board
    o Comment "#define MASTER_BOARD" and load the project in Slave Board
 - Run the example


 */
