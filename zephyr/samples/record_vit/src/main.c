/**
 * Copyright 2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 *  @file  main.c
 *  @brief Maestro example recognizing voice commands from DMIC microphone using VIT library
 */

#include <stdio.h>

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(main, LOG_LEVEL_ERR);
#include <zephyr/audio/dmic.h>
#include <zephyr/drivers/gpio.h>

#include "streamer_api.h"
#include "streamer_element_properties.h"
#include "maestro_logging.h"
#include "error.h"

#include "app_streamer_element_pcm.h"
#include "app_streamer_element_vit.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define STREAMER_OUT_QUEUE_NAME      "Streamer outQueue"
#define STREAMER_TASK_NAME           "Streamer task"
#define STREAMER_TASK_STACK_SIZE_B   (CONFIG_MAESTRO_EXAMPLE_STREAMER_TASK_STACK_SIZE_B)
#define STREAMER_IN_DEV_NAME         "DMIC"
#define STREAMER_OUT_DEV_NAME        "VIT library"

#define DEMO_MIC_CHANNEL_NUM            (1u)
#define DEMO_MIC_FRAME_LENGTH_MS        (30u)
#define DEMO_MIC_AUDIO_BIT_WIDTH        (16u)
#define DEMO_MIC_AUDIO_SAMPLE_RATE_HZ   (16000u)

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

