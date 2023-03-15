/**
  ******************************************************************************
  * @file    EEPROM_Emulation/Src/eeprom.c 
  * @author  MCD Application Team
  * @brief   This file provides all the EEPROM emulation firmware functions.
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

/** @defgroup EEPROM_Emulation EEPROM_Emulation
  * @{
  */

/* Private define -----------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/** @defgroup EEPROM_Private_Structures EEPROM Private Structures
  * @{
  */

/**
  * @brief  EE Find Type structure definition.
  */
/* Type of find requested :
       READ  --> page in active state
       WRITE --> page in receive state or active state
       ERASE --> page in erased state */
typedef enum {
   FIND_READ_PAGE,
   FIND_WRITE_PAGE,
   FIND_ERASE_PAGE
} EE_Find_type;

/**
  * @brief  EE State Type structure definition.
  */
/* Type of state requested :
       ERASED  --> page is erased
       RECEIVE --> page used during data transfer when no more space available in the system
       ACTIVE  --> page contains valid data and is not full
       VALID   --> page contains valid data and is full
       ERASING --> page used during transfer, should be erased after transfer
       INVALID --> page invalid state */
typedef enum {
   STATE_PAGE_ERASED,
   STATE_PAGE_RECEIVE,
   STATE_PAGE_ACTIVE,
   STATE_PAGE_VALID,
   STATE_PAGE_ERASING,
   STATE_PAGE_INVALID
} EE_State_type;

/**
  * @brief  EE Transfer Type structure definition.
  */
/* Definition of the different type of page transfer
        NORMAL  -> copy data page source to page destination
        RECOVER -> resume page transfer that has been interrupted */
typedef enum {
  EE_TRANSFER_NORMAL,
  EE_TRANSFER_RECOVER
} EE_Transfer_type;

/**
  * @brief  EE State Reliability structure definition.
  */
/* Reliability of page state:
        RELIABLE  -> header of page is not corrupted, state is reliable
        CORRUPTED -> header of page is corrupted, state is corrupted */
typedef enum {
  STATE_RELIABLE,
  STATE_CORRUPTED
} EE_State_Reliability;

/**
  * @}
  */

/* Private variables ---------------------------------------------------------*/
/** @defgroup EEPROM_Private_Variables EEPROM Private Variables
  * @{
  */

/* Global variables used to store eeprom status */
uint16_t uhNbWrittenElements = 0U;                  /*!< Nb of elements written in valid and active pages */
uint8_t ubCurrentActivePage = 0U;                   /*!< Current active page (can be active or receive state) */
uint32_t uwAddressNextWrite = PAGE_HEADER_SIZE;     /*!< Initialize write position just after page header */
    
/* During the cleanup phase in EE_Init, AddressRead is the address being read */ 
__IO uint32_t AddressRead = 0;
/* Flag equal to 1 when the cleanup phase is in progress, 0 if not */
__IO uint8_t CleanupPhase = 0;


/**
  * @}
  */

/* Private function prototypes -----------------------------------------------*/
/** @defgroup EEPROM_Private_Functions EEPROM Private Functions
  * @{
  */

static EE_Status ReadVariable(uint16_t VirtAddress, EE_DATA_TYPE* pData);
static EE_Status WriteVariable(uint16_t VirtAddress, EE_DATA_TYPE Data);
static EE_Status VerifyPageFullyErased(uint32_t Address, uint32_t PageSize);
static uint32_t FindPage(EE_Find_type Operation);
static EE_Status PagesTransfer(uint16_t VirtAddress, EE_DATA_TYPE Data, EE_Transfer_type type);
static EE_Status VerifyPagesFullWriteVariable(uint16_t VirtAddress, EE_DATA_TYPE Data);
static EE_Status SetPageState(uint32_t Page, EE_State_type State);
static EE_State_type GetPageState(uint32_t Address);
void ConfigureCrc(void);
uint16_t CalculateCrc(EE_DATA_TYPE Data, uint16_t VirtAddress);

/**
  * @}
  */

/* Exported functions -------------------------------------------------------*/
/** @addtogroup EEPROM_Exported_Functions
  * @{
  */

/**
  * @brief  Restore the pages to a known good state in case of power loss.
  *         If a page is in RECEIVE state, resume transfer.
  *         Then if some pages are ERASING state, erase these pages.
  * @param  EraseType: Type of erase to apply on page requiring to be erased.
  *         This parameter can be one of the following values:
  *          @arg @ref EE_FORCED_ERASE      pages to erase are erased unconditionally
  *          @arg @ref EE_CONDITIONAL_ERASE pages to erase are erased only if not fully erased
  * @retval EE_Status
  *           - EE_OK in case of success
  *           - EE error code in case of error
  */
