/**
  ******************************************************************************
  * @file    Demonstrations/Src/main.c 
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
#include "main.h"

/** @addtogroup STM32F0xx_HAL_Demonstrations
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
#define ABS(x)         (x < 0) ? (-x) : x

/* Private variables ---------------------------------------------------------*/
__IO uint32_t ButtonPressed = 0;
extern __IO uint32_t Gv_EOA;
USBD_HandleTypeDef USBD_Device;
TSC_HandleTypeDef TscHandle;
TSC_IOConfigTypeDef IoConfig;

/* Private function prototypes -----------------------------------------------*/
static void Demo(void);
static void LED_Test(void);
static void MEMS_Test(void);

static void USB_Demo(void);
static void USB_Test(void);
static void USB_GetPointerData_Test(uint8_t *pbuf);
static void USB_GetPointerData_Demo(uint8_t *pbuf);
static void Demo_GyroReadAngRate (float* pfData);

static void TSL_Test(void);

static void ProcessSensors(void);

static void SystemClock_Config(void);
static void Error_Handler(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
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

  /* Initialize LEDs, User_Button on STM32F052B-Discovery board ***********/
  BSP_PB_Init(BUTTON_USER, BUTTON_MODE_EXTI);
  
  BSP_LED_Init(LED3);
  BSP_LED_Init(LED4);
  BSP_LED_Init(LED5);
  BSP_LED_Init(LED6);

  /* Configure the system clock to have a system clock = 48 Mhz */
  SystemClock_Config();

  /* Init USB Device Library */
  USBD_Init(&USBD_Device, &HID_Desc, 0);
  
  /* Register the USB HID class */
  USBD_RegisterClass(&USBD_Device, &USBD_HID);
  
  /* Start Device Process */
  USBD_Start(&USBD_Device);
  
  /* Delay 1s to select Test Program or to go directly througt the demo*/
  HAL_Delay(1000);

  if(BSP_PB_GetState(BUTTON_USER) == GPIO_PIN_SET)
  {
    /* Wait for User button is released */
    while(BSP_PB_GetState(BUTTON_USER) != GPIO_PIN_RESET)
    {
    }
    
    /* Set ButtonPressed at 0 value  */ 
    ButtonPressed = 0;
    
    /* LED test : Blinking LEDs  */ 
    LED_Test();
    
    /* Wait for User button to be pressed to switch to USB Test
    the cursor move in square path and led On corresponding to such direction  */
    USB_Test();

    /* Move Discovery board to execute MEMS Test, Mems detect the angular rate
    and led On corresponding to such direction*/ 
    MEMS_Test();

    /* Wait for User button to be pressed to switch to Touch Sensor Test
    each bank pointed correspond to specific Leds On, test can performed
    in both direction */ 
    TSL_Test();
  }
  else
  {
    Demo();
  }
  
  /* Infinite loop */
  while (1)
  {
    Demo();
  }
}

/**
* @brief  Demo.
* @param  None
* @retval None
*/
static void Demo(void)
{
  /*  Blinking LEDs & Wait for User button to be pressed to switch to MEMES demo */
  while(BSP_PB_GetState(BUTTON_USER) != GPIO_PIN_SET)
  {
    /* Toggle LED3, LED4, LED5 & LED6 */
    BSP_LED_Toggle(LED3);
    BSP_LED_Toggle(LED4);
    BSP_LED_Toggle(LED5);
    BSP_LED_Toggle(LED6);

    /* Insert 100ms delay */
    HAL_Delay(100);   
  }

  /* Wait for User button is released */
  while(BSP_PB_GetState(BUTTON_USER) != GPIO_PIN_RESET)
  {
  }
  
  /* Turn Off Leds */   
  BSP_LED_Off(LED3);
  BSP_LED_Off(LED4);
  BSP_LED_Off(LED5);
  BSP_LED_Off(LED6);

  /* Move Discovery board to execute MEMS demo, Mems detect the angular rate
  and led On corresponding to such direction*/ 
  MEMS_Test();
  
  /* Wait for User button to be pressed to switch to USB demo
  Mouse cursor moving corresponding to board move direction  */
  USB_Demo();
  
  /* Wait for User button to be pressed to switch to Touch Sensor Test
  each bank pointed correspond to specific Leds On, test can performed
  in both direction */ 
  TSL_Test();
}

