/*
 * Copyright 2018-2022 NXP.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*!
 * @file    streamer_audiosrc.c
 * @brief   Implementation of streamer audio source support
 *
 */

/*
 * INCLUDE FILES
 */
#include <string.h>

#include "audio_cfg.h"
#include "pipeline.h"
#include "streamer_audiosrc.h"
#include "streamer_element_properties.h"

#ifdef STREAMER_ENABLE_MEM_SRC
#include "mem_src.h"
#endif

/*
 * CONSTANTS, DEFINES AND MACROS
 */

int streamer_build_audiosrc_pipeline(int8_t pipeline_index,
                                     const char *in_dev_name,
                                     const char *out_dev_name,
                                     StreamPipelineType pipeline_type,
                                     STREAMER_T *task_data)
{
    int32_t ret             = STREAM_OK;
    uint32_t level          = 0;
    ELEMENT_PROPERTY_T prop = {0};
    AUDSRC_SET_NAME_T audsrc_name;
    StreamElement *element;

    int audio_src_idx  = ELEMENT_LAST_INDEX;
    int audio_sink_idx = ELEMENT_LAST_INDEX;

    STREAMER_FUNC_ENTER(DBG_CORE);

    switch (pipeline_type)
    {
        case STREAM_PIPELINE_PCM:
            audio_src_idx  = ELEMENT_MICROPHONE_INDEX;
            audio_sink_idx = ELEMENT_SPEAKER_INDEX;
            break;

        case STREAM_PIPELINE_PCM_USB_MIC:
            audio_src_idx  = ELEMENT_MICROPHONE_INDEX;
            audio_sink_idx = ELEMENT_USB_SINK_INDEX;
            break;

        case STREAM_PIPELINE_PCM_USB_SPEAKER:
            audio_src_idx  = ELEMENT_USB_SRC_INDEX;
            audio_sink_idx = ELEMENT_SPEAKER_INDEX;
            break;
        default:
            audio_src_idx  = ELEMENT_LAST_INDEX;
            audio_sink_idx = ELEMENT_LAST_INDEX;
            break;
    }

    task_data->pipeline_type[pipeline_index] = pipeline_type;

    /* Create the pipeline */
    ret = create_pipeline(&task_data->pipes[pipeline_index], pipeline_index, pipeline_type, &task_data->mq_out);
    if (STREAM_OK != ret)
    {
        STREAMER_LOG_ERR(DBG_CORE, ERRCODE_INTERNAL, "create pipeline failed:%d\n", ret);
        goto err_catch;
    }

    /* Create each of the pipeline elements: AUDIOSRC, AUDIOSINK */
    ret = create_element(&task_data->elems[pipeline_index][audio_src_idx], TYPE_ELEMENT_AUDIO_SRC, 0);
    if (STREAM_OK != ret)
    {
        STREAMER_LOG_ERR(DBG_CORE, ERRCODE_INTERNAL, "create element(%d) failed:%d\n", audio_src_idx, ret);
        goto err_catch;
    }

    ret = create_element(&task_data->elems[pipeline_index][audio_sink_idx], TYPE_ELEMENT_AUDIO_SINK, 0);
    if (STREAM_OK != ret)
    {
        STREAMER_LOG_ERR(DBG_CORE, ERRCODE_INTERNAL, "create element(%d) failed:%d\n", audio_sink_idx, ret);
        goto err_catch;
    }

    element = (StreamElement *)task_data->elems[pipeline_index][audio_sink_idx];
    /* Configure as push mode */
    element->sink_pad[0].scheduling = SCHEDULING_PUSH;

    /* Add each of the created elements to the pipeline */
    ret = add_element_pipeline(task_data->pipes[pipeline_index], task_data->elems[pipeline_index][audio_src_idx],
                               level++);
    if (STREAM_OK != ret)
    {
        STREAMER_LOG_ERR(DBG_CORE, ERRCODE_INTERNAL, "add element(%d) failed:%d\n", audio_src_idx, ret);
        goto err_catch;
    }

    ret = add_element_pipeline(task_data->pipes[pipeline_index], task_data->elems[pipeline_index][audio_sink_idx],
                               level++);
    if (STREAM_OK != ret)
    {
        STREAMER_LOG_ERR(DBG_CORE, ERRCODE_INTERNAL, "add element(%d) failed:%d\n", audio_sink_idx, ret);
        goto err_catch;
    }

    /* Pipeline:
     *   Link each of the elements in the pipeline together
     *    Level-0        Level-1(optional)  Level-2
     *   Audiosrc[src]->[sink]Queue[src]->[sink]Audio
     */
    ret = link_elements(task_data->elems[pipeline_index][audio_src_idx], 0,
                        task_data->elems[pipeline_index][audio_sink_idx], 0);
    if (STREAM_OK != ret)
    {
        STREAMER_LOG_ERR(DBG_CORE, ERRCODE_INTERNAL, "link element(%d) to element(%d) failed:%d\n", audio_src_idx,
                         audio_sink_idx, ret);
        goto err_catch;
    }

    /* Set the properties of the audio source */

    /* Set the device name */
    audsrc_name.device_name        = in_dev_name;
    audsrc_name.output_device_name = out_dev_name;
    prop.prop =
        (audio_src_idx == ELEMENT_USB_SRC_INDEX) ? PROP_USB_SRC_SET_DEVICE_NAME : PROP_MICROPHONE_SET_DEVICE_NAME;
    prop.val = (uintptr_t)&audsrc_name;

    ret = element_set_property(task_data->elems[pipeline_index][audio_src_idx], prop.prop, prop.val);

    if (STREAM_OK != ret)
    {
        STREAMER_LOG_ERR(DBG_CORE, ERRCODE_INTERNAL, "set element(%d) property(%d) failed:%d\n", audio_src_idx,
                         prop.prop, ret);
        goto err_catch;
    }

    /* Set the sample rate */
    prop.prop =
        (audio_src_idx == ELEMENT_USB_SRC_INDEX) ? PROP_USB_SRC_SET_SAMPLE_RATE : PROP_MICROPHONE_SET_SAMPLE_RATE;
    prop.val = STREAMER_AUDIO_SRC_DEFAULT_SAMPLE_RATE;

    ret = element_set_property(task_data->elems[pipeline_index][audio_src_idx], prop.prop, prop.val);

    if (STREAM_OK != ret)
    {
        STREAMER_LOG_ERR(DBG_CORE, ERRCODE_INTERNAL, "set element(%d) property(%d) failed:%d\n", audio_src_idx,
                         prop.prop, ret);
        goto err_catch;
    }

    /* Set the device type based on the input device name */

    prop.val = (uint32_t)AUDIOSRC_PCMRTOS;
    prop.prop =
        (audio_src_idx == ELEMENT_USB_SRC_INDEX) ? PROP_USB_SRC_SET_DEVICE_TYPE : PROP_MICROPHONE_SET_DEVICE_TYPE;

    ret = element_set_property(task_data->elems[pipeline_index][audio_src_idx], prop.prop, prop.val);

    if (STREAM_OK != ret)
    {
        STREAMER_LOG_ERR(DBG_CORE, ERRCODE_INTERNAL, "set element(%d) property(%d) failed:%d\n", audio_src_idx,
                         prop.prop, ret);
        goto err_catch;
    }

    /* Set the audio sink output device */
    prop.prop =
        (audio_sink_idx == ELEMENT_USB_SINK_INDEX) ? PROP_USB_SINK_DEVICE_DRIVER_TYPE : PROP_SPEAKER_DEVICE_DRIVER_TYPE;
    prop.val = STREAMER_DEFAULT_OUT_DEVICE;
    ret      = element_set_property(task_data->elems[pipeline_index][audio_sink_idx], prop.prop, prop.val);

    if (STREAM_OK != ret)
    {
        STREAMER_LOG_ERR(DBG_CORE, ERRCODE_INTERNAL, "set element(%d) property(%d) failed:%d\n", audio_sink_idx,
                         prop.prop, ret);
        goto err_catch;
    }

    /* Set the audio sink device name */
    prop.prop = (audio_sink_idx == ELEMENT_USB_SINK_INDEX) ? PROP_USB_SINK_DEVICE_DRIVER_STRING_NAME :
                                                             PROP_SPEAKER_DEVICE_DRIVER_STRING_NAME;
    prop.val  = (uintptr_t)out_dev_name;

    ret = element_set_property(task_data->elems[pipeline_index][audio_sink_idx], prop.prop, prop.val);

    if (STREAM_OK != ret)
    {
        STREAMER_LOG_ERR(DBG_CORE, ERRCODE_INTERNAL, "set element(%d) property(%d) failed:%d\n", audio_sink_idx,
                         prop.prop, ret);
        goto err_catch;
    }

err_catch:
    /* If any of the above steps failed, destroy the pipeline and
       return an error.
    */
    if (STREAM_OK != ret)
    {
        streamer_destroy_audiosrc_pipeline(pipeline_index, task_data);
    }
    STREAMER_FUNC_EXIT(DBG_CORE);
    return ret;
}

