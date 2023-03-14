/**
  ******************************************************************************
  * @file    stm32072b_eval_tsensor.h
  * @author  MCD Application Team
  * @brief   This file contains all the functions prototypes for the 
  *          stm32072b_eval_tsensor.c firmware driver.
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
#ifndef __STM32072B_EVAL_TSENSOR_H
#define __STM32072B_EVAL_TSENSOR_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32072b_eval.h"
#include "../Components/stlm75/stlm75.h"   

/** @addtogroup BSP
  * @{
  */

/** @addtogroup STM32072B_EVAL
  * @{
  */

/** @defgroup STM32072B_EVAL_TSENSOR STM32072B_EVAL TSENSOR
  * @{
  */

/** @defgroup STM32072B_EVAL_TSENSOR_Exported_Types Exported Types
  * @{
  */

/** 
  * @brief  TSENSOR Status  
  */ 
typedef enum
{
  TSENSOR_OK = 0,
  TSENSOR_ERROR
}TSENSOR_Status_TypDef;

/**
  * @}
  */

/** @defgroup STM32072B_EVAL_TSENSOR_Exported_Constants Exported Constants
  * @{
  */
/* Temperature Sensor hardware I2C address */ 
#define TSENSOR_I2C_ADDRESS_A01 0x90
#define TSENSOR_I2C_ADDRESS_A02 0x92

/* Maximum number of trials use for STTS751_IsReady function */
#define TSENSOR_MAX_TRIALS      50

/**
  * @}
  */

/** @defgroup STM32072B_EVAL_TSENSOR_Exported_Functions Exported Functions
  * @{
  */
uint32_t BSP_TSENSOR_Init(void);
uint8_t  BSP_TSENSOR_ReadStatus(void);
uint16_t BSP_TSENSOR_ReadTemp(void);

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

#endif /* __STM32072B_EVAL_TSENSOR_H */

