
/**
  ******************************************************************************
  * @file    lowpower_app.c
  * @author  MCD Application Team
  * @brief   thermometer system information.
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
#define __LOWPOWER_APP_C

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include "k_config.h"
#include "k_module.h"
#include "k_menu.h"
#include "k_window.h"

/** @addtogroup MODULES
  * @{
  */

/** @defgroup LOW_POWER 
  * @brief low power routines 
  * @{
  */

/* External function prototypes -----------------------------------------------*/
extern void SystemClock_STOPExit(void);
/* Private typedef ----------------------------------------------------------*/    
/* Private constants ----------------------------------------------------------*/
#define RTC_ASYNCH_PREDIV  0x7F   /* LSE as RTC clock */
#define RTC_SYNCH_PREDIV   0x00FF /* LSE as RTC clock */

/* Private function prototypes -----------------------------------------------*/
KMODULE_RETURN _LowPowerDemoExec(void);
KMODULE_RETURN _LowPowerDemoConfig(void);
KMODULE_RETURN _LowPowerDemoUnConfig(void);

void LowPowerDemo(void);
void LowPowerUserAction(uint8_t sel);
void LowPowerStandbyWakeupPin(void);
void LowPowerStandbyRTCAlarm(void);
void LowPowerStopEXTI(void);
void LowPowerStopRTCAlarm(void);

static void LowPowerHandleAlarm(void);
static void RTC_Config(void);

/* Private Variable ----------------------------------------------------------*/
/* RTC Handle variable */
static RTC_HandleTypeDef RtcHandle;

/* standby mode menu */
const tMenuItem StandbyModeMenuItems[] =
{
    {"EXIT WAKEUP"    , 14, 30,    SEL_EXEC, MODULE_NONE, LowPowerStandbyWakeupPin, LowPowerUserAction, NULL, NULL },
    {"EXIT RTC Alarm" , 14, 30,    SEL_EXEC, MODULE_NONE, LowPowerStandbyRTCAlarm, LowPowerUserAction, NULL, NULL },
    {"RETURN"         ,  0,  0,    SEL_EXIT, MODULE_NONE, NULL, NULL, NULL, NULL }
};

const tMenu StandbyModeMenu = {
  "Standby mode", StandbyModeMenuItems, countof(StandbyModeMenuItems), TYPE_TEXT, 1, 1
};


/* stop mode menu */
const tMenuItem StopModeMenuItems[] =
{
    {"EXIT EXTI"     , 14, 30,    SEL_EXEC, MODULE_NONE,     LowPowerStopEXTI, LowPowerUserAction, NULL, NULL },
    {"EXIT RTC Alarm", 14, 30,    SEL_EXEC, MODULE_NONE, LowPowerStopRTCAlarm, LowPowerUserAction, NULL, NULL },
    {"RETURN"        ,  0,  0,    SEL_EXIT, MODULE_NONE, NULL, NULL, NULL, NULL }
};

const tMenu StopModeMenu = {
  "Stop mode", StopModeMenuItems, countof(StopModeMenuItems), TYPE_TEXT, 1, 1
};


/* Main menu */
const tMenuItem LowPowerMenuItems[] =
{
    {"STOP    mode", 14, 30, SEL_SUBMENU, MODULE_NONE, NULL, NULL, (const tMenu*)&StopModeMenu, NULL },
    {"STANDBY mode", 14, 30, SEL_SUBMENU, MODULE_NONE, NULL, NULL, (const tMenu*)&StandbyModeMenu, NULL },
    {"RETURN"      ,  0,  0,    SEL_EXIT, MODULE_NONE, NULL, NULL, NULL, NULL }
};

const tMenu LowpowerMenu = {
  "Low power", LowPowerMenuItems, countof(LowPowerMenuItems), TYPE_TEXT, 1, 1};

/* used to exit application */
static __IO uint8_t user_event=0;
static __IO uint8_t user_action=0;

/* Private typedef -----------------------------------------------------------*/
/* External variables --------------------------------------------------------*/
const K_ModuleItem_Typedef ModuleLowPower =
{
  MODULE_LOWPOWER,
  _LowPowerDemoConfig,
  _LowPowerDemoExec,
  _LowPowerDemoUnConfig,
  NULL
};

