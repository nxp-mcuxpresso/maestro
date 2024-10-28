/*
 * Copyright 2022 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdint.h>
#include <stdbool.h>
#include "unity.h"
#include "assert.h"
#include "string.h"

#include "streamer_api.h"
#include "streamer_message.h"
#include "streamer_element_properties.h"
#include "streamer_pcm.h"
#include "streamer_test.h"

#define STREAMER_TASK_STACK_SIZE         30 * 1024
#define STREAMER_MESSAGE_TASK_STACK_SIZE 1024
#define STREAMER_TASK_NAME               "streamer"
#define STREAMER_MSGTASK_NAME            "streamerMsg"
#define STREAMER_MSGQ_NAME               "streamerMsqQ"

OSA_TASK_HANDLE_DEFINE(msgThread);

bool end_of_stream = false;

void MessageThreadProc(void *arg)
{
    STREAMER_MSG_T msg;
    bool exit_thread = false;
    osa_status_t ret;
    OSA_SR_ALLOC()

    STREAMER_T *streamer = (STREAMER_T *)arg;

    while (streamer->is_mq_out_created == false)
    {
        OSA_TimeDelay(1);
    }

    if (streamer->is_mq_out_created == false)
    {
        OSA_TaskDestroy(msgThread);
        PRINTF("[MessageThreadProc] osa_mq_open failed: %d\r\n");
        return;
    }

    do
    {
        ret = OSA_MsgQGet(&streamer->mq_out, (void *)&msg, osaWaitForever_c);
        if (ret != KOSA_StatusSuccess)
        {
            PRINTF("[MessageThreadProc] OSA_MsgQGet error: %d\r\n", ret);
            continue;
        }

        switch (msg.id)
        {
            case STREAM_MSG_DESTROY_PIPELINE:
                // PRINTF("[MessageThreadProc] STREAM_MSG_DESTROY_PIPELINE\r\n", ret);
                break;
            case STREAM_MSG_ERROR:
                // PRINTF("[MessageThreadProc] STREAM_MSG_ERROR\r\n");
                break;
            case STREAM_MSG_EOS:
                // PRINTF("[MessageThreadProc] STREAM_MSG_EOS  %d\r\n", msg.pipeline_type);
                end_of_stream = true;
                break;
            case STREAM_MSG_UPDATE_POSITION:
                // PRINTF("[MessageThreadProc] STREAM_MSG_UPDATE_POSITION..\r\n");
                break;
            case STREAM_MSG_UPDATE_DURATION:
                // PRINTF("[MessageThreadProc] STREAM_MSG_UPDATE_DURATION: %d\r\n", msg.event_data);
                break;
            case STREAM_MSG_CLOSE_TASK:
                // PRINTF("[MessageThreadProc] STREAM_MSG_CLOSE_TASK\r\n");
                exit_thread = true;
                break;
            default:
                break;
        }
    } while (!exit_thread);

    OSA_ENTER_CRITICAL();
    OSA_MsgQDestroy(&streamer->mq_out);
    streamer->is_mq_out_created = false;
    OSA_EXIT_CRITICAL();

    OSA_TaskDestroy(msgThread);
}

STREAMER_T *TestStreamerCreate(STREAMER_CREATE_PARAM *paramPtr, bool assertCreate, const char *pipeline_name)
{
    ElementIndex element_ids[]        = {ELEMENT_NETBUF_SRC_INDEX};
    PipelineElements pipelineElements = {element_ids, sizeof(element_ids) / sizeof(ElementIndex)};
    // Fill local parameters if neccessary
    STREAMER_CREATE_PARAM localParams;
    if (paramPtr == NULL)
    {
        localParams.stack_size   = STREAMER_TASK_STACK_SIZE;
        localParams.task_name    = STREAMER_TASK_NAME;
        localParams.in_dev_name  = "";
        localParams.out_dev_name = "";
        localParams.elements     = pipelineElements;
        strcpy(localParams.out_mq_name, STREAMER_MSGQ_NAME);
    }
    else
    {
        paramPtr->stack_size = STREAMER_TASK_STACK_SIZE;
        paramPtr->task_name  = STREAMER_TASK_NAME;
        strcpy(paramPtr->out_mq_name, STREAMER_MSGQ_NAME);
    }

    streamer_pcm_init();

    // Create the streamer
    STREAMER_T *streamer = streamer_create(paramPtr != NULL ? paramPtr : &localParams);
    if (streamer == NULL)
    {
        if (assertCreate)
        {
            TestStreamerCreateMessage(streamer == NULL, "streamer_create returned NULL", pipeline_name);
        }
        return NULL;
    }

    // Create message thread
    osa_task_def_t thread_attr;
    thread_attr.tpriority = OSA_PRIORITY_HIGH;
    thread_attr.tname     = (uint8_t *)STREAMER_MSGTASK_NAME;
    thread_attr.pthread   = &MessageThreadProc;
    thread_attr.stacksize = STREAMER_MESSAGE_TASK_STACK_SIZE;
    int ret               = OSA_TaskCreate(&msgThread, &thread_attr, (void *)streamer);
    if (ret != KOSA_StatusSuccess)
    {
        (void)streamer_destroy(streamer);
        streamer = NULL;
        TestStreamerCreateMessage(true, "failed to create streamer's message thread", pipeline_name);
    }

    return streamer;
}

void TestStreamerDestroy(STREAMER_T **streamer, bool success, const char *pipeline_name)
{
    int32_t ret = 0;

    TestStreamerCreateMessage(*streamer == NULL, "bad streamer argument in TestStreamerDestroy", pipeline_name);

    if (*streamer != NULL)
    {
        ret       = streamer_destroy(*streamer);
        *streamer = NULL;
    }

    if (success)
    {
        TestStreamerCreateMessage(ret != 0, "failed to destroy streamer", pipeline_name);
    }
    else
    {
        TestStreamerCreateMessage(ret == 0, "streamer destroy was successful", pipeline_name);
    }
}

void TestStreamerCreateMessage(bool createMsg, char *message_in, const char *pipeline_name)
{
    char message_out[80] = {0};

    if (createMsg == true)
    {
        if (pipeline_name == NULL)
        {
            TEST_FAIL_MESSAGE(message_in);
            return;
        }

        strcpy(message_out, message_in);
        strcat(message_out, " in pipeline: ");
        strcat(message_out, pipeline_name);

        TEST_FAIL_MESSAGE(message_out);
    }
}

bool TestStreamerGetEOSValue(void)
{
    return end_of_stream;
}

void TestStreamerClearEOSValue(void)
{
    end_of_stream = false;
}
