/**
  ******************************************************************************
  * @file    Examples_LL/PWR/PWR_EnterStopMode/Src/main.c
  * @author  MCD Application Team
  * @brief   This example describes how to  enter and exit the STOP_LPREGU mode
  *          through the STM32F0xx PWR LL API.
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
#include "main.h"

/** @addtogroup STM32F0xx_LL_Examples
  * @{
  */

/** @addtogroup PWR_EnterStopMode
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
void     SystemClock_Config(void);
void     Configure_PWR(void);
void     LED_Init(void);
void     LED_Off(void);
void     LED_Blinking_5s(void);
void     EnterSTOP_LPREGUMode(void);

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

  /* Configure Power IP */
  Configure_PWR();

  /* Led blinking during 5s in RUN mode */
  LED_Blinking_5s();

  /* Switch OFF LED2 */
  LED_Off();

  /* Enter STOP_LPREGU mode */
  EnterSTOP_LPREGUMode();

  /* Here Device is in STOP_LPREGU mode */

  /* Infinite loop */
  while (1)
  {
  }
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
  * @brief  Turn-off LED2.
  * @param  None
  * @retval None
  */
void LED_Off(void)
{
  /* Turn LED2 off */
  LL_GPIO_ResetOutputPin(LED2_GPIO_PORT, LED2_PIN);
}

/**
  * @brief  Set LED2 to Blinking mode during 5s.
  * @param  None

  * @retval None
  */
void LED_Blinking_5s(void)
{
  uint32_t i = 0;

  /* Toggle IO in during 5s (25*200ms) */
  for(i = 0; i < 25; i++)
  {
    LL_GPIO_TogglePin(LED2_GPIO_PORT, LED2_PIN);  
    LL_mDelay(200);
  }
}

/**
  * @brief  Function to configure and initialize PWR IP.
  * @param  None
  * @retval None
  */
void Configure_PWR(void)
{
  /* Enable Power Clock */
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);

}

/**
  * @brief  Function to configure and enter in STOP_LPREGU Mode.
  * @param  None
  * @retval None
  */
void EnterSTOP_LPREGUMode(void)
{
  LL_GPIO_InitTypeDef gpio_initstruct = {LL_GPIO_PIN_ALL, LL_GPIO_MODE_ANALOG, 
                                         LL_GPIO_SPEED_FREQ_HIGH, LL_GPIO_OUTPUT_PUSHPULL, 
                                         LL_GPIO_PULL_NO, LL_GPIO_AF_0};

  /* Set all GPIO in analog state to reduce power consumption,                */
  /* Note: Debug using ST-Link is not possible during the execution of this   */
  /*       example because communication between ST-link and the device       */
  /*       under test is done through UART. All GPIO pins are disabled (set   */
  /*       to analog input mode) including  UART I/O pins.                    */
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA |
                           LL_AHB1_GRP1_PERIPH_GPIOB |
                           LL_AHB1_GRP1_PERIPH_GPIOC |
                           LL_AHB1_GRP1_PERIPH_GPIOD |
                           LL_AHB1_GRP1_PERIPH_GPIOE |
                           LL_AHB1_GRP1_PERIPH_GPIOF);

  LL_GPIO_Init(GPIOA, &gpio_initstruct);
  LL_GPIO_Init(GPIOB, &gpio_initstruct);
  LL_GPIO_Init(GPIOC, &gpio_initstruct);
  LL_GPIO_Init(GPIOD, &gpio_initstruct);
  LL_GPIO_Init(GPIOE, &gpio_initstruct);
  LL_GPIO_Init(GPIOF, &gpio_initstruct);

  LL_AHB1_GRP1_DisableClock(LL_AHB1_GRP1_PERIPH_GPIOA |
                           LL_AHB1_GRP1_PERIPH_GPIOB |
                           LL_AHB1_GRP1_PERIPH_GPIOC |
                           LL_AHB1_GRP1_PERIPH_GPIOD |
                           LL_AHB1_GRP1_PERIPH_GPIOE |
                           LL_AHB1_GRP1_PERIPH_GPIOF);

  /** Request to enter STOP_LPREGU mode
    * Following procedure describe in STM32F0xx Reference Manual
    * See PWR part, section Low-power modes, STOP_LPREGU mode
    */
  /* Set STOP_LPREGU mode when CPU enters deepsleep */
  LL_PWR_SetPowerMode(LL_PWR_MODE_STOP_LPREGU);

  /* Set SLEEPDEEP bit of Cortex System Control Register */
  LL_LPM_EnableDeepSleep();  

  /* Request Wait For Interrupt */
  __WFI();
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