/**
* @brief  LED Test.
* @param  None
* @retval None
*/
static void LED_Test(void)
{
  while(ButtonPressed != 1)
  {
    /* Toggle LED3 */
    BSP_LED_Toggle(LED3);

    /* Insert 50ms delay */
    HAL_Delay(50);
    
    /* Toggle LED5 */
    BSP_LED_Toggle(LED5);

    /* Insert 50ms delay */
    HAL_Delay(50);

    if(ButtonPressed == 1)
    {
      BSP_LED_Off(LED3);
      BSP_LED_Off(LED4);
      BSP_LED_Off(LED5);
      BSP_LED_Off(LED6);
    }
    else
    {
      /* Toggle LED6 */
      BSP_LED_Toggle(LED6);

      /* Insert 50ms delay */
      HAL_Delay(50);

      /* Toggle LED4 */
      BSP_LED_Toggle(LED4);

      /* Insert 50ms delay */
      HAL_Delay(50);
    }
  }

  /* Wait for User button is released */
  while(BSP_PB_GetState(BUTTON_USER) != GPIO_PIN_RESET)
  {
  }

  /* Turn Off Leds */   
  BSP_LED_Off(LED3);
  BSP_LED_Off(LED4);
  BSP_LED_Off(LED5);
  BSP_LED_Off(LED6);
}

/**
* @brief  MEMS Test.
* @param  None
* @retval None
*/
static void MEMS_Test(void)
{
  float Buffer[6] = {0};
  uint8_t Xval, Yval = 0;
  
  /* Demo Gyroscope */
  if(BSP_GYRO_Init() != GYRO_OK)
  {
    Error_Handler();
  }

  while(BSP_PB_GetState(BUTTON_USER) != GPIO_PIN_SET)
  {
    /* Read Gyro Angular data */
    BSP_GYRO_GetXYZ(Buffer);
    
    /* Update autoreload and capture compare registers value */
    Xval = ABS((int8_t)(Buffer[0]));
    Yval = ABS((int8_t)(Buffer[1]));
    
    if(Xval > Yval)
    {
      if(Buffer[0] > 5000.0f)
      {
        /*  LED5 On */   
        BSP_LED_Off(LED3);
        BSP_LED_Off(LED4);
        BSP_LED_On(LED5);
        BSP_LED_Off(LED6);

        /* Insert 250ms delay */ 
        HAL_Delay(250);
      }
      
      if(Buffer[0] < -5000.0f)
      {
        /*  LED4 On */   
        BSP_LED_Off(LED3);
        BSP_LED_On(LED4);
        BSP_LED_Off(LED5);
        BSP_LED_Off(LED6);

        /* Insert 250ms delay */ 
        HAL_Delay(250);
      }
    }
    else
    {
      if(Buffer[1] > 5000.0f)
      {
        /*  LED3 On */   
        BSP_LED_On(LED3);
        BSP_LED_Off(LED4);
        BSP_LED_Off(LED5);
        BSP_LED_Off(LED6);

        /* Insert 250ms delay */ 
        HAL_Delay(250);
      }
      
      if(Buffer[1] < -5000.0f)
      {
        /*  LED6 On */   
        BSP_LED_Off(LED3);
        BSP_LED_Off(LED4);
        BSP_LED_Off(LED5);
        BSP_LED_On(LED6);

        /* Insert 250ms delay */ 
        HAL_Delay(250);
      }
    }
  }

  /* Wait for User button is released */
  while(BSP_PB_GetState(BUTTON_USER) != GPIO_PIN_RESET)
  {
  }

  /* Turn Off Leds */   
  BSP_LED_Off(LED3);
  BSP_LED_Off(LED4);
  BSP_LED_Off(LED5);
  BSP_LED_Off(LED6);
}

/**
* @brief  USB Mouse cursor moving
* @param  None 
* @retval None
*/
static void USB_Demo(void)
{
  uint8_t HID_Buffer[4];
  
  BSP_LED_On(LED3);
  BSP_LED_Off(LED6);
  
  while ((BSP_PB_GetState(BUTTON_USER) != GPIO_PIN_SET))
  {
    USB_GetPointerData_Demo(HID_Buffer);
    
    /* send data though IN endpoint*/
    if((HID_Buffer[1] != 0) || (HID_Buffer[2] != 0))
    {
      USBD_HID_SendReport(&USBD_Device, HID_Buffer, 4);
    }
  }
  
  /* Wait for User button is released */ 
  while (BSP_PB_GetState(BUTTON_USER) != GPIO_PIN_RESET)
  {} 
  /* Turn Off Leds */   
  BSP_LED_Off(LED3);
  BSP_LED_Off(LED4);
  BSP_LED_Off(LED5);
  BSP_LED_Off(LED6);
}

/**
  * @brief  Calculate the angular Data rate Gyroscope.
  * @param  pfData : Data out pointer
  * @retval None
  */