/**
  * @brief  Configure the Lowpower application 
  * @param  None.
  * @note   run and display information about the lowpower feature.  
  * @retval None.
  */
KMODULE_RETURN _LowPowerDemoConfig(void)
{

  /* Enable Clock Security System(CSS) */
  HAL_RCC_EnableCSS();
  
  /* Configure RTC */
  RTC_Config();

  /*#### Disable all used wakeup sources ####*/
  HAL_RTC_DeactivateAlarm(&RtcHandle, RTC_ALARM_A);
  /* Disable all previous wake up interrupt */
  HAL_PWR_DisableWakeUpPin(PWR_WAKEUP_PIN1);

  /*#### Clear all related wakeup flags ####*/
  /* Clear the Alarm interrupt pending bit */
  __HAL_RTC_ALARM_CLEAR_FLAG(&RtcHandle,RTC_FLAG_ALRAF);
  /* Clear PWR wake up Flag */
  __HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);

    return KMODULE_OK;
}

/**
  * @brief  un-Configure the Lowpower application 
  * @param  None.
  * @note   run and display information about the lowpower feature.  
  * @retval None.
  */
KMODULE_RETURN _LowPowerDemoUnConfig(void)
{
  /* Enable Clock Security System(CSS) */
  HAL_RCC_DisableCSS();
  
  /* DeInit Rtc instance */
  if(HAL_RTC_DeInit(&RtcHandle) != HAL_OK)
  {
    /* Initialization Error */
    while(1);
  }

  return KMODULE_OK;
}

/**
  * @brief  Run the Lowpower application 
  * @param  None.
  * @note   run and display information about the lowpower feature.  
  * @retval None.
  */
KMODULE_RETURN _LowPowerDemoExec(void)
{
  /* Prepare Execute the main MMI of lowpower application */
  kMenu_Execute(LowpowerMenu);
  return KMODULE_OK;
}

/**
  * @brief  Get User action 
  * @param  sel : User selection (JOY_SEL,...)
  * @note   This example is the only way to get user information.  
  * @retval None
  */
void LowPowerUserAction(uint8_t sel)
{
  if (user_action == 0)
  {
    user_action = 1;
    user_event = sel;
  }
}

/**
  * @brief  Run the Lowpower Standby mode Wakeup pin 
  * @param  None.
  * @note   run and display information about the lowpower feature.  
  * @retval None.
  */
void LowPowerStandbyWakeupPin(void)
{
  kWindow_Popup("STANDBY WAKEUP", LCD_COLOR_WHITE, LCD_COLOR_BLUE,\
                "JOY sel will\nstart\nstandby mode\n",                  \
                LCD_COLOR_WHITE, LCD_COLOR_BLUE );

  HAL_Delay(100);
  user_event = JOY_UP;
  while(user_event != JOY_SEL)
  {
    user_action = 0;
  };

  kWindow_Popup("", LCD_COLOR_WHITE, LCD_COLOR_WHITE,         \
                "\n\n\n\npress JOY SEL\nto exit\nand reset\n",\
                 LCD_COLOR_BLUE, LCD_COLOR_WHITE );
  
  /* Enable WKUP pin */
  HAL_PWR_EnableWakeUpPin(PWR_WAKEUP_PIN1);
  
  /*#### Enter StandBy mode ####*/
  HAL_PWR_EnterSTANDBYMode();
}

/**
  * @brief  Run the Lowpower Standby mode RTC Alarm
  * @param  None.
  * @note   run and display information about the lowpower feature.  
  * @retval None.
  */
void LowPowerStandbyRTCAlarm(void)
{
  kWindow_Popup("STANDBY Alarm", LCD_COLOR_WHITE, LCD_COLOR_BLUE,\
                "\nset delay time\n",                            \
                LCD_COLOR_BLUE, LCD_COLOR_WHITE );
  
  /* Set the alarm */
  LowPowerHandleAlarm();
  
  kWindow_Popup("", LCD_COLOR_WHITE, LCD_COLOR_WHITE, \
                "\n\nstandby mode\nstarted\nwait alarm\nto exit\nand reset\n", \
                LCD_COLOR_BLUE, LCD_COLOR_WHITE );  
  
  BSP_JOY_Init(JOY_MODE_GPIO);

  /*#### Enter StandBy mode ####*/
  HAL_PWR_EnterSTANDBYMode();
}