EE_Status EE_Init(EE_Erase_type EraseType)
{
  EE_State_type pagestatus = STATE_PAGE_INVALID;
  uint32_t page = 0U, pageaddress = 0U, varidx = 0U,
           nbactivepage = 0U, nbactivereceivepage = 0U, nbvalidpage = 0U,
           lastvalidpage = 0U, firstvalidpage = 0U,
           recoverytransfer = 0U;
  EE_ELEMENT_TYPE addressvalue = 0U;
  EE_State_Reliability pagestate = STATE_RELIABLE;
  EE_Status status = EE_OK;

  /* Check if the configuration is 128-bits bank or 2*64-bits bank */
  if (FI_CheckBankConfig() != EE_OK)
  {
    return EE_INVALID_BANK_CFG;
  }

  /***************************************************************************/
  /* Step 0: Perform initial configuration     */
  /***************************************************************************/
  /* Configure CRC peripheral for eeprom emulation usage */
  ConfigureCrc();

  /***************************************************************************/
  /* Step 1: Read all lines of the flash pages of eeprom emulation to        */
  /*         delete corrupted lines detectable through NMI                   */
  /***************************************************************************/
  /* We set the flag indicating the cleanup phase is operating to 1 */
  CleanupPhase = 1;
  for (page = START_PAGE; page < (START_PAGE + PAGES_NUMBER); page++)
  {
    pageaddress = PAGE_ADDRESS(page);
    for (varidx = 0U; varidx < PAGE_SIZE; varidx += EE_ELEMENT_SIZE)
    {
      /* 
        During the cleanup phase and only during it,
        we save the address read to set its content to 0 in case it triggered an NMI (see NMI_Handler in stm32lxxx_it.c).
        In the rest of the program, we do nothing in case a NMI is triggers by a reading because no NMI should be triggered
        since we have cleanup the EEPROM emulated. By the way, there is still the CRC code associated to each EEPROM line
        that allows to verify its valid state.
      */
      AddressRead = pageaddress + varidx;
      addressvalue = (*(__IO EE_ELEMENT_TYPE*)(pageaddress + varidx));
    }
  }
  /* We set the flag indicating the cleanup phase is operating to 0 because it just ended */
  CleanupPhase = 0;

  /***************************************************************************/
  /* Step 2: Handle case of reset during transfer with no receive page       */
  /*         present, by setting missing receive page state                  */
  /***************************************************************************/
  /* Check if no active page and no receive page present */
  /* Browse all pages */
  for (page = START_PAGE; page < (START_PAGE + PAGES_NUMBER); page++)
  {
    pageaddress = PAGE_ADDRESS(page);
    pagestatus = GetPageState(pageaddress);

    /* Search for active and receive page */
    if ((pagestatus == STATE_PAGE_ACTIVE) || (pagestatus == STATE_PAGE_RECEIVE))
    {
      nbactivereceivepage++;
    }
    /* Keep index of first valid page, and last valid page */
    else if (pagestatus == STATE_PAGE_VALID)
    {
      if (nbvalidpage == 0U)
      {
        firstvalidpage = page;
      }
      lastvalidpage = page;
      nbvalidpage++;
    }
  }

  /* Check if no active and no receive page have been detected */
  if (nbactivereceivepage == 0U)
  {
    /* Check if valid pages have been detected */
    if (nbvalidpage > 0U)
    {
      /* Check state of page just before first valid page.
      If it is erasing page, then page after last valid page shall be set
      to receiving state */
      if (GetPageState(PAGE_ADDRESS(PREVIOUS_PAGE(firstvalidpage))) == STATE_PAGE_ERASING)
      {
        if (SetPageState(FOLLOWING_PAGE(lastvalidpage), STATE_PAGE_RECEIVE) != EE_OK)
        {
          return EE_WRITE_ERROR;
        }
      }
    }
    /* Format flash pages used for eeprom emulation in case no active, no receive, no valid pages are found */
    else
    {
      return EE_Format(EE_FORCED_ERASE);
    }
  }

  /*********************************************************************/
  /* Step 3: Handle case of reset during transfer, by performing       */
  /*         transfer recovery                                         */
  /*********************************************************************/
  /* Browse all pages */
  for (page = START_PAGE; page < (START_PAGE + PAGES_NUMBER); page++)
  {
    pageaddress = PAGE_ADDRESS(page);
    pagestatus = GetPageState(pageaddress);

    /* Check if there is receive page, meaning transfer has been interrupted */
    if (pagestatus == STATE_PAGE_RECEIVE)
    {
      /* Verify that receive page is a true one, not a corrupted page state */
      /* Check if page is not the first page of a block */
      if ((page != START_PAGE) && (page != (uint32_t)(START_PAGE + (PAGES_NUMBER / 2U))))
      {
        /* Check that previous page is valid state */
        if (GetPageState(PAGE_ADDRESS(PREVIOUS_PAGE(page))) == STATE_PAGE_VALID)
        {
          /* The receive page is a true receive page */
          pagestate = STATE_RELIABLE;
        }
        else /* Previous page is not valid state */
        {
          /* The receive page is false receive page due to header corruption */
          pagestate = STATE_CORRUPTED;
        }
      }
      else /* The receive page is the first page of a block */
      {
        /* Check that following page is erased state */
        if (GetPageState(PAGE_ADDRESS(FOLLOWING_PAGE(page))) == STATE_PAGE_ERASED)
        {
          /* The receive page is a true receive page */
          pagestate = STATE_RELIABLE;
        }
        else /* Following page is not erased state */
        {
          /* The receive page is false receive page due to header corruption */
          pagestate = STATE_CORRUPTED;
        }
      }

      /* If the receive page is a true receive page, resume pages transfer */
      if (pagestate == STATE_RELIABLE)
      {
        /* Initialize current active page */
        ubCurrentActivePage = page;

        /* Resume the interrupted page transfer, using dummy new data */
        if (PagesTransfer(0U, 0U, EE_TRANSFER_RECOVER) != EE_CLEANUP_REQUIRED)
        {
          return EE_TRANSFER_ERROR;
        }

        /* Memorize transfer recovery occurred */
        recoverytransfer = 1U;

        /* transfer recovery is done, then stop searching receive page */
        break;
      }
    }
  }

  /*********************************************************************/
  /* Step 4: Verify presence of one unique active page                 */
  /*         If more than one active page, raise error                 */
  /*         If no active page present, set missing active page        */
  /*********************************************************************/
  /* Browse all pages to search for active pages */
  nbactivepage = 0U;
  for (page = START_PAGE; page < (START_PAGE + PAGES_NUMBER); page++)
  {
    pageaddress = PAGE_ADDRESS(page);
    pagestatus = GetPageState(pageaddress);

    /* Search for active page */
    if (pagestatus == STATE_PAGE_ACTIVE)
    {
      /* Verify that active page is a true one, not a corrupted page state */
      /* Check if page is not the first page of a block */
      if ((page != START_PAGE) && (page != (uint32_t)(START_PAGE + (PAGES_NUMBER / 2U))))
      {
        /* Check that previous page is valid state */
        if (GetPageState(PAGE_ADDRESS(PREVIOUS_PAGE(page))) == STATE_PAGE_VALID)
        {
          /* The active page is a true active page */
          pagestate = STATE_RELIABLE;
        }
        else /* Previous page is not valid state */
        {
          /* The active page is false active page due to header corruption */
          pagestate = STATE_CORRUPTED;
        }
      }
      else /* The active page is the first page of a block */
      {
        /* Check that following page is erased state */
        if (GetPageState(PAGE_ADDRESS(FOLLOWING_PAGE(page))) == STATE_PAGE_ERASED)
        {
          /* The active page is a true active page */
          pagestate = STATE_RELIABLE;
        }
        else /* Following page is not erased state */
        {
          /* The active page is false active page due to header corruption */
          pagestate = STATE_CORRUPTED;
        }
      }

      /* If the active page is a true active page, initialize global variables */
      if (pagestate == STATE_RELIABLE)
      {
        if (nbactivepage == 0U)
        {
          ubCurrentActivePage = page;
          nbactivepage++;
        }
        else
        {
          /* Error: More than one reliable active page is present */
          return EE_INVALID_PAGE_SEQUENCE;
        }
      }
    }
    /* Keep index of last valid page, will be required in case no active page is found */
    else if (pagestatus == STATE_PAGE_VALID)
    {
      lastvalidpage = page;
    }
  }

  /* In case no active page is found, set page after last valid page to active state */
  if (nbactivepage == 0U)
  {
    ubCurrentActivePage = FOLLOWING_PAGE(lastvalidpage);
    if (SetPageState(ubCurrentActivePage, STATE_PAGE_ACTIVE) != EE_OK)
    {
      return EE_WRITE_ERROR;
    }
  }

  /*********************************************************************/
  /* Step 5: Initialize eeprom emulation global variables relative     */
  /*         to active page                                            */
  /*********************************************************************/
  /* Initialize global variables, with elements detected in active page */
  uhNbWrittenElements = 0U;
  uwAddressNextWrite = PAGE_HEADER_SIZE;

  for (varidx = PAGE_HEADER_SIZE; varidx < PAGE_SIZE; varidx += EE_ELEMENT_SIZE)
  {
    /* Check elements present in active page */
    addressvalue = (*(__IO EE_ELEMENT_TYPE*)(PAGE_ADDRESS(ubCurrentActivePage) + varidx));
    if (addressvalue != EE_MASK_FULL)
    {
      /* Then increment uhNbWrittenElements and uwAddressNextWrite */
      uhNbWrittenElements++;
      uwAddressNextWrite += EE_ELEMENT_SIZE;
    }
    else /* no more element in the page */
    {
      break;
    }
  }

  /*********************************************************************/
  /* Step 6: Finalize eeprom emulation global variables relative       */
  /*         to valid pages, and check consistency of pages sequence   */
  /*********************************************************************/
  /* Check consistency of pages sequence: one active page, optionally some valid pages before */
  /* Update global variable uhNbWrittenElements if valid pages are found */
  page = ubCurrentActivePage;
  firstvalidpage = ubCurrentActivePage;
  while ((page != START_PAGE) && (page != (uint32_t)(START_PAGE + (PAGES_NUMBER / 2U))))
  {
    /* Decrement page index among circular pages list */
    page = PREVIOUS_PAGE(page);
    pagestatus = GetPageState(PAGE_ADDRESS(page));

    /* Check if page is valid state */
    if (pagestatus == STATE_PAGE_VALID)
    {
      /* Update uhNbWrittenElements with number of elements in full page */
      uhNbWrittenElements += NB_MAX_ELEMENTS_BY_PAGE;

      /* Keep index of first valid page */
      firstvalidpage = page;
    }
    else
    {
      /* Error: Pages sequence is not consistent */
      return EE_INVALID_PAGE_SEQUENCE;
    }
  }

  /*********************************************************************/
  /* Step 7: Ensure empty pages are erased                             */
  /*********************************************************************/
  /* Ensure all pages after active page, until first valid page, are erased */
  page = FOLLOWING_PAGE(ubCurrentActivePage);
  pageaddress = PAGE_ADDRESS(page);

  while (page != firstvalidpage)
  {
    /* Check if page erase has to be forced unconditionally (default case) */
    if (EraseType == EE_FORCED_ERASE)
    {
      /* Force page erase independently of its content */
      if (FI_PageErase(page, 1U) != EE_OK)
      {
        return EE_ERASE_ERROR;
      }
    }
    else /* EraseType == EE_CONDITIONAL_ERASE */
    {
      /* Check if page is fully erased */
      if (VerifyPageFullyErased(pageaddress, PAGE_SIZE) == EE_PAGE_NOTERASED)
      {
        /* Erase pages if not fully erased */
        if (FI_PageErase(page, 1U) != EE_OK)
        {
          return EE_ERASE_ERROR;
        }
      }
    }

    /* Increment page index among circular pages list, to get first page to erased */
    page = FOLLOWING_PAGE(page);
    pageaddress = PAGE_ADDRESS(page);
  }

  /*********************************************************************/
  /* Step 8: Perform dummy write '0' to get rid of potential           */
  /*         instability of line value 0xFFFFFFFF consecutive to a     */
  /*         reset during write here                                   */
  /*         Only needed if recovery transfer did not occurred          */
  /*********************************************************************/
  if (recoverytransfer == 0U)
  {
    status = VerifyPagesFullWriteVariable(0U, 0U);
    
    /* The dummy write can be skipped in case pages are full
       because in this case potential instability can not happen */
    if ((status != EE_OK) && (status != EE_PAGE_FULL))
    {
      return EE_WRITE_ERROR;
    }
  }

  return EE_OK;
}

