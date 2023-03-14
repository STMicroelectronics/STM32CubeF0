/**
  ******************************************************************************
  * @file    k_mem.h
  * @author  MCD Application Team
  * @brief   Header for k_mem.c file
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
#ifndef _K_WINDOW_H
#define _K_WINDOW_H

#ifdef __cplusplus
 extern "C" {
#endif

#ifdef _K_WINDOW_C
#define GLOBAL
#else
#define GLOBAL extern
#endif
   
   
/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
GLOBAL void kWindow_Error(char *msg);
GLOBAL void kWindow_Popup(char *title, uint16_t title_tc, uint16_t title_bc ,char *Msg, uint16_t msg_tc, uint16_t msg_bc);


#ifdef __cplusplus
}
#endif

#endif /* _K_MEM_H */
