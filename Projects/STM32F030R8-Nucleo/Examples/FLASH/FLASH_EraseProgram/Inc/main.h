/**
  ******************************************************************************
  * @file    FLASH/FLASH_EraseProgram/Inc/main.h
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
#include "stm32f0xx_nucleo.h"


/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Base address of the Flash sectors */
#define ADDR_FLASH_PAGE_0     ((uint32_t)0x08000000) /* Base @ of Page 0, 1 Kbyte */
#define ADDR_FLASH_PAGE_1     ((uint32_t)0x08000400) /* Base @ of Page 1, 1 Kbyte */
#define ADDR_FLASH_PAGE_2     ((uint32_t)0x08000800) /* Base @ of Page 2, 1 Kbyte */
#define ADDR_FLASH_PAGE_3     ((uint32_t)0x08000C00) /* Base @ of Page 3, 1 Kbyte */
#define ADDR_FLASH_PAGE_4     ((uint32_t)0x08001000) /* Base @ of Page 4, 1 Kbyte */
#define ADDR_FLASH_PAGE_5     ((uint32_t)0x08001400) /* Base @ of Page 5, 1 Kbyte */
#define ADDR_FLASH_PAGE_6     ((uint32_t)0x08001800) /* Base @ of Page 6, 1 Kbyte */
#define ADDR_FLASH_PAGE_7     ((uint32_t)0x08001C00) /* Base @ of Page 7, 1 Kbyte */
#define ADDR_FLASH_PAGE_8     ((uint32_t)0x08002000) /* Base @ of Page 8, 1 Kbyte */
#define ADDR_FLASH_PAGE_9     ((uint32_t)0x08002400) /* Base @ of Page 9, 1 Kbyte */
#define ADDR_FLASH_PAGE_10    ((uint32_t)0x08002800) /* Base @ of Page 10, 1 Kbyte */
#define ADDR_FLASH_PAGE_11    ((uint32_t)0x08002C00) /* Base @ of Page 11, 1 Kbyte */
#define ADDR_FLASH_PAGE_12    ((uint32_t)0x08003000) /* Base @ of Page 12, 1 Kbyte */
#define ADDR_FLASH_PAGE_13    ((uint32_t)0x08003400) /* Base @ of Page 13, 1 Kbyte */
#define ADDR_FLASH_PAGE_14    ((uint32_t)0x08003800) /* Base @ of Page 14, 1 Kbyte */
#define ADDR_FLASH_PAGE_15    ((uint32_t)0x08003C00) /* Base @ of Page 15, 1 Kbyte */
#define ADDR_FLASH_PAGE_16    ((uint32_t)0x08004000) /* Base @ of Page 16, 1 Kbyte */
#define ADDR_FLASH_PAGE_17    ((uint32_t)0x08004400) /* Base @ of Page 17, 1 Kbyte */
#define ADDR_FLASH_PAGE_18    ((uint32_t)0x08004800) /* Base @ of Page 18, 1 Kbyte */
#define ADDR_FLASH_PAGE_19    ((uint32_t)0x08004C00) /* Base @ of Page 19, 1 Kbyte */
#define ADDR_FLASH_PAGE_20    ((uint32_t)0x08005000) /* Base @ of Page 20, 1 Kbyte */
#define ADDR_FLASH_PAGE_21    ((uint32_t)0x08005400) /* Base @ of Page 21, 1 Kbyte */
#define ADDR_FLASH_PAGE_22    ((uint32_t)0x08005800) /* Base @ of Page 22, 1 Kbyte */
#define ADDR_FLASH_PAGE_23    ((uint32_t)0x08005C00) /* Base @ of Page 23, 1 Kbyte */
#define ADDR_FLASH_PAGE_24    ((uint32_t)0x08006000) /* Base @ of Page 24, 1 Kbyte */
#define ADDR_FLASH_PAGE_25    ((uint32_t)0x08006400) /* Base @ of Page 25, 1 Kbyte */
#define ADDR_FLASH_PAGE_26    ((uint32_t)0x08006800) /* Base @ of Page 26, 1 Kbyte */
#define ADDR_FLASH_PAGE_27    ((uint32_t)0x08006C00) /* Base @ of Page 27, 1 Kbyte */
#define ADDR_FLASH_PAGE_28    ((uint32_t)0x08007000) /* Base @ of Page 28, 1 Kbyte */
#define ADDR_FLASH_PAGE_29    ((uint32_t)0x08007400) /* Base @ of Page 29, 1 Kbyte */
#define ADDR_FLASH_PAGE_30    ((uint32_t)0x08007800) /* Base @ of Page 30, 1 Kbyte */
#define ADDR_FLASH_PAGE_31    ((uint32_t)0x08007C00) /* Base @ of Page 31, 1 Kbyte */
#define ADDR_FLASH_PAGE_32    ((uint32_t)0x08008000) /* Base @ of Page 32, 1 Kbyte */
#define ADDR_FLASH_PAGE_33    ((uint32_t)0x08008400) /* Base @ of Page 33, 1 Kbyte */
#define ADDR_FLASH_PAGE_34    ((uint32_t)0x08008800) /* Base @ of Page 34, 1 Kbyte */
#define ADDR_FLASH_PAGE_35    ((uint32_t)0x08008C00) /* Base @ of Page 35, 1 Kbyte */
#define ADDR_FLASH_PAGE_36    ((uint32_t)0x08009000) /* Base @ of Page 36, 1 Kbyte */
#define ADDR_FLASH_PAGE_37    ((uint32_t)0x08009400) /* Base @ of Page 37, 1 Kbyte */
#define ADDR_FLASH_PAGE_38    ((uint32_t)0x08009800) /* Base @ of Page 38, 1 Kbyte */
#define ADDR_FLASH_PAGE_39    ((uint32_t)0x08009C00) /* Base @ of Page 39, 1 Kbyte */
#define ADDR_FLASH_PAGE_40    ((uint32_t)0x0800A000) /* Base @ of Page 40, 1 Kbyte */
#define ADDR_FLASH_PAGE_41    ((uint32_t)0x0800A400) /* Base @ of Page 41, 1 Kbyte */
#define ADDR_FLASH_PAGE_42    ((uint32_t)0x0800A800) /* Base @ of Page 42, 1 Kbyte */
#define ADDR_FLASH_PAGE_43    ((uint32_t)0x0800AC00) /* Base @ of Page 43, 1 Kbyte */
#define ADDR_FLASH_PAGE_44    ((uint32_t)0x0800B000) /* Base @ of Page 44, 1 Kbyte */
#define ADDR_FLASH_PAGE_45    ((uint32_t)0x0800B400) /* Base @ of Page 45, 1 Kbyte */
#define ADDR_FLASH_PAGE_46    ((uint32_t)0x0800B800) /* Base @ of Page 46, 1 Kbyte */
#define ADDR_FLASH_PAGE_47    ((uint32_t)0x0800BC00) /* Base @ of Page 47, 1 Kbyte */
#define ADDR_FLASH_PAGE_48    ((uint32_t)0x0800C000) /* Base @ of Page 48, 1 Kbyte */
#define ADDR_FLASH_PAGE_49    ((uint32_t)0x0800C400) /* Base @ of Page 49, 1 Kbyte */
#define ADDR_FLASH_PAGE_50    ((uint32_t)0x0800C800) /* Base @ of Page 50, 1 Kbyte */
#define ADDR_FLASH_PAGE_51    ((uint32_t)0x0800CC00) /* Base @ of Page 51, 1 Kbyte */
#define ADDR_FLASH_PAGE_52    ((uint32_t)0x0800D000) /* Base @ of Page 52, 1 Kbyte */
#define ADDR_FLASH_PAGE_53    ((uint32_t)0x0800D400) /* Base @ of Page 53, 1 Kbyte */
#define ADDR_FLASH_PAGE_54    ((uint32_t)0x0800D800) /* Base @ of Page 54, 1 Kbyte */
#define ADDR_FLASH_PAGE_55    ((uint32_t)0x0800DC00) /* Base @ of Page 55, 1 Kbyte */
#define ADDR_FLASH_PAGE_56    ((uint32_t)0x0800E000) /* Base @ of Page 56, 1 Kbyte */
#define ADDR_FLASH_PAGE_57    ((uint32_t)0x0800E400) /* Base @ of Page 57, 1 Kbyte */
#define ADDR_FLASH_PAGE_58    ((uint32_t)0x0800E800) /* Base @ of Page 58, 1 Kbyte */
#define ADDR_FLASH_PAGE_59    ((uint32_t)0x0800EC00) /* Base @ of Page 59, 1 Kbyte */
#define ADDR_FLASH_PAGE_60    ((uint32_t)0x0800F000) /* Base @ of Page 60, 1 Kbyte */
#define ADDR_FLASH_PAGE_61    ((uint32_t)0x0800F400) /* Base @ of Page 61, 1 Kbyte */
#define ADDR_FLASH_PAGE_62    ((uint32_t)0x0800F800) /* Base @ of Page 62, 1 Kbyte */
#define ADDR_FLASH_PAGE_63    ((uint32_t)0x0800FC00) /* Base @ of Page 63, 1 Kbyte */

/* Exported functions ------------------------------------------------------- */

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