/**
  * @brief  Erases all flash pages of eeprom emulation, and set first page
  *         header as ACTIVE.
  * @note   This function can be called the very first time eeprom emulation is
  *         used, to prepare flash pages for eeprom emulation with empty eeprom
            variables. It can also be called at any time, to flush all eeprom
  *         variables.
  * @param  EraseType: Type of erase to apply on page requiring to be erased.
  *         This parameter can be one of the following values:
  *          @arg @ref EE_FORCED_ERASE      pages to erase are erased unconditionally
  *          @arg @ref EE_CONDITIONAL_ERASE pages to erase are erased only if not fully erased
  * @retval EE_Status
  *           - EE_OK: on success
  *           - EE error code: if an error occurs
  */
EE_Status EE_Format(EE_Erase_type EraseType)
{
  uint32_t page = 0U;

  /* Check if the configuration is 128-bits bank or 2*64-bits bank */
  if (FI_CheckBankConfig() != EE_OK)
  {
    return EE_INVALID_BANK_CFG;
  }
  
  /* Erase All Pages */
  for (page = START_PAGE; page < (START_PAGE + PAGES_NUMBER); page++)
  {
    /* Check if page erase has to be forced unconditionally (default case) */
    if (EraseType == EE_FORCED_ERASE)
    {
      /* Force page erase independently of its content */
      if (FI_PageErase(page, 1U) != EE_OK)
      {
        return EE_ERASE_ERROR;
      }
    }
    else /* EraseType == EE_CONDITIONAL_ERASE */
    {
      /* Check if Page is not yet fully erased */
      if (VerifyPageFullyErased(PAGE_ADDRESS(page), PAGE_SIZE) == EE_PAGE_NOTERASED)
      {
        /* Erase the page */
        /* If Erase operation was failed, a Flash error code is returned */
        if (FI_PageErase(page, 1U) != EE_OK)
        {
          return EE_ERASE_ERROR;
        }
      }
    }
  }

  /* Set first Page in Active State */
  /* If program operation was failed, a Flash error code is returned */
  if (SetPageState(START_PAGE, STATE_PAGE_ACTIVE) != EE_OK)
  {
    return EE_WRITE_ERROR;
  }

  /* Reset global variables */
  uhNbWrittenElements = (uint16_t)0U;
  ubCurrentActivePage = START_PAGE;
  uwAddressNextWrite = PAGE_HEADER_SIZE; /* Initialize write position just after page header */

  return EE_OK;
}

#if defined(EE_ACCESS_32BITS)
/**
  * @brief  Returns the last stored variable data, if found, which correspond to
  *         the passed virtual address
  * @param VirtAddress Variable virtual address on 16 bits (can't be 0x0001 or 0xFFFE)
  * @param  pData Variable containing the 32bits read variable value
  * @retval EE_Status
  *           - EE_OK: if variable was found
  *           - EE error code: if an error occurs
  */
