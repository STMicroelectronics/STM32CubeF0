/**
  ******************************************************************************
  * @file    system_win.c
  * @author  MCD Application Team
  * @brief   System information functions
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

