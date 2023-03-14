/**
  ******************************************************************************
  * @file    Demonstrations\Inc\tsl_user.h
  * @author  MCD Application Team
  * @brief   Touch-Sensing user configuration and api file.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2014 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __TSL_USER_H
#define __TSL_USER_H

#include "tsl.h"
#include "stm32f0xx_hal.h"
#include "stm32f072b_discovery.h"

/*========================*/
/* Channel IOs definition */
/*========================*/

#define CHANNEL_0_IO_MSK    (TSC_GROUP1_IO3)
#define CHANNEL_0_GRP_MSK   (TSC_GROUP1)
#define CHANNEL_0_SRC       (TSC_GROUP1_IDX) /* Index in source register (TSC->IOGXCR[]) */
#define CHANNEL_0_DEST      (0) /* Index in destination result array */

#define CHANNEL_1_IO_MSK    (TSC_GROUP2_IO3)
#define CHANNEL_1_GRP_MSK   (TSC_GROUP2)
#define CHANNEL_1_SRC       (TSC_GROUP2_IDX) /* Index in source register (TSC->IOGXCR[]) */
#define CHANNEL_1_DEST      (1) /* Index in destination result array */

#define CHANNEL_2_IO_MSK    (TSC_GROUP3_IO2)
#define CHANNEL_2_GRP_MSK   (TSC_GROUP3)
#define CHANNEL_2_SRC       (TSC_GROUP3_IDX) /* Index in source register (TSC->IOGXCR[]) */
#define CHANNEL_2_DEST      (2) /* Index in destination result array */

/*=======================*/
/* Shield IOs definition */
/*=======================*/

#define SHIELD_IO_MSK       (0) /* No shield on this board */

/*==================*/
/* Banks definition */
/*==================*/

#define BANK_0_NBCHANNELS    (3)
#define BANK_0_MSK_CHANNELS  (CHANNEL_0_IO_MSK  | CHANNEL_1_IO_MSK | CHANNEL_2_IO_MSK)
#define BANK_0_MSK_GROUPS    (CHANNEL_0_GRP_MSK | CHANNEL_1_GRP_MSK | CHANNEL_2_GRP_MSK) /* Only these groups will be acquired */

/* User Parameters */
extern CONST TSL_Bank_T MyBanks[];
extern CONST TSL_LinRot_T MyLinRots[];
extern CONST TSL_Object_T MyObjects[];
extern TSL_ObjectGroup_T MyObjGroup;

void TSL_user_Init(void);
TSL_Status_enum_T TSL_user_Action(void);
void MyLinRots_ErrorStateProcess(void);
void MyLinRots_OffStateProcess(void);

#endif /* __TSL_USER_H */
