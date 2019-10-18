/**
  ******************************************************************************
  * @file    main.h 
  * @author  MCD Application Team
  * @brief   Header for main.c file
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2016 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
 extern "C" {
#endif
   
/* Includes ------------------------------------------------------------------*/
#include "stddef.h"
//#include "stm32f0xx_hal.h"
//#include "stm32f0xx_it.h"

/* EVAL includes component */
#include "stm32091c_eval.h"
#include "stm32091c_eval_tsensor.h"
#include "stm32091c_eval_lcd.h"
#include "stm32091c_eval_sd.h"

/* FatFs includes component */
#include "ff_gen_drv.h"
#include "sd_diskio.h"
#include "main.h"

/* Exported types ------------------------------------------------------------*/
typedef union
{
  uint32_t d32;
  struct
  {
    uint32_t enable_sprite     : 1;
    uint32_t enable_background : 1;
    uint32_t use_180Mhz        : 1;
    uint32_t disable_flex_skin : 1;  
  }b;
}
SystemSettingsTypeDef;

/* Exported variables --------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define NUM_DISK_UNITS       2
#define USB_DISK_UNIT        0
#define MSD_DISK_UNIT        1

#define CALIBRATION_BKP0                    RTC_BKP_DR0
#define CALIBRATION_BKP1                    RTC_BKP_DR1
#define CALIBRATION_GENERAL_SETTINGS_BKP    RTC_BKP_DR2
#define CALIBRATION_IMAGE_SETTINGS_BKP      RTC_BKP_DR3
#define CALIBRATION_AUDIOPLAYER_SETTING_BKP RTC_BKP_DR4
#define CALIBRATION_CAMERA_SETTING_BKP      RTC_BKP_DR5
#define CALIBRATION_BENCH_SETTING_BKP       RTC_BKP_DR6
#define CALIBRATION_VIDEOPLAYER_SETTING_BKP RTC_BKP_DR7

/* Exported macros -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */ 

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