EE_Status EE_ReadVariable32bits(uint16_t VirtAddress, uint32_t* pData)
{
  EE_DATA_TYPE datatmp = 0U;
  EE_Status status = EE_OK;

  /* Read variable of size EE_DATA_TYPE, then cast it to 32bits */
  status = ReadVariable(VirtAddress, &datatmp);
  *pData = (uint32_t) datatmp;

  return status;
}
#endif /* EE_ACCESS_32BITS */

/**
  * @brief  Returns the last stored variable data, if found, which correspond to
  *         the passed virtual address
  * @param VirtAddress Variable virtual address on 16 bits (can't be 0x0001 or 0xFFFE)
  * @param  pData Variable containing the 16bits read variable value
  * @retval EE_Status
  *           - EE_OK: if variable was found
  *           - EE error code: if an error occurs
  */
EE_Status EE_ReadVariable16bits(uint16_t VirtAddress, uint16_t* pData)
{
  EE_DATA_TYPE datatmp = 0U;
  EE_Status status = EE_OK;

  /* Read variable of size EE_DATA_TYPE, then cast it to 16bits */
  status = ReadVariable(VirtAddress, &datatmp);
  *pData = (uint16_t) datatmp;

  return status;
}

/**
  * @brief  Returns the last stored variable data, if found, which correspond to
  *         the passed virtual address
  * @param VirtAddress Variable virtual address on 16 bits (can't be 0x0001 or 0xFFFE)
  * @param  pData Variable containing the 8bits read variable value
  * @retval EE_Status
  *           - EE_OK: if variable was found
  *           - EE error code: if an error occurs
  */
EE_Status EE_ReadVariable8bits(uint16_t VirtAddress, uint8_t* pData)
{
  EE_DATA_TYPE datatmp = 0U;
  EE_Status status = EE_OK;

  /* Read variable of size EE_DATA_TYPE, then cast it to 8bits */
  status = ReadVariable(VirtAddress, &datatmp);
  *pData = (uint8_t) datatmp;

  return status;
}

#if defined(EE_ACCESS_32BITS)
/**
  * @brief  Writes/updates variable data in EEPROM.
  *         Trig internal Pages transfer if half of the pages are full.
  * @warning This function is not reentrant
  * @param VirtAddress Variable virtual address on 16 bits (can't be 0x0001 or 0xFFFE)
  * @param  Data 32bits data to be written
  * @retval EE_Status
  *           - EE_OK: on success
  *           - EE_CLEANUP_REQUIRED: success and user has to trig flash pages cleanup
  *           - EE error code: if an error occurs
  */
EE_Status EE_WriteVariable32bits(uint16_t VirtAddress, uint32_t Data)
{
  return WriteVariable(VirtAddress, (EE_DATA_TYPE) Data);
}
#endif /* EE_ACCESS_32BITS */

/**
  * @brief  Writes/updates variable data in EEPROM.
  *         Trig internal Pages transfer if half of the pages are full.
  * @warning This function is not reentrant
  * @param VirtAddress Variable virtual address on 16 bits (can't be 0x0001 or 0xFFFE)
  * @param  Data 16bits data to be written
  * @retval EE_Status
  *           - EE_OK: on success
  *           - EE_CLEANUP_REQUIRED: success and user has to trig flash pages cleanup
  *           - EE error code: if an error occurs
  */
EE_Status EE_WriteVariable16bits(uint16_t VirtAddress, uint16_t Data)
{
  return WriteVariable(VirtAddress, (EE_DATA_TYPE) Data);
}

/**
  * @brief  Writes/updates variable data in EEPROM.
  *         Trig internal Pages transfer if half of the pages are full.
  * @warning This function is not reentrant
  * @param VirtAddress Variable virtual address on 16 bits (can't be 0x0001 or 0xFFFE)
  * @param  Data 8bits data to be written
  * @retval EE_Status
  *           - EE_OK: on success
  *           - EE_CLEANUP_REQUIRED: success and user has to trig flash pages cleanup
  *           - EE error code: if an error occurs
  */
EE_Status EE_WriteVariable8bits(uint16_t VirtAddress, uint8_t Data)
{
  return WriteVariable(VirtAddress, (EE_DATA_TYPE) Data);
}

/**
  * @brief  Erase group of pages which are erasing state, in polling mode.
  *         Could be either first half or second half of total pages number.
  * @note   This function should be called when EE_WriteVariableXXbits has
  *         returned EE_CLEANUP_REQUIRED status (and only in that case)
  * @retval EE_Status
  *           - EE_OK: in case of success
  *           - EE error code: if an error occurs
  */
EE_Status EE_CleanUp(void)
{
  uint32_t firstpage = 0U, page = 0U;
  uint32_t firstpageaddress = 0U, pageaddress = 0U;
  EE_State_type firstpagestatus = STATE_PAGE_INVALID, pagestatus = STATE_PAGE_INVALID;

  /* Check first half and second half page group */
  for (firstpage = START_PAGE; firstpage < (START_PAGE + PAGES_NUMBER); firstpage += (PAGES_NUMBER / 2U))
  {
    /* Check status of first page of the group */
    firstpageaddress = PAGE_ADDRESS(firstpage);
    firstpagestatus = GetPageState(firstpageaddress);

    /* If first page of the group is erasing state, check that all other pages
    of the group are also erasing state */
    if (firstpagestatus == STATE_PAGE_ERASING)
    {
      for (page = (firstpage + 1U); page < (firstpage + (PAGES_NUMBER / 2U)); page++)
      {
        pageaddress = PAGE_ADDRESS(page);
        pagestatus = GetPageState(pageaddress);

        /* If page is not erasing, return error */
        if (pagestatus != STATE_PAGE_ERASING)
        {
          return EE_ERROR_NOERASING_PAGE;
        }
      }

      /* Erase all the pages of the group */
      /* If erase operation fails, a Flash error code is returned */
      if (FI_PageErase(firstpage, PAGES_NUMBER / 2U) != EE_OK)
      {
        return EE_ERASE_ERROR;
      }
      else
      {
        return EE_OK;
      }
    }
  }

  /* Error if no erasing pages group is found */
  return EE_ERROR_NOERASING_PAGE;
}

/**
  * @brief  Erase group of pages which are erasing state, in IT mode.
  *         Could be either first half or second half of total pages number.
  * @note   This function should be called when EE_WriteVariableXXbits has
  *         returned EE_CLEANUP_REQUIRED status (and only in that case)
  * @retval EE_Status
  *           - EE_OK: in case of success
  *           - EE error code: if an error occurs
  */
