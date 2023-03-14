/**
  ******************************************************************************
  * @file    stm32f072b_discovery_gyroscope.c
  * @author  MCD Application Team
  * @brief   This file provides a set of functions needed to manage the l3gd20
  *          MEMS accelerometer available on STM32F072B-Discovery Kit.
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
#include "stm32f072b_discovery_gyroscope.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup STM32F072B_DISCOVERY
  * @{
  */

/** @addtogroup STM32F072B_DISCOVERY_GYRO
  * @{
  */

/** @defgroup STM32F072B_DISCOVERY_GYRO_Private_Variables Private Variables
  * @{
  */
static GYRO_DrvTypeDef *GyroscopeDrv;

/**
  * @}
  */

/** @addtogroup STM32F072B_DISCOVERY_GYRO_Exported_Functions
  * @{
  */

/**
  * @brief  Set GYRO Initialization.
  * @retval GYRO_OK if no problem during initialization
  */
uint8_t BSP_GYRO_Init(void)
{
  uint16_t ctrl = 0x0000;
  uint8_t HIGHPASSFILTER_Status = 0x00;
  GYRO_InitTypeDef GYRO_InitStructure;
  GYRO_FilterConfigTypeDef GYRO_FilterStructure = {0, 0};

  if ((L3gd20Drv.ReadID() == I_AM_L3GD20) || (L3gd20Drv.ReadID() == I_AM_L3GD20_TR))
  {
    /* Initialize the gyroscope driver structure */
    GyroscopeDrv = &L3gd20Drv;

    /* Configure Mems : data rate, power mode, full scale and axes */
    GYRO_InitStructure.Power_Mode = L3GD20_MODE_ACTIVE;
    GYRO_InitStructure.Output_DataRate = L3GD20_OUTPUT_DATARATE_1;
    GYRO_InitStructure.Axes_Enable = L3GD20_AXES_ENABLE;
    GYRO_InitStructure.Band_Width = L3GD20_BANDWIDTH_4;
    GYRO_InitStructure.BlockData_Update = L3GD20_BlockDataUpdate_Continous;
    GYRO_InitStructure.Endianness = L3GD20_BLE_LSB;
    GYRO_InitStructure.Full_Scale = L3GD20_FULLSCALE_500;

    GYRO_FilterStructure.HighPassFilter_Mode_Selection = L3GD20_HPM_NORMAL_MODE_RES;
    GYRO_FilterStructure.HighPassFilter_CutOff_Frequency = L3GD20_HPFCF_0;
    HIGHPASSFILTER_Status = L3GD20_HIGHPASSFILTER_ENABLE;
  }
  else if ((I3g4250Drv.ReadID() == I_AM_I3G4250D))
  {
    /* Initialize the gyroscope driver structure */
    GyroscopeDrv = &I3g4250Drv;

    /* Configure Mems : data rate, power mode, full scale and axes */
    GYRO_InitStructure.Power_Mode = I3G4250D_MODE_ACTIVE;
    GYRO_InitStructure.Output_DataRate = I3G4250D_OUTPUT_DATARATE_1;
    GYRO_InitStructure.Axes_Enable = I3G4250D_AXES_ENABLE;
    GYRO_InitStructure.Band_Width = I3G4250D_BANDWIDTH_4;
    GYRO_InitStructure.BlockData_Update = I3G4250D_BlockDataUpdate_Continous;
    GYRO_InitStructure.Endianness = I3G4250D_BLE_LSB;
    GYRO_InitStructure.Full_Scale = I3G4250D_FULLSCALE_500;

    GYRO_FilterStructure.HighPassFilter_Mode_Selection = I3G4250D_HPM_NORMAL_MODE_RES;
    GYRO_FilterStructure.HighPassFilter_CutOff_Frequency = I3G4250D_HPFCF_0;
    HIGHPASSFILTER_Status = I3G4250D_HIGHPASSFILTER_ENABLE;
  }
  else
  {
    return (GYRO_ERROR);
  }
  /* Configure MEMS: data rate, power mode, full scale and axes */
  ctrl = (uint16_t)(GYRO_InitStructure.Power_Mode | GYRO_InitStructure.Output_DataRate | \
                    GYRO_InitStructure.Axes_Enable | GYRO_InitStructure.Band_Width);

  ctrl |= (uint16_t)((GYRO_InitStructure.BlockData_Update | GYRO_InitStructure.Endianness | \
                      GYRO_InitStructure.Full_Scale) << 8);

  /* Gyroscope Init */
  GyroscopeDrv->Init(ctrl);


  ctrl = (uint8_t)((GYRO_FilterStructure.HighPassFilter_Mode_Selection | \
                    GYRO_FilterStructure.HighPassFilter_CutOff_Frequency));

  GyroscopeDrv->FilterConfig(ctrl);

  GyroscopeDrv->FilterCmd(HIGHPASSFILTER_Status);

  return (GYRO_OK);
}

/**
  * @brief  Read ID of Gyroscope component
  * @retval ID
  */
uint8_t BSP_GYRO_ReadID(void)
{
  uint8_t id = 0x00;

  if (GyroscopeDrv->ReadID != NULL)
  {
    id = GyroscopeDrv->ReadID();
  }
  return id;
}

/**
  * @brief  Reboot memory content of GYRO
  * @retval None
  */
void BSP_GYRO_Reset(void)
{
  if (GyroscopeDrv->Reset != NULL)
  {
    GyroscopeDrv->Reset();
  }
}

/**
  * @brief  Configure INT1 interrupt
  * @param  pIntConfig pointer to a L3GD20_InterruptConfig_TypeDef
  *         structure that contains the configuration setting for the L3GD20 Interrupt.
  * @retval None
  */
void BSP_GYRO_ITConfig(GYRO_InterruptConfigTypeDef *pIntConfig)
{
  uint16_t interruptconfig = 0x0000;

  if (GyroscopeDrv->ConfigIT != NULL)
  {
    /* Configure latch Interrupt request and axe interrupts */
    interruptconfig |= ((uint8_t)(pIntConfig->Latch_Request | \
                                  pIntConfig->Interrupt_Axes) << 8);

    interruptconfig |= (uint8_t)(pIntConfig->Interrupt_ActiveEdge);

    GyroscopeDrv->ConfigIT(interruptconfig);
  }
}

/**
  * @brief  Enable INT1 or INT2 interrupt
  * @param  IntPin Interrupt pin
  *      This parameter can be:
  *        @arg L3GD20_INT1
  *        @arg L3GD20_INT2
  * @retval None
  */
void BSP_GYRO_EnableIT(uint8_t IntPin)
{
  if (GyroscopeDrv->EnableIT != NULL)
  {
    GyroscopeDrv->EnableIT(IntPin);
  }
}

/**
  * @brief  Disable INT1 or INT2 interrupt
  * @param  IntPin Interrupt pin
  *      This parameter can be:
  *        @arg L3GD20_INT1
  *        @arg L3GD20_INT2
  * @retval None
  */
void BSP_GYRO_DisableIT(uint8_t IntPin)
{
  if (GyroscopeDrv->DisableIT != NULL)
  {
    GyroscopeDrv->DisableIT(IntPin);
  }
}

/**
  * @brief  Get XYZ angular acceleration
  * @param pfData pointer on floating array
  * @retval None
  */
void BSP_GYRO_GetXYZ(float *pfData)
{
  if (GyroscopeDrv->GetXYZ != NULL)
  {
    GyroscopeDrv->GetXYZ(pfData);
  }
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

