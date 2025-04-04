/*
 * Copyright 2018-2022, 2024 NXP.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*!
 * @file    streamer.c
 * @brief   Af Streamer implementation
 */

/*
 * INCLUDE FILES
 */
#include <string.h>

#include "streamer_element.h"
#include "pipeline.h"

/*!
 * @ingroup streamer
 * @{
 */

/*
 * GLOBAL CONSTANTS
 */

/**
 * @brief Maximum messages received per streamer task cycle
 *
 */
#define MAX_MSG_RECV_PER_CYCLE (5)

/*
 * GLOBAL VARIABLES
 */

/**
 * @brief Element type lookup table for getting element type from element index
 */
static ElementTypeLookup type_lookup_table[] = {
    {TYPE_ELEMENT_FILE_SRC, ELEMENT_FILE_SRC_INDEX},     {TYPE_ELEMENT_MEM_SRC, ELEMENT_MEM_SRC_INDEX},
    {TYPE_ELEMENT_NETBUF_SRC, ELEMENT_NETBUF_SRC_INDEX}, {TYPE_ELEMENT_AUDIO_SRC, ELEMENT_MICROPHONE_INDEX},
    {TYPE_ELEMENT_FILE_SINK, ELEMENT_FILE_SINK_INDEX},   {TYPE_ELEMENT_MEM_SINK, ELEMENT_MEM_SINK_INDEX},
    {TYPE_ELEMENT_AUDIO_SINK, ELEMENT_SPEAKER_INDEX},    {TYPE_ELEMENT_DECODER, ELEMENT_DECODER_INDEX},
    {TYPE_ELEMENT_ENCODER, ELEMENT_ENCODER_INDEX},       {TYPE_ELEMENT_PROCESS_SINK, ELEMENT_VIT_INDEX},
    {TYPE_ELEMENT_AUDIO_PROC, ELEMENT_VIT_PROC_INDEX},   {TYPE_ELEMENT_AUDIO_PROC, ELEMENT_VOICESEEKER_INDEX},
    {TYPE_ELEMENT_AUDIO_PROC, ELEMENT_SRC_INDEX},        {TYPE_ELEMENT_AUDIO_SRC, ELEMENT_USB_SRC_INDEX},
    {TYPE_ELEMENT_AUDIO_SINK, ELEMENT_USB_SINK_INDEX},   {TYPE_ELEMENT_AUDIO_PROC, ELEMENT_ASRC_INDEX}};

/**
 * @brief Osa task handle object
 *
 */
OSA_TASK_HANDLE_DEFINE(thread_hdl);

/*!
 * Streamer Message Handler Table
 * Specifies the function to handle incoming message requests within the
 * streamer task.
 */
const STREAMER_MSG_HANDLER_T msg_handler_table[] = {
#ifdef STREAMER_ENABLE_FILESRC
    {STREAM_MSG_SET_FILE, streamer_msg_set_file},
#endif
    {STREAM_MSG_SET_STATE, streamer_msg_set_state}, {STREAM_MSG_GET_STATE, streamer_msg_get_state},         
    {STREAM_MSG_QUERY_PIPELINE, streamer_msg_send_query}, {STREAM_MSG_SET_PROPERTY, streamer_msg_set_property},   
    {STREAM_MSG_GET_PROPERTY, streamer_msg_get_property}, {STREAM_MSG_SEEK_PIPELINE, streamer_msg_seek_pipeline}, 
    {STREAM_MSG_SET_REPEAT, streamer_msg_set_repeat}};
/*!
 * @brief    Streamer deinit
 * @details  Deinit all data associated with Streamer Task
 *
 * @param    [in] streamer Pointer to streamer task data structure
 */
static int32_t streamer_deinit(STREAMER_T *streamer);

/*
 * GLOBAL FUNCTIONS
 */

