/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 *  @file  app_Streamer_element_pcm.c
 *  @brief Replaces fsl_Debug_console.h used in NXP's SDK.
 *
 *  Purpose of this file is to provide simple interface for text I/O.
 */

#ifndef _FSL_DEBUGCONSOLE_H_
#define _FSL_DEBUGCONSOLE_H_

#if (CONFIG_MAESTRO_RTOS_ZEPHYR == 1)

// #include <zephyr/toolchain.h>
#include "zephyr/sys/printk.h"

static inline int debugConsole_Disabled(void)
{
    return -1;
}

#define PRINTF       printk
#define SCANF(...)   debugConsole_Disabled()
#define PUTCHAR(...) debugConsole_Disabled()
#define GETCHAR(...) debugConsole_Disabled()

#endif

#endif /* _FSL_DEBUGCONSOLE_H_ */
