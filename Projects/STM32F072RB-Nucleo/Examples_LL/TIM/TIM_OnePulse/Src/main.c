/**
  ******************************************************************************
  * @file    Examples_LL/TIM/TIM_OnePulse/Src/main.c
  * @author  MCD Application Team
  * @brief   This example describes how to use a timer instance in one 
  *          pulse mode through the STM32F0xx TIM LL API.
  *          Peripheral initialization done using LL unitary services functions.
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

/** @addtogroup TIM_OnePulse
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Measured pulse delay (in us) */
__IO uint32_t uwMeasuredDelay = 0;

/* Measured pulse length (in us) */
__IO uint32_t uwMeasuredPulseLength = 0;

/* Private function prototypes -----------------------------------------------*/
__STATIC_INLINE void     SystemClock_Config(void);
__STATIC_INLINE void     ConfigureTIMOnePulse_SwTrigger(void);
__STATIC_INLINE void     ConfigureTIMOnePulse_TI2Trigger(void);
__STATIC_INLINE void     LED_Init(void);
__STATIC_INLINE void     UserButton_Init(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  /* Configure the system clock to 80 MHz */
  SystemClock_Config();

  /* Initialize LED2 */
  LED_Init();

  /* Initialize button in EXTI mode */
  UserButton_Init();
  
  /* Configure timer instance in one pulse mode: timer counter is started by  */
  /* software.                                                                */
  ConfigureTIMOnePulse_SwTrigger();

  /* Configure timer instance in one pulse mode: timer counter is started by  */
  /* rising edge on the TI2 input pin.       */
  ConfigureTIMOnePulse_TI2Trigger();

  /* Infinite loop */
  while (1)
  {
  }
}

/**
  * @brief  This function configures TIM1 to generate a positive pulse on OC1 
  *         with a length of 50 us and after a delay of 50 us after enabling
  *         the timer counter.
  * @note   The counter is enabled every time the user presses the user button.
  * @note   The delay and the pulse length are measured in the CC1 interrupt
  *         service routine.
  * @note   Peripheral configuration is minimal configuration from reset values.
  *         Thus, some useless LL unitary functions calls below are provided as
  *         commented examples - setting is default configuration from reset.
  * @param  None
  * @retval None
  */