STREAMER_T *streamer_create(STREAMER_CREATE_PARAM *task_param)
{
    int32_t ret = ERRCODE_NO_ERROR;
    osa_task_def_t thread_attr = {0};
    STREAMER_T *streamer = NULL;
    OSA_SR_ALLOC()

    STREAMER_FUNC_ENTER(DBG_CORE);

    /* Make sure an Application MsgQ was provided */
    if (NULL == task_param)
    {
        STREAMER_LOG_ERR(DBG_CORE, ERRCODE_GENERAL_ERROR, "STREAMER create- Invalid parameter provided\n");
        ret = ERRCODE_INVALID_ARGUMENT;
    }
    else
    {
        streamer = OSA_MemoryAllocate(sizeof(STREAMER_T));
        if (streamer)
        {
            streamer_init(streamer);

            if (task_param->out_mq_name[0] == 0)
            {
                STREAMER_LOG_WARN(DBG_CORE, 0,
                                  "No out message queue name "
                                  "provided, no streamer message will be sent out\n");
            }
            else
            {
                OSA_ENTER_CRITICAL();
                ret = OSA_MsgQCreate(streamer->message_channel_out.mq, MSG_POOL_NUM_MSGS, STREAMER_MSG_SIZE);
                if (ERRCODE_NO_ERROR != ret)
                {
                    streamer->message_channel_out.is_mq_created = false;
                    OSA_EXIT_CRITICAL();
                    STREAMER_LOG_ERR(DBG_CORE, ret, "streamer unable to open out message queue\n");
                } else
                {
                    streamer->message_channel_out.is_mq_created = true;
                    OSA_EXIT_CRITICAL();
                }
            }
            thread_attr.stacksize = task_param->stack_size;
            thread_attr.tpriority = OSA_PRIORITY_ABOVE_NORMAL;
            thread_attr.tname     = (uint8_t *)task_param->task_name;
            thread_attr.pthread   = &streamer_task;
            ret                   = OSA_TaskCreate(thread_hdl, &thread_attr, streamer);
            uint32_t wait_cnt     = 200;
            if (ERRCODE_NO_ERROR == ret)
            {
                /* wait for streamer task running */
                wait_cnt = 200;
                while (!streamer->is_active && wait_cnt--)
                {
                    OSA_TimeDelay(5);
                }

                if (streamer->is_active)
                {
                    /* create pipeline */
                    ret = streamer_create_pipeline(streamer, 0, task_param->in_dev_name, task_param->out_dev_name,
                                                   task_param->elements, false);
                    if (ERRCODE_NO_ERROR == ret)
                    {
                        STREAMER_FUNC_EXIT(DBG_CORE);
                        return streamer;
                    }
                }
            }

            STREAMER_LOG_ERR(DBG_CORE, ERRCODE_THREAD_FAILURE, "create streamer task failed:%d\n", ret);
            /* send an task close event to streamer to exit streamer task */
            STREAMER_MSG_T msg;
            msg.id = STREAM_MSG_CLOSE_TASK;

            if (streamer->streamer_mq[0] != (uint32_t) NULL)
            {
                wait_cnt = 200;
                OSA_MsgQPut(streamer->streamer_mq, (void *)&msg);
                while (streamer->is_active && wait_cnt--)
                    OSA_TimeDelay(2000);
            }

            if (streamer->is_active)
                STREAMER_LOG_ERR(DBG_CORE, ERRCODE_THREAD_FAILURE, "Failed to exit streamer task!\n");

            OSA_ENTER_CRITICAL();
            OSA_MsgQDestroy(streamer->message_channel_out.mq);
            streamer->message_channel_out.is_mq_created = false;
            OSA_EXIT_CRITICAL();

            OSA_MemoryFree(streamer);
            streamer = NULL;
        }
    }

    STREAMER_FUNC_EXIT(DBG_CORE);
    return streamer;
}

