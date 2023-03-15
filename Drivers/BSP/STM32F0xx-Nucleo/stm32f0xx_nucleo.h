/**
  ******************************************************************************
  * @file    stm32f0xx_nucleo.h
  * @author  MCD Application Team
  * @brief   This file contains definitions for:
  *          - LEDs and push-button available on STM32F0XX-Nucleo Kit 
  *            from STMicroelectronics
  *          - LCD, joystick and microSD available on Adafruit 1.8" TFT LCD 
  *            shield (reference ID 802)
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
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32F0XX_NUCLEO_H
#define __STM32F0XX_NUCLEO_H

#ifdef __cplusplus
 extern "C" {
#endif

/** @addtogroup BSP
  * @{
  */

/** @defgroup STM32F0XX_NUCLEO STM32F0XX-NUCLEO
  * @{
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx_hal.h"
   

/** @defgroup STM32F0XX_NUCLEO_Exported_Types Exported Types
  * @{
  */ 
typedef enum 
{
  LED2 = 0,
  LED_GREEN = LED2
} Led_TypeDef;

typedef enum 
{  
  BUTTON_USER = 0,
  /* Alias */
  BUTTON_KEY  = BUTTON_USER
} Button_TypeDef;

typedef enum 
{  
  BUTTON_MODE_GPIO = 0,
  BUTTON_MODE_EXTI = 1
} ButtonMode_TypeDef; 

typedef enum 
{ 
  JOY_NONE  = 0,
  JOY_SEL   = 1,
  JOY_DOWN  = 2,
  JOY_LEFT  = 3,
  JOY_RIGHT = 4,
  JOY_UP    = 5
} JOYState_TypeDef;

/**
  * @}
  */ 

/** @defgroup STM32F0XX_NUCLEO_Exported_Constants Exported Constants
  * @{
  */ 

/** 
* @brief	Define for STM32F0XX_NUCLEO board  
  */ 
#if !defined (USE_STM32F0XX_NUCLEO)
 #define USE_STM32F0XX_NUCLEO
#endif

/** @defgroup STM32F0XX_NUCLEO_LED LED Constants
  * @{
  */
#define LEDn                               1

#define LED2_PIN                           GPIO_PIN_5
#define LED2_GPIO_PORT                     GPIOA
#define LED2_GPIO_CLK_ENABLE()           __HAL_RCC_GPIOA_CLK_ENABLE()  
#define LED2_GPIO_CLK_DISABLE()          __HAL_RCC_GPIOA_CLK_DISABLE()
  
#define LEDx_GPIO_CLK_ENABLE(__INDEX__)   do { if((__INDEX__) == 0) LED2_GPIO_CLK_ENABLE();} while(0)
#define LEDx_GPIO_CLK_DISABLE(__INDEX__)  (((__INDEX__) == 0) ? LED2_GPIO_CLK_DISABLE() : 0)

/**
  * @}
  */ 
  
/** @defgroup STM32F0XX_NUCLEO_BUTTON BUTTON Constants
  * @{
  */  
#define BUTTONn                            1

/**
  * @brief User push-button
  */
#define USER_BUTTON_PIN                         GPIO_PIN_13
#define USER_BUTTON_GPIO_PORT                   GPIOC
#define USER_BUTTON_GPIO_CLK_ENABLE()         __HAL_RCC_GPIOC_CLK_ENABLE()   
#define USER_BUTTON_GPIO_CLK_DISABLE()        __HAL_RCC_GPIOC_CLK_DISABLE()  
#define USER_BUTTON_EXTI_LINE                   GPIO_PIN_13
#define USER_BUTTON_EXTI_IRQn                   EXTI4_15_IRQn
/* Aliases */
#define KEY_BUTTON_PIN                        USER_BUTTON_PIN
#define KEY_BUTTON_GPIO_PORT                  USER_BUTTON_GPIO_PORT
#define KEY_BUTTON_GPIO_CLK_ENABLE()          USER_BUTTON_GPIO_CLK_ENABLE()
#define KEY_BUTTON_GPIO_CLK_DISABLE()         USER_BUTTON_GPIO_CLK_DISABLE()
#define KEY_BUTTON_EXTI_LINE                  USER_BUTTON_EXTI_LINE
#define KEY_BUTTON_EXTI_IRQn                  USER_BUTTON_EXTI_IRQn

#define BUTTONx_GPIO_CLK_ENABLE(__INDEX__)    do { if((__INDEX__) == 0) USER_BUTTON_GPIO_CLK_ENABLE();} while(0)
#define BUTTONx_GPIO_CLK_DISABLE(__INDEX__)   (((__INDEX__) == 0) ? USER_BUTTON_GPIO_CLK_DISABLE() : 0)
/**
  * @}
  */ 

/** @defgroup STM32F0XX_NUCLEO_BUS BUS Constants
  * @{
  */ 
/*###################### SPI1 ###################################*/
#define NUCLEO_SPIx                                 SPI1
#define NUCLEO_SPIx_CLK_ENABLE()                  __HAL_RCC_SPI1_CLK_ENABLE()

#define NUCLEO_SPIx_SCK_AF                          GPIO_AF0_SPI1
#define NUCLEO_SPIx_SCK_GPIO_PORT                   GPIOA
#define NUCLEO_SPIx_SCK_PIN                         GPIO_PIN_5
#define NUCLEO_SPIx_SCK_GPIO_CLK_ENABLE()         __HAL_RCC_GPIOA_CLK_ENABLE()
#define NUCLEO_SPIx_SCK_GPIO_CLK_DISABLE()        __HAL_RCC_GPIOA_CLK_DISABLE()

