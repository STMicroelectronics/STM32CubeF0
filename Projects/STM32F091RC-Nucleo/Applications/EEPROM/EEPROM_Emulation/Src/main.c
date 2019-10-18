/**
  ******************************************************************************
  * @file    EEPROM_Emulation/Src/main.c
  * @author  MCD Application Team
  * @brief   Main program body
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
#include "eeprom.h"

/** @addtogroup STM32F0xx_HAL_Applications
  * @{
  */

/** @addtogroup EEPROM_Emulation
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Virtual address defined by the user: 0xFFFF value is prohibited */
uint16_t VirtAddVarTab[NB_OF_VAR] = {0x5555, 0x6666, 0x7777};
uint16_t VarDataTab[NB_OF_VAR] = {0, 0, 0};
uint16_t VarValue = 0;

/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{

  /* STM32F0xx HAL library initialization:
       - Configure the Flash prefetch
       - Systick timer is configured by default as source of time base, but user 
         can eventually implement his proper time base source (a general purpose 
         timer for example or other time source), keeping in mind that Time base 
         duration should be kept 1ms since PPP_TIMEOUT_VALUEs are defined and 
         handled in milliseconds basis.
       - Low Level Initialization
     */
  HAL_Init();

  /* Configure the system clock to 48 MHz */
  SystemClock_Config();

  /* Unlock the Flash Program Erase controller */
  HAL_FLASH_Unlock();

  /* EEPROM Init */
  EE_Init();

/* --- Store successively many values of the three variables in the EEPROM ---*/
  /* Store 0x1000 values of Variable1 in EEPROM */
  for (VarValue = 1; VarValue <= 0x1000; VarValue++)
  {
    EE_WriteVariable(VirtAddVarTab[0], VarValue);
  }

  /* read the last stored variables data*/
  EE_ReadVariable(VirtAddVarTab[0], &VarDataTab[0]);


  /* Store 0x2000 values of Variable2 in EEPROM */
  for (VarValue = 1; VarValue <= 0x2000; VarValue++)
  {
    EE_WriteVariable(VirtAddVarTab[1], VarValue);
  }

  /* read the last stored variables data*/
  EE_ReadVariable(VirtAddVarTab[0], &VarDataTab[0]);
  EE_ReadVariable(VirtAddVarTab[1], &VarDataTab[1]);


  /* Store 0x3000 values of Variable3 in EEPROM */
  for (VarValue = 1; VarValue <= 0x3000; VarValue++)
  {
    EE_WriteVariable(VirtAddVarTab[2], VarValue);
  }

  /* read the last stored variables data*/
  EE_ReadVariable(VirtAddVarTab[0], &VarDataTab[0]);
  EE_ReadVariable(VirtAddVarTab[1], &VarDataTab[1]);
  EE_ReadVariable(VirtAddVarTab[2], &VarDataTab[2]);

  while (1)
  {
  };
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
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;
  
  /* Select HSI48 Oscillator as PLL source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI48;
  RCC_OscInitStruct.HSI48State = RCC_HSI48_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI48;
  RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV2;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct)!= HAL_OK)
  {
    /* Initialization Error */
    while(1); 
  }

  /* Select PLL as system clock source and configure the HCLK and PCLK1 clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1)!= HAL_OK)
  {
    /* Initialization Error */
    while(1); 
  }
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
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

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