int32_t streamer_destroy(STREAMER_T *streamer)
{
    int32_t ret = ERRCODE_NO_ERROR;

    if (NULL == streamer)
    {
        STREAMER_FUNC_EXIT(DBG_CORE);
        return ERRCODE_INVALID_ARGUMENT;
    }

    /* Send a state-changing to STATE_NULL event to close resources allocated by elements down. */
    for (size_t i = 0; i < MAX_PIPELINES; i++)
    {
        if (streamer->pipes[i] != (uintptr_t)NULL)
        {
            ret |= streamer_set_state(streamer, i, STATE_NULL, true);
        }
    }

    /* send an task close event to streamer to exit streamer task */
    STREAMER_MSG_T msg;
    msg.id = STREAM_MSG_CLOSE_TASK;

    uint32_t wait_cnt = 1000;
    OSA_MsgQPut(streamer->streamer_mq, (void *)&msg);
    while (streamer->is_active && wait_cnt--)
        OSA_TimeDelay(10);

    if (streamer->is_active)
        STREAMER_LOG_ERR(DBG_CORE, ERRCODE_THREAD_FAILURE, "Failed to exit streamer task!\n");

    for (size_t i = 0; i < MAX_PIPELINES; i++)
    {
        if (streamer->pipes[i] != (uintptr_t)NULL)
        {
            ret |= streamer_destroy_pipeline(streamer, i, false);
        }
    }
    if (streamer->message_channel_out.is_mq_created)
    {
        OSA_MsgQPut(streamer->message_channel_out.mq, (void *)&msg);
        while (streamer->message_channel_out.is_mq_created)
        {
            OSA_TimeDelay(1);
        }
    }
    ret |= streamer_deinit(streamer);
    OSA_MemoryFree(streamer);
    streamer = NULL;

    STREAMER_FUNC_EXIT(DBG_CORE);

    return ret;
}

/*! @} */

/*
 * LOCAL FUNCTIONS
 */