#define NUCLEO_SPIx_MISO_MOSI_AF                    GPIO_AF0_SPI1
#define NUCLEO_SPIx_MISO_MOSI_GPIO_PORT             GPIOA
#define NUCLEO_SPIx_MISO_MOSI_GPIO_CLK_ENABLE()   __HAL_RCC_GPIOA_CLK_ENABLE()
#define NUCLEO_SPIx_MISO_MOSI_GPIO_CLK_DISABLE()  __HAL_RCC_GPIOA_CLK_DISABLE()
#define NUCLEO_SPIx_MISO_PIN                        GPIO_PIN_6
#define NUCLEO_SPIx_MOSI_PIN                        GPIO_PIN_7
/* Maximum Timeout values for flags waiting loops. These timeouts are not based
   on accurate values, they just guarantee that the application will not remain
   stuck if the SPI communication is corrupted.
   You may modify these timeout values depending on CPU frequency and application
   conditions (interrupts routines ...). */   
#define NUCLEO_SPIx_TIMEOUT_MAX                   1000


/**
  * @brief  SD Control Lines management
  */
#define SD_CS_LOW()       HAL_GPIO_WritePin(SD_CS_GPIO_PORT, SD_CS_PIN, GPIO_PIN_RESET)
#define SD_CS_HIGH()      HAL_GPIO_WritePin(SD_CS_GPIO_PORT, SD_CS_PIN, GPIO_PIN_SET)

/**
  * @brief  LCD Control Lines management
  */
#define LCD_CS_LOW()      HAL_GPIO_WritePin(LCD_CS_GPIO_PORT, LCD_CS_PIN, GPIO_PIN_RESET)
#define LCD_CS_HIGH()     HAL_GPIO_WritePin(LCD_CS_GPIO_PORT, LCD_CS_PIN, GPIO_PIN_SET)
#define LCD_DC_LOW()      HAL_GPIO_WritePin(LCD_DC_GPIO_PORT, LCD_DC_PIN, GPIO_PIN_RESET)
#define LCD_DC_HIGH()     HAL_GPIO_WritePin(LCD_DC_GPIO_PORT, LCD_DC_PIN, GPIO_PIN_SET)
     
/**
  * @brief  SD Control Interface pins (shield D4)
  */
#define SD_CS_PIN                                 GPIO_PIN_5
#define SD_CS_GPIO_PORT                           GPIOB
#define SD_CS_GPIO_CLK_ENABLE()                 __HAL_RCC_GPIOB_CLK_ENABLE()
#define SD_CS_GPIO_CLK_DISABLE()                __HAL_RCC_GPIOB_CLK_DISABLE()

/**
  * @brief  LCD Control Interface pins (shield D10)
  */
#define LCD_CS_PIN                                 GPIO_PIN_6
#define LCD_CS_GPIO_PORT                           GPIOB
#define LCD_CS_GPIO_CLK_ENABLE()                 __HAL_RCC_GPIOB_CLK_ENABLE()
#define LCD_CS_GPIO_CLK_DISABLE()                __HAL_RCC_GPIOB_CLK_DISABLE()
    
/**
  * @brief  LCD Data/Command Interface pins
  */
#define LCD_DC_PIN                                 GPIO_PIN_9
#define LCD_DC_GPIO_PORT                           GPIOA
#define LCD_DC_GPIO_CLK_ENABLE()                 __HAL_RCC_GPIOA_CLK_ENABLE()
#define LCD_DC_GPIO_CLK_DISABLE()                __HAL_RCC_GPIOA_CLK_DISABLE()

/*##################### ADC1 ###################################*/
/**
  * @brief  ADC Interface pins
  *         used to detect motion of Joystick available on Adafruit 1.8" TFT shield
  */
#define NUCLEO_ADCx                                 ADC1
#define NUCLEO_ADCx_CLK_ENABLE()                  __HAL_RCC_ADC1_CLK_ENABLE()
#define NUCLEO_ADCx_CLK_DISABLE()                 __HAL_RCC_ADC1_CLK_DISABLE()

#define NUCLEO_ADCx_GPIO_PORT                       GPIOB
#define NUCLEO_ADCx_GPIO_PIN                        GPIO_PIN_0
#define NUCLEO_ADCx_GPIO_CLK_ENABLE()             __HAL_RCC_GPIOB_CLK_ENABLE()
#define NUCLEO_ADCx_GPIO_CLK_DISABLE()            __HAL_RCC_GPIOB_CLK_DISABLE()

/**
  * @}
  */

/**
  * @}
  */

/** @defgroup STM32F0XX_NUCLEO_Exported_Functions Exported Functions
  * @{
  */
uint32_t  BSP_GetVersion(void);
/** @defgroup STM32F0XX_NUCLEO_LED_Functions  LED Functions
  * @{
  */ 
void      BSP_LED_Init(Led_TypeDef Led);
void      BSP_LED_DeInit(Led_TypeDef Led);
void      BSP_LED_On(Led_TypeDef Led);
void      BSP_LED_Off(Led_TypeDef Led);
void      BSP_LED_Toggle(Led_TypeDef Led);
/**
  * @}
  */ 

/** @addtogroup STM32F0XX_NUCLEO_BUTTON_Functions
  * @{
  */                
void      BSP_PB_Init(Button_TypeDef Button, ButtonMode_TypeDef ButtonMode);
void      BSP_PB_DeInit(Button_TypeDef Button);
uint32_t  BSP_PB_GetState(Button_TypeDef Button);
#if defined(HAL_ADC_MODULE_ENABLED)
uint8_t          BSP_JOY_Init(void);
JOYState_TypeDef BSP_JOY_GetState(void);
void             BSP_JOY_DeInit(void);
#endif /* HAL_ADC_MODULE_ENABLED */


/**
  * @}
  */

/**
  * @}
  */ 

/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* __STM32F0XX_NUCLEO_H */

    

