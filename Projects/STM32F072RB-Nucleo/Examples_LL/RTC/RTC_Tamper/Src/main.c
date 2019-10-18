/**
  ******************************************************************************
  * @file    Examples_LL/RTC/RTC_Tamper/Src/main.c
  * @author  MCD Application Team
  * @brief   This example describes how to configure Time Stamp feature through
  *          the STM32F0xx RTC LL API.
  *          Peripheral initialization done using LL unitary services functions.
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

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/** @addtogroup STM32F0xx_LL_Examples
  * @{
  */

/** @addtogroup RTC_Tamper
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define BACKUP_COUNT RTC_BKP_NUMBER

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
__IO FlagStatus TamperStatus;

/* Backup registers table */
uint32_t aBKPDataReg[BACKUP_COUNT] =
{
  LL_RTC_BKP_DR0, LL_RTC_BKP_DR1, LL_RTC_BKP_DR2,
  LL_RTC_BKP_DR3, LL_RTC_BKP_DR4
};

/* Private function prototypes -----------------------------------------------*/
void     SystemClock_Config(void);
void     Configure_RTC(void);
void     Configure_RTC_Tamper(void);
void     LED_Init(void);
void     LED_On(void);
void     LED_Blinking(uint32_t Period);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  register uint32_t index = 0;

  /* Configure the system clock to 48 MHz */
  SystemClock_Config();

  /* Initialize LED2 */
  LED_Init();

  /* Configure the RTC peripheral */
  Configure_RTC();

  /* Configure RTC Tamper */
  Configure_RTC_Tamper();

  /* Write Data on the Back Up registers  */
  for (index = 0; index < BACKUP_COUNT; index++)
  {
    LL_RTC_BAK_SetRegister(RTC, aBKPDataReg[index], 0xDF59 + (index * 0x5A));
  }

  /* Check Data is stored on the Back Up registers */
  for (index = 0; index < BACKUP_COUNT; index++)
  {
    if (LL_RTC_BAK_GetRegister(RTC, aBKPDataReg[index]) != (0xDF59 + (index * 0x5A)))
    {
      LED_Blinking(LED_BLINK_ERROR);
    }
  }

  /* Reset flag after writing of backup register in order to wait for new button press */
  TamperStatus = RESET;

  /* Wait for Tamper detection */
  while(TamperStatus != SET)
  {
    LL_GPIO_TogglePin(LED2_GPIO_PORT, LED2_PIN);  
    LL_mDelay(LED_BLINK_FAST);
  }
  
  /* LED2 On: Tamper button pressed */
  LED_On();
    
  /* Infinite loop */
  while (1)
  {
  }
}

/**
  * @brief  Configure RTC.
  * @param  None
  * @retval None
  */
void Configure_RTC(void)
{
  /* Enables the PWR Clock and Enables access to the backup domain */
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);
  LL_PWR_EnableBkUpAccess();

  /* Enable RTC Clock */ 
  LL_RCC_EnableRTC();
}

/**
  * @brief  Configure the current time and date and activate Tamper.
  * @note   Peripheral configuration is minimal configuration from reset values.
  *         Thus, some useless LL unitary functions calls below are provided as
  *         commented examples - setting is default configuration from reset.
  * @param  None
  * @retval None
  */
void Configure_RTC_Tamper(void)
{
  /* Sampling frequency */
  /* Reset value is LL_RTC_TAMPER_SAMPLFREQDIV_32768 */
  //LL_RTC_TAMPER_SetSamplingFreq(RTC, LL_RTC_TAMPER_SAMPLFREQDIV_32768);
  /* Precharge duration */
  /* Reset value is LL_RTC_TAMPER_DURATION_1RTCCLK */
  //LL_RTC_TAMPER_SetPrecharge(RTC, LL_RTC_TAMPER_DURATION_1RTCCLK);
  /* Enable Pull up */
  /* Reset value is pull-up enabled */
  //LL_RTC_TAMPER_EnablePullUp(RTC);
  /* Disable timestamp on tamper detection */
  /* Reset value is timestamp on tamper disabled */
  //LL_RTC_TS_DisableOnTamper(RTC);
  
  /* Set Tamper trigger to falling edge */
  /* Reset value is LL_RTC_TAMPER_FILTER_DISABLE */
  //LL_RTC_TAMPER_SetFilterCount(RTC, LL_RTC_TAMPER_FILTER_DISABLE);
  LL_RTC_TAMPER_EnableActiveLevel(RTC, LL_RTC_TAMPER_ACTIVELEVEL_TAMP1);
  
  /* Enable tamper detection */
  LL_RTC_TAMPER_Enable(RTC, LL_RTC_TAMPER_1);
  
  /* Enable IT TAMPER */
  LL_RTC_EnableIT_TAMP(RTC);

  /* Configure the NVIC for RTC Tamper */
  NVIC_SetPriority(RTC_IRQn, 0x0F);
  NVIC_EnableIRQ(RTC_IRQn);
  
  /* RTC Tamper Interrupt Configuration: EXTI configuration */
  LL_EXTI_EnableIT_0_31(LL_EXTI_LINE_19);
  LL_EXTI_EnableRisingTrig_0_31(LL_EXTI_LINE_19);

  /* Clear the Tamper interrupt pending bit */
  LL_RTC_ClearFlag_TAMP1(RTC);
}