void streamer_task(void *args)
{
    uint32_t loop_count;
    int32_t msg_count;
    uint32_t msg_timed_out;
    int32_t ret;
    STREAMER_MSG_T msg;
    uint32_t active_pipes;

    STREAMER_FUNC_ENTER(DBG_CORE);

    STREAMER_T *streamer = (STREAMER_T *)args;

    /* Open Streamer Task MsgQ */
    if (OSA_MsgQCreate(streamer->streamer_mq, MSG_POOL_NUM_MSGS, STREAMER_MSG_SIZE))
    {
        STREAMER_LOG_ERR(DBG_CORE, ERRCODE_SYSTEM_LIMIT, "Streamer open msg queue failed\n");
        STREAMER_FUNC_EXIT(DBG_CORE);
        return;
    }

    /* MAIN LOOP */
    streamer->is_active = true;
    while ((uint8_t) false == streamer->exit_thread)
    {
        active_pipes = 0;
        /* See if any pipelines are active */
        for (loop_count = 0; loop_count < MAX_PIPELINES; loop_count++)
        {
            if (streamer->pipes[loop_count])
            {
                if (STATE_PLAYING == get_state_pipeline(streamer->pipes[loop_count]))
                {
                    active_pipes = 1;
                    break;
                }
            }
        }

        if (0 == active_pipes)
        {
            /* Blocking for message */
            ret = OSA_MsgQGet(streamer->streamer_mq, (char *)&msg, osaWaitForever_c);
            if (ERRCODE_NO_ERROR == ret)
            {
                /* Handle a message */
                if (msg.id == STREAM_MSG_CLOSE_TASK)
                {
                    streamer->exit_thread = true;
                    break;
                }

                ret = streamer_msg_handler(streamer, &msg);
                if (ERRCODE_NOT_SEEKABLE == ret)
                {
                    STREAMER_LOG_ERR(DBG_CORE, ret, "STREAMER - ERROR PROCESSING MESSAGE,%d\n", msg.id);
                }
                else if (ERRCODE_NO_ERROR != ret)
                {
                    STREAMER_LOG_ERR(DBG_CORE, ret, "STREAMER - ERROR PROCESSING MESSAGE,%d\n", msg.id);

                    msg.id         = STREAM_MSG_ERROR;
                    msg.errorcode  = ret;
                    msg.event_data = ret;
                    msg.state      = STATE_NULL;
                    if (ERRCODE_NO_ERROR != OSA_MsgQPut(streamer->message_channel_out.mq, (char *)&msg))
                    {
                        STREAMER_LOG_ERR(DBG_CORE, ERRCODE_MSG_FAILURE, "STREAMER : Send MSG failed\n");
                    }
                }
            }
            else
            {
                /* TODO - Set Error: Block Receive Failed */
                STREAMER_LOG_ERR(DBG_CORE, ERRCODE_INTERNAL, "STREAMER - Block Receive Failed (ret = %x)\n", ret);
            }
        }
        else
        {
            /* Poll for messages */
            msg_count = 0;
            do
            {
                /* Polling for message */
                msg_timed_out = false;
                ret           = OSA_MsgQGet(streamer->streamer_mq, (char *)&msg, osaWaitNone_c);
                if (ERRCODE_NO_ERROR == ret)
                {
                    /* Handle a message */
                    if (msg.id == STREAM_MSG_CLOSE_TASK)
                    {
                        streamer->exit_thread = true;
                        break;
                    }

                    ret = streamer_msg_handler(streamer, &msg);
                    if (ERRCODE_NOT_SEEKABLE == ret)
                    {
                        STREAMER_LOG_ERR(DBG_CORE, ret, "STREAMER - ERROR PROCESSING MESSAGE,%d\n", msg.id);
                    }
                    else if (ERRCODE_NO_ERROR != ret)
                    {
                        STREAMER_LOG_ERR(DBG_CORE, ret, "STREAMER - ERROR PROCESSING MESSAGE,%d\n", msg.id);

                        msg.id         = STREAM_MSG_ERROR;
                        msg.errorcode  = ret;
                        msg.event_data = ret;
                        msg.state      = STATE_NULL;
                        if (ERRCODE_NO_ERROR != OSA_MsgQPut(streamer->message_channel_out.mq, (char *)&msg))
                        {
                            STREAMER_LOG_ERR(DBG_CORE, ERRCODE_MSG_FAILURE, "STREAMER : Send MSG failed\n");
                        }
                    }
                    msg_count++;
                    if (msg_count >= MAX_MSG_RECV_PER_CYCLE)
                    {
                        /*
                         * Max # of messages handled - Timeout of message loop!
                         * Limit the number of messages we will handle during
                         * a single iteration of the Streamer Task Loop
                         */
                        msg_timed_out = true;
                    }
                }
                else
                {
                    msg_timed_out = true;
                }
            } while (((uint8_t) false == msg_timed_out) && ((uint8_t) false == streamer->exit_thread));

            streamer_process_pipelines(streamer);
        }
    }

    /* Task Exit == true,  Close the task */
    OSA_MsgQDestroy(streamer->streamer_mq);
    streamer->is_active = false;

    OSA_TaskDestroy(thread_hdl);

    STREAMER_FUNC_EXIT(DBG_CORE);
}

int32_t streamer_deinit(STREAMER_T *streamer)
{
    unsigned int x, y = 0;
    int32_t ret = ERRCODE_NO_ERROR;

    STREAMER_FUNC_ENTER(DBG_CORE);

    for (x = 0; x < MAX_PIPELINES; x++)
    {
        if (streamer->pipes[x])
        {
            ret |= set_state_pipeline(streamer->pipes[x], STATE_NULL);

            for (y = 0; y < MAX_ELEMENTS; y++)
            {
                if (streamer->elems[x][y])
                {
                    ret |= destroy_element(streamer->elems[x][y]);
                    streamer->elems[x][y] = (ElementHandle)NULL;
                }
            }
        }
    }

    for (x = 0; x < MAX_PIPELINES; x++)
    {
        if (streamer->pipes[x])
        {
            ret |= destroy_pipeline(streamer->pipes[x]);
            streamer->pipes[x] = (PipelineHandle)NULL;
        }
    }

    STREAMER_FUNC_EXIT(DBG_CORE);

    return ret;
}

void streamer_init(STREAMER_T *streamer)
{
    unsigned int x, y = 0;

    STREAMER_FUNC_ENTER(DBG_CORE);

    for (x = 0; x < MAX_PIPELINES; x++)
    {
        streamer->pipes[x] = 0;

        for (y = 0; y < MAX_ELEMENTS; y++)
        {
            streamer->elems[x][y] = 0;
        }
    }

    STREAMER_FUNC_EXIT(DBG_CORE);
}

