/**
  ******************************************************************************
  * @file    Examples_LL/I2C/I2C_TwoBoards_WakeUpFromStop_IT/Src/main.c
  * @author  MCD Application Team
  * @brief   This example describes how to send/receive bytes over I2C IP using
  *          the STM32F0xx I2C LL API.
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

/** @addtogroup I2C_TwoBoards_WakeUpFromStop_IT
  * @{
  */

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
/**
  * @brief I2C devices settings
  */
/* Timing register value is computed with the STM32CubeMX Tool,
  * Standard Mode @100kHz with I2CCLK = 8 MHz,
  * rise time = 50ns, fall time = 10ns
  * Timing Value = (uint32_t)0x00201D2C
  */
#define I2C_TIMING               0x00201D2C

/**
  * @brief Define related to SlaveTransmit process
  */
#define SLAVE_BYTE_TO_SEND       0xA5

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
#ifndef SLAVE_BOARD
/**
  * @brief Variables related to MasterReceive process
  */
uint8_t      aReceiveBuffer[0xF] = {0};
__IO uint8_t ubReceiveIndex      = 0;
__IO uint8_t ubButtonPress       = 0;
#endif /* SLAVE_BOARD */

/* Private function prototypes -----------------------------------------------*/
void     SystemClock_Config(void);
void     LED_Init(void);
void     LED_On(void);
void     LED_Off(void);
void     LED_Blinking(uint32_t Period);
#ifdef SLAVE_BOARD
void     Configure_I2C_Slave(void);
void     Configure_PWR(void);
void     EnterSTOP_LPREGUMode(void);
#else /* MASTER_BOARD */
void     Configure_I2C_Master(void);
void     UserButton_Init(void);
void     WaitForUserButtonPress(void);
void     Handle_I2C_Master(void);
#endif /* SLAVE_BOARD */

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  /* Configure the system clock to 8 MHz */
  SystemClock_Config();

  /* Initialize LED2 */
  LED_Init();

  /* Set LED2 Off */
  LED_Off();

#ifdef SLAVE_BOARD
  /* Configure Power IP */
  Configure_PWR();

  /* Configure I2C1 (I2C IP configuration in Slave mode and related GPIO initialization) */
  Configure_I2C_Slave();

  /* Enter Stop mode */
  EnterSTOP_LPREGUMode();
#else /* MASTER_BOARD */
  /* Initialize User push-button in EXTI mode */
  UserButton_Init();

  /* Configure I2C1 (I2C IP configuration in Master mode and related GPIO initialization) */
  Configure_I2C_Master();

  /* Wait for User push-button press to start transfer */
  WaitForUserButtonPress();

  /* Handle I2C1 events (Master) */
  Handle_I2C_Master();
#endif /* SLAVE_BOARD */
  
  /* Infinite loop */
  while (1)
  {
  }
}

#ifdef SLAVE_BOARD
/**
  * @brief  This function configures I2C1 in Slave mode.
  * @note   This function is used to :
  *         -1- Enables GPIO clock and configures the I2C1 pins.
  *         -2- Enable the I2C1 peripheral clock and I2C1 clock source.
  *         -3- Configure NVIC for I2C1.
  *         -4- Configure I2C1 functional parameters.
  *         -5- Enable I2C1.
  *         -6- Enable I2C1 address match/error interrupts.
  * @note   Peripheral configuration is minimal configuration from reset values.
  *         Thus, some useless LL unitary functions calls below are provided as
  *         commented examples - setting is default configuration from reset.
  * @param  None
  * @retval None
  */
