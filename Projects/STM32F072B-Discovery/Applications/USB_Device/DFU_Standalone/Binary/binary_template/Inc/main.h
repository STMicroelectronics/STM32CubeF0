/**
  ******************************************************************************
  * @file    DFU_Standalone/Binary/binary_template/Inc/main.h
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
#include "stm32f072b_discovery.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Uncomment the line below to select your USB clock source */
#define USE_CLKSOURCE_CRSHSI48   1
/* #define USE_CLKSOURCE_PLL        1 */

#if !defined (USE_CLKSOURCE_PLL) && !defined (USE_CLKSOURCE_CRSHSI48)
 #error "Missing System clock definition"
#endif
/* Exported functions ------------------------------------------------------- */

#endif /* __MAIN_H */
