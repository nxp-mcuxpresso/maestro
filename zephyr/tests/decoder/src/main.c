/**
 * Copyright 2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 *  @file  main.c
 *  @brief Maestro decoder test - Compares decoded data with its reference.
 */

#include <stdio.h>

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(maestro_demo, LOG_LEVEL_WRN);

#include "fsl_common.h"
#include "fsl_os_abstraction.h"
#include "fsl_debug_console.h"
#include "maestro_logging.h"

#include "streamer_api.h"
#include "streamer_element_properties.h"
#include "error.h"

#include "Tone_CH0_440HZ_CH1_660HZ_48KHZ_2CHAN_16b.raw.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define DEMO_OUT_QUEUE_NAME             "demo_streamer_outQueue"
#define DEMO_STREAMER_TASK_STACK_SIZE_B (CONFIG_MAESTRO_EXAMPLE_STREAMER_TASK_STACK_SIZE_B)
#define DEMO_STREAMER_TASK_NAME         "demo_streamer_task"
#define DEMO_IN_DEV_NAME                "input memory"
#define DEMO_OUT_DEV_NAME               "output memory"
#define DEMO_TIMEOUT_MS                 (50u)

#if defined(CONFIG_MAESTRO_EXAMPLE_DECODER_SELECTED_MP3)
    #include "Tone_CH0_440HZ_CH1_660HZ_48KHZ_2CHAN_16b.mp3.h"
    #define DEMO_DECODER_NAME "MP3"
    #define DEMO_SOURCE_TRACK_ENCODED_DATA Tone_CH0_440HZ_CH1_660HZ_48KHZ_2CHAN_16b_mp3
    #define DEMO_SOURCE_TRACK_DECODED_DATA Tone_CH0_440HZ_CH1_660HZ_48KHZ_2CHAN_16b_raw
    #define DEMO_MAESTRO_DECODER_TYPE (DECODER_TYPE_MP3)
#elif defined(CONFIG_MAESTRO_EXAMPLE_DECODER_SELECTED_OPUS)
    #warning "TODO: Need to specify OPUS encoded file"
    #define DEMO_DECODER_NAME "OPUS"
    #define DEMO_SOURCE_TRACK_ENCODED_DATA Tone_CH0_440HZ_CH1_660HZ_opus
    #define DEMO_SOURCE_TRACK_DECODED_DATA Tone_CH0_440HZ_CH1_660HZ_raw
    #define DEMO_MAESTRO_DECODER_TYPE (DECODER_TYPE_OPUS)
#elif defined(CONFIG_MAESTRO_EXAMPLE_DECODER_SELECTED_OPUS_OGG)
    #include "Tone_CH0_440HZ_CH1_660HZ_48KHZ_2CHAN_16b.opus_ogg.h"
    #include "Tone_CH0_440HZ_CH1_660HZ_48KHZ_2CHAN_16b.opus_ogg_raw.h"
    #define DEMO_DECODER_NAME "OPUS OGG"
    #define DEMO_SOURCE_TRACK_ENCODED_DATA Tone_CH0_440HZ_CH1_660HZ_48KHZ_2CHAN_16b_opus_ogg
    #define DEMO_SOURCE_TRACK_DECODED_DATA Tone_CH0_440HZ_CH1_660HZ_48KHZ_2CHAN_16b_opus_ogg_raw
    #define DEMO_MAESTRO_DECODER_TYPE (DECODER_TYPE_OGG_OPUS)
#elif defined(CONFIG_MAESTRO_EXAMPLE_DECODER_SELECTED_WAV)
    #include "Tone_CH0_440HZ_CH1_660HZ_48KHZ_2CHAN_16b.wav.h"
    #define DEMO_DECODER_NAME "WAV"
    #define DEMO_SOURCE_TRACK_ENCODED_DATA Tone_CH0_440HZ_CH1_660HZ_48KHZ_2CHAN_16b_wav
    #define DEMO_SOURCE_TRACK_DECODED_DATA Tone_CH0_440HZ_CH1_660HZ_48KHZ_2CHAN_16b_raw
    #define DEMO_MAESTRO_DECODER_TYPE (DECODER_TYPE_WAV)
#elif defined(CONFIG_MAESTRO_EXAMPLE_DECODER_SELECTED_FLAC)
    #include "Tone_CH0_440HZ_CH1_660HZ_48KHZ_2CHAN_16b.flac.h"
    #define DEMO_DECODER_NAME "FLAC"
    #define DEMO_SOURCE_TRACK_ENCODED_DATA Tone_CH0_440HZ_CH1_660HZ_48KHZ_2CHAN_16b_flac
    #define DEMO_SOURCE_TRACK_DECODED_DATA Tone_CH0_440HZ_CH1_660HZ_48KHZ_2CHAN_16b_raw
    #define DEMO_MAESTRO_DECODER_TYPE (DECODER_TYPE_FLAC)