static void Demo_GyroReadAngRate (float* pfData)
{
  uint8_t tmpbuffer[6] ={0};
  int16_t RawData[3] = {0};
  uint8_t tmpreg = 0;
  float sensitivity = 0;
  int i =0;
  
  GYRO_IO_Read(&tmpreg,L3GD20_CTRL_REG4_ADDR,1);
  
  GYRO_IO_Read(tmpbuffer,L3GD20_OUT_X_L_ADDR,6);
  
  /* check in the control register 4 the data alignment (Big Endian or Little Endian)*/
  if(!(tmpreg & L3GD20_BLE_MSB))
  {
    for(i=0; i<3; i++)
    {
      RawData[i]=(int16_t)(((uint16_t)tmpbuffer[2*i+1] << 8) + tmpbuffer[2*i]);
    }
  }
  else
  {
    for(i=0; i<3; i++)
    {
      RawData[i]=(int16_t)(((uint16_t)tmpbuffer[2*i] << 8) + tmpbuffer[2*i+1]);
    }
  }
  
  /* Switch the sensitivity value set in the CRTL4 */
  switch(tmpreg & L3GD20_FULLSCALE_SELECTION)
  {
  case L3GD20_FULLSCALE_250:
    sensitivity=L3GD20_SENSITIVITY_250DPS;
    break;
    
  case L3GD20_FULLSCALE_500:
    sensitivity=L3GD20_SENSITIVITY_500DPS;
    break;
    
  case L3GD20_FULLSCALE_2000:
    sensitivity=L3GD20_SENSITIVITY_2000DPS;
    break;
    
    default:
      sensitivity=L3GD20_SENSITIVITY_250DPS;
  }
  /* divide by sensitivity */
  for(i=0; i<3; i++)
  {
    pfData[i]=(float)(RawData[i] / sensitivity);
  }
}

/**
  * @brief  USB Test : Configure the USB 
  * @param  None 
  * @retval None
  */
static void USB_Test(void)
{
  uint8_t HID_Buffer[4];
  
  while ((BSP_PB_GetState(BUTTON_USER) != GPIO_PIN_SET))
  {
    USB_GetPointerData_Test(HID_Buffer);
    
    /* send data though IN endpoint*/
    if((HID_Buffer[1] != 0) || (HID_Buffer[2] != 0))
    {
      USBD_HID_SendReport(&USBD_Device, HID_Buffer, 4);
      HAL_Delay (50);
    }
  }
  
  /* Wait for User button is released */ 
  while (BSP_PB_GetState(BUTTON_USER) != GPIO_PIN_RESET)
  {} 
  /* Turn Off Leds */   
  BSP_LED_Off(LED3);
  BSP_LED_Off(LED4);
  BSP_LED_Off(LED5);
  BSP_LED_Off(LED6);
}

/**
  * @brief  USBD_HID_GetPos
  * @param  None
  * @retval Pointer to report
*/
static void USB_GetPointerData_Test(uint8_t *pbuf)
{ 
  static int8_t x = 0;
  static int8_t y = 0;
  static int8_t Sens = 0;
  static int8_t Pas = 0;
  
  if (Pas == 20)
  {
    Pas=0;
    Sens++;
  }
  
  if(Sens == 0)
  {
    x=Pas++;
    y=0;
    BSP_LED_Toggle(LED5);
  }      
  if(Sens == 1)
  {
    y=Pas++;
    x=0;
    BSP_LED_Toggle(LED6);
  }      
  if (Sens == 2)
  {
    x=256-Pas++;
    y=0;
    BSP_LED_Toggle(LED4);
  }      
  if (Sens == 3)
  {
    y=256-Pas++;
    x=0;
    BSP_LED_Toggle(LED3);
  }      
  if (Sens == 4)
  { 
    Sens=0;
    x=0;
    y=0;
  }
  
  pbuf[0] = 0;
  pbuf[1] = x;
  pbuf[2] = y;
  pbuf[3] = 0;
  
}

/**
  * @brief  USBD_HID_GetPos
  * @param  None
  * @retval Pointer to report
  */
static void USB_GetPointerData_Demo(uint8_t *pbuf)
{ 
  static float Buffer[6] = {0};
  
  BSP_GYRO_Init();
  
  /* Read Gyro Angular data */
  Demo_GyroReadAngRate(Buffer); 
  
  pbuf[0] = 0;
  pbuf[1] = -(int8_t)(Buffer[2])/6;
  pbuf[2] = (int8_t)(Buffer[1])/6;
  pbuf[3] = 0;
  
  BSP_LED_Toggle(LED3);
  BSP_LED_Toggle(LED6);
  
}