void Configure_I2C_Slave(void)
{
  uint32_t timing = 0;

  /* (1) Enables GPIO clock and configures the I2C1 pins **********************/
  /*    (SCL on PB.6, SDA on PB.7)                     **********************/

  /* Enable the peripheral clock of GPIOB */
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB);

  /* Configure SCL Pin as : Alternate function, High Speed, Open drain, Pull up */
  LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_6, LL_GPIO_MODE_ALTERNATE);
  LL_GPIO_SetAFPin_0_7(GPIOB, LL_GPIO_PIN_6, LL_GPIO_AF_1);
  LL_GPIO_SetPinSpeed(GPIOB, LL_GPIO_PIN_6, LL_GPIO_SPEED_FREQ_HIGH);
  LL_GPIO_SetPinOutputType(GPIOB, LL_GPIO_PIN_6, LL_GPIO_OUTPUT_OPENDRAIN);
  LL_GPIO_SetPinPull(GPIOB, LL_GPIO_PIN_6, LL_GPIO_PULL_UP);

  /* Configure SDA Pin as : Alternate function, High Speed, Open drain, Pull up */
  LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_7, LL_GPIO_MODE_ALTERNATE);
  LL_GPIO_SetAFPin_0_7(GPIOB, LL_GPIO_PIN_7, LL_GPIO_AF_1);
  LL_GPIO_SetPinSpeed(GPIOB, LL_GPIO_PIN_7, LL_GPIO_SPEED_FREQ_HIGH);
  LL_GPIO_SetPinOutputType(GPIOB, LL_GPIO_PIN_7, LL_GPIO_OUTPUT_OPENDRAIN);
  LL_GPIO_SetPinPull(GPIOB, LL_GPIO_PIN_7, LL_GPIO_PULL_UP);

  /* (2) Enable the I2C1 peripheral clock and I2C1 clock source ***************/

  /* Enable the peripheral clock for I2C1 */
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_I2C1);

  /* Set I2C1 clock source as SYSCLK */
  LL_RCC_SetI2CClockSource(LL_RCC_I2C1_CLKSOURCE_HSI);

  /* (3) Configure NVIC for I2C1 **********************************************/

  /* Configure Event and Error IT:
   *  - Set priority for I2C1_IRQn
   *  - Enable I2C1_IRQn
   */
  NVIC_SetPriority(I2C1_IRQn, 0);
  NVIC_EnableIRQ(I2C1_IRQn);

  /* (4) Configure I2C1 functional parameters *********************************/

  /* Disable I2C1 prior modifying configuration registers */
  LL_I2C_Disable(I2C1);

  /* Configure the SDA setup, hold time and the SCL high, low period */
  /* Timing register value is computed with the STM32CubeMX Tool,
    * Standard Mode @100kHz with I2CCLK = 8 MHz,
    * rise time = 50ns, fall time = 10ns
    * Timing Value = (uint32_t)0x00201D2C
    */
  timing = __LL_I2C_CONVERT_TIMINGS(0x0, 0x2, 0x0, 0x1D, 0x2C);
  LL_I2C_SetTiming(I2C1, timing);

  /* Configure the Own Address1 :
   *  - OwnAddress1 is SLAVE_OWN_ADDRESS
   *  - OwnAddrSize is LL_I2C_OWNADDRESS1_7BIT
   *  - Own Address1 is enabled
   */
  LL_I2C_SetOwnAddress1(I2C1, SLAVE_OWN_ADDRESS, LL_I2C_OWNADDRESS1_7BIT);
  LL_I2C_EnableOwnAddress1(I2C1);

  /* Enable Wake Up From Stop */
  LL_I2C_EnableWakeUpFromStop(I2C1);

  /* Enable Clock stretching */
  /* Reset Value is Clock stretching enabled */
  //LL_I2C_EnableClockStretching(I2C1);

  /* Configure Digital Noise Filter */
  /* Reset Value is 0x00            */
  //LL_I2C_SetDigitalFilter(I2C1, 0x00);

  /* Enable Analog Noise Filter           */
  /* Reset Value is Analog Filter enabled */
  //LL_I2C_EnableAnalogFilter(I2C1);

  /* Enable General Call                  */
  /* Reset Value is General Call disabled */
  //LL_I2C_EnableGeneralCall(I2C1);

  /* Configure the 7bits Own Address2               */
  /* Reset Values of :
   *     - OwnAddress2 is 0x00
   *     - OwnAddrMask is LL_I2C_OWNADDRESS2_NOMASK
   *     - Own Address2 is disabled
   */
  //LL_I2C_SetOwnAddress2(I2C1, 0x00, LL_I2C_OWNADDRESS2_NOMASK);
  //LL_I2C_DisableOwnAddress2(I2C1);

  /* Enable Peripheral in I2C mode */
  /* Reset Value is I2C mode */
  //LL_I2C_SetMode(I2C1, LL_I2C_MODE_I2C);

  /* (5) Enable I2C1 **********************************************************/
  LL_I2C_Enable(I2C1);

  /* (6) Enable I2C1 address match/error interrupts:
   *  - Enable Address Match Interrupt
   *  - Enable Not acknowledge received interrupt
   *  - Enable Error interrupts
   *  - Enable Stop interrupt
   */
  LL_I2C_EnableIT_ADDR(I2C1);
  LL_I2C_EnableIT_NACK(I2C1);
  LL_I2C_EnableIT_ERR(I2C1);
  LL_I2C_EnableIT_STOP(I2C1);
}