EE_Status EE_CleanUp_IT(void)
{
  uint32_t firstpage = 0U, page = 0U;
  uint32_t firstpageaddress = 0U, pageaddress = 0U;
  EE_State_type firstpagestatus = STATE_PAGE_INVALID, pagestatus = STATE_PAGE_INVALID;

  /* Check first half and second half page group */
  for (firstpage = START_PAGE; firstpage < (START_PAGE + PAGES_NUMBER); firstpage += (PAGES_NUMBER / 2U))
  {
    /* Check status of first page of the group */
    firstpageaddress = PAGE_ADDRESS(firstpage);
    firstpagestatus = GetPageState(firstpageaddress);

    /* If first page of the group is erasing state, check that all other pages
    of the group are also erasing state */
    if (firstpagestatus == STATE_PAGE_ERASING)
    {
      for (page = (firstpage + 1U); page < (firstpage + (PAGES_NUMBER / 2U)); page++)
      {
        pageaddress = PAGE_ADDRESS(page);
        pagestatus = GetPageState(pageaddress);

        /* If page is not erasing, return error */
        if (pagestatus != STATE_PAGE_ERASING)
        {
          return EE_ERROR_NOERASING_PAGE;
        }
      }
      
      /* Erase all the pages of the group */
      /* If erase operation fails, a Flash error code is returned */
      if (FI_PageErase_IT(firstpage, PAGES_NUMBER / 2U) != EE_OK)
      {
        return EE_ERASE_ERROR;
      }
      else
      {
        return EE_OK;
      }
    }
  }

  /* Error if no erasing pages group is found */
  return EE_ERROR_NOERASING_PAGE;
}

/**
  * @brief  Delete corrupted Flash address, can be called under NMI.
  * @param  Address Address of the FLASH Memory to delete
  * @retval EE_Status
  *           - EE_OK: on success
  *           - EE error code: if an error occurs
  */
EE_Status EE_DeleteCorruptedFlashAddress(uint32_t Address)
{
  return FI_DeleteCorruptedFlashAddress(Address);
}

/**
  * @brief  Clean Up end of operation interrupt callback.
  * @retval None
  */
__weak void EE_EndOfCleanup_UserCallback(void)
{
  /* NOTE : This function should not be modified, when the callback is needed,
            the EE_EndOfCleanup_UserCallback could be implemented in the user file
   */
}

/**
  * @}
  */

/* Private functions ---------------------------------------------------------*/
/** @addtogroup EEPROM_Private_Functions
  * @{
  */

/**
  * @brief  Returns the last stored variable data, if found, which correspond to
  *         the passed virtual address
  * @param VirtAddress Variable virtual address on 16 bits (can't be 0x0001 or 0xFFFE)
  * @param  pData Variable containing the EE_DATA_TYPE read variable value
  * @retval EE_Status
  *           - EE_OK: if variable was found
  *           - EE error code: if an error occurs
  */
static EE_Status ReadVariable(uint16_t VirtAddress, EE_DATA_TYPE* pData)
{
  EE_ELEMENT_TYPE addressvalue = 0U;
  uint32_t page = 0U, pageaddress = 0U, counter = 0U, crc = 0U;
  EE_State_type pagestate = STATE_PAGE_INVALID;

  /* Get active Page for read operation */
  page = FindPage(FIND_READ_PAGE);

  /* Check if there is no active page */
  if (page == EE_NO_PAGE_FOUND)
  {
    return EE_ERROR_NOACTIVE_PAGE;
  }
  pageaddress = PAGE_ADDRESS(page);
  pagestate = GetPageState(pageaddress);

  /* Search variable in active page and valid pages until erased page is found
     or in erasing pages until erased page is found */
  while ((pagestate == STATE_PAGE_ACTIVE) || (pagestate == STATE_PAGE_VALID) || (pagestate == STATE_PAGE_ERASING))
  {
    /* Set counter index to last element position in the page */
    counter = PAGE_SIZE - EE_ELEMENT_SIZE;

    /* Check each page address starting from end */
    while (counter >= PAGE_HEADER_SIZE)
    {
      /* Get the current location content to be compared with virtual address */
      addressvalue = (*(__IO EE_ELEMENT_TYPE*)(pageaddress + counter));
      if (addressvalue != EE_PAGESTAT_ERASED)
      {
        /* Compare the read address with the virtual address */
        if (EE_VIRTUALADDRESS_VALUE(addressvalue) == VirtAddress)
        {
          /* Calculate crc of variable data and virtual address */
          crc = CalculateCrc(EE_DATA_VALUE(addressvalue), EE_VIRTUALADDRESS_VALUE(addressvalue));

          /* if crc verification pass, data is correct and is returned.
             if crc verification fails, data is corrupted and has to be skip */
          if (crc == EE_CRC_VALUE(addressvalue))
          {
            /* Get content of variable value */
            *pData = EE_DATA_VALUE(addressvalue);

            return EE_OK;
          }
        }
      }
      /* Next address location */
      counter -= EE_ELEMENT_SIZE;
    }

    /* Decrement page index circularly, among pages allocated to eeprom emulation */
    page = PREVIOUS_PAGE(page);
    pageaddress = PAGE_ADDRESS(page);
    pagestate = GetPageState(pageaddress);
  }

  /* Variable is not found */
  return EE_NO_DATA;
}

/**
  * @brief  Writes/updates variable data in EEPROM
  *         Trig internal Pages transfer if half of the pages are full
  * @param VirtAddress Variable virtual address on 16 bits (can't be 0x0001 or 0xFFFE)
  * @param  Data EE_DATA_TYPE data to be written
  * @retval EE_Status
  *           - EE_OK: on success, without page transfer
  *           - EE_CLEANUP_REQUIRED: on success, with page transfer occurred
  *           - EE_FLASH_USED: flash currently used by CPU2
  *           - EE error code: if an error occurs
  */
static EE_Status WriteVariable(uint16_t VirtAddress, EE_DATA_TYPE Data)
{
  EE_Status status = EE_OK;

  /* Write the variable virtual address and value in the EEPROM, if not full */
  status = VerifyPagesFullWriteVariable(VirtAddress, Data);
  if (status == EE_PAGE_FULL)
  {
#if defined (__CC_ARM) || defined(__GNUC__)
    HAL_Delay(2);
#endif /* __CC_ARM || __GNUC__ */
    /* In case the EEPROM pages are full, perform Pages transfer */
    return PagesTransfer(VirtAddress, Data, EE_TRANSFER_NORMAL);
  }

  /* Return last operation status */
  return status;
}

/**
  * @brief  Verify if specified page is fully erased.
  * @param  Address page address
  * @param  PageSize page size
  * @retval EE_Status
  *           - EE_PAGE_NOTERASED : if Page not erased
  *           - EE_PAGE_ERASED    : if Page erased
  */
static EE_Status VerifyPageFullyErased(uint32_t Address, uint32_t PageSize)
{
  EE_Status readstatus = EE_PAGE_ERASED;
  uint32_t counter = 0U;

  /* Check each element in the page */
  while (counter < PageSize)
  {
    /* Compare the read address with the virtual address */
    if ((*(__IO EE_ELEMENT_TYPE*)(Address+counter)) != EE_PAGESTAT_ERASED)
    {
      /* In case one element is not erased, reset readstatus flag */
      readstatus = EE_PAGE_NOTERASED;
    }
    /* Next address location */
    counter = counter + EE_ELEMENT_SIZE;
  }

  /* Return readstatus value */
  return readstatus;
}

