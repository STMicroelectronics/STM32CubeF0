/**
  ******************************************************************************
  * @file    stm32091c_eval_eeprom.h
  * @author  MCD Application Team
  * @brief   This file contains all the functions prototypes for 
  *          the stm32091c_eval_eeprom.c firmware driver.
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
#ifndef __STM32091C_EVAL_EEPROM_H
#define __STM32091C_EVAL_EEPROM_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32091c_eval.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup STM32091C_EVAL
  * @{
  */

/** @defgroup STM32091C_EVAL_EEPROM STM32091C-EVAL EEPROM
  * @{
  */

/** @defgroup STM32091C_EVAL_EEPROM_Private_Variables Private Variables
  * @{
  */ 
/**
  * @}
  */ 
  
/** @defgroup STM32091C_EVAL_EEPROM_Exported_Types Exported Types
  * @{
  */ 
typedef struct
{
  uint32_t  (*Init)(void);
  uint32_t  (*ReadBuffer)(uint8_t* , uint16_t , uint32_t* );
  uint32_t  (*WritePage)(uint8_t* , uint16_t , uint32_t* );
}EEPROM_DrvTypeDef;
/**
  * @}
  */

/** @defgroup STM32091C_EVAL_EEPROM_Exported_Constants Exported Constants
  * @{
  */
/* EEPROMs hardware address and page size */ 
#define EEPROM_ADDRESS_M24LR64_A01      0xA0    /* RF EEPROM ANT7-M24LR-A01 used */
#define EEPROM_ADDRESS_M24LR64_A02      0xA6    /* RF EEPROM ANT7-M24LR-A02 used */

#define EEPROM_PAGESIZE_M24LR64         4       /* RF EEPROM ANT7-M24LR-A used */
      
/* EEPROM BSP return values */
#define EEPROM_OK                       0
#define EEPROM_FAIL                     1   
#define EEPROM_TIMEOUT                  2   
 
/* EEPROM BSP devices definition list supported */
#define BSP_EEPROM_M24LR64              1       /* RF I2C EEPROM M24LR64 */

/* Maximum number of trials for EEPROM_I2C_WaitEepromStandbyState() function */
#define EEPROM_MAX_TRIALS               300
/**
  * @}
  */

/** @defgroup STM32091C_EVAL_EEPROM_Exported_Functions Exported Functions
  * @{
  */
uint32_t          BSP_EEPROM_Init(void);
uint32_t          BSP_EEPROM_ReadBuffer(uint8_t* pBuffer, uint16_t ReadAddr, uint32_t* NumByteToRead);
uint32_t          BSP_EEPROM_WriteBuffer(uint8_t* pBuffer, uint16_t WriteAddr, uint32_t NumByteToWrite);

/* USER Callbacks: This function is declared as __weak in EEPROM driver and 
   should be implemented into user application.  
   BSP_EEPROM_TIMEOUT_UserCallback() function is called whenever a timeout condition 
   occur during communication (waiting on an event that doesn't occur, bus 
   errors, busy devices ...). */
void              BSP_EEPROM_TIMEOUT_UserCallback(void);


/* Link functions for I2C EEPROM peripheral */
void              EEPROM_IO_Init(void);
HAL_StatusTypeDef EEPROM_IO_WriteData(uint16_t DevAddress, uint16_t MemAddress, uint8_t* pBuffer, uint32_t BufferSize);
HAL_StatusTypeDef EEPROM_IO_ReadData(uint16_t DevAddress, uint16_t MemAddress, uint8_t* pBuffer, uint32_t BufferSize);
HAL_StatusTypeDef EEPROM_IO_IsDeviceReady(uint16_t DevAddress, uint32_t Trials);

/**
  * @}
  */

/** @defgroup STM32091C_EVAL_EEPROM_Private_Functions Private Functions
  * @{
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

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* __STM32091C_EVAL_EEPROM_H */