#else /* MASTER_BOARD */

/**
  * @brief  This function configures I2C1 in Master mode.
  * @note   This function is used to :
  *         -1- Enables GPIO clock and configures the I2C1 pins.
  *         -2- Enable the I2C1 peripheral clock and I2C1 clock source.
  *         -3- Configure NVIC for I2C1.
  *         -4- Configure I2C1 functional parameters.
  *         -5- Enable I2C1.
  *         -6- Enable I2C1 transfer complete/error interrupts.
  * @note   Peripheral configuration is minimal configuration from reset values.
  *         Thus, some useless LL unitary functions calls below are provided as
  *         commented examples - setting is default configuration from reset.
  * @param  None
  * @retval None
  */
void Configure_I2C_Master(void)
{

  /* (1) Enables GPIO clock and configures the I2C1 pins **********************/
  /*    (SCL on PB.6, SDA on PB.7)                     **********************/

  /* Enable the peripheral clock of GPIOB */
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB);

  /* Configure SCL Pin as : Alternate function, High Speed, Open drain, Pull up */
  LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_6, LL_GPIO_MODE_ALTERNATE);
  LL_GPIO_SetAFPin_0_7(GPIOB, LL_GPIO_PIN_6, LL_GPIO_AF_1);
  LL_GPIO_SetPinSpeed(GPIOB, LL_GPIO_PIN_6, LL_GPIO_SPEED_FREQ_HIGH);
  LL_GPIO_SetPinOutputType(GPIOB, LL_GPIO_PIN_6, LL_GPIO_OUTPUT_OPENDRAIN);
  LL_GPIO_SetPinPull(GPIOB, LL_GPIO_PIN_6, LL_GPIO_PULL_UP);

  /* Configure SDA Pin as : Alternate function, High Speed, Open drain, Pull up */
  LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_7, LL_GPIO_MODE_ALTERNATE);
  LL_GPIO_SetAFPin_0_7(GPIOB, LL_GPIO_PIN_7, LL_GPIO_AF_1);
  LL_GPIO_SetPinSpeed(GPIOB, LL_GPIO_PIN_7, LL_GPIO_SPEED_FREQ_HIGH);
  LL_GPIO_SetPinOutputType(GPIOB, LL_GPIO_PIN_7, LL_GPIO_OUTPUT_OPENDRAIN);
  LL_GPIO_SetPinPull(GPIOB, LL_GPIO_PIN_7, LL_GPIO_PULL_UP);

  /* (2) Enable the I2C1 peripheral clock and I2C1 clock source ***************/

  /* Enable the peripheral clock for I2C1 */
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_I2C1);

  /* Set I2C1 clock source as SYSCLK */
  LL_RCC_SetI2CClockSource(LL_RCC_I2C1_CLKSOURCE_HSI);

  /* (3) Configure NVIC for I2C1 **********************************************/

  /* Configure Event and Error IT:
   *  - Set priority for I2C1_IRQn
   *  - Enable I2C1_IRQn
   */
  NVIC_SetPriority(I2C1_IRQn, 0);
  NVIC_EnableIRQ(I2C1_IRQn);

  /* (4) Configure I2C1 functional parameters ********************************/

  /* Disable I2C1 prior modifying configuration registers */
  LL_I2C_Disable(I2C1);

  /* Configure the SDA setup, hold time and the SCL high, low period */
  /* (uint32_t)0x00201D2C = I2C_TIMING*/
  LL_I2C_SetTiming(I2C1, I2C_TIMING);

  /* Configure the Own Address1                   */
  /* Reset Values of :
   *     - OwnAddress1 is 0x00
   *     - OwnAddrSize is LL_I2C_OWNADDRESS1_7BIT
   *     - Own Address1 is disabled
   */
  //LL_I2C_SetOwnAddress1(I2C1, 0x00, LL_I2C_OWNADDRESS1_7BIT);
  //LL_I2C_DisableOwnAddress1(I2C1);

  /* Enable Clock stretching */
  /* Reset Value is Clock stretching enabled */
  //LL_I2C_EnableClockStretching(I2C1);

  /* Configure Digital Noise Filter */
  /* Reset Value is 0x00            */
  //LL_I2C_SetDigitalFilter(I2C1, 0x00);

  /* Enable Analog Noise Filter           */
  /* Reset Value is Analog Filter enabled */
  //LL_I2C_EnableAnalogFilter(I2C1);

  /* Enable General Call                  */
  /* Reset Value is General Call disabled */
  //LL_I2C_EnableGeneralCall(I2C1);

  /* Configure the 7bits Own Address2               */
  /* Reset Values of :
   *     - OwnAddress2 is 0x00
   *     - OwnAddrMask is LL_I2C_OWNADDRESS2_NOMASK
   *     - Own Address2 is disabled
   */
  //LL_I2C_SetOwnAddress2(I2C1, 0x00, LL_I2C_OWNADDRESS2_NOMASK);
  //LL_I2C_DisableOwnAddress2(I2C1);

  /* Configure the Master to operate in 7-bit or 10-bit addressing mode */
  /* Reset Value is LL_I2C_ADDRESSING_MODE_7BIT                         */
  //LL_I2C_SetMasterAddressingMode(I2C1, LL_I2C_ADDRESSING_MODE_7BIT);

  /* Enable Peripheral in I2C mode */
  /* Reset Value is I2C mode */
  //LL_I2C_SetMode(I2C1, LL_I2C_MODE_I2C);

  /* (5) Enable I2C1 **********************************************************/
  LL_I2C_Enable(I2C1);

  /* (6) Enable I2C1 transfer complete/error interrupts:
   *  - Enable Receive Interrupt
   *  - Enable Not acknowledge received interrupt
   *  - Enable Error interrupts
   *  - Enable Stop interrupt
   */
  LL_I2C_EnableIT_RX(I2C1);
  LL_I2C_EnableIT_NACK(I2C1);
  LL_I2C_EnableIT_ERR(I2C1);
  LL_I2C_EnableIT_STOP(I2C1);
}
#endif /* SLAVE_BOARD */

