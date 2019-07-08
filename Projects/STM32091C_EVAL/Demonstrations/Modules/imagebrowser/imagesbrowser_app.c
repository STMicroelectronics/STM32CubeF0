/**
  ******************************************************************************
  * @file    images_broswer_app.c
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
#define __IMAGEBROWSER_APP_C

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
KMODULE_RETURN _ImageBrowserDemoExec(void);

void ImageBrowserDemo(void);
void ImageBrowserMenuUserAction(uint8_t sel);
void ImageBrowserUserInformation(int16_t tempvalue);
void ImageBrowserUserHeader(void);

/* Private Variable ----------------------------------------------------------*/
const tMenuItem ImageBrowserMenuMenuItems[] =
{
    {NULL, 14, 30, TYPE_EXEC, MODULE_NONE, ImageBrowserDemo, ImageBrowserMenuUserAction, NULL, NULL },
};

const tMenu ImageBrowserMenu = {
  NULL, ImageBrowserMenuMenuItems, countof(ImageBrowserMenuMenuItems), TYPE_EXEC, 1, 1 };

/* used to exit application */
static __IO uint8_t user_event=0;
static __IO uint8_t user_action=0;

/* Private typedef -----------------------------------------------------------*/
/* External variables --------------------------------------------------------*/
const K_ModuleItem_Typedef ModuleImageBrowser =
{
  MODULE_TSENSOR,
  NULL,
  _ImageBrowserDemoExec,
  NULL,
  NULL
};

/**
  * @brief  Run the 8 uart application 
  * @param  None.
  * @note   run and display information about the uart transaction.  
  * @retval None.
  */
KMODULE_RETURN _ImageBrowserDemoExec(void)
{
  /* Prepare the main MMI */
  kMenu_Execute(ImageBrowserMenu);
  
  /* Execute the app 8uart */
  /* App initialization    */
  return KMODULE_OK;
}


/**
  * @brief  Run the image browser 
  * @param  None.
  * @note   run and display image accordng the user action.  
  * @retval None.
  */
void ImageBrowserDemo(void)
{
  uint8_t  filename[15];
  uint8_t  PathImage[50];
  
  /* Lecture du folder source */
  kStorage_GetDirectoryFiles((uint8_t *)"USER", KSTORAGE_FINDFIRST, filename, (uint8_t *)"BMP");
  
  /* Affichage de la premiere image */
  sprintf((char*)PathImage,"USER/%s",filename);
  kStorage_OpenFileDrawPixel(0,0,PathImage);
  
  /* reset the user action event handler */
  user_action = 0;
  
  /* Wait User event                    */
  /*       JOY_LEFT  : next picture     */
  /*       JOY_RIGHT : next picture     */
  /*       JOY_SEL   : exit application */
  do
  {
    while(user_action == 0);
    
    if((user_event == JOY_RIGHT) || (user_event == JOY_LEFT))
    {
     
      if(kStorage_GetDirectoryFiles((uint8_t *)"USER", ((user_event == JOY_LEFT) ? KSTORAGE_FINDPREV : KSTORAGE_FINDNEXT) , filename, (uint8_t *)"BMP") == KSTORAGE_NOERROR)
      {
        /* display the picture */
        sprintf((char *)PathImage,"USER/%s",filename);
        kStorage_OpenFileDrawPixel(0,0,PathImage);
      }
    }
    
    /* exit on UP or Down */
    if((user_event == JOY_UP) || (user_event == JOY_DOWN))
    {
      user_event = JOY_SEL;
    }

    /* reset the user action event handler */
    user_action = 0;
  } 
  while(user_event != JOY_SEL);
  
  /* Close the find */
  kStorage_GetDirectoryFiles((uint8_t *)"USER", KSTORAGE_FINDCLOSE, filename, (uint8_t *)"BMP");
}

/**
  * @brief  Get User action 
  * @param  sel : User selection (JOY_SEL,...)
  * @note   This example is the only way to get user information.  
  * @retval None
  */
void ImageBrowserMenuUserAction(uint8_t sel)
{
  if (user_action == 0 )
  {
    user_action = 1;
    user_event = sel;
  }
}

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

