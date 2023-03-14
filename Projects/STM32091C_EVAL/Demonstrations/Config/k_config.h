/**
  ******************************************************************************
  * @file    k_conifg.h
  * @author  MCD Application Team
  * @brief   Header for k_config.c file
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
#ifndef __K_CONFIG_H
#define __K_CONFIG_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
#define EVAL_BOARD   "STM32091C-EVAL"
#define DEMO_VERSION "Version 1.10.0"
#define DEMO_DATE    "27 - October - 2017"   

#define DEMO_INFO1   "MCD Application Team"
#define DEMO_INFO2   "COPYRIGHT 2016"   
#define DEMO_INFO3   "STMicroelectronics"
   
#define CHOOSEFILE_MAXLEN   255
   
typedef enum{
 MODULE_MAIN_APP,
 MODULE_LOWPOWER,
 MODULE_8UART,
 MODULE_TSENSOR,
 MODULE_LOG_DEMO,
 MODULE_VIEWBMP,
 MODULE_FILESBRO,
 MODULE_SYSTEM_INFO,
 MODULE_MAX,
 MODULE_NONE = 0xFF
} MODULES_INFO;

#define countof(a) (sizeof(a) / sizeof(*(a)))
/* Exported functions ------------------------------------------------------- */ 

#ifdef __cplusplus
}
#endif

#endif /*__K_LOG_H */
