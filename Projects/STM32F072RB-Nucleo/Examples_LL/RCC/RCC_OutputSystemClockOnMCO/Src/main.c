/**
  ******************************************************************************
  * @file    Examples_LL/RCC/RCC_OutputSystemClockOnMCO/Src/main.c
  * @author  MCD Application Team
  * @brief   This example describes how to how to configure MCO pin to output 
  *          the system clock through the STM32F0xx RCC LL API.
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

/** @addtogroup RCC_OutputSystemClockOnMCO
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Structure based on parameters used for MCO config */
typedef struct
{
  uint32_t Source;	      /*!< Source of the MCO output */
  uint32_t Prescaler;	    /*!< Prescaler applied to MCO output */
} RCC_MCO_ConfigTypeDef;

/* Private define ------------------------------------------------------------*/
/* Number of MCO Config */
#define RCC_MCO_CONFIG_NB   3

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Variable to set different MCO config */
static RCC_MCO_ConfigTypeDef aMCO_Config[RCC_MCO_CONFIG_NB] = {
  {LL_RCC_MCO1SOURCE_SYSCLK, LL_RCC_MCO1_DIV_1},
  {LL_RCC_MCO1SOURCE_HSI48, LL_RCC_MCO1_DIV_2},
  {LL_RCC_MCO1SOURCE_PLLCLK, LL_RCC_MCO1_DIV_4},
  };

/* MCO Config index */
__IO uint8_t bMCOIndex = 0;

/* Private function prototypes -----------------------------------------------*/
void     SystemClock_Config(void);
void     MCO_ConfigGPIO(void);
void     LED_Init(void);
void     UserButton_Init(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  /* Configure the system clock to 48 MHz */
  SystemClock_Config();

  /* Initialize LED2 */
  LED_Init();

  /* Initialize button in EXTI mode */
  UserButton_Init();

  /* Configure GPIO for MCO */
  MCO_ConfigGPIO();

  /* Infinite loop */
  while (1)
  {
    /* All the process are managed in UserButton_Callback function */
    /* Each time, user press user-button, a new MCO config is set */
  }
}

/**
  * @brief  Configure MCO pin (PA8).
  * @param  None
  * @retval None
  */
void MCO_ConfigGPIO(void)
{
  /* MCO Clock Enable */
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);

  /* Configue the MCO pin in alternate function mode */
  LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_8, LL_GPIO_MODE_ALTERNATE);
  LL_GPIO_SetPinOutputType(GPIOA, LL_GPIO_PIN_8, LL_GPIO_OUTPUT_PUSHPULL);
  LL_GPIO_SetPinSpeed(GPIOA, LL_GPIO_PIN_8, LL_GPIO_SPEED_FREQ_HIGH);
  LL_GPIO_SetPinPull(GPIOA, LL_GPIO_PIN_8, LL_GPIO_PULL_NO);
  LL_GPIO_SetAFPin_8_15(GPIOA, LL_GPIO_PIN_8, LL_GPIO_AF_0);
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
  * @brief  Configures User push-button in GPIO or EXTI Line Mode.
  * @param  None
  * @retval None
  */
void UserButton_Init(void)
{
  /* Enable the BUTTON Clock */
  USER_BUTTON_GPIO_CLK_ENABLE();
  
  /* Configure GPIO for BUTTON */
  LL_GPIO_SetPinMode(USER_BUTTON_GPIO_PORT, USER_BUTTON_PIN, LL_GPIO_MODE_INPUT);
  LL_GPIO_SetPinPull(USER_BUTTON_GPIO_PORT, USER_BUTTON_PIN, LL_GPIO_PULL_NO);

  /* Connect External Line to the GPIO*/
  USER_BUTTON_SYSCFG_SET_EXTI();
  
  /* Enable a rising trigger EXTI line 13 Interrupt */
  USER_BUTTON_EXTI_LINE_ENABLE();
  USER_BUTTON_EXTI_FALLING_TRIG_ENABLE();
  
  /* Configure NVIC for USER_BUTTON_EXTI_IRQn */
  NVIC_EnableIRQ(USER_BUTTON_EXTI_IRQn); 
  NVIC_SetPriority(USER_BUTTON_EXTI_IRQn,0x03);  
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
  * @brief  Function to manage User button press
  * @param  None
  * @retval None
  */
void UserButton_Callback(void)
{
  register uint32_t source = 0, prescaler = 0;
  
  /* Get the MCO config to apply */
  source = aMCO_Config[bMCOIndex].Source;
  prescaler = aMCO_Config[bMCOIndex].Prescaler;
  
  /* Toggle LED2 to indicate a button press*/
  LL_GPIO_TogglePin(LED2_GPIO_PORT, LED2_PIN);  
  
  /* Select MCO clock source and prescaler */
  LL_RCC_ConfigMCO(source, prescaler);

  /* Set new MCO config Index */
  bMCOIndex = (bMCOIndex + 1) % RCC_MCO_CONFIG_NB;
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