/**
  * @brief  Find suitable page for read/write/erase operation.
  *   It also update pages state if current page is full.
  *   And it force cleanup if all pages are full.
  * @param  Operation Type of page to be requested.
  *   This parameter can be one of the following values:
  *     @arg @ref FIND_READ_PAGE return the active page index
  *     @arg @ref FIND_WRITE_PAGE return the write page index
  *     @arg @ref FIND_ERASE_PAGE return the erase page index
  * @retval Page_Index
  *           - Page Index: on success
  *           - @ref EE_NO_PAGE_FOUND : if an error occurs
  */
static uint32_t FindPage(EE_Find_type Operation)
{
  EE_State_type currentpagestatus = STATE_PAGE_INVALID, followingpagestatus = STATE_PAGE_INVALID;
  uint32_t currentpage = 0U, followingpage = 0U, previouspage = 0U;

  /* Get currentpage status */
  currentpage = ubCurrentActivePage;
  currentpagestatus = GetPageState(PAGE_ADDRESS(currentpage));

  /* Get followingpage status */
  followingpage = FOLLOWING_PAGE(currentpage);
  followingpagestatus = GetPageState(PAGE_ADDRESS(followingpage));

  /* Get previouspage status */
  previouspage = PREVIOUS_PAGE(currentpage);

  /* Write, read or erase operation */
  switch (Operation)
  {
    case FIND_WRITE_PAGE:   /* ---- Write operation ---- */
      /* Normal operation, no page transfer on going */
      if (currentpagestatus == STATE_PAGE_ACTIVE)
      {
        /* Check if active page is not full */
        if (uwAddressNextWrite < PAGE_SIZE)
        {
          /* Return current Active page */
          return currentpage;
        }
        else
        /* No more space in current active page */
        {
          /* Check if following page is erasing state */
          if (followingpagestatus == STATE_PAGE_ERASING)
          {
            /* Force Cleanup, as not yet performed by user */
            if (EE_CleanUp() != EE_OK)
            {
              return EE_NO_PAGE_FOUND;
            }
          }

          /* Set current active page in valid state */
          if (SetPageState(currentpage, STATE_PAGE_VALID) != EE_OK)
          {
            return EE_NO_PAGE_FOUND;
          }

          /* Set following page as active */
          if (SetPageState(followingpage, STATE_PAGE_ACTIVE) != EE_OK)
          {
            return EE_NO_PAGE_FOUND;
          }
          uwAddressNextWrite = PAGE_HEADER_SIZE;   /* Skip page header */
          return followingpage;         /* Following page is now active one */
        }
      }
      /* Transfer is on going, page receiving data */
      else
      {
        if (currentpagestatus == STATE_PAGE_RECEIVE)
        {
          /* Check if receive page is not full */
          if (uwAddressNextWrite < PAGE_SIZE)
          {
            /* Return current receive page */
            return currentpage;
          }
          else
          /* No more space in current receive page */
          {
            /* Check if following page is erasing state */
            if (followingpagestatus == STATE_PAGE_ERASING)
            {
              /* Force Cleanup, as not yet performed by user */
              if (EE_CleanUp() != EE_OK)
              {
                return EE_NO_PAGE_FOUND;
              }
            }

            /* Set current receive page in valid state */
            if (SetPageState(currentpage, STATE_PAGE_VALID) != EE_OK)
            {
              return EE_NO_PAGE_FOUND;
            }

            /* Set following page as receive */
            if (SetPageState(followingpage, STATE_PAGE_RECEIVE) != EE_OK)
            {
              return EE_NO_PAGE_FOUND;
            }
            uwAddressNextWrite = PAGE_HEADER_SIZE;   /* Skip page header */
            return followingpage;         /* Following page is now active one */
          }
        }
        else
        {
          return EE_NO_PAGE_FOUND;   /* No active Page */
        }
      }

    case FIND_READ_PAGE:  /* ---- Read operation ---- */
      if (currentpagestatus == STATE_PAGE_ACTIVE)
      {
        return currentpage;
      }
      else
      {
        if (currentpagestatus == STATE_PAGE_RECEIVE)
        {
          return previouspage;
        }
        else
        {
          return EE_NO_PAGE_FOUND;   /* No active Page */
        }
      }

    case FIND_ERASE_PAGE: /* ---- Return the erased page */
      if (followingpagestatus == STATE_PAGE_ERASED)
      {
        return followingpage;
      }
      else
      {
        return EE_NO_PAGE_FOUND;  /* No erased Page */
      }

    default:
      ;
  }

  return EE_NO_PAGE_FOUND;
}

/**
  * @brief  Writes a new variable data in fresh new page in case of normal
  *         transfer, and transfers last updated elements from full pages to
  *         empty pages in any cases.
  * @param  VirtAddress 16 bit virtual address of the new variable data
  * @param  Data @ref EE_DATA_TYPE data value of the new variable data
  * @param  Type Type of transfer.
  *         This parameter can be one of the EE_Transfer_type enum values.
  *            @arg @ref EE_TRANSFER_NORMAL Pages transfer during normal processing
  *            @arg @ref EE_TRANSFER_RECOVER Recovering pages transfer at Init
  * @retval EE_Status
  *           - EE_CLEANUP_REQUIRED: on success
  *           - EE error code: if an error occurs
  */