__STATIC_INLINE void ConfigureTIMOnePulse_SwTrigger(void)
{  
  /*************************/
  /* GPIO AF configuration */
  /*************************/
  /* Enable the peripheral clock of GPIOs */
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);
  
  /* GPIO TIM1_CH1 configuration */
  LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_8, LL_GPIO_MODE_ALTERNATE);
  LL_GPIO_SetPinPull(GPIOA, LL_GPIO_PIN_8, LL_GPIO_PULL_DOWN);
  LL_GPIO_SetPinSpeed(GPIOA, LL_GPIO_PIN_8, LL_GPIO_SPEED_FREQ_HIGH);
  LL_GPIO_SetAFPin_8_15(GPIOA, LL_GPIO_PIN_8, LL_GPIO_AF_2);

  /***********************************************/
  /* Configure the NVIC to handle TIM1 interrupt */
  /***********************************************/
  NVIC_SetPriority(TIM1_CC_IRQn, 0);
  NVIC_EnableIRQ(TIM1_CC_IRQn);
  
  /******************************/
  /* Peripheral clocks enabling */
  /******************************/
  /* Enable the peripheral clock of TIM1 */
  LL_APB1_GRP2_EnableClock(LL_APB1_GRP2_PERIPH_TIM1);
  
  /*********************************/
  /* Output waveform configuration */
  /*********************************/
  /* Select counter mode: counting up */
  LL_TIM_SetCounterMode(TIM1, LL_TIM_COUNTERMODE_UP);
  
  /* Set the one pulse mode:  generate only 1 pulse*/
  LL_TIM_SetOnePulseMode(TIM1, LL_TIM_ONEPULSEMODE_SINGLE);
  
  /* Set the TIM1 prescaler to get counter clock frequency at 10 MHz          */
  /* In this example TIM21 input clock (TIM1CLK) is set to APB1 clock (PCLK2), */
  /* since APB1 pre-scaler is equal to 1.                                       */
  /*    TIM1CLK = PCLK2                                                        */
  /*    PCLK2 = HCLK                                                            */
  /*    => TIM1CLK = SystemCoreClock (48 MHz)                                  */  
  LL_TIM_SetPrescaler(TIM1, __LL_TIM_CALC_PSC(SystemCoreClock, 10000000));
  
  /* Set the capture/compare register to get a pulse delay of 50 us */
  LL_TIM_OC_SetCompareCH1(TIM1, __LL_TIM_CALC_DELAY(SystemCoreClock, LL_TIM_GetPrescaler(TIM1), 50));
  
  /* Set the autoreload register to get a pulse length of 50s */
  LL_TIM_SetAutoReload(TIM1, __LL_TIM_CALC_PULSE(SystemCoreClock, LL_TIM_GetPrescaler(TIM1), 50, 50));

  /* Set output channel 1 in PWM2 mode */
  LL_TIM_OC_SetMode(TIM1,  LL_TIM_CHANNEL_CH1,  LL_TIM_OCMODE_PWM2);

  /* Configure output channel 1 */
  LL_TIM_OC_ConfigOutput(TIM1, LL_TIM_CHANNEL_CH1, LL_TIM_OCPOLARITY_HIGH | LL_TIM_OCIDLESTATE_LOW);
  
  /**************************/
  /* TIM1 interrupts set-up */
  /**************************/
  /* Enable the capture/compare interrupt for channel 1 */
  LL_TIM_EnableIT_CC1(TIM1);
  
  /**************************/
  /* Start pulse generation */
  /**************************/
  /* Enable channel 1 */
  LL_TIM_CC_EnableChannel(TIM1, LL_TIM_CHANNEL_CH1);
  
  /* Enable TIM1 outputs */
  LL_TIM_EnableAllOutputs(TIM1);
  
  /* Enable auto-reload register preload */
  LL_TIM_EnableARRPreload(TIM1);

  /* Force update generation */
  LL_TIM_GenerateEvent_UPDATE(TIM1);  
}

/**
  * @brief  This function configures TIM2 to generate a positive pulse on OC1 
  *         with a length of 3 s and after a delay of 2 s as soon as a positive
  *         edge is detected on the TI2 input pin.
  * @param  None
  * @retval None
  */