/**
  * @brief  Enter in stop mode and exit by pressingf the tamper button
  * @param  None
  * @note   This example enter in stop mode.  
  * @retval None
  */
void LowPowerStopEXTI(void)
{
  kWindow_Popup("STOP EXTI", LCD_COLOR_WHITE, LCD_COLOR_BLUE,        \
                "\n\nPres JOY sel to\nstart\nstop mode\n", \
                 LCD_COLOR_WHITE, LCD_COLOR_BLUE );
  HAL_Delay(100);
  user_event = JOY_UP;
  while(user_event != JOY_SEL)
  {
    user_action = 0;
  };

  kWindow_Popup("STOP EXTI", LCD_COLOR_WHITE, LCD_COLOR_BLUE,        \
                "\n\nstop mode\nstarted\npress tamper\nto exit\n", \
                 LCD_COLOR_BLUE, LCD_COLOR_WHITE );  
  
  /* User push-button (EXTI_Line0) will be used to wakeup the system from STOP mode */
  BSP_PB_Init(BUTTON_TAMPER, BUTTON_MODE_EXTI);
  /* avoid wakeup from JOY event */
  BSP_JOY_Init(JOY_MODE_GPIO);  
  
  /* Clear PWR wake up Flag */
  __HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);
  
  /* Enter Stop Mode */
  HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFI);
  
  /* Restore the clock configuration */
  SystemClock_STOPExit();
  
  /* User push-button (EXTI_Line0) will be used to wakeup the system from STOP mode */
  BSP_PB_Init(BUTTON_TAMPER, BUTTON_MODE_GPIO);  
  BSP_JOY_Init(JOY_MODE_EXTI);  
}

/**
  * @brief  Enter in stop mode and exit by an alarm
  * @param  None
  * @note   This example enter in stop mode.  
  * @retval None
  */
void LowPowerStopRTCAlarm(void)
{
  kWindow_Popup("STOP Alarme", LCD_COLOR_WHITE, LCD_COLOR_BLUE,\
                "\nset delay time\n",                          \
                LCD_COLOR_BLUE, LCD_COLOR_WHITE ); 
  
  /* set the alarm */
  LowPowerHandleAlarm();
  
  kWindow_Popup("STOP Alarm", LCD_COLOR_WHITE, LCD_COLOR_BLUE, \
                "\n\nstop mode\nstarted\nwait alarm\nto exit\n",        \
                LCD_COLOR_BLUE, LCD_COLOR_WHITE );  
  
  BSP_JOY_Init(JOY_MODE_GPIO);  
  
  /* Enter Stop Mode */
  HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFE);
  
  /* Restore the clock configuration */
  SystemClock_STOPExit();
  
  HAL_RTC_DeactivateAlarm(&RtcHandle, RTC_ALARM_A);
  /* Clear the Alarm interrupt pending bit */
  __HAL_RTC_ALARM_CLEAR_FLAG(&RtcHandle,RTC_FLAG_ALRAF);
  /* Clear the EXTI's line Flag for RTC Alarm */
  __HAL_RTC_ALARM_EXTI_CLEAR_FLAG();
  
  BSP_JOY_Init(JOY_MODE_EXTI);
}
                  
/**
  * @brief RTC MSP Initialization 
  *        This function configures the hardware resources used in this example
  * @param hrtc: RTC handle pointer
  * 
  * @note  Care must be taken when HAL_RCCEx_PeriphCLKConfig() is used to select 
  *        the RTC clock source; in this case the Backup domain will be reset in  
  *        order to modify the RTC Clock source, as consequence RTC registers (including 
  *        the backup registers) and RCC_BDCR register are set to their reset values.
  *             
  * @retval None
  */                  
