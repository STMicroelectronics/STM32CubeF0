/**
  ******************************************************************************
  * @file    k_mem.h
  * @author  MCD Application Team
  * @brief   Header for k_mem.c file
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
#ifndef _K_TOOLS_H
#define _K_TOOLS_H

#ifdef __cplusplus
 extern "C" {
#endif

#ifdef _K_TOOLS_C
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
GLOBAL uint16_t kTools_Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength);

#ifdef __cplusplus
}
#endif

#endif /* _K_MEM_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
