/**
  ******************************************************************************
  * @file    SMBUS/SMBUS_TSENSOR/Src/stm32f0xx_tsensor.h
  * @author  MCD Application Team
  * @brief   This file contains all the functions prototypes for
  *          the stm32f0xx_tsensor.c temperature sensor driver.
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
#ifndef __TSENSOR_H
#define __TSENSOR_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx_hal.h"
   
/** @addtogroup BSP
  * @{
  */

/** @addtogroup Components
  * @{
  */

/** @addtogroup TSENSOR
  * @{
  */

/** @defgroup TSENSOR_Exported_Types
  * @{
  */ 
/** 
  * @brief  Temperature Sensor driver structure definition  
  */
typedef struct
{
  uint8_t AlertMode;            /* Alert Mode Temperature out of range*/
  uint8_t ConversionMode;       /* Continuous/One Shot Mode */
  uint8_t TemperatureLimitHigh; /* High Temperature Limit Range */
  uint8_t TemperatureLimitLow;  /* Low Temperature Limit Range */
}TSENSOR_InitTypeDef;

/** 
  * @brief  TSENSOR Status structures definition  
  */  
typedef enum 
{
  TSENSOR_OK       = 0x00,
  TSENSOR_ERROR    = 0x01,
  TSENSOR_ALERT    = 0x02
} TSENSOR_StatusTypeDef;

/** @defgroup TSENSOR_Exported_Constants
  * @{
  */
/******************************************************************************/
/*************************** START REGISTER MAPPING  **************************/
/******************************************************************************/
/***************************** Read Access Only *******************************/
#define TSENSOR_REG_TEMP       0x00  /*!< Temperature Register of STLM75 */


   
/***************************** Read/Write Access ******************************/
#define TSENSOR_REG_CONF       0x01  /*!< Configuration Register of STLM75 */
#define TSENSOR_REG_THYS       0x02  /*!< Temperature Register of STLM75 */
#define TSENSOR_REG_TOS        0x03  /*!< Over-temp Shutdown threshold Register of STLM75 */


/******************************************************************************/
/**************************** END REGISTER MAPPING  ***************************/
/******************************************************************************/
/** @defgroup Conversion_Mode_Selection 
  * @{
  */
#define TSENSOR_CONTINUOUS_MODE                  ((uint8_t)0x00)
#define TSENSOR_ONE_SHOT_MODE                    ((uint8_t)0x01)
/**
  * @}
  */

/** @defgroup Operation_Mode 
  * @{
  */
#define TSENSOR_COMPARATOR_MODE                  ((uint8_t)0x00)
#define TSENSOR_INTERRUPT_MODE                   ((uint8_t)0x02)
/**
  * @}
  */

/**
  * @}
  */
 
/** @defgroup TSENSOR_Exported_Functions
  * @{
  */
/* Sensor Configuration Functions */ 
TSENSOR_StatusTypeDef   TSENSOR_Init(uint16_t DeviceAddr, TSENSOR_InitTypeDef *pInitStruct);
TSENSOR_StatusTypeDef   TSENSOR_IsReady(uint16_t DeviceAddr, uint32_t Trials);

/* Sensor Request Functions */
uint8_t                 TSENSOR_ReadStatus(uint16_t DeviceAddr);
uint16_t                TSENSOR_ReadTemp(uint16_t DeviceAddr);
uint8_t                 TSENSOR_AlerteResponseAddressRead(void);

/* Sensor Callbacks */
void                    TSENSOR_ErrorCallback(uint16_t Error);
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
  
#endif /* __TSENSOR_H */
