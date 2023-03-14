/**
  ******************************************************************************
  * @file    USB_Device/MSC_Standalone/Inc/main.h
  * @author  MCD Application Team
  * @brief   Header for main.c module
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
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "stm32072b_eval_sd.h"
#include "usbd_core.h"
#include "usbd_desc.h"
#include "usbd_msc.h"
#include "usbd_storage.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/

/* Uncomment the line below to select your USB clock source */
#define USE_USB_CLKSOURCE_CRSHSI48   1
/*#define USE_USB_CLKSOURCE_PLL        1*/

#if !defined (USE_USB_CLKSOURCE_PLL) && !defined (USE_USB_CLKSOURCE_CRSHSI48)
 #error "Missing USB clock definition"
#endif
/* Exported functions ------------------------------------------------------- */

#endif /* __MAIN_H */