/**
* @brief  TS Test.
* @param  None 
* @retval None
*/
static void TSL_Test(void)
{
  /* Configure the TSC peripheral */
  TscHandle.Instance = TSCx;

  if (HAL_TSC_DeInit(&TscHandle) != HAL_OK)
  {
    Error_Handler();
  }

  TscHandle.Init.AcquisitionMode         = TSC_ACQ_MODE_NORMAL;
  TscHandle.Init.CTPulseHighLength       = TSC_CTPH_2CYCLES;
  TscHandle.Init.CTPulseLowLength        = TSC_CTPL_2CYCLES;
  TscHandle.Init.IODefaultMode           = /*TSC_IODEF_IN_FLOAT*/TSC_IODEF_OUT_PP_LOW;
  TscHandle.Init.MaxCountInterrupt       = DISABLE;
  TscHandle.Init.MaxCountValue           = TSC_MCV_8191;
  TscHandle.Init.PulseGeneratorPrescaler = TSC_PG_PRESC_DIV64;
  TscHandle.Init.SpreadSpectrum          = DISABLE;
  TscHandle.Init.SpreadSpectrumDeviation = 127;
  TscHandle.Init.SpreadSpectrumPrescaler = TSC_SS_PRESC_DIV1;
  TscHandle.Init.SynchroPinPolarity      = TSC_SYNC_POLARITY_FALLING;
  /* All channel, shield and sampling IOs must be declared below */
  TscHandle.Init.ChannelIOs              = (TSC_GROUP1_IO3 | TSC_GROUP2_IO3 | TSC_GROUP3_IO2);
  TscHandle.Init.SamplingIOs             = (TSC_GROUP1_IO4 | TSC_GROUP2_IO4 | TSC_GROUP3_IO3);
  TscHandle.Init.ShieldIOs               = 0;

  if (HAL_TSC_Init(&TscHandle) != HAL_OK)
  {
    Error_Handler();
  }

  /* Init STMTouch driver */
  TSL_user_Init(); 

  while(BSP_PB_GetState(BUTTON_USER) != GPIO_PIN_SET)
  {
    /* Execute STMTouch Driver state machine */
    if (TSL_user_Action() == TSL_STATUS_OK)
    {       
      ProcessSensors(); // Execute sensors related tasks
    }
  }
  
  /* Wait for User button is released */
  while(BSP_PB_GetState(BUTTON_USER) != GPIO_PIN_RESET)
  {
  }

  /* Turn Off Leds */   
  BSP_LED_Off(LED3);
  BSP_LED_Off(LED4);
  BSP_LED_Off(LED5);
  BSP_LED_Off(LED6);
}

/**
  * @brief  Manage the activity on sensors when touched/released (example)
  * @param  None
  * @retval None
  */
static void ProcessSensors(void)
{
  BSP_LED_Off(LED3);
  BSP_LED_Off(LED4);
  BSP_LED_Off(LED5);
  BSP_LED_Off(LED6);

  if ((MyLinRots[0].p_Data->StateId == TSL_STATEID_DETECT) ||
      (MyLinRots[0].p_Data->StateId == TSL_STATEID_DEB_RELEASE_DETECT))
  {
    if (MyLinRots[0].p_Data->Position > 0)
    {
      BSP_LED_On(LED6);
    }

    if (MyLinRots[0].p_Data->Position >= 48)
    {
      BSP_LED_On(LED5);
    }

    if (MyLinRots[0].p_Data->Position >= 80)
    {
      BSP_LED_On(LED3);
    }

    if (MyLinRots[0].p_Data->Position >= 112)
    {
      BSP_LED_On(LED4);
    }
  }
}

/**
  * @brief  EXTI line detection callbacks.
  * @param GPIO_Pin: Specifies the pins connected EXTI line
  * @retval None
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if(GPIO_Pin == USER_BUTTON_PIN)
  {
    ButtonPressed = 0x01;
  }
}

/**
  * @brief  Acquisition completed callback in non blocking mode 
  * @param  htsc: pointer to a TSC_HandleTypeDef structure that contains
  *         the configuration information for the specified TSC.
  * @retval None
  */
void HAL_TSC_ConvCpltCallback(TSC_HandleTypeDef* htsc)
{
#if TSLPRM_TSC_IODEF > 0 // Default = Input Floating
  /* Set IO default in Output PP Low to discharge all capacitors */
  HAL_TSC_IODischarge(htsc, ENABLE);
#endif

  Gv_EOA = 1; /* To inform the main loop routine of the End Of Acquisition */
}

/**
  * @brief  Error callback in non blocking mode
  * @param  htsc: pointer to a TSC_HandleTypeDef structure that contains
  *         the configuration information for the specified TSC.
  * @retval None
  */
void HAL_TSC_ErrorCallback(TSC_HandleTypeDef* htsc)
{
  Error_Handler();
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
static void SystemClock_Config(void)
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
    Error_Handler();
  }

  /* Select PLL as system clock source and configure the HCLK and PCLK1 clocks dividers */
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
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
static void Error_Handler(void)
{
  /* User may add here some code to deal with this error */
  while(1)
  {
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
void assert_failed(uint8_t* file, uint32_t line)
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


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