int streamer_msg_handler(STREAMER_T *task_data, STREAMER_MSG_T *msg)
{
    unsigned int x;
    unsigned int handled = false;
    int ret              = ERRCODE_NO_ERROR;

    STREAMER_FUNC_ENTER(DBG_CORE);

    if (NULL == msg)
    {
        STREAMER_FUNC_EXIT(DBG_CORE);
        return ERRCODE_INVALID_ARGUMENT;
    }

    for (x = 0; x < (sizeof(msg_handler_table) / sizeof(STREAMER_MSG_HANDLER_T)); x++)
    {
        if (msg->id == msg_handler_table[x].id)
        {
            /* Message Found - Call Handler*/
            ret = (msg_handler_table[x].handler)(task_data, msg);

            handled = true;
            break;
        }
    }

    if ((uint8_t) false == handled)
    {
        /* Message ID never found - Return Invalid Parameter */
        ret = ERRCODE_INVALID_ARGUMENT;
    }

    /* Save the return value and signal the condtion to unblock the waiting
     * caller */
    if (msg->ret)
        *(msg->ret) = ret;

    *(msg->processed) = true;
    if(msg->cond != NULL)
    {
        OSA_EventSet(msg->cond, EVENT_WAIT_BIT);
    }

    STREAMER_FUNC_EXIT(DBG_CORE);
    return ret;
}

int streamer_process_pipelines(STREAMER_T *task_data)
{
    unsigned int loop_count;
    int ret = 0;

    STREAMER_FUNC_ENTER(DBG_CORE);

    for (loop_count = 0; loop_count < MAX_PIPELINES; loop_count++)
    {
        if (STATE_PLAYING == get_state_pipeline(task_data->pipes[loop_count]))
        {
            ret = (int)process_pipeline(task_data->pipes[loop_count]);

            if (0 != ret)
            {
                STREAMER_LOG_ERR(DBG_CORE, ERRCODE_INTERNAL,
                                 "STREAMER - Process Pipeline Error!\
                        Pipeline Index = %d, error = %d\n",
                                 loop_count, ret);

                set_state_pipeline(task_data->pipes[loop_count], STATE_NULL);

                if (task_data->message_channel_out.is_mq_created)
                {
                    STREAMER_MSG_T msg = {0};

                    msg.id         = STREAM_MSG_ERROR;
                    msg.errorcode  = ret;
                    msg.event_data = ret;
                    msg.state      = STATE_NULL;

                    if (ERRCODE_NO_ERROR != OSA_MsgQPut(task_data->message_channel_out.mq, (char *)&msg))
                    {
                        STREAMER_LOG_ERR(DBG_CORE, ERRCODE_MSG_FAILURE, "STREAMER : Send MSG failed\n");
                    }
                }
            }
        }
    }

    STREAMER_FUNC_EXIT(DBG_CORE);
    return ret;
}

/**
 * @brief Streamer handle message
 *
 * @param streamer Pointer to a streamer object
 * @param msg      Streamer message
 * @param block    Blocking/non-blocking
 * @return int32_t
 */