#ifdef SLAVE_BOARD
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
  * @brief  Function to configure and enter in Stop Mode.
  * @param  None
  * @retval None
  */
void EnterSTOP_LPREGUMode(void)
{
    /** Request to enter Stop mode:
      *  - Following procedure describe in STM32F0xx Reference Manual
      *  - See PWR part, section Low-power modes, STOP_LPREGU mode
      */
    /* Set STOP_LPREGU mode when CPU enters deepsleep */
    LL_PWR_SetPowerMode(LL_PWR_MODE_STOP_LPREGU);

    /* Set SLEEPDEEP bit of Cortex System Control Register */
    LL_LPM_EnableDeepSleep();  

    /* Request Wait For Interrupt */
    __WFI();
}
#endif /* SLAVE_BOARD */
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
  /* Turn LED2 on */
  LL_GPIO_SetOutputPin(LED2_GPIO_PORT, LED2_PIN);

  /* Toggle IO in an infinite loop */
  while (1)
  {
    LL_GPIO_TogglePin(LED2_GPIO_PORT, LED2_PIN);  
    LL_mDelay(Period);
  }
}

#ifndef SLAVE_BOARD
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

  /* Enable a rising trigger External line 13 Interrupt */
  USER_BUTTON_EXTI_LINE_ENABLE();
  USER_BUTTON_EXTI_FALLING_TRIG_ENABLE();

  /* Configure NVIC for USER_BUTTON_EXTI_IRQn */
  NVIC_EnableIRQ(USER_BUTTON_EXTI_IRQn); 
  NVIC_SetPriority(USER_BUTTON_EXTI_IRQn,0x03);  
}
#endif /* SLAVE_BOARD */

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow :
  *            System Clock source            = PLL (HSI)
  *            SYSCLK(Hz)                     = 8000000
  *            HCLK(Hz)                       = 8000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 1
  *            HSI Frequency(Hz)              = 8000000
  *            Flash Latency(WS)              = 0
  * @param  None
  * @retval None
  */
void SystemClock_Config(void)
{
  /* HSI configuration and activation */
  /* Reset Value is HSI enabled */
//  LL_RCC_HSI_Enable();
//  while(LL_RCC_HSI_IsReady() != 1) 
//  {
//  };
  
  /* Sysclk activation on the HSI */
  /* Reset Value is Sysclk activated on the HSI */
//  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_HSI);
//  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_HSI) 
//  {
//  };
  
  /* Set AHB & APB1 prescaler */
  /* Reset Value is AHB & APB1 prescaler DIV1 */
//  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
//  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);

  /* Set systick to 1ms in using frequency set to 8MHz */
  LL_Init1msTick(8000000);
  
  /* Update CMSIS variable (which can be updated also through SystemCoreClockUpdate function) */
  /* Reset Value is SystemCoreClock at 8Mhz */
//  LL_SetSystemCoreClock(8000000);
}
/******************************************************************************/
/*   IRQ HANDLER TREATMENT Functions                                          */
/******************************************************************************/
#ifdef SLAVE_BOARD
/**
  * @brief  Function called from I2C IRQ Handler when TXIS flag is set
  *         Function is in charge of transmit a byte on I2C lines.
  * @param  None
  * @retval None
  */
