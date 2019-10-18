/**
  ******************************************************************************
  * @file    UART/UART_OneBoards_8UART/Inc/main.h 
  * @author  MCD Application Team
  * @brief   Header for main.c module
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2016 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx_hal.h"
#include "stm32091c_eval.h"

#ifdef __MAIN_C__
#define GLOBAL
#else
#define GLOBAL extern
#endif

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
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

#define PACKET_SIZE   18
#define BUFFER_SIZE   180

#define NB_PACKET     (BUFFER_SIZE / PACKET_SIZE)

/* Exported variables ---------------------------------------------------------*/
GLOBAL UART_HandleTypeDef  UartHandle[USART__INDEX_MAX];

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