static int32_t streamer_handle_msg(STREAMER_T *streamer, STREAMER_MSG_T *msg, bool block)
{
    int32_t ret             = ERRCODE_NO_ERROR;
    int32_t streamer_result = ERRCODE_NO_ERROR;
    volatile bool msg_processed      = false;

    STREAMER_FUNC_ENTER(DBG_CORE);

    if (block)
    {
        OSA_MUTEX_HANDLE_DEFINE(mutex);      /*!< @brief Mutex to protect signal */
        OSA_EVENT_HANDLE_DEFINE(cond);       /*!< @brief Signal to unlock wait for streamer response */

        msg->mutex = mutex;
        msg->cond = cond;

        OSA_MutexCreate(msg->mutex);
        OSA_EventCreate(msg->cond, true);

        msg->ret       = &streamer_result;
        msg->processed = &msg_processed;

        ret = OSA_MsgQPut(streamer->streamer_mq, (void *)msg);
        if (ERRCODE_NO_ERROR == ret)
        {
            OSA_MutexLock(msg->mutex, osaWaitForever_c);
            if (!msg_processed)
            {
                ret = OSA_EventWait(msg->cond, EVENT_WAIT_BIT, false, WAIT_STREAMER_MSG_RESPONSE_TIME_OUT, NULL);
            }
            OSA_MutexUnlock(msg->mutex);
        }

        OSA_MutexDestroy(msg->mutex);
        OSA_EventDestroy(msg->cond);

        if (ERRCODE_NO_ERROR == ret)
            ret = streamer_result; /* return the streamer returned value */
    }
    else
    {
        ret = OSA_MsgQPut(streamer->streamer_mq, (void *)msg);
    }

    if (ERRCODE_NO_ERROR != ret)
        STREAMER_LOG_ERR(DBG_CORE, ret, "send msg to streamer failed\n");

    STREAMER_FUNC_EXIT(DBG_CORE);
    return ret;
}

int32_t streamer_create_pipeline(STREAMER_T *streamer,
                                 int32_t pipeline_id,
                                 const char *in_dev_name,
                                 const char *out_dev_name,
                                 PipelineElements elements,
                                 bool block)
{
    int32_t ret = ERRCODE_NO_ERROR, i = 0, level = 0;

    STREAMER_FUNC_ENTER(DBG_CORE);

    if (!streamer || elements.num_elements < 2 || elements.element_ids == NULL)
    {
        STREAMER_FUNC_EXIT(DBG_CORE);
        return ERRCODE_INVALID_ARGUMENT;
    }

    streamer->elements[pipeline_id]               = OSA_MemoryAllocate(sizeof(PipelineElements));
    streamer->elements[pipeline_id]->element_ids  = OSA_MemoryAllocate(elements.num_elements * sizeof(ElementIndex));
    streamer->elements[pipeline_id]->num_elements = elements.num_elements;
    for (; i < elements.num_elements; i++)
    {
        streamer->elements[pipeline_id]->element_ids[i] = elements.element_ids[i];
    }

    CHECK_RET(create_pipeline(&streamer->pipes[pipeline_id], pipeline_id, &(streamer->message_channel_out)), "create_pipeline");

    for (i = 0; i < elements.num_elements; i++)
    {
        CHECK_RET(create_element(&(streamer->elems[pipeline_id][elements.element_ids[i]]),
                                 type_lookup_table[elements.element_ids[i]].type, 0),
                  "create_element");

        CHECK_RET(add_element_pipeline(streamer->pipes[pipeline_id],
                                       streamer->elems[pipeline_id][elements.element_ids[i]], level++),
                  "add_element_pipeline");
    }

    for (i = 0; i < elements.num_elements - 1; i++)
    {
        CHECK_RET(link_elements(streamer->elems[pipeline_id][elements.element_ids[i]], 0,
                                streamer->elems[pipeline_id][elements.element_ids[i + 1]], 0),
                  "link_elements");
    }

    STREAMER_FUNC_EXIT(DBG_CORE);
    return ret;
}

int32_t streamer_destroy_pipeline(STREAMER_T *streamer, int32_t pipeline_id, bool block)
{
    int32_t ret = ERRCODE_NO_ERROR;
    int i       = 0;

    STREAMER_FUNC_ENTER(DBG_CORE);

    if (!streamer)
    {
        STREAMER_FUNC_EXIT(DBG_CORE);
        return ERRCODE_INVALID_ARGUMENT;
    }
    PipelineElements *pipelineElements = streamer->elements[pipeline_id];
    ElementIndex *element_ids          = pipelineElements->element_ids;

    for (; i < pipelineElements->num_elements - 1; i++)
    {
        CHECK_RET(unlink_elements(streamer->elems[pipeline_id][element_ids[i]], 0,
                                  streamer->elems[pipeline_id][element_ids[i + 1]], 0),
                  "unlink_elements");
    }

    for (i = 0; i < pipelineElements->num_elements; i++)
    {
        CHECK_RET(remove_element_pipeline(streamer->pipes[pipeline_id], streamer->elems[pipeline_id][element_ids[i]]),
                  "remove_element_pipeline");
    }

    OSA_MemoryFree(streamer->elements[pipeline_id]);
    OSA_MemoryFree(streamer->elements[pipeline_id]->element_ids);
    streamer->elements[pipeline_id] = NULL;

    STREAMER_FUNC_EXIT(DBG_CORE);
    return ret;
}

