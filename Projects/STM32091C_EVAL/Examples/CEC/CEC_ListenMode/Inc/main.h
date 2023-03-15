/**
  ******************************************************************************
  * @file    CEC/CEC_ListenMode/Inc/main.h 
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
#include "stm32f0xx_hal.h"
#include "stm32091c_eval.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define CEC_MAX_PAYLOAD                  16

/* Uncomment the line below if you use the CEC peripheral as a Device 1 */
//#define DEVICE_1   
/* Uncomment the line below if you use the CEC peripheral as a Device 2 */ 
//#define DEVICE_2
/* Uncomment the line below if you use the CEC peripheral as Spy Device    */ 
#define DEVICE_3  

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#endif /* __MAIN_H */