/**
  * @brief  Initialize LED2.
  * @param  None
  * @retval None
  */
void LED_Init(void)
{
  /* Enable the LED2 Clock */
  LED2_GPIO_CLK_ENABLE();

  /* Configure IO in output push-pull mode to drive external LED2 */
  LL_GPIO_SetPinMode(LED2_GPIO_PORT, LED2_PIN, LL_GPIO_MODE_OUTPUT);
  /* Reset value is LL_GPIO_OUTPUT_PUSHPULL */
  //LL_GPIO_SetPinOutputType(LED2_GPIO_PORT, LED2_PIN, LL_GPIO_OUTPUT_PUSHPULL);
  /* Reset value is LL_GPIO_SPEED_FREQ_LOW */
  //LL_GPIO_SetPinSpeed(LED2_GPIO_PORT, LED2_PIN, LL_GPIO_SPEED_FREQ_LOW);
  /* Reset value is LL_GPIO_PULL_NO */
  //LL_GPIO_SetPinPull(LED2_GPIO_PORT, LED2_PIN, LL_GPIO_PULL_NO);
}

/**
  * @brief  Turn-on LED2.
  * @param  None
  * @retval None
  */
void LED_On(void)
{
  /* Turn LED2 on */
  LL_GPIO_SetOutputPin(LED2_GPIO_PORT, LED2_PIN);
}

/**
  * @brief  Set LED2 to Blinking mode for an infinite loop (toggle period based on value provided as input parameter).
  * @param  Period : Period of time (in ms) between each toggling of LED
  *   This parameter can be user defined values. Pre-defined values used in that example are :
  *     @arg LED_BLINK_FAST : Fast Blinking
  *     @arg LED_BLINK_SLOW : Slow Blinking
  *     @arg LED_BLINK_ERROR : Error specific Blinking
  * @retval None
  */
void LED_Blinking(uint32_t Period)
{
  /* Toggle IO in an infinite loop */
  while (1)
  {
    LL_GPIO_TogglePin(LED2_GPIO_PORT, LED2_PIN);  
    LL_mDelay(Period);
  }
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
  *            System Clock source            = PLL (HSI48)
  *            SYSCLK(Hz)                     = 48000000
  *            HCLK(Hz)                       = 48000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 1
  *            HSI Frequency(Hz)              = 48000000
  *            PREDIV                         = 2
  *            PLLMUL                         = 2
  *            Flash Latency(WS)              = 1
  * @param  None
  * @retval None
  */
void SystemClock_Config(void)
{
  /* Set FLASH latency */ 
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_1);

  /* Enable HSI48 and wait for activation*/
  LL_RCC_HSI48_Enable(); 
  while(LL_RCC_HSI48_IsReady() != 1) 
  {
  };
  
  /* Main PLL configuration and activation */
  LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSI48, LL_RCC_PLL_MUL_2, LL_RCC_PREDIV_DIV_2);
  
  LL_RCC_PLL_Enable();
  while(LL_RCC_PLL_IsReady() != 1)
  {
  };
  
  /* Sysclk activation on the main PLL */
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL)
  {
  };
  
  /* Set APB1 prescaler */
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
  
  /* Set systick to 1ms in using frequency set to 48MHz */
  /* This frequency can be calculated through LL RCC macro */
  /* ex: __LL_RCC_CALC_PLLCLK_FREQ (HSI48_VALUE, LL_RCC_PLL_MUL_2, LL_RCC_PREDIV_DIV_2) */
  LL_Init1msTick(48000000);
  
  /* Update CMSIS variable (which can be updated also through SystemCoreClockUpdate function) */
  LL_SetSystemCoreClock(48000000);
}

/******************************************************************************/
/*   USER IRQ HANDLER TREATMENT                                               */
/******************************************************************************/
/**
  * @brief  Tamper event callback function
  * @param  None
  * @retval None
  */
void Tamper_Callback(void)
{
  register uint32_t index = 0;
  
  /* Deactivate the tamper */
  LL_RTC_TAMPER_Disable(RTC, LL_RTC_TAMPER_1);
    
  /* Check Data is cleared on the Back Up registers */
  for (index = 0; index < BACKUP_COUNT; index++)
  {
    if (LL_RTC_BAK_GetRegister(RTC, aBKPDataReg[index]) != 0x00)
    {
      LED_Blinking(LED_BLINK_ERROR);
    }
  }
  TamperStatus = SET;
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