static EE_Status PagesTransfer(uint16_t VirtAddress, EE_DATA_TYPE Data, EE_Transfer_type Type)
{
  EE_State_type pagestatus = STATE_PAGE_INVALID;
  uint32_t pageaddress = 0U;
  uint32_t page = 0U;
  uint32_t varidx = 0U;
  EE_ELEMENT_TYPE addressvalue = 0U;
  EE_Status status = EE_OK;
  EE_DATA_TYPE DataValue = 0U;

  /* Get receive Page for transfer operation */
  page = FindPage((Type == EE_TRANSFER_NORMAL?FIND_ERASE_PAGE:FIND_WRITE_PAGE));
  if (page == EE_NO_PAGE_FOUND)
  {
    return EE_ERROR_NOERASE_PAGE;
  }

  /* Reinitialize number of data written in the pages, and current active page */
  uhNbWrittenElements = 0U;
  ubCurrentActivePage = page;
  uwAddressNextWrite = PAGE_HEADER_SIZE;

  /* Mark the erased page at receive state in case of normal transfer */
  /* It is already the case in recover transfer case */
  /* If program operation was failed, a Flash error code is returned */
  if (Type == EE_TRANSFER_NORMAL)
  {
    if (SetPageState(page, STATE_PAGE_RECEIVE) != EE_OK)
    {
      return EE_WRITE_ERROR;
    }
  }

  /* Set the previous active page and all previous valid pages to erasing state */
  /* In case of recover transfer, some of these pages may already be marked erasing */
  page = PREVIOUS_PAGE(page);
  pageaddress = PAGE_ADDRESS(page);
  pagestatus = GetPageState(pageaddress);

  if ((pagestatus == STATE_PAGE_ACTIVE) || (pagestatus == STATE_PAGE_ERASING))
  {
    /* Set active page to erasing */
    if (pagestatus == STATE_PAGE_ACTIVE)
    {
      if (SetPageState(page, STATE_PAGE_ERASING) != EE_OK)
      {
        return EE_WRITE_ERROR;
      }
    }

    /* Inspect the previous pages to set all valid pages to erasing state */
    /* In case of recover, some valid pages may be already erasing state */
    page = PREVIOUS_PAGE(page);
    pageaddress = PAGE_ADDRESS(page);
    pagestatus = GetPageState(pageaddress);

    while ((pagestatus == STATE_PAGE_VALID) || (pagestatus == STATE_PAGE_ERASING))
    {
      /* Set valid page to erasing */
      if (pagestatus == STATE_PAGE_VALID)
      {
        if (SetPageState(page, STATE_PAGE_ERASING) != EE_OK)
        {
          return EE_WRITE_ERROR;
        }
      }

      /* decrement page index */
      page = PREVIOUS_PAGE(page);
      pageaddress = PAGE_ADDRESS(page);
      pagestatus = GetPageState(pageaddress);
    }
  }
  else
  {
    if ((Type == EE_TRANSFER_RECOVER) && (pagestatus == STATE_PAGE_VALID))
    {
      /* This can happen in case of recover transfer. It indicates that previous */
      /* transfer goes far enough to fill a complete receive page at least */
      /* (valid state). Then erasing state marking was already completed */
    }
    else
    {
      /* Inconsistent previous page state */
      return EE_INVALID_PAGE_SEQUENCE;
    }
  }

  /* In case of recover transfer, transfer must be resumed where it has been stopped */
  /* Update global variables to reflect current transfer status */
  if (Type == EE_TRANSFER_RECOVER)
  {
    /* Count number of elements already transferred in current receive page */
    for (varidx = PAGE_HEADER_SIZE; varidx < PAGE_SIZE; varidx += EE_ELEMENT_SIZE)
    {
      /* Get next element in receive page */
      addressvalue = (*(__IO EE_ELEMENT_TYPE*)(PAGE_ADDRESS(ubCurrentActivePage) + varidx));

      /* Check if element is valid */
      if (addressvalue != EE_PAGESTAT_ERASED)
      {
        /* Update global variables accordingly */
        uhNbWrittenElements++;
        uwAddressNextWrite += EE_ELEMENT_SIZE;
      }
      else
      {
        break;
      }
    }

    /* Count number of elements already transferred in previous valid pages */
    page = ubCurrentActivePage;
    for (varidx = 0U; varidx < PAGES_NUMBER; varidx++)
    {
      /* Decrement page index among circular pages list */
      page = PREVIOUS_PAGE(page);
      pagestatus = GetPageState(PAGE_ADDRESS(page));

      /* Check if page is valid state */
      if (pagestatus == STATE_PAGE_VALID)
      {
        /* Update uhNbWrittenElements with number of elements in page */
        uhNbWrittenElements += NB_MAX_ELEMENTS_BY_PAGE;
      }
      else
      {
        break;
      }
    }
  }
  /* Write the variable passed as parameter in the new active page */
  /* If program operation was failed, a Flash error code is returned */
  if (VerifyPagesFullWriteVariable(VirtAddress, Data) != EE_OK)
  {
    return EE_WRITE_ERROR;
  }

  /* Transfer process: transfer variables from old to the new active page */
  /* First element in receive page can be any one, the following elements are */
  /* ordered from the beginning. */
  /* In case of recovery, Pre-Last element in receive page could be */
  /* corrupted if reset occurred during write of this element, */
  /* and last element is dummy value that we have just written. */
  /* Transfer shall then resume from (uhNbWrittenElements-3) variable index */
  for (varidx = (uhNbWrittenElements >= 3U?(uhNbWrittenElements-3U+1U):1U); varidx < NB_OF_VARIABLES+1; varidx++)
  {
    /* Check each variable except the one passed as parameter */
    if (varidx != VirtAddress)
    {
      /* Read the last variable updates */
      status = ReadVariable(varidx, &DataValue);
      if (status == EE_OK)
      {
        /* In case variable corresponding to the virtual address was found */
        /* Transfer the variable to the new active page */
        /* If program operation was failed, a Flash error code is returned */
        status = VerifyPagesFullWriteVariable(varidx, DataValue);
        if (status != EE_OK)
        {
          return status;
        }
      }
      else
      {
        if (status != EE_NO_DATA)
        {
          /* In case variable is not found , do nothing */
          /* Any other status is error code occurs during variable read */
          return status;
        }
      }
    }
  }

  /* Transfer is now done, mark the receive state page as active */
  if (SetPageState(ubCurrentActivePage, STATE_PAGE_ACTIVE) != EE_OK)
  {
    return EE_WRITE_ERROR;
  }

  /* Return last operation flash status */
  return EE_CLEANUP_REQUIRED;
}

/**
  * @brief  Verify if pages are full
  *   then if not the case, writes variable in EEPROM.
  * @param  VirtAddress 16 bit virtual address of the variable
  * @param  Data @ref EE_DATA_TYPE data to be written as variable value
  * @param  Write_type Type of writing on going (see EE_Write_type)
  * @retval EE_Status
  *           - EE_OK: on success
  *           - EE_FULL: if half pages are full
  *           - EE_FLASH_USED: flash currently used by CPU2
  *           - EE error code: if an error occurs
  */
static EE_Status VerifyPagesFullWriteVariable(uint16_t VirtAddress, EE_DATA_TYPE Data)
{
  uint32_t crc = 0U;

  /* Check if pages are full, i.e. max number of written elements achieved */
  if (uhNbWrittenElements >= NB_MAX_WRITTEN_ELEMENTS)
  {
    return EE_PAGE_FULL;
  }

  /* Get active Page for write operation */
  uint32_t activepage = FindPage(FIND_WRITE_PAGE);
  uint32_t activepageaddress = 0U;

  /* Check if there is no active page */
  if (activepage == EE_NO_PAGE_FOUND)
  {
    return EE_ERROR_NOACTIVE_PAGE;
  }

  activepageaddress = PAGE_ADDRESS(activepage);

  /* Force crc to 0 in case of Data/VirtAddress are 0*/
  if ((Data == 0U) && (VirtAddress == 0U))
  {
    crc = 0U;
  }
  else
  {
    /* Calculate crc of variable data and virtual address */
    crc = CalculateCrc(Data, VirtAddress);
  }

  /* Program variable data + virtual address + crc */
  /* If program operation was failed, a Flash error code is returned */
  if (FI_WriteDoubleWord(activepageaddress+uwAddressNextWrite, EE_ELEMENT_VALUE(VirtAddress,Data,crc)) != HAL_OK)
  {
    return EE_WRITE_ERROR;
  }

  /* Increment global variables relative to write operation done*/
  uwAddressNextWrite += EE_ELEMENT_SIZE;
  uhNbWrittenElements++;

  return EE_OK;
}

