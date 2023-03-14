/**
  ******************************************************************************
  * @file    EEPROM_Emulation/Inc/flash_interface.h
  * @author  MCD Application Team
  * @brief   Main program body
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
#ifndef __FLASH_INTERFACE_H
#define __FLASH_INTERFACE_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx_hal.h"
#include "stm32f0xx_ll_crc.h"
#include "stm32f0xx_ll_bus.h"

/** @addtogroup EEPROM_Emulation
  * @{
  */

/* Exported types ------------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
/* Private types -------------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
/** @addtogroup EEPROM_Private_Constants
  * @{
  */

/** @addtogroup Private_Other_Constants
  * @{
  */

#define BANK_SIZE            FLASH_BANK1_END                   /*!< Alias to FLASH_BANK_SIZE definition from HAL STM32L4 */

//#define EE_ACCESS_32BITS                                       /*!< Enable EEPROM 32bits R/W functions, only valid for flash allowing 64bits access*/

/* Page state header */
#define EE_PAGESTAT_ERASED      (uint64_t)0xFFFFFFFFFFFFFFFFU  /*!< State saved in 1st,2nd,3rd,4th data type of page header */
#define EE_PAGESTAT_RECEIVE     (uint64_t)0xAAAAAAAAAAAAAAAAU  /*!< State saved in 1st data type of page header */
#define EE_PAGESTAT_ACTIVE      (uint64_t)0xAAAAAAAAAAAAAAAAU  /*!< State saved in 2nd data type of page header */
#define EE_PAGESTAT_VALID       (uint64_t)0xAAAAAAAAAAAAAAAAU  /*!< State saved in 3rd data type of page header */
#define EE_PAGESTAT_ERASING     (uint64_t)0xAAAAAAAAAAAAAAAAU  /*!< State saved in 4th data type of page header */

/* Description of the 8 Bytes (64 bits) element in flash   */
/* Bit:  63                  32  31      16  15         0  */
/*       <--- Data Value ----->  <-unused->  <-VirtAddr->  */
#define EE_ELEMENT_SIZE         8U                            /*!< Size of element in Bytes */
#define EE_ELEMENT_TYPE         uint64_t                      /*!< Type of element */
#define EE_VIRTUALADDRESS_TYPE  uint16_t                      /*!< Type of Virtual Address */
#define EE_VIRTUALADDRESS_SHIFT 0U                            /*!< Bits Shifting to get Virtual Address in element */
#define EE_CRC_TYPE             uint16_t                      /*!< Type of Crc */
#define EE_CRC_SHIFT            16U                           /*!< Bits Shifting to get Crc in element */

#if defined(EE_ACCESS_32BITS)
#define EE_DATA_TYPE            uint32_t                      /*!< Type of Data */
#else
#define EE_DATA_TYPE            uint16_t                      /*!< Type of Data */
#endif 

#define EE_DATA_SHIFT           32U                           /*!< Bits Shifting to get Data value in element */
#define EE_MASK_VIRTUALADDRESS  (uint64_t)0x000000000000FFFFU
#define EE_MASK_CRC             (uint64_t)0x00000000FFFF0000U
#define EE_MASK_DATA            (uint64_t)0xFFFFFFFF00000000U
#define EE_MASK_FULL            (uint64_t)0xFFFFFFFFFFFFFFFFU

/* Configuration of eeprom emulation in flash, can be custom */
#define START_PAGE_ADDRESS      ((uint32_t)0x08005800)       /*!< 0x08005800 Base @ of Page 11, 2 Kbytes */

#define CYCLES_NUMBER           12U                          /*!< Number of 10Kcycles requested, minimum 1 for 10Kcycles (default),
                                                                  for instance 10 to reach 100Kcycles. This factor will increase
                                                                  pages number */
#define GUARD_PAGES_NUMBER      2U                           /*!< Number of guard pages avoiding frequent transfers (must be multiple of 2): 0,2,4.. */

/* Configuration of crc calculation for eeprom emulation in flash */
#define CRC_POLYNOMIAL_LENGTH   LL_CRC_POLYLENGTH_16B        /*!< CRC polynomial length 16 bits */
#define CRC_POLYNOMIAL_VALUE    0x8005U                      /*!< Polynomial to use for CRC calculation */
   
#define NB_OF_VARIABLES          1000U                       /* !< Number of variables to handle in eeprom */
/**
  * @}
  */

/**
  * @}
  */

/* Private macro -------------------------------------------------------------*/
/** @addtogroup EEPROM_Private_Macros
  * @{
  */

/**
  * @}
  */

/* Private functions ---------------------------------------------------------*/
/** @addtogroup EEPROM_Private_Functions
  * @{
  */
HAL_StatusTypeDef FI_WriteDoubleWord(uint32_t Address, uint64_t Data);
EE_Status FI_PageErase(uint32_t Page, uint16_t NbPages);
EE_Status FI_PageErase_IT(uint32_t Page, uint16_t NbPages);
EE_Status FI_DeleteCorruptedFlashAddress(uint32_t Address);
EE_Status FI_CheckBankConfig(void);

/**
  * @}
  */

/**
  * @}
  */

#endif /* __FLASH_INTERFACE_H */
