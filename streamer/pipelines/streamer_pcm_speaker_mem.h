/*
 * Copyright 2023 NXP.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __STREAMER_PCM_AUDIO_PROC_SPEAKER_MEM_H__
#define __STREAMER_PCM_AUDIO_PROC_SPEAKER_MEM_H__

int streamer_build_pcm_speaker_mem_pipeline(int8_t pipeline_index,
                                            StreamPipelineType pipeline_type,
                                            STREAMER_T *task_data);

int streamer_destroy_pcm_speaker_mem_pipeline(int8_t pipeline_index, STREAMER_T *task_data);

#endif