/**
  * @brief  Set page state in page header
  * @param  Page Index of the page
  * @param  State State of the page
  * @retval EE_Status
  *           - EE_OK: on success
  *           - EE error code: if an error occurs
  */
static EE_Status SetPageState(uint32_t Page, EE_State_type State)
{
  uint32_t header1 = 0U, header2 = 0U, header3 = 0U, header4 = 0U;

  header1 = PAGE_ADDRESS(Page);
  header2 = PAGE_ADDRESS(Page) + EE_ELEMENT_SIZE;
  header3 = PAGE_ADDRESS(Page) + (EE_ELEMENT_SIZE*2U);
  header4 = PAGE_ADDRESS(Page) + (EE_ELEMENT_SIZE*3U);

  switch(State)
  {
  case STATE_PAGE_RECEIVE:
    {
      /* Set new Page status to STATE_PAGE_RECEIVE status */
      if (FI_WriteDoubleWord(header1, EE_PAGESTAT_RECEIVE) != HAL_OK)
      {
        return EE_WRITE_ERROR;
      }
      ubCurrentActivePage = Page;
    }
    break;
  case STATE_PAGE_ACTIVE:
    {
      /* Set new Page status to STATE_PAGE_ACTIVE status */
      if (FI_WriteDoubleWord(header2, EE_PAGESTAT_ACTIVE) != HAL_OK)
      {
        return EE_WRITE_ERROR;
      }
      ubCurrentActivePage = Page;
    }
    break;
  case STATE_PAGE_VALID:
    {
      /* Set new Page status to STATE_PAGE_VALID status */
      if (FI_WriteDoubleWord(header3, EE_PAGESTAT_VALID) != HAL_OK)
      {
        return EE_WRITE_ERROR;
      }
    }
    break;
  case STATE_PAGE_ERASING:
    {
      /* Set new Page status to STATE_PAGE_ERASING status */
      if (FI_WriteDoubleWord(header4, EE_PAGESTAT_ERASING) != HAL_OK)
      {
        return EE_WRITE_ERROR;
      }
    }
    break;
  default:
    break;
  }

  /* Return last operation flash status */
  return EE_OK;
}

/**
  * @brief  Get page state in page header
  * @param  Address Address of the FLASH Memory page
  * @retval State State of the page
  */
static EE_State_type GetPageState(uint32_t Address)
{
  EE_ELEMENT_TYPE status1 = 0U, status2 = 0U, status3 = 0U, status4 = 0U;

  /* Get page state information from page header (3 first elements) */
  status1 = (*(__IO EE_ELEMENT_TYPE*)Address);
  status2 = (*(__IO EE_ELEMENT_TYPE*)(Address + EE_ELEMENT_SIZE));
  status3 = (*(__IO EE_ELEMENT_TYPE*)(Address + (EE_ELEMENT_SIZE*2U)));
  status4 = (*(__IO EE_ELEMENT_TYPE*)(Address + (EE_ELEMENT_SIZE*3U)));

  /* Return erasing status, if element4 is not EE_PAGESTAT_ERASED value */
  if (status4 != EE_PAGESTAT_ERASED)
  {
    return STATE_PAGE_ERASING;
  }

  /* Return valid status, if element3 is not EE_PAGESTAT_ERASED value */
  if (status3 != EE_PAGESTAT_ERASED)
  {
    return STATE_PAGE_VALID;
  }

  /* Return active status, if element2 is not EE_PAGESTAT_ERASED value */
  if (status2 != EE_PAGESTAT_ERASED)
  {
    return STATE_PAGE_ACTIVE;
  }

  /* Return receive status, if element1 is not EE_PAGESTAT_ERASED value */
  if (status1 != EE_PAGESTAT_ERASED)
  {
    return STATE_PAGE_RECEIVE;
  }

  /* Return erased status, if 4 first elements are EE_PAGESTAT_ERASED value */
  return STATE_PAGE_ERASED;
}

/**
  * @brief  This function configures CRC Instance.
  * @note   This function is used to :
  *         -1- Enable peripheral clock for CRC.
  *         -2- Configure CRC functional parameters.
  * @note   Peripheral configuration is minimal configuration from reset values.
  *         Thus, some useless LL unitary functions calls below are provided as
  *         commented examples - setting is default configuration from reset.
  * @param  None
  * @retval None
  */
void ConfigureCrc(void)
{
  /* (1) Enable peripheral clock for CRC */
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_CRC);

  /* (2) Configure CRC functional parameters */

  /* Configure CRC calculation unit with user defined polynomial */
  LL_CRC_SetPolynomialCoef(CRC, CRC_POLYNOMIAL_VALUE);
  LL_CRC_SetPolynomialSize(CRC, CRC_POLYNOMIAL_LENGTH);

  /* Initialize default CRC initial value */
  /* Reset value is LL_CRC_DEFAULT_CRC_INITVALUE */
  /* LL_CRC_SetInitialData(CRC, LL_CRC_DEFAULT_CRC_INITVALUE);*/

  /* Set input data inversion mode : No inversion*/
  /* Reset value is LL_CRC_INDATA_REVERSE_NONE */
  /* LL_CRC_SetInputDataReverseMode(CRC, LL_CRC_INDATA_REVERSE_NONE); */

  /* Set output data inversion mode : No inversion */
  /* Reset value is LL_CRC_OUTDATA_REVERSE_NONE */
  /* LL_CRC_SetOutputDataReverseMode(CRC, LL_CRC_OUTDATA_REVERSE_NONE); */
}

/**
  * @brief  This function performs CRC calculation on Data and Virtual Address.
  * @param  Data value of  the eeprom variable.
  * @param  VirtAddress address of the eeprom variable.
  * @retval 16-bit CRC value computed on Data and Virtual Address.
  */
uint16_t CalculateCrc(EE_DATA_TYPE Data, uint16_t VirtAddress)
{
  /* Reset CRC calculation unit */
  LL_CRC_ResetCRCCalculationUnit(CRC);

  /* Feed Data and Virtual Address */
  LL_CRC_FeedData32(CRC, Data);
  LL_CRC_FeedData16(CRC, VirtAddress);

  /* Return computed CRC value */
  return(LL_CRC_ReadData16(CRC));
}

/**
  * @}
  */

/**
  * @}
  */
