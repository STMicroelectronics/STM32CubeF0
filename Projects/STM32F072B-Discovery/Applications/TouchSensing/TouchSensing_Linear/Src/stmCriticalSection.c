/**
  ******************************************************************************
  * @file    TouchSensing\TouchSensing_Linear\Src\stmCriticalSection.c
  * @author  MCD Application Team
  * @brief   This file provides a mechanism for STMStudio host/target
  *          synchronization. Based on a critical section, using few
  *          target resources (in term of code and RAM), but
  *          potentially impacting the application runtime (possible
  *          waiting loop when enterring the critical section).
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

/* Implementation of Peterson's algorithm for mutual exclusive access to
   data, between the STMStudio host and the target processor.
   The use of the critical section ensures the coherence of a set of data: it is
   not possible for the STMStudio host to read data while the target is in the
   critical section.
   The target should enter the critical section (waiting loop possible, if the
   STMStudio host is being reading) before modifying data that are identified as
   critical ones. Then leave the critical section in order to allow the
   STMStudio host to read them.
   The host also enters the critical section before each reading, and leaves it
   afterwards.
   Note that it is not mandatory for the target to protect all spied variables
   into the critical section; and that the synchronization might generate
   relatively long waiting loops on the target side. As a result the critical
   section should be used only for word-variables or group of variables for
   which the coherence is important.
*/

#include "stmCriticalSection.h"

#define TARGET_LOCK_ID 0 // Do not modify - shared with STMStudio host software
#define HOST_LOCK_ID   1 // Do not modify - shared with STMStudio host software

typedef struct petersons_t {
    volatile unsigned char flag[2]; // Do not modify - shared with STMStudio host software
    volatile unsigned char turn;    // Do not modify - shared with STMStudio host software
} petersons_t;

// stm_studio_lock symbol used by the STMStudio host software for synchronization
petersons_t stm_studio_lock = { { 0, 0 }, TARGET_LOCK_ID }; // Do not modify - shared with STMStudio host software

void enterLock (void) {
    stm_studio_lock.flag[TARGET_LOCK_ID] = 1;
    stm_studio_lock.turn = HOST_LOCK_ID;
    while (stm_studio_lock.flag[HOST_LOCK_ID] && (stm_studio_lock.turn == HOST_LOCK_ID)) {}
}

void exitLock (void) {
    stm_studio_lock.flag[TARGET_LOCK_ID] = 0;
}
