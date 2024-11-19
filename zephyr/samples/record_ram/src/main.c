/**
 * Copyright 2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 *  @file  main.c
 *  @brief Maestro example recording data from DMIC to RAM using the Maestro library.
 */

#include <stdio.h>

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(main, LOG_LEVEL_WRN);
#include <zephyr/audio/dmic.h>

#include "streamer_api.h"
#include "streamer_element_properties.h"
#include "maestro_logging.h"
#include "error.h"

#include "app_streamer_element_pcm.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define STREAMER_OUT_QUEUE_NAME      "streamer outQueue"
#define STREAMER_TASK_NAME           "streamer task"
#define STREAMER_TASK_STACK_SIZE_B   (CONFIG_MAESTRO_EXAMPLE_STREAMER_TASK_STACK_SIZE_B)
#define STREAMER_MESSAGE_TASK_NAME   "streamer message task"
#define STREAMER_MESSAGE_TASK_STACK_SIZE (256u)
#define STREAMER_IN_DEV_NAME         "DMIC"
#define STREAMER_OUT_DEV_NAME        "RAM memory"

#define DEMO_MIC_FRAME_LENGTH_MS        (30u)
#define DEMO_MIC_AUDIO_BIT_WIDTH        (16u)
#define DEMO_MIC_AUDIO_SAMPLE_RATE_HZ   (16000u)
#define DEMO_MIC_CHANNEL_NUM            (1u)

#define DEMO_SAMPLE_SIZE_B        (DEMO_MIC_AUDIO_BIT_WIDTH/8)
#define DEMO_SAMPLES_COUNT        ((CONFIG_MAESTRO_EXAMPLE_RECORD_TIME_MS * DEMO_MIC_AUDIO_SAMPLE_RATE_HZ)/ 1000)
#define DEMO_BUFFER_SIZE_B        (DEMO_SAMPLES_COUNT * DEMO_SAMPLE_SIZE_B)
#define DEMO_BUFFER_INIT_SYMBOL   (0x00)

#if (CONFIG_MAESTRO_EXAMPLE_RECORD_TIME_MS < 100)
    #error "Recorded duration cnonstrain is too small. Increase CONFIG_MAESTRO_EXAMPLE_RECORD_TIME_MS."
#endif

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