__STATIC_INLINE void ConfigureTIMOnePulse_TI2Trigger(void)
{
  uint32_t TIM2_clk;

  /******************************/
  /* Peripheral clocks enabling */
  /******************************/
  /* Enable the peripheral clock of GPIOs */
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);
  
  /* Enable the peripheral clock of TIM2 */
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM2);

  /*************************/
  /* GPIO AF configuration */
  /*************************/
  /* GPIO TIM2_CH1 configuration */
  LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_5, LL_GPIO_MODE_ALTERNATE);
  LL_GPIO_SetPinPull(GPIOA, LL_GPIO_PIN_5, LL_GPIO_PULL_DOWN);
  LL_GPIO_SetPinSpeed(GPIOA, LL_GPIO_PIN_5, LL_GPIO_SPEED_FREQ_HIGH);
  LL_GPIO_SetAFPin_0_7(GPIOA, LL_GPIO_PIN_5, LL_GPIO_AF_2);

  /* GPIO TIM2_CH2 configuration */
  LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_1, LL_GPIO_MODE_ALTERNATE);
  LL_GPIO_SetPinPull(GPIOA, LL_GPIO_PIN_1, LL_GPIO_PULL_DOWN);
  LL_GPIO_SetPinSpeed(GPIOA, LL_GPIO_PIN_1, LL_GPIO_SPEED_FREQ_HIGH);
  LL_GPIO_SetAFPin_0_7(GPIOA, LL_GPIO_PIN_1, LL_GPIO_AF_2);
  
  /*******************************/
  /* Input trigger configuration */
  /*******************************/
  /* Map TI2FP2 on TI2 */
  LL_TIM_IC_SetActiveInput(TIM2, LL_TIM_CHANNEL_CH2, LL_TIM_ACTIVEINPUT_DIRECTTI);
  
  /* TI2FP2 must detect a rising edge */
  LL_TIM_IC_SetPolarity(TIM2, LL_TIM_CHANNEL_CH2, LL_TIM_IC_POLARITY_RISING);
  
  /* Configure TI2FP2 as trigger */
  LL_TIM_SetTriggerInput(TIM2, LL_TIM_TS_TI2FP2);
  
  /* Enable the slave mode controller: TI2FP2 is used to start the counter */
  LL_TIM_SetSlaveMode(TIM2, LL_TIM_SLAVEMODE_TRIGGER);
  
  /*********************************/
  /* Output waveform configuration */
  /*********************************/
  /* Select counter mode: counting up */
  LL_TIM_SetCounterMode(TIM2, LL_TIM_COUNTERMODE_UP);
  
  /* Set the one pulse mode: generate only 1 pulse */
  LL_TIM_SetOnePulseMode(TIM2, LL_TIM_ONEPULSEMODE_SINGLE);
  
  /* In this example TIM2 input clock (TIM2CLK) is set to APB1 clock (PCLK1), */
  /* since APB1 pre-scaler is equal to /2*2 = 1.                             */
  /*    TIM2CLK = PCLK1                                                     */
  /*    PCLK1 = HCLK                                                          */
  /*    => TIM2CLK = SystemCoreClock (48 MHz)                               */  
  TIM2_clk = SystemCoreClock/1;

  /* Set the TIM2 prescaler to get counter clock frequency at 2 kHz */ 
  LL_TIM_SetPrescaler(TIM2, __LL_TIM_CALC_PSC(TIM2_clk, 2000));
  
  /* Set the capture/compare register to get a pulse delay of 2s (2000000 us)*/
  LL_TIM_OC_SetCompareCH1(TIM2, __LL_TIM_CALC_DELAY(TIM2_clk, LL_TIM_GetPrescaler(TIM2), 2000000));
  
  /* Set the autoreload register to get a pulse length of 3s (3000000 us)*/
  LL_TIM_SetAutoReload(TIM2, __LL_TIM_CALC_PULSE(TIM2_clk, LL_TIM_GetPrescaler(TIM2), 2000000, 3000000));
  
  /* Set output channel 1 in PWM2 mode */
  LL_TIM_OC_SetMode(TIM2,  LL_TIM_CHANNEL_CH1,  LL_TIM_OCMODE_PWM2);
  
  /* Configure output channel 1 configuration */
  LL_TIM_OC_ConfigOutput(TIM2, LL_TIM_CHANNEL_CH1, LL_TIM_OCPOLARITY_HIGH | LL_TIM_OCIDLESTATE_LOW);
  
  /**************************/
  /* Start pulse generation */
  /**************************/
  /* Enable channel 1 */
  LL_TIM_CC_EnableChannel(TIM2, LL_TIM_CHANNEL_CH1);
  
  /* Enable auto-reload register preload */
  LL_TIM_EnableARRPreload(TIM2);

  /* Force update generation */
  LL_TIM_GenerateEvent_UPDATE(TIM2);
}

/**
  * @brief  Initialize LED2.
  * @param  None
  * @retval None
  */
__STATIC_INLINE void LED_Init(void)
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
__STATIC_INLINE void UserButton_Init(void)
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
  * @brief  User button interrupt processing
  * @note   TIM1 counter is enabled every time the user button is presssed. 
  * @param  None
  * @retval None
  */
void UserButton_Callback(void)
{
  /* Enable counter. Note that the counter will stop automatically at the     */
  /* next update event (UEV).                                                 */
  LL_TIM_EnableCounter(TIM1);
}

/**
  * @brief  Timer capture/compare interrupt processing
  * @note   Calculates the pulse delay and pulse length of the output waveform
  *         generated by TIM1.
  * @param  None
  * @retval None
  */
void TimerCaptureCompare_Callback(void)
{
  uint32_t CNT;
  uint32_t PSC; 
  uint32_t ARR; 
  
  CNT = LL_TIM_GetCounter(TIM1);
  PSC = LL_TIM_GetPrescaler(TIM1);
  ARR = LL_TIM_GetAutoReload(TIM1);
  
  uwMeasuredDelay = (CNT * 1000000)/(SystemCoreClock/(PSC + 1));
  uwMeasuredPulseLength = ((ARR - CNT) * 1000000)/(SystemCoreClock/(PSC + 1)); 
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