void HAL_RTC_MspInit(RTC_HandleTypeDef *hrtc)
{
  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_PeriphCLKInitTypeDef  PeriphClkInitStruct;
  
  /*##-1- Configue LSE as RTC clock soucre ###################################*/ 
  RCC_OscInitStruct.OscillatorType =  RCC_OSCILLATORTYPE_LSI | RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.LSIState = RCC_LSI_OFF;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    while(1);
  }
  
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RTC;
  PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
  if(HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  { 
    while(1);
  }

  /*##-2- Enable RTC peripheral Clocks #######################################*/ 
  /* Enable RTC Clock */
  __HAL_RCC_RTC_ENABLE();
}
                  
/**
  * @brief RTC MSP De-Initialization
  *        This function freeze the hardware resources used in this example:
  *          - Disable the Peripheral's clock
  * @param hrtc: RTC handle pointer
  * @retval None
  */
void HAL_RTC_MspDeInit(RTC_HandleTypeDef *hrtc)
{
  RCC_OscInitTypeDef RCC_OscInitStruct;
  
  /*##-1- Reset peripherals ##################################################*/
  __HAL_RCC_RTC_DISABLE();
  
  __HAL_RCC_PWR_CLK_DISABLE();
  
  RCC_OscInitStruct.OscillatorType =  RCC_OSCILLATORTYPE_LSI | RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  RCC_OscInitStruct.LSEState = RCC_LSE_OFF;
  RCC_OscInitStruct.LSIState = RCC_LSI_OFF;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    while(1);
  }
}

/**
  * @brief  Configures the RTC.
  * @param  None
  * @retval None
  */
static void RTC_Config(void)
{
  /* Enable Power Clock*/
  __HAL_RCC_PWR_CLK_ENABLE();
  
  /* Allow Access to RTC Backup domaine */
  HAL_PWR_EnableBkUpAccess();
  
  RtcHandle.Instance= RTC;
  
  /* Check if the system was resumed from StandBy mode */
  if (__HAL_PWR_GET_FLAG(PWR_FLAG_SB) != RESET)
  {
    /* Clear StandBy flag */
    __HAL_PWR_CLEAR_FLAG(PWR_FLAG_SB);

    /* Disable the write protection for RTC registers */
    __HAL_RTC_WRITEPROTECTION_DISABLE(&RtcHandle);

    /* Wait for RTC APB registers synchronisation (needed after start-up from Reset)*/
    if (HAL_RTC_WaitForSynchro(&RtcHandle) != HAL_OK)
    {
      while(1);
    }

    /* Enable the write protection for RTC registers */
    __HAL_RTC_WRITEPROTECTION_ENABLE(&RtcHandle);
    /* No need to configure the RTC as the RTC config(clock source, enable,
    prescaler,...) are kept after wake-up from STANDBY */
  }
  else
  {
    /* Reset Backup Domaine */
    __HAL_RCC_BACKUPRESET_FORCE();
    __HAL_RCC_BACKUPRESET_RELEASE();

    /* Set the RTC time base to 1s */    
    /* Configure RTC prescaler and RTC data registers as follows:
    - Hour Format = Format 24
    - Asynch Prediv = Value according to source clock
    - Synch Prediv = Value according to source clock
    - OutPut = Output Disable
    - OutPutPolarity = High Polarity
    - OutPutType = Open Drain */
    RtcHandle.Init.HourFormat = RTC_HOURFORMAT_24;
    RtcHandle.Init.AsynchPrediv = RTC_ASYNCH_PREDIV;
    RtcHandle.Init.SynchPrediv = RTC_SYNCH_PREDIV;
    RtcHandle.Init.OutPut = RTC_OUTPUT_DISABLE;
    RtcHandle.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
    RtcHandle.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
    if(HAL_RTC_Init(&RtcHandle) != HAL_OK)
    {
      /* Initialization Error */
      while(1);
    }
  }
}

/**
  * @brief  Get user info to setup an alarm
  * @param  RTC handle
  * @note   This function wait user info to setup the alarm.  
  * @retval None
  */
