/**
  ******************************************************************************
  * @file    system_win.c
  * @author  MCD Application Team
  * @brief   System information functions
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

#undef GLOBAL
#ifdef __8UART_APP_C
#define GLOBAL
#else
#define GLOBAL extern
#endif

/* Includes ------------------------------------------------------------------*/

/** @addtogroup SYSTEM_INFO_MODULE
  * @{
  */

/** @defgroup SYSTEM_INFO
  * @brief system info routines 
  * @{
  */

/* external defines -----------------------------------------------------------*/
enum {
  USART1_INDEX =0,
  USART2_INDEX,
  USART3_INDEX,
  USART4_INDEX,
  USART5_INDEX,
  USART6_INDEX,
  USART7_INDEX,
  USART8_INDEX, 
  USART__INDEX_MAX
};

/* External variables --------------------------------------------------------*/
GLOBAL const K_ModuleItem_Typedef Module8Uart;
GLOBAL UART_HandleTypeDef  *UartHandle;
/* Private function prototypes -----------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/

/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @}
  */

/**
  * @}
  */
