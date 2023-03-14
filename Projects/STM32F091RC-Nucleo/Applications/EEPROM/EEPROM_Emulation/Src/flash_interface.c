/**
  ******************************************************************************
  * @file    EEPROM_Emulation/Src/flash_interface.c
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

/* Includes ------------------------------------------------------------------*/
#include "eeprom.h"
#include "flash_interface.h"

/** @addtogroup EEPROM_Emulation
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
//static uint32_t GetBankNumber(uint32_t Address);

/* Exported functions --------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/** @addtogroup EEPROM_Private_Functions
  * @{
  */

/**
  * @brief  Write a double word at the given address in Flash
  * @param  Address Where to write
  * @param  Data What to write
  * @retval EE_Status
  *           - EE_OK: on success
  *           - EE_WRITE_ERROR: if an error occurs
  */
HAL_StatusTypeDef FI_WriteDoubleWord(uint32_t Address, uint64_t Data)
{
  return HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, Address, Data); 
}

/**
  * @brief  Erase a page in polling mode
  * @param  Page Page number
  * @param  NbPages Number of pages to erase
  * @retval EE_Status
  *           - EE_OK: on success
  *           - EE error code: if an error occurs
  */
EE_Status FI_PageErase(uint32_t Page, uint16_t NbPages)
{
  EE_Status status = EE_OK;
  FLASH_EraseInitTypeDef s_eraseinit;
  uint32_t page_error = 0U;

  s_eraseinit.TypeErase   = FLASH_TYPEERASE_PAGES;
  s_eraseinit.NbPages     = NbPages;
  s_eraseinit.PageAddress = PAGE_ADDRESS(Page);

  /* Erase the Page: Set Page status to ERASED status */
  if (HAL_FLASHEx_Erase(&s_eraseinit, &page_error) != HAL_OK)
  {
    status = EE_ERASE_ERROR;
  }
#if defined (__CC_ARM) || defined(__GNUC__)
    HAL_Delay(2);
#endif /* __CC_ARM || __GNUC__ */
  return status;
}

/**
  * @brief  Erase a page with interrupt enabled
  * @param  Page Page number
  * @param  NbPages Number of pages to erase
  * @retval EE_Status
  *           - EE_OK: on success
  *           - EE error code: if an error occurs
  */
EE_Status FI_PageErase_IT(uint32_t Page, uint16_t NbPages)
{
  EE_Status status = EE_OK;
  FLASH_EraseInitTypeDef s_eraseinit;

  s_eraseinit.TypeErase   = FLASH_TYPEERASE_PAGES;
  s_eraseinit.NbPages     = NbPages;
  s_eraseinit.PageAddress = Page * PAGE_SIZE + START_PAGE_ADDRESS;

  /* Erase the Page: Set Page status to ERASED status */
  if (HAL_FLASHEx_Erase_IT(&s_eraseinit) != HAL_OK)
  {
    status = EE_ERASE_ERROR;
  }
  return status;
}

/**
  * @brief  Delete corrupted Flash address, can be called from NMI. No Timeout.
  * @param  Address Address of the FLASH Memory to delete
  * @retval EE_Status
  *           - EE_OK: on success
  *           - EE error code: if an error occurs
  */
EE_Status FI_DeleteCorruptedFlashAddress(uint32_t Address)
{
  EE_Status status = EE_OK;

  /* Set FLASH Programmation bit */
  SET_BIT(FLASH->CR, FLASH_CR_PG);

  /* Program double word of value 0 */
  *(__IO uint32_t*)(Address) = (uint32_t)0U;
  *(__IO uint32_t*)(Address+4U) = (uint32_t)0U;

  /* Wait programmation completion */
  while(__HAL_FLASH_GET_FLAG(FLASH_FLAG_BSY))
  {
  }

  /* Check if error occurred */
  if((__HAL_FLASH_GET_FLAG(FLASH_FLAG_WRPERR))  || (__HAL_FLASH_GET_FLAG(FLASH_FLAG_PGERR)))
  {
    status = EE_DELETE_ERROR;
  }

  /* Check FLASH End of Operation flag  */
  if (__HAL_FLASH_GET_FLAG(FLASH_FLAG_EOP))
  {
    /* Clear FLASH End of Operation pending bit */
    __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP);
  }

  /* Clear FLASH Programmation bit */
  CLEAR_BIT(FLASH->CR, FLASH_CR_PG);

  return status;
}

/**
  * @brief  Check if the configuration is 128-bits bank or 2*64-bits bank
  * @param  None
  * @retval EE_Status
  *           - EE_OK: on success
  *           - EE error code: if an error occurs
  */
EE_Status FI_CheckBankConfig(void)
{
  /* No feature 128-bits single bank, so always 64-bits dual bank */
  return EE_OK;
}


/**
  * @}
  */

/**
  * @}
  */