int streamer_destroy_audiosrc_pipeline(int8_t pipeline_index, STREAMER_T *task_data)
{
    int ret;
    int audio_src_idx  = ELEMENT_LAST_INDEX;
    int audio_sink_idx = ELEMENT_LAST_INDEX;

    STREAMER_FUNC_ENTER(DBG_CORE);

    switch (task_data->pipeline_type[pipeline_index])
    {
        case STREAM_PIPELINE_PCM:
            audio_src_idx  = ELEMENT_MICROPHONE_INDEX;
            audio_sink_idx = ELEMENT_SPEAKER_INDEX;
            break;

        case STREAM_PIPELINE_PCM_USB_MIC:
            audio_src_idx  = ELEMENT_MICROPHONE_INDEX;
            audio_sink_idx = ELEMENT_USB_SINK_INDEX;
            break;

        case STREAM_PIPELINE_PCM_USB_SPEAKER:
            audio_src_idx  = ELEMENT_USB_SRC_INDEX;
            audio_sink_idx = ELEMENT_SPEAKER_INDEX;
            break;
        default:
            audio_src_idx  = ELEMENT_LAST_INDEX;
            audio_sink_idx = ELEMENT_LAST_INDEX;
            break;
    }

    /* unlink elements in pipeline */
    ret = unlink_elements(task_data->elems[pipeline_index][audio_src_idx], 0,
                          task_data->elems[pipeline_index][audio_sink_idx], 0);
    if (STREAM_OK != ret)
    {
        STREAMER_LOG_ERR(DBG_CORE, ERRCODE_INTERNAL, "Failed to unlink audio src and audio sink\n");
        return ret;
    }

    /* remove_elements from pipeline*/
    ret = remove_element_pipeline(task_data->pipes[pipeline_index], task_data->elems[pipeline_index][audio_src_idx]);
    if (STREAM_OK != ret)
    {
        STREAMER_LOG_ERR(DBG_CORE, ERRCODE_INTERNAL, "Failed to remove audio src\n");
        return ret;
    }

    ret = remove_element_pipeline(task_data->pipes[pipeline_index], task_data->elems[pipeline_index][audio_sink_idx]);
    if (STREAM_OK != ret)
    {
        STREAMER_LOG_ERR(DBG_CORE, ERRCODE_INTERNAL, "Failed to remove audio sink\n");
        return ret;
    }

    /* destroy elements*/
    ret = destroy_element(task_data->elems[pipeline_index][audio_src_idx]);
    if (STREAM_OK != ret)
    {
        STREAMER_LOG_ERR(DBG_CORE, ERRCODE_INTERNAL, "Failed to remove audio src\n");
        return ret;
    }
    task_data->elems[pipeline_index][audio_src_idx] = (uintptr_t)NULL;

    ret = destroy_element(task_data->elems[pipeline_index][audio_sink_idx]);
    if (STREAM_OK != ret)
    {
        STREAMER_LOG_ERR(DBG_CORE, ERRCODE_INTERNAL, "Failed to remove audio sink\n");
        return ret;
    }
    task_data->elems[pipeline_index][audio_sink_idx] = (uintptr_t)NULL;

    /* destroy pipeline*/
    ret = destroy_pipeline(task_data->pipes[pipeline_index]);
    if (STREAM_OK != ret)
    {
        STREAMER_LOG_ERR(DBG_CORE, ERRCODE_INTERNAL, "Failed to destroy pipeline\n");
        return ret;
    }
    task_data->pipes[pipeline_index] = (uintptr_t)NULL;

    return ret;
}