#elif defined(CONFIG_MAESTRO_EXAMPLE_DECODER_SELECTED_AAC)
    #include "Tone_CH0_440HZ_CH1_660HZ_48KHZ_2CHAN_16b.m4a.h"
    #define DEMO_DECODER_NAME "AAC"
    #define DEMO_SOURCE_TRACK_ENCODED_DATA Tone_CH0_440HZ_CH1_660HZ_48KHZ_2CHAN_16b_m4a
    #define DEMO_SOURCE_TRACK_DECODED_DATA Tone_CH0_440HZ_CH1_660HZ_48KHZ_2CHAN_16b_raw
    #define DEMO_MAESTRO_DECODER_TYPE (DECODER_TYPE_AAC)
#else
    #error "No decoder selected"
#endif

#define DEMO_SOURCE_TRACK_ENCODED_SIZE_B sizeof(DEMO_SOURCE_TRACK_ENCODED_DATA)
#define DEMO_SOURCE_TRACK_DECODED_SIZE_B sizeof(DEMO_SOURCE_TRACK_DECODED_DATA)
#define DEMO_SOURCE_TRACK_DECODED_SIZE_B_MARGIN (128u)

#define DEMO_BUFFER_SOURCE_SIZE_B (DEMO_SOURCE_TRACK_ENCODED_SIZE_B)
#define DEMO_BUFFER_DEST_SIZE_B   (DEMO_SOURCE_TRACK_DECODED_SIZE_B + DEMO_SOURCE_TRACK_DECODED_SIZE_B_MARGIN)


/*******************************************************************************
 * Prototypes
 ******************************************************************************/

