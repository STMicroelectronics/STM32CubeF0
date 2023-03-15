/**
  ******************************************************************************
  * @file    stm32f072b_discovery_eeprom.h
  * @author  MCD Application Team
  * @brief   This file contains all the functions prototypes for 
  *          the stm32072b_discovery_eeprom.c firmware driver.
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
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32072B_DISCOVERY_EEPROM_H
#define __STM32072B_DISCOVERY_EEPROM_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f072b_discovery.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup STM32F072B_DISCOVERY
  * @{
  */

/** @defgroup STM32F072B_DISCOVERY_EEPROM STM32F072B_DISCOVERY EEPROM
  * @{
  */  

/** @defgroup STM32F072B_DISCOVERY_EEPROM_Exported_Constants Exported Constants
  * @{
  */
  
/* EEPROM hardware address and page size */ 
#define EEPROM_PAGESIZE             4
#define EEPROM_MAX_SIZE             0x2000 /* 64Kbit*/
                                
/* Maximum Timeout values for flags and events waiting loops. 
This timeout is based on systick set to 1ms*/   
#define EEPROM_LONG_TIMEOUT         ((uint32_t)(1000))

/* Maximum number of trials for EEPROM_WaitEepromStandbyState() function */
#define EEPROM_MAX_TRIALS_NUMBER     300
      
#define EEPROM_OK                    0
#define EEPROM_FAIL                  1
#define EEPROM_TIMEOUT               2

/**
  * @}
  */ 

/** @defgroup STM32F072B_DISCOVERY_EEPROM_Exported_Functions Exported Functions
  * @{
  */ 
uint32_t          BSP_EEPROM_Init(void);
uint32_t          BSP_EEPROM_ReadBuffer(uint8_t* pBuffer, uint16_t ReadAddr, uint16_t* NumByteToRead);
uint32_t          BSP_EEPROM_WritePage(uint8_t* pBuffer, uint16_t WriteAddr, uint8_t* NumByteToWrite);
uint32_t          BSP_EEPROM_WriteBuffer(uint8_t* pBuffer, uint16_t WriteAddr, uint16_t NumByteToWrite);
uint32_t          BSP_EEPROM_WaitEepromStandbyState(void);

/* USER Callbacks: This function is declared as __weak in EEPROM driver and 
   should be implemented into user application.  
   BSP_EEPROM_TIMEOUT_UserCallback() function is called whenever a timeout condition 
   occur during communication (waiting on an event that doesn't occur, bus 
   errors, busy devices ...). */
__weak uint32_t   BSP_EEPROM_TIMEOUT_UserCallback(void);

/* Link function for I2C EEPROM peripheral */
void              EEPROM_IO_Init(void);
uint32_t          EEPROM_IO_WriteData(uint16_t DevAddress, uint16_t MemAddress, uint32_t pBuffer, uint32_t BufferSize);
uint32_t          EEPROM_IO_ReadData(uint16_t DevAddress, uint16_t MemAddress, uint32_t pBuffer, uint32_t BufferSize);
HAL_StatusTypeDef EEPROM_IO_IsDeviceReady(uint16_t DevAddress, uint32_t Trials);

/**
  * @}
  */ 

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */ 

#ifdef __cplusplus
}
#endif

#endif /* __STM32072B_DISCOVERY_EEPROM_H */