static void LowPowerHandleAlarm(void)
{
  enum {
    HOURS,
    MINUTES,
    SECONDS,
    END
  };
  
  RTC_DateTypeDef currentdate;
  RTC_TimeTypeDef time, currenttime;
  RTC_AlarmTypeDef Alarm;
  uint8_t temp[16];
  uint8_t exit = 0;
  uint8_t index = SECONDS;
  uint8_t position;
  
  /* Get the current time */
  memset(&Alarm, 0x00, sizeof(RTC_AlarmTypeDef));
  memset(&time,0x00,sizeof(RTC_TimeTypeDef));
  memset(&currenttime,0x00,sizeof(RTC_TimeTypeDef));
  
  /* Get the alarm time from user */
  do
  {
    BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
    sprintf((char *)temp, "    %.2d:%.2d:%.2d", time.Hours, time.Minutes, time.Seconds);
    BSP_LCD_DisplayStringAt(0, 5 * Font24.Height, temp, NO_MODE);
    BSP_LCD_SetTextColor(LCD_COLOR_RED);
    
    switch(index)
    {
    case HOURS :
      position = 4;
      sprintf((char *)temp, "%.2d", time.Hours);
      break;
    case MINUTES:
      position = 7;
      sprintf((char *)temp, "%.2d", time.Minutes);
      break;
    case SECONDS :
      position = 10;
      sprintf((char *)temp, "%.2d", time.Seconds);
      break;
    }
    BSP_LCD_DisplayStringAt(position* Font24.Width, 5 * Font24.Height, temp, NO_MODE);  
    
    user_event = JOY_NONE;
    user_action = 0;
    while(user_event == JOY_NONE);
    switch(user_event)
    {
    case JOY_UP :
      if(index == HOURS)
      {
        if( time.Hours == 23 ) time.Hours = 0;
        else
          time.Hours++;
      }
      
      if(index == MINUTES)
      {
        if(time.Minutes == 59 ) time.Minutes= 0;
        else
          time.Minutes++;
      }
      if(index == SECONDS)
      {
        if(time.Seconds == 59 ) time.Seconds =0;
        else
          time.Seconds++;
      }
      break;
    case JOY_DOWN :
      if(index == HOURS)
      {
        if (time.Hours == 0 ) time.Hours = 23;
        else time.Hours--;
      }
      if(index == MINUTES)
      {
        if(time.Minutes == 0) time.Minutes=59;
        else 
          time.Minutes--;
      }
      if(index == SECONDS)
      {
        if(time.Seconds == 0) time.Seconds = 59;
        else
          time.Seconds--;
      }
      break;
    case JOY_RIGHT :
      if(index != SECONDS ) index++; 
      break;
    case JOY_LEFT :
      if(index != HOURS ) index--;
      break;
    case JOY_SEL :
      exit = 1;
      break;
    }
  } while(exit == 0);

  HAL_RTC_GetTime(&RtcHandle, &currenttime,  RTC_FORMAT_BIN);
  HAL_RTC_GetDate(&RtcHandle, &currentdate, RTC_FORMAT_BIN);
  time.SubSeconds = currenttime.SubSeconds;
  if((time.Seconds + currenttime.Seconds) > 60 )  time.Minutes++;
  time.Seconds = ((time.Seconds + currenttime.Seconds) % 60);
  
  if((time.Minutes + currenttime.Minutes) > 60 )  time.Hours++;
  time.Minutes = ((time.Minutes + currenttime.Minutes) % 60);
  
  time.Hours = ((time.Hours + currenttime.Hours) % 24);
  
  /* Set the alarm */
  Alarm.Alarm = RTC_ALARM_A;
  Alarm.AlarmDateWeekDay = RTC_WEEKDAY_MONDAY;
  Alarm.AlarmDateWeekDaySel = RTC_ALARMDATEWEEKDAYSEL_DATE;
  Alarm.AlarmMask = RTC_ALARMMASK_DATEWEEKDAY | RTC_ALARMMASK_HOURS | RTC_ALARMMASK_MINUTES;
  Alarm.AlarmSubSecondMask = RTC_ALARMSUBSECONDMASK_ALL;
  Alarm.AlarmTime.TimeFormat = RTC_HOURFORMAT_24;
  Alarm.AlarmTime.Hours = time.Hours;
  Alarm.AlarmTime.Minutes = time.Minutes;
  Alarm.AlarmTime.Seconds = time.Seconds;
  if(HAL_RTC_SetAlarm_IT(&RtcHandle, &Alarm, RTC_FORMAT_BIN) != HAL_OK)
  {
    while(1);
  }
}

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