static int s_demoRun(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/

static STREAMER_T *s_demo_streamerPtr;
static uint8_t s_demo_bufferDst[DEMO_BUFFER_SIZE_B] = {0};
static SET_BUFFER_DESC_T s_demo_streamer_bufferDst  = {0};
OSA_TASK_HANDLE_DEFINE(s_demo_task_streamerHandler);
static struct k_timer s_demo_timerStreamerStop_handle;

/*******************************************************************************
 * Code
 *******************************************************************************/

int main(void)
{
    printk("== Record demo ==\r\n");

    return s_demoRun();
}

static void s_demo_prepareBuffers(void)
{
    for (uint32_t i = 0; i < DEMO_BUFFER_SIZE_B; i++)
    {
        s_demo_bufferDst[i] = DEMO_BUFFER_INIT_SYMBOL;
    }
}

static bool s_demo_isBufferUnchanged(void)
{
    for (uint32_t i = 0; i < DEMO_BUFFER_SIZE_B; i++)
    {
        if (s_demo_bufferDst[i] != DEMO_BUFFER_INIT_SYMBOL)
        {
            return false;
        }
    }
    return true;
}

static void s_demo_streamerInitLogging(void)
{
    /* Initialize logging */
    init_logging();

    #if (MAESTRO_EXAMPLE_MAESTRO_LOGGING_ON)
        set_debug_module(0xffffffff);
        set_debug_level(LOGLVL_DEBUG);
        get_debug_state();
    #endif
}

static void s_demo_timerStreamerStop_expireFunc(struct k_timer * handle)
{
    LOG_DBG("Timer streamer Stop\r\n");

    /* Stop the streamer */
    streamer_set_state(s_demo_streamerPtr, 0, STATE_PAUSED, false);
}

static void s_demo_controlPrepare()
{
    /* Initialize external timer closing streamer after timeout is expired */
    k_timer_init(&s_demo_timerStreamerStop_handle, s_demo_timerStreamerStop_expireFunc, NULL);
}

static osa_status_t s_demo_streamerPrepare()
{
    int retVal = STREAM_OK;

    /* Define memory buffers for streamer */
    s_demo_streamer_bufferDst =
        (SET_BUFFER_DESC_T){.ptr = (int8_t *)s_demo_bufferDst, .size = DEMO_BUFFER_SIZE_B};

    /* Define element parameters */
    STREAMER_CREATE_PARAM streamer_params;
    ELEMENT_PROPERTY_T prop;

    /* Setup Pipeline elements */
    ElementIndex elementsList_ids[]    = {ELEMENT_MICROPHONE_INDEX, ELEMENT_MEM_SINK_INDEX};
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

    /* Setting Memsink properties */
    prop.prop = PROP_MEMSINK_BUFFER_DESC;
    prop.val  = (uintptr_t)&s_demo_streamer_bufferDst;
    retVal   |= streamer_set_property(s_demo_streamerPtr, 0, prop, true);

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
    k_timer_stop(&s_demo_timerStreamerStop_handle);

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
    /* Start timer stopping the streamer after timeout */
    k_timer_start(&s_demo_timerStreamerStop_handle, K_MSEC(CONFIG_MAESTRO_EXAMPLE_RECORD_TIME_MS), K_NO_WAIT);

    LOG_INF("Streamer start encoding\r\n");
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
        /* Try to obtain message from streamer task */
        retVal = OSA_MsgQGet(s_demo_streamerPtr->message_channel_out.mq, (void *)&msg, 50);
        switch (retVal)
        {
            case KOSA_StatusTimeout:
                /* No message from streamer */
                PipelineState pipState;
                streamer_get_state(s_demo_streamerPtr, 0, &pipState, false);

                /* Detects if Pipeline was stopped by timer s_demo_timerStreamerStop_handle after timeout */
                if(pipState == STATE_PAUSED)
                {
                    LOG_DBG("Pipeline was stopped\r\n");
                    s_demo_streamerCleanup();
                    return KOSA_StatusSuccess;
                } else
                {
                    continue;
                }

            case KOSA_StatusSuccess:
                LOG_DBG("Message queue get message id: %u\r\n", msg.id);

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
                        LOG_ERR("Streamer MSG - got message with undefined reaction. Msg ID: %u\r\n", msg.id);
                        s_demo_streamerCleanup();
                        return KOSA_StatusError;
                        break;
                }

            case KOSA_StatusError:
                LOG_ERR("Message queue get error\r\n");
                return KOSA_StatusError;

            default:
                LOG_ERR("Unhandled message get response");
                break;
        }
    } while (true); /* Repeat processing messages from streamer */

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
    printk("Demo Preparation\r\n");
    printk("Recorded length set: %u [ms]\r\n", CONFIG_MAESTRO_EXAMPLE_RECORD_TIME_MS);
    s_demo_prepareBuffers();
    s_demo_controlPrepare();
    retVal = s_demo_streamerPrepare();

    /* =================================
     *  Run phase
     * ================================= */
    if (KOSA_StatusSuccess == retVal)
    {
        printk("Demo Start\r\n");
        retVal = s_demo_streamerRun();
    }

    /* =================================
     *  Control phase
     * ================================= */
    bool isStreamerFail = (retVal != KOSA_StatusSuccess);
    bool isBufferEmpty  = (s_demo_isBufferUnchanged());

    if (isStreamerFail || isBufferEmpty)
    {
        printk("\r\nDemo result: FAIL\r\n");
        return -1;
    } else
    {
#if 0
        printk("\n\rData: ");
        for(int i = 0; i < RECORD_SAMPLES_COUNT; i = i + 2)
        {
            printk("%x%x ", s_mem2mem_bufferDst[i], s_mem2mem_bufferDst[i+1]);
        }
#endif
    }

    printk("\r\nDemo result: OK\r\n");
    return 0;
}