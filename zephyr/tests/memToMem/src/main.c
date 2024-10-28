/**
 *  Copyright 2023-2024 NXP
 *
 *  SPDX-License-Identifier: Apache-2.0
 *
 *  @file  main.c
 *  @brief Maestro test - Testing streamer pipeline moving data from source to destination memory.
 */

#include <stdio.h>

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(maestro_mem2mem, LOG_LEVEL_WRN);

#include "fsl_common.h"
#include "fsl_os_abstraction.h"
#include "fsl_debug_console.h"

#include "streamer_api.h"
#include "streamer_element_properties.h"
#include "error.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define MEM2MEM_OUT_QUEUE_NAME             "mem2Mem_streamer_outQueue"
#define MEM2MEM_STREAMER_TASK_STACK_SIZE_B (1024u)
#define MEM2MEM_STREAMER_TASK_NAME         "mem2Mem_streamer_task"
#define MEM2MEM_IN_DEV_NAME                "input memory"
#define MEM2MEM_OUT_DEV_NAME               "output memory"

#define MEM2MEM_TIMEOUT_MS    (50u)
#define MEM2MEM_BUFFER_SIZE_B (0xFFu)

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

static int s_runTest(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/

static STREAMER_T *s_streamerPtr;

static uint8_t s_mem2mem_bufferSrc[MEM2MEM_BUFFER_SIZE_B] = {0};
static uint8_t s_mem2mem_bufferDst[MEM2MEM_BUFFER_SIZE_B] = {0};

static MEMSRC_SET_BUFFER_T s_mem2mem_streamer_bufferSrc = {0};
static SET_BUFFER_DESC_T s_mem2mem_streamer_bufferDst   = {0};

/*******************************************************************************
 * Code
 *******************************************************************************/

int main(void)
{
    // Run the test
    return s_runTest();
}

static void s_mem2mem_prepareBuffers(void)
{
    for (uint32_t i = 0; i < MEM2MEM_BUFFER_SIZE_B; i++)
    {
        s_mem2mem_bufferSrc[i] = i;
        s_mem2mem_bufferDst[i] = 0;
    }
}

static bool s_mem2mem_compareBuffers(void)
{
    for (uint32_t i = 0; i < MEM2MEM_BUFFER_SIZE_B; i++)
    {
        if (s_mem2mem_bufferSrc[i] != s_mem2mem_bufferDst[i])
        {
            return false;
        }
    }
    return true;
}

static osa_status_t s_mem2mem_prepareStreamer()
{
    int retVal;

    // Define memory buffers for streamer
    s_mem2mem_streamer_bufferSrc =
        (MEMSRC_SET_BUFFER_T){.location = (int8_t *)s_mem2mem_bufferSrc, .size = MEM2MEM_BUFFER_SIZE_B};
    s_mem2mem_streamer_bufferDst =
        (SET_BUFFER_DESC_T){.ptr = (int8_t *)s_mem2mem_bufferDst, .size = MEM2MEM_BUFFER_SIZE_B};

    // Define element parameters
    STREAMER_CREATE_PARAM streamer_params;
    ELEMENT_PROPERTY_T element_prop;

    // Setup Pipeline elements
    ElementIndex elementsList_ids[]    = {ELEMENT_MEM_SRC_INDEX, ELEMENT_MEM_SINK_INDEX};
    int elements_cnt                   = sizeof(elementsList_ids) / sizeof(ElementIndex);
    PipelineElements pipeline_elements = {elementsList_ids, elements_cnt};

    // Setup streamer parameters
    strcpy(streamer_params.out_mq_name, MEM2MEM_OUT_QUEUE_NAME);
    streamer_params.stack_size   = MEM2MEM_STREAMER_TASK_STACK_SIZE_B;
    streamer_params.task_name    = MEM2MEM_STREAMER_TASK_NAME;
    streamer_params.in_dev_name  = MEM2MEM_IN_DEV_NAME;
    streamer_params.out_dev_name = MEM2MEM_OUT_DEV_NAME;
    streamer_params.elements     = pipeline_elements;

    // Create streamer
    s_streamerPtr = streamer_create(&streamer_params);
    if (s_streamerPtr == NULL)
    {
        LOG_ERR("Streamer create failed\r\n");
        return KOSA_StatusError;
    }

    // Setting element properties
    element_prop.prop = PROP_MEMSRC_SET_BUFF;
    element_prop.val  = (uintptr_t)&s_mem2mem_streamer_bufferSrc;
    retVal            = streamer_set_property(s_streamerPtr, 0, element_prop, true);
    if (retVal != STREAM_OK)
    {
        LOG_ERR("Streamer set property failed\r\n");
        streamer_destroy(s_streamerPtr);
        s_streamerPtr = NULL;

        return KOSA_StatusError;
    }

    element_prop.prop = PROP_MEMSINK_BUFFER_DESC;
    element_prop.val  = (uintptr_t)&s_mem2mem_streamer_bufferDst;
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

static osa_status_t s_mem2mem_cleanup()
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

static osa_status_t s_mem2mem_runStreamer()
{
    // Start streamer
    LOG_DBG("Start encoding\r\n");
    streamer_set_state(s_streamerPtr, 0, STATE_PLAYING, true);

    // Process responses from streamer task
    osa_status_t retVal;
    STREAMER_MSG_T msg;
    do
    {
        retVal = OSA_MsgQGet(s_streamerPtr->message_channel_out.mq, (void *)&msg, MEM2MEM_TIMEOUT_MS);
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
            s_mem2mem_cleanup();
            return KOSA_StatusError;
        }

        switch (msg.id)
        {
            case STREAM_MSG_ERROR:
                LOG_ERR("Streamer MSG - error id: %u\r\n", msg.id);
                s_mem2mem_cleanup();
                return KOSA_StatusError;
            case STREAM_MSG_EOS:
                LOG_DBG("Streamer MSG - end of stream\r\n");
                s_mem2mem_cleanup();
                return KOSA_StatusSuccess;
            case STREAM_MSG_CLOSE_TASK:
                LOG_DBG("Streamer MSG - close task\r\n");
                s_mem2mem_cleanup();
                return KOSA_StatusSuccess;
            default:
                LOG_WRN("Streamer MSG - got message with undefined reaction\r\n");
                break;
        }
    } while (true);

    // Should not get there
    return KOSA_StatusError;
}

// static int s_shell_startMem2Mem(const struct shell *sh, size_t argc, char **argv)
static int s_runTest(void)
{
    osa_status_t retVal = KOSA_StatusSuccess;
    LOG_DBG("Test Preparation\r\n");

    // =================================
    // Prepare phase
    // =================================
    s_mem2mem_prepareBuffers();
    retVal = s_mem2mem_prepareStreamer();

    // =================================
    // Run phase
    // =================================
    if (KOSA_StatusSuccess == retVal)
    {
        LOG_DBG("Test Start\r\n");
        retVal = s_mem2mem_runStreamer();
    }

    // =================================
    // Control phase
    // =================================
    bool isStreamerFail   = (retVal != KOSA_StatusSuccess);
    bool areBuffersDiffer = (s_mem2mem_compareBuffers() != true);

    if (isStreamerFail || areBuffersDiffer)
    {
        printk("Result: FAIL\r\n");
        return -1;
    }

    printk("Result: OK\r\n");
    return 0;
}
