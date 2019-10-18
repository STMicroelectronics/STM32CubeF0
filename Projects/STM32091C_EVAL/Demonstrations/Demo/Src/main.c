/**
  ******************************************************************************
  * @file    main.c 
  * @author  MCD Application Team
  * @brief   This file provides main program functions
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

/* Includes ------------------------------------------------------------------*/
#include "k_config.h"
#include "main.h"
#include "k_window.h" 
#include "k_demo.h" 
#include "k_menu.h"
#include "k_module.h"
#include "k_storage.h"
#include "main_app.h" 
#include "8uart_app.h" 
#include "thermometer_app.h" 
#include "imagesbrowser_app.h"
#include "filesbrowser_app.h" 
#include "lowpower_app.h" 

/** @addtogroup CORE
  * @{
  */

/** @defgroup MAIN
* @brief main file
* @{
*/ 
 
/** @defgroup MAIN_Private_TypesDefinitions
* @{
*/ 
/**
* @}
*/ 

/** @defgroup MAIN_Private_Defines
* @{
*/ 
/**
* @}
*/ 


/** @defgroup MAIN_Private_Macros
* @{
*/ 
/**
* @}
*/ 


/** @defgroup MAIN_Private_Variables
* @{
*/
/**
* @}
*/ 


/** @defgroup MAIN_Private_FunctionPrototypes
* @{
*/ 
void SystemClock_Config(void);
void SystemClock_STOPExit(void);
static void Error_Handler(void);

/**
* @}
*/ 

/** @defgroup MAIN_Private_Functions
* @{
*/ 

/**
* @brief  Main program
* @param  None
* @retval int
*/
int main(void)
{
  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();
 
  /* LED init */
  BSP_LED_Init(LED1);
  BSP_LED_Init(LED2);
  BSP_LED_Init(LED3);
  BSP_LED_Init(LED4);
 
  /* Configure the system clock @ CPU max */
  SystemClock_Config();
    
  /* Start the demo thread */
  kDemo_Start();
  return 0;
}

/** @brief EXTI line detection callbacks
  * @param GPIO_Pin: Specifies the pins connected EXTI line
  * @retval None
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  switch(GPIO_Pin)
  {
  case SD_DETECT_PIN :
    kStorage_SdDetection(BSP_SD_IsDetected());
    break;
  case DOWN_JOY_PIN :
  case UP_JOY_PIN :
  case SEL_JOY_PIN :
  case RIGHT_JOY_PIN :
  case LEFT_JOY_PIN :
  case TAMPER_BUTTON_PIN :
    kMenu_EventHandler(GPIO_Pin);
    break;
  default : 
    Error_Handler();
    break;
  }
}

/** @brief EXTI line detection callbacks
  * @param GPIO_Pin: Specifies the pins connected EXTI line
  * @retval None
  */
void kDemo_Initialization(void)
{
  /* Initialize the SD */
  BSP_SD_Init();
  
  /* Initialize the LCD */
  BSP_LCD_Init();
  BSP_LCD_Clear(LCD_COLOR_WHITE);

  /* Check the SD presence else wait until SD insertion */
  if(BSP_SD_IsDetected() != SD_PRESENT)
  {
    kWindow_Error("sd card present\n...\nwait sd insertion\n");
    while(BSP_SD_IsDetected() != SD_PRESENT); 
  }
                
  /* FatFs initialisation */
  if(kStorage_Init() != KSTORAGE_NOERROR)
  {
    kWindow_Error("FatFs initialization failed\nplease check the SD card\n");
    while(1);
  }
  
  /* Display the demonstration window */
  BSP_LCD_SetFont(&Font24);
  BSP_LCD_Clear(LCD_COLOR_WHITE);
  BSP_LCD_SetBackColor(LCD_COLOR_BLUE);
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
  BSP_LCD_FillRect(0, 0, BSP_LCD_GetXSize(), Font24.Height*2);
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
  BSP_LCD_DisplayStringAt( 0,              0,      (uint8_t *)EVAL_BOARD, CENTER_MODE);
  BSP_LCD_DisplayStringAt( 0,  Font24.Height, (uint8_t *)"DEMONSTRATION", CENTER_MODE);
  kStorage_OpenFileDrawPixel(80, 90, (uint8_t *)"STFILES/STLogo.bmp");
  /* Let time user to see information */
  HAL_Delay(2000);
  
  /* Module Initialization */
  kModule_Init();
 
  /* control the ressources */
  if(kModule_CheckRessource() != KMODULE_OK)
  {
    kWindow_Error("ressource file\nmissing\nplease update\nsd card content\n");
    while(1); 
  }
}

void kModule_Init(void)
{
   kModule_Add(MODULE_MAIN_APP, ModuleAppMain);
   kModule_Add(MODULE_8UART   , Module8Uart);
   kModule_Add(MODULE_TSENSOR , ModuleTsensor);
   kModule_Add(MODULE_VIEWBMP , ModuleImageBrowser);
   kModule_Add(MODULE_FILESBRO, ModuleFilesBrowser);
   kModule_Add(MODULE_LOWPOWER, ModuleLowPower);
}

void kDemo_UnInitialization(void)
{
  /* Nothing to do */
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 48000000
  *            HCLK(Hz)                       = 48000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 1
  *            HSE Frequency(Hz)              = 8000000
  *            PREDIV                         = 1
  *            PLLMUL                         = 6
  *            Flash Latency(WS)              = 1
  * @param  None
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;
  
  /* Enable HSE Oscillator and Activate PLL with HSE as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV1;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL6;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct)!= HAL_OK)
  {
    Error_Handler(); 
  }

  /* Select PLL as system clock source and configure the HCLK, PCLK1 clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1)!= HAL_OK)
  {
    Error_Handler(); 
  }
}

/**
  * @brief  Configures system clock after wake-up from STOP: enable HSE, PLL
  *         and select PLL as system clock source.
  * @param  None
  * @retval None
  */
void SystemClock_STOPExit(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;

  /* Enable Power Control clock */
  __HAL_RCC_PWR_CLK_ENABLE(); /* ???? */

  uint32_t pFLatency = 0;

  /* Get the Oscillators configuration according to the internal RCC registers */
  HAL_RCC_GetOscConfig(&RCC_OscInitStruct);

  /* After wake-up from STOP reconfigure the system clock: Enable HSE and PLL */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /* Get the Clocks configuration according to the internal RCC registers */
  HAL_RCC_GetClockConfig(&RCC_ClkInitStruct, &pFLatency);

  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2
     clocks dividers */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_SYSCLK;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, pFLatency) != HAL_OK)
  {
    Error_Handler();
  }
}

void Error_Handler(void)
{
  while(1)
  {
    HAL_Delay(100);
    BSP_LED_Toggle(LED1);
    BSP_LED_Toggle(LED2);
    BSP_LED_Toggle(LED3);
    BSP_LED_Toggle(LED4);
  }
}

#ifdef USE_FULL_ASSERT
/**
* @brief  assert_failed
*         Reports the name of the source file and the source line number
*         where the assert_param error has occurred.
* @param  File: pointer to the source file name
* @param  Line: assert_param error line source number
* @retval None
*/
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line
  number,ex: printf("Wrong parameters value: file %s on line %d\r\n", 
  file, line) */
  
  /* Infinite loop */
  while (1)
  {}
}

#endif


/**
* @}
*/ 

/**
* @}
*/ 

/**
* @}
*/ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