void Slave_Ready_To_Transmit_Callback(void)
{
  /* Send the Byte requested by the Master */
  LL_I2C_TransmitData8(I2C1, SLAVE_BYTE_TO_SEND);
}

/**
  * @brief  Function called from I2C IRQ Handler when STOP flag is set
  *         LED2 is On if data are correct.
  * @param  None
  * @retval None
  */
void Slave_Complete_Callback(void)
{
  /* Turn LED2 On:
   *  - Expected bytes have been sent
   *  - Slave Tx sequence completed successfully
   */
  LED_On();
}
#else /* MASTER_BOARD */
/**
  * @brief  Wait for User push-button press to start transfer.
  * @param  None 
  * @retval None
  */
  /*  */
void WaitForUserButtonPress(void)
{
  while (ubButtonPress == 0)
  {
    LL_GPIO_TogglePin(LED2_GPIO_PORT, LED2_PIN);
    LL_mDelay(LED_BLINK_FAST);
  }
  /* Turn LED2 off */
  LL_GPIO_ResetOutputPin(LED2_GPIO_PORT, LED2_PIN);
}

/**
  * @brief  Function to manage User push-button
  * @param  None
  * @retval None
  */
void UserButton_Callback(void)
{
  /* Update User push-button variable : to be checked in waiting loop in main program */
  ubButtonPress = 1;
}

/**
  * @brief  This Function handle Master events to perform a transmission process
  * @note  This function is composed in one step :
  *        -1- Initiate a Start condition to the Slave device
  * @param  None
  * @retval None
  */
void Handle_I2C_Master(void)
{
  /* (1) Initiate a Start condition to the Slave device ***********************/

  /* Master Generate Start condition for a read request:
   *  - to the Slave with a 7-Bit SLAVE_OWN_ADDRESS
   *  - with a auto stop condition generation when receive 1 byte
   */
  LL_I2C_HandleTransfer(I2C1, SLAVE_OWN_ADDRESS, LL_I2C_ADDRSLAVE_7BIT, 1, LL_I2C_MODE_AUTOEND, LL_I2C_GENERATE_START_READ);
}

/**
  * @brief  Function called from I2C IRQ Handler when RXNE flag is set
  *         Function is in charge of reading byte received on I2C lines.
  * @param  None
  * @retval None
  */
void Master_Reception_Callback(void)
{
  /* Read character in Receive Data register.
  RXNE flag is cleared by reading data in RXDR register */
  aReceiveBuffer[ubReceiveIndex++] = LL_I2C_ReceiveData8(I2C1);
}

/**
  * @brief  Function called from I2C IRQ Handler when STOP flag is set
  *         Function is in charge of checking data received,
  *         LED2 is On if data are correct.
  * @param  None
  * @retval None
  */
void Master_Complete_Callback(void)
{
  /* Read Received character. RXNE flag is cleared by reading of RXDR register */
  if(aReceiveBuffer[ubReceiveIndex-1] == SLAVE_BYTE_TO_SEND)
  {
    /* Turn LED2 On:
     *  - Expected byte has been received
     *  - Master Rx sequence completed successfully
     */
    LED_On();
  }
  else
  {
    /* Call Error function */
    Error_Callback();
  }
}
#endif /* SLAVE_BOARD */

/**
  * @brief  Function called in case of error detected in I2C IT Handler
  * @param  None
  * @retval None
  */
void Error_Callback(void)
{
#ifdef SLAVE_BOARD
  /* Disable I2C1_IRQn */
  NVIC_DisableIRQ(I2C1_IRQn);
#else
  /* Disable I2C1_IRQn */
  NVIC_DisableIRQ(I2C1_IRQn);
#endif /* SLAVE_BOARD */

  /* Unexpected event : Set LED2 to Blinking mode to indicate error occurs */
  LED_Blinking(LED_BLINK_ERROR);
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