static int s_demoRun(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/

static STREAMER_T *s_demo_streamerPtr;

/*******************************************************************************
 * Code
 *******************************************************************************/

int main(void)
{
    printk("== Voice recognition demo ==");

    return s_demoRun();
}

static void s_demo_streamerInitLogging(void)
{
    /* Initialize logging */
    init_logging();

    #if (CONFIG_MAESTRO_EXAMPLE_MAESTRO_LOGGING_ON)
        set_debug_module(0xffffffff);
        set_debug_level(LOGLVL_DEBUG);
        get_debug_state();
    #endif
}

static osa_status_t s_demo_streamerPrepare(void)
{
    int retVal = STREAM_OK;

    /* Define element parameters */
    STREAMER_CREATE_PARAM streamer_params;
    ELEMENT_PROPERTY_T prop;

    /* Setup Pipeline elements */
    ElementIndex elementsList_ids[]    = {ELEMENT_MICROPHONE_INDEX, ELEMENT_VIT_INDEX};
    int elements_cnt                   = sizeof(elementsList_ids) / sizeof(ElementIndex);
    PipelineElements pipeline_elems = {elementsList_ids, elements_cnt};

    /* Setup streamer parameters */
    strcpy(streamer_params.out_mq_name, STREAMER_OUT_QUEUE_NAME);
    streamer_params.stack_size   = STREAMER_TASK_STACK_SIZE_B;
    streamer_params.task_name    = STREAMER_TASK_NAME;
    streamer_params.in_dev_name  = STREAMER_IN_DEV_NAME;
    streamer_params.out_dev_name = STREAMER_OUT_DEV_NAME;
    streamer_params.elements     = pipeline_elems;

    s_demo_streamerInitLogging();

    /* Create streamer */
    s_demo_streamerPtr = streamer_create(&streamer_params);
    if (s_demo_streamerPtr == NULL)
    {
        LOG_ERR("Streamer create failed\r\n");
        return KOSA_StatusError;
    }

    /* Setting Microphone properties */
    prop.prop = PROP_MICROPHONE_SET_SAMPLE_RATE;
    prop.val  = DEMO_MIC_AUDIO_SAMPLE_RATE_HZ;
    retVal   |= streamer_set_property(s_demo_streamerPtr, 0, prop, true);

    prop.prop = PROP_MICROPHONE_SET_BITS_PER_SAMPLE;
    prop.val  = DEMO_MIC_AUDIO_BIT_WIDTH;
    retVal   |= streamer_set_property(s_demo_streamerPtr, 0, prop, true);

    prop.prop = PROP_MICROPHONE_SET_NUM_CHANNELS;
    prop.val  = DEMO_MIC_CHANNEL_NUM;
    retVal   |= streamer_set_property(s_demo_streamerPtr, 0, prop, true);

    prop.prop = PROP_MICROPHONE_SET_FRAME_MS;
    prop.val  = DEMO_MIC_FRAME_LENGTH_MS;
    retVal   |= streamer_set_property(s_demo_streamerPtr, 0, prop, true);

    EXT_AUDIOELEMENT_DESC_T appFunctions = {
        .open_func      = streamer_pcm_rx_open,
        .close_func     = streamer_pcm_rx_close,
        .start_func     = NULL,
        .process_func   = streamer_pcm_read,
        .set_param_func = streamer_pcm_setparams,
        .get_param_func = streamer_pcm_getparams,
        .mute_func      = streamer_pcm_mute,
        .volume_func    = streamer_pcm_set_volume,
    };
    prop.prop = PROP_MICROPHONE_SET_APP_FUNCTIONS;
    prop.val  = (uintptr_t)&appFunctions;
    retVal   |= streamer_set_property(s_demo_streamerPtr, 0, prop, true);

    /* Setting VIT properties */
    EXT_PROCESS_DESC_T vit_proc = {VIT_Initialize_func, VIT_Execute_func, VIT_Deinit_func, NULL};

    prop.prop = PROP_VITSINK_FUNCPTR;
    prop.val = (uintptr_t)&vit_proc;
    streamer_set_property(s_demo_streamerPtr, 0, prop, true);

    if (retVal != STREAM_OK)
    {
        LOG_ERR("Streamer set property failed\r\n");
        streamer_destroy(s_demo_streamerPtr);
        s_demo_streamerPtr = NULL;

        return KOSA_StatusError;
    }

    return KOSA_StatusSuccess;
}

static osa_status_t s_demo_streamerCleanup()
{
    if (OSA_MsgQDestroy(s_demo_streamerPtr->message_channel_out.mq) != KOSA_StatusSuccess)
    {
        LOG_ERR("Could not destroy out message queue\r\n");
        return KOSA_StatusError;
    }
    s_demo_streamerPtr->message_channel_out.is_mq_created = false;

    if (streamer_destroy(s_demo_streamerPtr) != ERRCODE_NO_ERROR)
    {
        LOG_ERR("Could not destroy streamer\r\n");
        return KOSA_StatusError;
    }

    return KOSA_StatusSuccess;
}

static osa_status_t s_demo_streamerRunStart()
{
    LOG_DBG("Streamer start encoding\r\n");
    if(streamer_set_state(s_demo_streamerPtr, 0, STATE_PLAYING, true) != ERRCODE_NO_ERROR)
    {
        s_demo_streamerCleanup();
        return KOSA_StatusError;
    }

    return KOSA_StatusSuccess;
}

static osa_status_t s_demo_streamerRunProcess()
{
    /* Process responses from streamer task */
    osa_status_t retVal;
    STREAMER_MSG_T msg;
    do
    {
        retVal = OSA_MsgQGet(s_demo_streamerPtr->message_channel_out.mq, (void *)&msg, 0xFFFFFFFF);
        switch (retVal)
        {
            case KOSA_StatusTimeout:
                LOG_ERR("Message queue get timeouted\r\n");
                break;
            case KOSA_StatusError:
                LOG_ERR("Message queue get error\r\n");
                break;
            case KOSA_StatusSuccess:
                /* Message obtained */
                LOG_DBG("Message queue get message id: %u\r\n", msg.id);
                break;
            default:
                LOG_ERR("Unhandled message get response");
                break;
        }
        if (retVal != KOSA_StatusSuccess)
        {
            s_demo_streamerCleanup();
            return KOSA_StatusError;
        }

        /* Decode the message */
        switch (msg.id)
        {
            case STREAM_MSG_ERROR:
                LOG_ERR("Streamer MSG - error id: %u\r\n", msg.id);
                s_demo_streamerCleanup();
                return KOSA_StatusError;
            case STREAM_MSG_EOS:
                LOG_DBG("Streamer MSG - end of stream\r\n");
                s_demo_streamerCleanup();
                return KOSA_StatusSuccess;
            case STREAM_MSG_CLOSE_TASK:
                LOG_DBG("Streamer MSG - close task\r\n");
                s_demo_streamerCleanup();
                return KOSA_StatusSuccess;
            default:
                LOG_WRN("Streamer MSG - got message with undefined reaction\r\n");
                break;
        }
    } while (true);

    /* Should not get there */
    return KOSA_StatusError;
}

static osa_status_t s_demo_streamerRun()
{
    osa_status_t retVal = s_demo_streamerRunStart();

    if(retVal != KOSA_StatusSuccess)
    {
        return retVal;
    }

    return s_demo_streamerRunProcess();
}

static int s_demoRun(void)
{
    osa_status_t retVal = KOSA_StatusSuccess;

    /* =================================
     *  Prepare phase
     * ================================= */
    LOG_INF("Demo Preparation");
    retVal = s_demo_streamerPrepare();

    /* =================================
     *  Run phase
     * ================================= */
    if (KOSA_StatusSuccess == retVal)
    {
        LOG_DBG("Demo Start\r\n");
        retVal = s_demo_streamerRun();
    }

    /* =================================
     *  Control phase
     * ================================= */
    if (retVal != KOSA_StatusSuccess)
    {
        printk("Demo result: FAIL\r\n");
        return -1;
    }

    printk("Demo result: OK\r\n");
    return 0;
}