int32_t streamer_get_track_info(STREAMER_T *streamer, int32_t pipeline_id, TrackInfo *info, bool block)
{
    int32_t ret        = ERRCODE_NO_ERROR;
    STREAMER_MSG_T msg = {0};

    STREAMER_FUNC_ENTER(DBG_CORE);

    if (!streamer || !info)
    {
        STREAMER_FUNC_EXIT(DBG_CORE);
        return ERRCODE_GENERAL_ERROR;
    }

    msg.id             = STREAM_MSG_GET_TRACK_INFO;
    msg.pipeline_index = pipeline_id;
    msg.get_value      = info;
    if (block)
    {
        /* process throught message queue */
        ret = streamer_handle_msg(streamer, &msg, true);
    }
    else
    {
        /* call internal function directly */
        ret = streamer_msg_get_track_info(streamer, &msg);
    }

    STREAMER_FUNC_EXIT(DBG_CORE);
    return ret;
}

int32_t streamer_set_state(STREAMER_T *streamer, int32_t pipeline_id, PipelineState state, bool block)
{
    STREAMER_FUNC_ENTER(DBG_CORE);

    if (!streamer)
    {
        STREAMER_FUNC_EXIT(DBG_CORE);
        return ERRCODE_INVALID_ARGUMENT;
    }

    STREAMER_MSG_T msg = {0};
    msg.id             = STREAM_MSG_SET_STATE;
    msg.state          = state;
    msg.pipeline_index = pipeline_id;

    STREAMER_FUNC_EXIT(DBG_CORE);
    return streamer_handle_msg(streamer, &msg, block);
}

int32_t streamer_get_state(STREAMER_T *streamer, int32_t pipeline_id, PipelineState *state, bool block)
{
    int32_t ret        = ERRCODE_NO_ERROR;
    STREAMER_MSG_T msg = {0};

    STREAMER_FUNC_ENTER(DBG_CORE);

    if (!streamer || !state)
    {
        STREAMER_FUNC_EXIT(DBG_CORE);
        return ERRCODE_INVALID_ARGUMENT;
    }

    msg.id             = STREAM_MSG_GET_STATE;
    msg.pipeline_index = pipeline_id;
    msg.get_value      = state;
    if (block)
    {
        /* process throught message queue */
        ret = streamer_handle_msg(streamer, &msg, true);
    }
    else
    {
        /* call internal function directly */
        ret = streamer_msg_get_state(streamer, &msg);
    }

    STREAMER_FUNC_EXIT(DBG_CORE);
    return ret;
}

int32_t streamer_query_info(
    STREAMER_T *streamer, int32_t pipeline_id, StreamInfoType infoType, StreamData *query_data, bool block)
{
    int32_t ret        = ERRCODE_NO_ERROR;
    STREAMER_MSG_T msg = {0};

    STREAMER_FUNC_ENTER(DBG_CORE);

    msg.id             = STREAM_MSG_QUERY_PIPELINE;
    msg.pipeline_index = pipeline_id;
    msg.query_type     = infoType;
    msg.get_value      = query_data;
    if (block)
    {
        /* process throught message queue */
        ret = streamer_handle_msg(streamer, &msg, true);
    }
    else
    {
        /* call internal function directly */
        ret = streamer_msg_send_query(streamer, &msg);
    }

    STREAMER_FUNC_EXIT(DBG_CORE);
    return ret;
}

