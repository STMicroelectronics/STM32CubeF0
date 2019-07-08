/**
  ******************************************************************************
  * @file    system_win.c
  * @author  MCD Application Team
  * @brief   System information functions
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2016 STMicroelectronics International N.V. 
  * All rights reserved.</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "k_config.h"
#include "k_module.h"
#include "k_menu.h"
#include "k_storage.h"

/* Private function prototypes -----------------------------------------------*/
KMODULE_RETURN AppMainExec(void);
static void AppMain_About(void);
static void AppMain_UserEvent(uint8_t sel);
KMODULE_RETURN AppMainExecCheckRessource(void);

/* Private Variable ----------------------------------------------------------*/
/* Variable used to exit an execution context */
__IO uint8_t exit_func;

const tMenuItem MainMenuItems[] =
{
    {"Images Viewer"     , 32, 60, SEL_MODULE, MODULE_VIEWBMP, NULL, NULL, NULL, "STFILES/ICON3.BMP" },
    {"8 uarts"           ,128, 60, SEL_MODULE,   MODULE_8UART, NULL, NULL, NULL, "STFILES/8Uart.BMP" },
    {"Thermometer"       ,224, 60, SEL_MODULE, MODULE_TSENSOR, NULL, NULL, NULL, "STFILES/ICON6.BMP" },
    {"Low Power Mode"    , 32,144, SEL_MODULE,MODULE_LOWPOWER, NULL, NULL, NULL, "STFILES/ICON7.BMP" },
    {"Files Browser"     ,128,144, SEL_MODULE,MODULE_FILESBRO, NULL, NULL, NULL, "STFILES/filebw.BMP" },
    {"About"             ,224,144,   SEL_EXEC,              0, AppMain_About, AppMain_UserEvent, NULL, "STFILES/ICON12.BMP"}
};

const tMenu MainMenu = {
  "Main menu", MainMenuItems, countof(MainMenuItems), TYPE_ICON, 2, 3
  };

    

/* Private typedef -----------------------------------------------------------*/
/* External variables --------------------------------------------------------*/
const K_ModuleItem_Typedef ModuleAppMain =
{
  MODULE_MAIN_APP,
  NULL,
  AppMainExec,
  NULL,
  AppMainExecCheckRessource
};  

/**
  * @brief  Run the main application 
  * @param  None.
  * @note   run and display main menu.  
  * @retval None.
  */
KMODULE_RETURN AppMainExec(void)
{
  kMenu_Execute(MainMenu);
  return KMODULE_OK;
}

/**
  * @brief  Run the About information 
  * @param  None.
  * @note   display About information.  
  * @retval None.
  */
static void AppMain_About(void)
{
  exit_func = 0;
  BSP_LCD_SetFont(&Font20);
  BSP_LCD_Clear(LCD_COLOR_BLUE);
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
  
  BSP_LCD_DisplayStringAt(0,2*Font20.Height, (uint8_t *)EVAL_BOARD, CENTER_MODE);
  BSP_LCD_DisplayStringAt(0,3*Font20.Height, (uint8_t *)"Demonstration", CENTER_MODE);
  
  BSP_LCD_DisplayStringAt(0,5*Font20.Height, (uint8_t *)DEMO_VERSION, CENTER_MODE);
  BSP_LCD_DisplayStringAt(0,6*Font20.Height, (uint8_t *)DEMO_DATE, CENTER_MODE);
  
  BSP_LCD_DisplayStringAt(0,8*Font20.Height, (uint8_t *)DEMO_INFO1, CENTER_MODE);
  BSP_LCD_DisplayStringAt(0,9*Font20.Height, (uint8_t *)DEMO_INFO2, CENTER_MODE);
  BSP_LCD_DisplayStringAt(0,10*Font20.Height, (uint8_t *)DEMO_INFO3, CENTER_MODE);
  while(exit_func==0);
}

/**
  * @brief  Callback used to get user feedback 
  * @param  None.
  * @note   None.  
  * @retval None.
  */
static void AppMain_UserEvent(uint8_t sel)
{
  switch(sel)
  {
  case JOY_DOWN :
	exit_func = 1;
    break;
  }
}


/**
  * @brief  check the main application ressources 
  * @param  None.
  * @note   None.  
  * @retval None.
  */
KMODULE_RETURN AppMainExecCheckRessource(void)
{
 uint8_t index; 
 
  /* check icon menu */
  for(index = 0; index <  countof(MainMenuItems); index++)
  {
    if(kStorage_FileExist((uint8_t *)MainMenuItems[index].pIconPath) != KSTORAGE_NOERROR)
    {
      return KMODULE_ERROR_ICON;
    }
  }
  return KMODULE_OK;
}

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