static int s_runTest(void);
static void s_demo_streamerInitLogging(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/

static STREAMER_T *s_streamerPtr;

static uint8_t s_demo_bufferDst[DEMO_BUFFER_DEST_SIZE_B]   = {0};

static MEMSRC_SET_BUFFER_T s_demo_streamer_bufferSrc = {0};
static SET_BUFFER_DESC_T s_demo_streamer_bufferDst   = {0};

/*******************************************************************************
 * Code
 *******************************************************************************/

int main(void)
{
    printk("== %s Decoder test ==", DEMO_DECODER_NAME);

    return s_runTest();
}

static void s_demo_prepareBuffers(void)
{
    memset(s_demo_bufferDst, 0x00, DEMO_BUFFER_DEST_SIZE_B);
}

static bool s_demo_compareBuffers(void)
{
    if(0 == memcmp(s_demo_bufferDst, DEMO_SOURCE_TRACK_DECODED_DATA, DEMO_SOURCE_TRACK_DECODED_SIZE_B))
    {
        return true;
    }

    return false;
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

static osa_status_t s_demo_prepareStreamer()
{
    int retVal;

    // Define memory buffers for streamer
    s_demo_streamer_bufferSrc =
        (MEMSRC_SET_BUFFER_T){.location = (int8_t *)DEMO_SOURCE_TRACK_ENCODED_DATA, .size = DEMO_BUFFER_SOURCE_SIZE_B};
    s_demo_streamer_bufferDst =
        (SET_BUFFER_DESC_T){.ptr = (int8_t *)s_demo_bufferDst, .size = DEMO_BUFFER_DEST_SIZE_B};

    // Define element parameters
    STREAMER_CREATE_PARAM streamer_params;
    ELEMENT_PROPERTY_T element_prop;

    // Setup Pipeline elements
    ElementIndex elementsList_ids[]    = {ELEMENT_MEM_SRC_INDEX, ELEMENT_DECODER_INDEX, ELEMENT_MEM_SINK_INDEX};
    int elements_cnt                   = sizeof(elementsList_ids) / sizeof(ElementIndex);
    PipelineElements pipeline_elements = {elementsList_ids, elements_cnt};

    // Setup streamer parameters
    strcpy(streamer_params.out_mq_name, DEMO_OUT_QUEUE_NAME);
    streamer_params.stack_size   = DEMO_STREAMER_TASK_STACK_SIZE_B;
    streamer_params.task_name    = DEMO_STREAMER_TASK_NAME;
    streamer_params.in_dev_name  = DEMO_IN_DEV_NAME;
    streamer_params.out_dev_name = DEMO_OUT_DEV_NAME;
    streamer_params.elements     = pipeline_elements;

    s_demo_streamerInitLogging();

    // Create streamer
    s_streamerPtr = streamer_create(&streamer_params);
    if (s_streamerPtr == NULL)
    {
        LOG_ERR("Streamer create failed\r\n");
        return KOSA_StatusError;
    }

    // Setting element properties
    element_prop.prop = PROP_MEMSRC_SET_BUFF;
    element_prop.val  = (uintptr_t)&s_demo_streamer_bufferSrc;
    retVal            = streamer_set_property(s_streamerPtr, 0, element_prop, true);
    if (retVal != STREAM_OK)
    {
        LOG_ERR("Streamer set property failed\r\n");
        streamer_destroy(s_streamerPtr);
        s_streamerPtr = NULL;

        return KOSA_StatusError;
    }

    element_prop.prop = PROP_MEMSINK_BUFFER_DESC;
    element_prop.val  = (uintptr_t)&s_demo_streamer_bufferDst;
    retVal            = streamer_set_property(s_streamerPtr, 0, element_prop, true);
    if (retVal != STREAM_OK)
    {
        LOG_ERR("Streamer set property failed\r\n");
        streamer_destroy(s_streamerPtr);
        s_streamerPtr = NULL;

        return KOSA_StatusError;
    }

    element_prop.prop = PROP_DECODER_DECODER_TYPE;
    element_prop.val  = (uintptr_t) DEMO_MAESTRO_DECODER_TYPE;
    retVal            = streamer_set_property(s_streamerPtr, 0, element_prop, true);
    if (retVal != STREAM_OK)
    {
        LOG_ERR("Streamer set property failed\r\n");
        streamer_destroy(s_streamerPtr);
        s_streamerPtr = NULL;

        return KOSA_StatusError;
    }

    return kStatus_Success;
}

static osa_status_t s_demo_cleanup()
{
    if (OSA_MsgQDestroy(s_streamerPtr->message_channel_out.mq) != KOSA_StatusSuccess)
    {
        LOG_ERR("Could not destroy out message queue\r\n");
        return KOSA_StatusError;
    }
    s_streamerPtr->message_channel_out.is_mq_created = false;

    if (streamer_destroy(s_streamerPtr) != ERRCODE_NO_ERROR)
    {
        LOG_ERR("Could not destroy streamer\r\n");
        return KOSA_StatusError;
    }

    return KOSA_StatusSuccess;
}

static osa_status_t s_demo_runStreamer()
{
    // Start streamer
    LOG_DBG("Start encoding\r\n");
    streamer_set_state(s_streamerPtr, 0, STATE_PLAYING, true);

    // Process responses from streamer task
    osa_status_t retVal;
    STREAMER_MSG_T msg;
    do
    {
        retVal = OSA_MsgQGet(s_streamerPtr->message_channel_out.mq, (void *)&msg, DEMO_TIMEOUT_MS);
        switch (retVal)
        {
            case KOSA_StatusTimeout:
                // When there is no message in queue, timeout should occur.
                // Leading to cleaning process and jumping out of this function.
                LOG_ERR("Message queue get timeouted\r\n");
                break;
            case KOSA_StatusError:
                LOG_ERR("Message queue get error\r\n");
                break;
            case KOSA_StatusSuccess:
                // Message obtained
                LOG_DBG("Message queue get message id: %u\r\n", msg.id);
                break;
            default:
                LOG_ERR("Unhandled message get response");
                break;
        }
        if (retVal != KOSA_StatusSuccess)
        {
            s_demo_cleanup();
            return KOSA_StatusError;
        }

        switch (msg.id)
        {
            case STREAM_MSG_ERROR:
                LOG_ERR("Streamer MSG - error id: %d\r\n", msg.id);
                s_demo_cleanup();
                return KOSA_StatusError;
            case STREAM_MSG_EOS:
                LOG_DBG("Streamer MSG - end of stream\r\n");
                s_demo_cleanup();
                return KOSA_StatusSuccess;
            case STREAM_MSG_CLOSE_TASK:
                LOG_DBG("Streamer MSG - close task\r\n");
                s_demo_cleanup();
                return KOSA_StatusSuccess;
            case STREAM_MSG_UPDATE_DURATION:
            case STREAM_MSG_UPDATE_POSITION:
            case STREAM_MSG_UPDATE_TRACK_INFO:
                break;
            default:
                LOG_WRN("Streamer MSG - got message with undefined reaction\r\n");
                break;
        }
    } while (true);

    // Should not get there
    return KOSA_StatusError;
}

static int s_runTest(void)
{
    osa_status_t retVal = KOSA_StatusSuccess;
    LOG_DBG("Test Preparation\r\n");

    // =================================
    // Prepare phase
    // =================================
    s_demo_prepareBuffers();
    retVal = s_demo_prepareStreamer();

    // =================================
    // Run phase
    // =================================
    if (KOSA_StatusSuccess == retVal)
    {
        LOG_DBG("Test Start\r\n");
        retVal = s_demo_runStreamer();
    }

    // =================================
    // Control phase
    // =================================
    bool isStreamerFail   = (retVal != KOSA_StatusSuccess);
    bool areBuffersDiffer = (s_demo_compareBuffers() != true);

    if (isStreamerFail || areBuffersDiffer)
    {
        printk("Result: FAIL\r\n");
        return -1;
    }

    printk("Result: OK\r\n");
    return 0;
}
