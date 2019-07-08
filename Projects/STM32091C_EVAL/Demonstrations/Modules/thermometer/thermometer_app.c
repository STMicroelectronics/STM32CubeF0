/**
  ******************************************************************************
  * @file    thermometer_app.c
  * @author  MCD Application Team
  * @brief   thermometer system information.
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

#define __THERMOMETER_APP_C

/* Includes ------------------------------------------------------------------*/
#include "string.h"
#include "stdio.h"
#include "k_config.h"
#include "k_module.h"
#include "k_menu.h"
#include "k_storage.h"

/** @addtogroup 8UART_MODULE
  * @{
  */

/** @defgroup SYSTEM_INFO
  * @brief system info routines 
  * @{
  */

/* Private typedef ----------------------------------------------------------*/    
/* Private constants ----------------------------------------------------------*/


/* Private function prototypes -----------------------------------------------*/
KMODULE_RETURN _ThermometerDemoExec(void);
KMODULE_RETURN _ThermometerConfig(void);

void ThermometerDemo(void);
void ThermometerMenuUserAction(uint8_t sel);
void ThermometerUserInformation(int16_t tempvalue);
void ThermometerUserHeader(void);

/* Private Variable ----------------------------------------------------------*/
const tMenuItem ThermometerMenuMenuItems[] =
{
    {"Press down to exit", 14, 30, TYPE_EXEC, MODULE_NONE, ThermometerDemo, ThermometerMenuUserAction, NULL, NULL },
};

const tMenu ThermometerMenu = {
  "Temperature (LM75)", ThermometerMenuMenuItems, countof(ThermometerMenuMenuItems), TYPE_EXEC, 1, 1 };

/* used to exit application */
static uint8_t user_stop=0;

/* Private typedef -----------------------------------------------------------*/
/* External variables --------------------------------------------------------*/
const K_ModuleItem_Typedef ModuleTsensor =
{
  MODULE_TSENSOR,
  _ThermometerConfig,
  _ThermometerDemoExec,
  NULL,
  NULL
};  


/**
  * @brief  setup the HW for the 8 uart application 
  * @param  None.
  * @note   set the memeory + Hw initialisation.  
  * @retval None.
  */
KMODULE_RETURN _ThermometerConfig(void)
{
  BSP_TSENSOR_Init();
  return KMODULE_OK;
}

/**
  * @brief  Run the 8 uart application 
  * @param  None.
  * @note   run and display information about the uart transaction.  
  * @retval None.
  */
KMODULE_RETURN _ThermometerDemoExec(void)
{
  /* Prepare the main MMI */
  BSP_LCD_Clear(LCD_COLOR_WHITE);
  kMenu_Execute(ThermometerMenu);
  
  /* Execute the app 8uart */
  /* App initialization    */
  return KMODULE_OK;
}


void ThermometerDemo(void)
{
uint16_t tempvalue = 0;

  /* Display user information */
  ThermometerUserHeader();
  
  /* Display temperature until user event */
  user_stop = 0;
  while(user_stop == 0) 
  {
      /* Get temperature value */     
      tempvalue = BSP_TSENSOR_ReadTemp();
      ThermometerUserInformation(tempvalue);
  }
}

/**
  * @brief  Display uart transaction state 
  * @param  None.
  * @note   This display information about the uart transaction.  
  * @retval None.
  */
void ThermometerUserHeader(void)
{
  /* Display the Thermometer icon */
  kStorage_OpenFileDrawBMP(130, 80, (uint8_t *)"STFILES/ICON6.BMP");
  /* Set the Back Color */
  BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
  /* Set the Text Color */
  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
}  


/**
  * @brief  Display uart transaction state 
  * @param  None.
  * @note   This display information about the uart transaction.  
  * @retval None.
  */
void ThermometerUserInformation(int16_t temperature)
{
  float TempCelsiusDisplay = 0;
  uint8_t LCDStr[30];
  
  if (temperature & 0x8000)
  {
    /* Display negative temperature */
    BSP_LCD_SetTextColor(LCD_COLOR_RED);
    TempCelsiusDisplay = -((temperature & 0x7FFF)>>1);
    if (temperature & 0x0001)
    {
      TempCelsiusDisplay += 0.5;
    }
  }
  else
  {
    /* Display positive temperature */
    BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
    TempCelsiusDisplay = ((temperature & 0x7FFF)>>1);
    if (temperature & 0x0001)
    {
      TempCelsiusDisplay += 0.5;
    }
  }
  
  sprintf((char*)LCDStr, "       %5.1f C      ", TempCelsiusDisplay);
  BSP_LCD_DisplayStringAtLine(6, (uint8_t*)LCDStr);
  sprintf((char*)LCDStr, "       %2.1f F      ", 1.8f * TempCelsiusDisplay + 32);
  BSP_LCD_DisplayStringAtLine(7, (uint8_t*)LCDStr);
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
}  

/**
  * @brief  Get User action 
  * @param  sel : User selection (JOY_SEL,...)
  * @note   This example is the only way to get user information.  
  * @retval None
  */
void ThermometerMenuUserAction(uint8_t sel)
{
  switch(sel)
  {
  case JOY_DOWN : 
       user_stop = 1;
    break;
  default :
    break;
  }
}


/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

