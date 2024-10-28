/*
 * Copyright 2020-2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 *  @file  app_Streamer_element_pcm.c
 *  @brief Implementation of application dependent vit element
 */

#ifndef _STREAMER_ELEMENT_VIT_H_
#define _STREAMER_ELEMENT_VIT_H_

#if defined(__cplusplus)
extern "C" {
#endif

#include "PL_platformTypes_CortexM.h"
#include "VIT.h"

typedef int (*VIT_Initialize_T)(void *arg);
typedef int (*VIT_Execute_T)(void *arg, void *inputBuffer, int size);
typedef int (*VIT_Deinit_T)(void);

extern VIT_Initialize_T VIT_Initialize_func;
extern VIT_Execute_T VIT_Execute_func;
extern VIT_Deinit_T VIT_Deinit_func;

#if defined(__cplusplus)
}
#endif

#endif // _STREAMER_ELEMENT_VIT_H_