int32_t streamer_seek_pipeline(STREAMER_T *streamer, int32_t pipeline_id, int32_t seek_time_ms, bool block)
{
    STREAMER_MSG_T msg = {0};

    STREAMER_FUNC_ENTER(DBG_CORE);

    if (!streamer)
    {
        STREAMER_FUNC_EXIT(DBG_CORE);
        return ERRCODE_INVALID_ARGUMENT;
    }

    msg.id             = STREAM_MSG_SEEK_PIPELINE;
    msg.time_ms        = seek_time_ms;
    msg.pipeline_index = pipeline_id;

    STREAMER_FUNC_EXIT(DBG_CORE);
    return streamer_handle_msg(streamer, &msg, block);
}

int32_t streamer_set_property(STREAMER_T *streamer, int8_t pipeline_id, ELEMENT_PROPERTY_T prop, bool block)
{
    STREAMER_FUNC_ENTER(DBG_CORE);

    if (!streamer)
    {
        STREAMER_FUNC_EXIT(DBG_CORE);
        return ERRCODE_INVALID_ARGUMENT;
    }

    STREAMER_MSG_T msg        = {0};
    msg.id                    = STREAM_MSG_SET_PROPERTY;
    msg.element_property.val  = prop.val;
    msg.element_property.prop = prop.prop;
    msg.pipeline_index        = pipeline_id;

    STREAMER_FUNC_EXIT(DBG_CORE);
    return streamer_handle_msg(streamer, &msg, block);
}

int32_t streamer_get_property(STREAMER_T *streamer, int8_t pipeline_id, uint16_t prop, uint32_t *val_ptr, bool block)
{
    int32_t ret        = ERRCODE_NO_ERROR;
    STREAMER_MSG_T msg = {0};

    STREAMER_FUNC_ENTER(DBG_CORE);

    if (!streamer || !val_ptr)
    {
        STREAMER_FUNC_EXIT(DBG_CORE);
        return ERRCODE_INVALID_ARGUMENT;
    }

    msg.id                    = STREAM_MSG_GET_PROPERTY;
    msg.element_property.prop = prop;
    msg.get_value             = val_ptr;
    msg.pipeline_index        = pipeline_id;

    if (block)
    {
        /* process throught message queue */
        ret = streamer_handle_msg(streamer, &msg, true);
    }
    else
    {
        /* call internal function directly */
        ret = streamer_msg_get_property(streamer, &msg);
    }

    STREAMER_FUNC_EXIT(DBG_CORE);
    return ret;
}

/** TEMP - remove when CCI metadata is fixed **/
char g_cci_meta_location[MAX_LOCATION_PATH_LENGTH];
int32_t streamer_set_file(STREAMER_T *streamer, int32_t pipeline_id, char *path, PipelineState state, bool block)
{
    STREAMER_FUNC_ENTER(DBG_CORE);

    if (!streamer)
    {
        STREAMER_FUNC_EXIT(DBG_CORE);
        return ERRCODE_INVALID_ARGUMENT;
    }

    STREAMER_MSG_T msg       = {0};
    msg.id                   = STREAM_MSG_SET_FILE;
    msg.state                = state;
    msg.pipeline_index       = pipeline_id;
    msg.element_property.val = (uintptr_t)path;

    if (strlen(path) < MAX_LOCATION_PATH_LENGTH)
    {
        strcpy(g_cci_meta_location, path); /*TEMP - remove */
    }
    else
    {
        return ERRCODE_OUT_OF_SPACE;
    }

    STREAMER_FUNC_EXIT(DBG_CORE);
    return streamer_handle_msg(streamer, &msg, block);
}

int32_t streamer_set_repeat(STREAMER_T *streamer, int32_t pipeline_id, bool repeat_on, bool block)
{
    STREAMER_FUNC_ENTER(DBG_CORE);

    if (!streamer)
    {
        STREAMER_FUNC_EXIT(DBG_CORE);
        return ERRCODE_INVALID_ARGUMENT;
    }

    STREAMER_MSG_T msg       = {0};
    msg.id                   = STREAM_MSG_SET_REPEAT;
    msg.pipeline_index       = pipeline_id;
    msg.element_property.val = (uintptr_t)repeat_on;

    STREAMER_FUNC_EXIT(DBG_CORE);
    return streamer_handle_msg(streamer, &msg, block);
}
