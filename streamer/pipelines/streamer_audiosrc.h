/*
 * Copyright 2018-2022 NXP.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _STREAMER_AUDIOSRC_H_
#define _STREAMER_AUDIOSRC_H_
/*!
 * @file    streamer_audiosrc.h
 * @brief   This file contains the definition of the streamer audio source
 *          functionality.
 */

/*
 * CONSTANTS, DEFINES AND MACROS
 */

/**
 * @brief Audio source default sample rate
 *
 */
#define STREAMER_AUDIO_SRC_DEFAULT_SAMPLE_RATE (48000)

/**
 * @brief Build audio source pipeline
 *
 * @param pipeline_index Pipeline index
 * @param in_dev_name    Input device name
 * @param out_dev_name   Output devie name
 * @param pipeline_type  Pipeline type
 * @param task_data      Pointer to streamer task data
 * @return int
 */
int streamer_build_audiosrc_pipeline(int8_t pipeline_index,
                                     const char *in_dev_name,
                                     const char *out_dev_name,
                                     StreamPipelineType pipeline_type,
                                     STREAMER_T *task_data);

/**
 * @brief Destroy audio source pipeline
 *
 * @param pipeline_index Pipeline index
 * @param task_data      Pointer to streamer task data
 * @return int
 */
int streamer_destroy_audiosrc_pipeline(int8_t pipeline_index, STREAMER_T *task_data);

#endif /* _STREAMER_AUDIOSRC_H_ */
