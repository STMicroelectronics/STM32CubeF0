/**
  ******************************************************************************
  * @file    stm32091c_eval_tsensor.c
  * @author  MCD Application Team
  * @brief   This file provides a set of functions needed to manage the I2C STLM75 
  *          temperature sensor mounted on STM32091C-EVAL board . 
  *          It implements a high level communication layer for read and write 
  *          from/to this sensor. The needed STM32F0xx hardware resources (I2C and 
  *          GPIO) are defined in stm32091c_eval.h file, and the initialization is 
  *          performed in TSENSOR_IO_Init() function declared in stm32091c_eval.c 
  *          file.
  *          You can easily tailor this driver to any other development board, 
  *          by just adapting the defines for hardware resources and 
  *          TSENSOR_IO_Init() function. 
  *
  *     +-----------------------------------------------------------------+
  *     |                        Pin assignment                           |
  *     +---------------------------------------+-----------+-------------+
  *     |  STM32F0xx I2C Pins                   |   STLM75  |   Pin       |
  *     +---------------------------------------+-----------+-------------+
  *     | STLM75_I2C_SDA_PIN/ SDA               |   SDA     |    1        |
  *     | STLM75_I2C_SCL_PIN/ SCL               |   SCL     |    2        |
  *     | STLM75_I2C_SMBUSALERT_PIN/ SMBUS ALERT|   OS/INT  |    3        |
  *     | .                                     |   GND     |    4  (0V)  |
  *     | .                                    |   GND     |    5  (0V)  |
  *     | .                                     |   GND     |    6  (0V)  |
  *     | .                                     |   GND     |    7  (0V)  |
  *     | .                                    |   VDD     |    8  (3.3V)|
  *     +---------------------------------------+-----------+-------------+
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

/* Includes ------------------------------------------------------------------*/
#include "stm32091c_eval_tsensor.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup STM32091C_EVAL
  * @{
  */

/** @addtogroup STM32091C_EVAL_TSENSOR
  * @brief      This file includes the STLM75 Temperature Sensor driver of 
  *             STM32091C-EVAL boards.
  * @{
  */ 

/** @addtogroup STM32091C_EVAL_TSENSOR_Private_Variables
  * @{
  */ 
static TSENSOR_DrvTypeDef  *tsensor_drv; 
__IO uint16_t  TSENSORAddress = 0;

/**
  * @}
  */ 

/** @addtogroup STM32091C_EVAL_TSENSOR_Private_Functions
  * @{
  */ 

/**
  * @brief  Initializes peripherals used by the I2C Temperature Sensor driver.
  * @retval TSENSOR status
  */
uint32_t BSP_TSENSOR_Init(void)
{ 
  uint8_t ret = TSENSOR_ERROR;
  TSENSOR_InitTypeDef STLM75_InitStructure;

  /* Temperature Sensor Initialization */
  if(Stlm75Drv.IsReady(TSENSOR_I2C_ADDRESS_A01, TSENSOR_MAX_TRIALS) == HAL_OK)
  {
    /* Initialize the temperature sensor driver structure */
    TSENSORAddress = TSENSOR_I2C_ADDRESS_A01;
    tsensor_drv = &Stlm75Drv;

    ret = TSENSOR_OK;
  }
  else
  {
    if(Stlm75Drv.IsReady(TSENSOR_I2C_ADDRESS_A02, TSENSOR_MAX_TRIALS) == HAL_OK)
    {
      /* Initialize the temperature sensor driver structure */
      TSENSORAddress = TSENSOR_I2C_ADDRESS_A02;
      tsensor_drv = &Stlm75Drv;

      ret = TSENSOR_OK;
    }
    else
    {
      ret = TSENSOR_ERROR;
    }
  }

  if (ret == TSENSOR_OK)
  {
    /* Configure Temperature Sensor : Conversion 9 bits in continuous mode */
    /* Alert outside range Limit Temperature 12° <-> 24°c */
    STLM75_InitStructure.AlertMode             = STLM75_INTERRUPT_MODE;
    STLM75_InitStructure.ConversionMode        = STLM75_CONTINUOUS_MODE;
    STLM75_InitStructure.TemperatureLimitHigh  = 24;
    STLM75_InitStructure.TemperatureLimitLow   = 12;
        
    /* TSENSOR Init */   
    tsensor_drv->Init(TSENSORAddress, &STLM75_InitStructure);

    ret = TSENSOR_OK;
  }
  
  return ret;
}

/**
  * @brief  Returns the Temperature Sensor status.
  * @retval The Temperature Sensor status.
  */
uint8_t BSP_TSENSOR_ReadStatus(void)
{
  return (tsensor_drv->ReadStatus(TSENSORAddress));
}

/**
  * @brief  Read Temperature register of STLM75.
  * @retval STLM75 measured temperature value.
  */
uint16_t BSP_TSENSOR_ReadTemp(void)
{ 
  return tsensor_drv->ReadTemp(TSENSORAddress);

}

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

