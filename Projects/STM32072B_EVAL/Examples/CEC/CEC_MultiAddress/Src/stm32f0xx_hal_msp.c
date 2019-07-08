/**
  ******************************************************************************
  * @file    CEC/CEC_MultiAddress/Src/stm32f0xx_hal_msp.c 
  * @author  MCD Application Team
  * @brief   HAL MSP module.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2016 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/** @addtogroup STM32F0xx_HAL_Examples
  * @{
  */

/** @defgroup CEC_MultiAddress
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/** @defgroup HAL_MSP_Private_Functions
  * @{
  */

/**
  * @brief CEC MSP Initialization 
  *        This function configures the hardware resources used in this example: 
  *           - Peripheral's clock enable
  *           - Peripheral's GPIO Configuration  
  * @param hcec: CEC handle pointer
  * @retval None
  */  
void HAL_CEC_MspInit(CEC_HandleTypeDef *hcec)
{
  GPIO_InitTypeDef  GPIO_InitStruct;
  
  /* Enable CEC clock */
  __HAL_RCC_CEC_CLK_ENABLE();  
    
  /* Enable GPIO clock and initialize GPIO */
  HDMI_CEC_LINE_CLK_ENABLE();
  GPIO_InitStruct.Pin = HDMI_CEC_LINE_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.Alternate = HDMI_CEC_LINE_AF;
  HAL_GPIO_Init(HDMI_CEC_LINE_GPIO_PORT, &GPIO_InitStruct);
  
  /* Enable and set CEC Interrupt to the lowest priority */
  HAL_NVIC_SetPriority(HDMI_CEC_IRQn, 0x0F, 0);
  HAL_NVIC_EnableIRQ(HDMI_CEC_IRQn);

}

/**
  * @brief CEC MSP De-Initialization 
  *        This function disable the Peripheral's clock
  * @param hcec: CEC handle pointer
  * @retval None
  */
void HAL_CEC_MspDeInit(CEC_HandleTypeDef *hcec)
{ 
  /* Disable CEC clock */ 
  __HAL_RCC_CEC_CLK_DISABLE();
  /* Disable GPIO TX/RX clock */
  HDMI_CEC_LINE_CLK_DISABLE();
}

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
