/**
 * Copyright 2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 *  @file  main.c
 *  @brief Suite of tests written for ZTEST, testing Thread mode of OSA API for Zephyr.
 */

#include <zephyr/ztest.h>

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <stdio.h>

#include "fsl_common.h"
// #include "fsl_adapter_rfimu.h"
#include "fsl_os_abstraction.h"
#include "fsl_debug_console.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define TEST_WAIT_TIME_MS (50u)
// #define TEST_TASK_PRIORITY ((uint32_t) (-10))
#define TEST_TASK_PRIORITY    (0u)
#define TEST_NUM_OF_INSTANCES (50u)

#define TEST_MSGQUEUE_MESSAGE_CNT  (10u)
#define TEST_MSGQUEUE_MESSAGE_CHAR (0x05)

/*******************************************************************************
 * Variables
 ******************************************************************************/
OSA_MSGQ_HANDLE_DEFINE(msgqHandle, 1, sizeof(uint32_t));

OSA_MUTEX_HANDLE_DEFINE(mutexHandle);

OSA_SEMAPHORE_HANDLE_DEFINE(semaphoreHandle);

OSA_TASK_HANDLE_DEFINE(taskHandle1);
OSA_TASK_HANDLE_DEFINE(taskHandle2);

#ifdef CONFIG_EVENTS
OSA_EVENT_HANDLE_DEFINE(eventHandle);
#endif

/*******************************************************************************
 * Code
 ******************************************************************************/

/**
 * @brief Destroys all handlers possibly allocated in previous run.
 *
 */
void test_suite_before(void *fixture)
{
    OSA_MsgQDestroy(msgqHandle);
    OSA_MutexDestroy(mutexHandle);
    OSA_SemaphoreDestroy(semaphoreHandle);
    OSA_TaskDestroy(taskHandle1);

#ifdef CONFIG_EVENTS
    OSA_EventDestroy(eventHandle);
#endif
}

ZTEST_SUITE(maestro_tests, NULL, NULL, test_suite_before, NULL, NULL);

/*******************************************************************************
 * Assert test
 ******************************************************************************/

/**
 * @brief Test Asserts
 *
 * This test verifies various assert macros provided by ztest.
 *
 */
ZTEST(maestro_tests, test_assert)
{
    zassert_true(1, "1 was false");
    zassert_false(0, "0 was true");
    zassert_is_null(NULL, "NULL was not NULL");
    zassert_not_null("foo", "\"foo\" was NULL");
    zassert_equal(1, 1, "1 was not equal to 1");
    zassert_equal_ptr(NULL, NULL, "NULL was not equal to NULL");
}

/*******************************************************************************
 * Event test
 ******************************************************************************/

#ifdef CONFIG_EVENTS

void TEST_Event_task(void *param);
static OSA_TASK_DEFINE(TEST_Event_task, TEST_TASK_PRIORITY, 1, 256, false);
#define TEST_EVENT_FLAG_TO_SET (1u)

void TEST_Event_task(void *param)
{
    if (KOSA_StatusSuccess == OSA_EventSet((osa_event_handle_t)eventHandle, TEST_EVENT_FLAG_TO_SET))
    {
        zassert_equal(KOSA_StatusSuccess, OSA_TaskDestroy((osa_task_handle_t)taskHandle1));
    }
}

ZTEST(maestro_tests, test_OSA_2_event)
{
    osa_status_t status;
    osa_event_flags_t flagsToWait = 3;
    osa_event_flags_t pSetFlags;
    osa_event_flags_t eventFlags;

    uint32_t count = TEST_NUM_OF_INSTANCES;

    uint8_t eventHandleBuffer_test[TEST_NUM_OF_INSTANCES][OSA_EVENT_HANDLE_SIZE] = {0};
    osa_event_handle_t evevtHandle_test[TEST_NUM_OF_INSTANCES]                   = {0};

    for (uint32_t i = 0; i < TEST_NUM_OF_INSTANCES; i++)
    {
        evevtHandle_test[i] = &eventHandleBuffer_test[i][0];
        status              = OSA_EventCreate(evevtHandle_test[i], 0);
        if (status == KOSA_StatusError)
        {
            count = i;
            break;
        }
    }

    for (uint32_t i = 0; i < count; i++)
    {
        status = OSA_EventDestroy(evevtHandle_test[i]);
        zassert_equal(KOSA_StatusSuccess, status);
    }

    status = OSA_EventWait((osa_event_handle_t)eventHandle, flagsToWait, false, 0, NULL);
    zassert_equal(KOSA_StatusError, status);

    status = OSA_EventSet((osa_event_handle_t)eventHandle, 1);
    zassert_equal(KOSA_StatusError, status);

    status = OSA_EventClear((osa_event_handle_t)eventHandle, 0);
    zassert_equal(KOSA_StatusError, status);

    status = OSA_EventDestroy((osa_event_handle_t)eventHandle);
    zassert_equal(KOSA_StatusError, status);

    status = OSA_EventCreate((osa_event_handle_t)eventHandle, 0);
    zassert_equal(KOSA_StatusSuccess, status);

    status = OSA_EventGet((osa_event_handle_t)eventHandle, 0xFFFFFFFF, NULL);
    zassert_equal(KOSA_StatusError, status);

    status = OSA_EventGet((osa_event_handle_t)eventHandle, 0xFFFFFFFF, &eventFlags);
    zassert_equal(KOSA_StatusSuccess, status);
    zassert_equal(0U, eventFlags);

    status = OSA_EventSet((osa_event_handle_t)eventHandle, 0x101U);
    zassert_equal(KOSA_StatusSuccess, status);

    status = OSA_EventGet((osa_event_handle_t)eventHandle, 0xFFFFFFFFU, &eventFlags);
    zassert_equal(KOSA_StatusSuccess, status);
    zassert_equal(0x101U, eventFlags);

    status = OSA_EventGet((osa_event_handle_t)eventHandle, 1U, &eventFlags);
    zassert_equal(KOSA_StatusSuccess, status);
    zassert_equal(1U, eventFlags);

    status = OSA_EventClear((osa_event_handle_t)eventHandle, 0x1U);
    zassert_equal(KOSA_StatusSuccess, status);

    status = OSA_EventGet((osa_event_handle_t)eventHandle, 1U, &eventFlags);
    zassert_equal(KOSA_StatusSuccess, status);
    zassert_equal(0U, eventFlags);

    status = OSA_EventGet((osa_event_handle_t)eventHandle, 0x100U, &eventFlags);
    zassert_equal(KOSA_StatusSuccess, status);
    zassert_equal(0x100U, eventFlags);

    status = OSA_EventClear((osa_event_handle_t)eventHandle, 0x00FFFFFFU); /* FreeRTOS support up to 24 events */
    zassert_equal(KOSA_StatusSuccess, status);

    status = OSA_EventWait((osa_event_handle_t)eventHandle, flagsToWait, false, 0, NULL);
    zassert_equal(KOSA_StatusTimeout, status);

    status = OSA_EventSet((osa_event_handle_t)eventHandle, 1);
    zassert_equal(KOSA_StatusSuccess, status);

    status = OSA_EventWait((osa_event_handle_t)eventHandle, flagsToWait, false, 0, &pSetFlags);
    zassert_equal(KOSA_StatusSuccess, status);

    status = OSA_EventClear((osa_event_handle_t)eventHandle, 0);
    zassert_equal(KOSA_StatusSuccess, status);

    status = OSA_EventClear((osa_event_handle_t)eventHandle, 1);
    zassert_equal(KOSA_StatusSuccess, status);

    status = OSA_EventSet((osa_event_handle_t)eventHandle, flagsToWait);
    zassert_equal(KOSA_StatusSuccess, status);

    status = OSA_EventWait((osa_event_handle_t)eventHandle, flagsToWait, true, osaWaitForever_c, &pSetFlags);
    zassert_equal(KOSA_StatusSuccess, status);

    status = OSA_EventDestroy((osa_event_handle_t)eventHandle);
    zassert_equal(KOSA_StatusSuccess, status);

    status = OSA_EventCreate((osa_event_handle_t)eventHandle, 0);
    zassert_equal(KOSA_StatusSuccess, status);

    status = OSA_TaskCreate((osa_task_handle_t)taskHandle1, OSA_TASK(TEST_Event_task), NULL);
    zassert_equal(KOSA_StatusSuccess, status);

    status =
        OSA_EventWait((osa_event_handle_t)eventHandle, TEST_EVENT_FLAG_TO_SET, true, TEST_WAIT_TIME_MS, &pSetFlags);
    zassert_equal(KOSA_StatusSuccess, status);
    zassert_equal(TEST_EVENT_FLAG_TO_SET, pSetFlags);
}

#endif /* ifdef CONFIG_EVENTS */

/*******************************************************************************
 * Semaphore test
 ******************************************************************************/

void TEST_Semaphore_task(void *param);
static OSA_TASK_DEFINE(TEST_Semaphore_task, TEST_TASK_PRIORITY, 1, 256, false);

void TEST_Semaphore_task(void *param)
{
    osa_status_t retVal = OSA_SemaphorePost((osa_semaphore_handle_t)semaphoreHandle);
    zassert_equal(KOSA_StatusSuccess, retVal);
    zassert_equal(KOSA_StatusSuccess, OSA_TaskDestroy((osa_task_handle_t)taskHandle1));
}

uint8_t semaphoreHandleBuffer_test[TEST_NUM_OF_INSTANCES][OSA_SEM_HANDLE_SIZE];
osa_semaphore_handle_t semaphoreHandle_test[TEST_NUM_OF_INSTANCES];

ZTEST(maestro_tests, test_OSA_2_semaphore)
{
    osa_status_t status;
    uint32_t count = TEST_NUM_OF_INSTANCES;
    osa_semaphore_count_t semaCount = 0;

    for (uint32_t i = 0; i < TEST_NUM_OF_INSTANCES; i++)
    {
        semaphoreHandle_test[i] = &semaphoreHandleBuffer_test[i][0];
        status                  = OSA_SemaphoreCreate(semaphoreHandle_test[i], 0);
        if (status == KOSA_StatusError)
        {
            count = i;
            break;
        }
    }

    for (uint32_t i = 0; i < count; i++)
    {
        status = OSA_SemaphoreDestroy(semaphoreHandle_test[i]);
        zassert_equal(KOSA_StatusSuccess, status);
    }

    status = OSA_SemaphoreCreate((osa_semaphore_handle_t)semaphoreHandle, 0xFFFFFFFF - 1);
    zassert_equal(KOSA_StatusSuccess, status);
    semaCount = OSA_SemaphoreGetCount((osa_semaphore_handle_t)semaphoreHandle);
    zassert_equal((0xFFFFFFFF - 1), semaCount);

    // Zephyr does not return error when limit of semaphore is reached and incremented
    // status = OSA_SemaphorePost((osa_semaphore_handle_t)semaphoreHandle);
    // zassert_equal(KOSA_StatusSuccess, status);

    // status = OSA_SemaphorePost((osa_semaphore_handle_t)semaphoreHandle);
    // zassert_equal(KOSA_StatusError, status);

    status = OSA_SemaphoreWait((osa_semaphore_handle_t)semaphoreHandle, 0);
    zassert_equal(KOSA_StatusSuccess, status);

    status = OSA_SemaphoreDestroy((osa_semaphore_handle_t)semaphoreHandle);
    zassert_equal(KOSA_StatusSuccess, status);

    status = OSA_SemaphoreCreate((osa_semaphore_handle_t)semaphoreHandle, 0);
    zassert_equal(KOSA_StatusSuccess, status);

    semaCount = OSA_SemaphoreGetCount((osa_semaphore_handle_t)semaphoreHandle);
    zassert_equal(0, semaCount);

    status = OSA_SemaphoreWait((osa_semaphore_handle_t)semaphoreHandle, 0);
    zassert_equal(KOSA_StatusTimeout, status);

    status = OSA_SemaphoreDestroy((osa_semaphore_handle_t)semaphoreHandle);
    zassert_equal(KOSA_StatusSuccess, status);

    status = OSA_SemaphoreCreate((osa_semaphore_handle_t)semaphoreHandle, 0);
    zassert_equal(KOSA_StatusSuccess, status);

    status = OSA_TaskCreate((osa_task_handle_t)taskHandle1, OSA_TASK(TEST_Semaphore_task), NULL);
    zassert_equal(KOSA_StatusSuccess, status);

    status = OSA_SemaphoreWait((osa_semaphore_handle_t)semaphoreHandle, TEST_WAIT_TIME_MS);
    zassert_equal(KOSA_StatusSuccess, status);
}

/*******************************************************************************
 * Mutex test
 ******************************************************************************/

void TEST_Mutex_task(void *param);
static OSA_TASK_DEFINE(TEST_Mutex_task, TEST_TASK_PRIORITY, 0, 256, false);

void TEST_Mutex_task(void *param)
{
    osa_status_t retVal = OSA_MutexLock((osa_mutex_handle_t)mutexHandle, 0);
    zassert_equal(KOSA_StatusSuccess, retVal);

    OSA_TaskYield();
    OSA_TaskYield();

    retVal = OSA_MutexUnlock((osa_mutex_handle_t)mutexHandle);
    zassert_equal(KOSA_StatusSuccess, retVal);

    zassert_equal(KOSA_StatusSuccess, OSA_TaskDestroy((osa_task_handle_t)taskHandle1));
}

uint8_t mutexHandleBuffer_test[TEST_NUM_OF_INSTANCES][OSA_MUTEX_HANDLE_SIZE];
osa_semaphore_handle_t mutexHandle_test[TEST_NUM_OF_INSTANCES];

ZTEST(maestro_tests, test_OSA_2_mutex)
{
    osa_status_t status;
    uint32_t count = TEST_NUM_OF_INSTANCES;

    uint8_t mutexHandleBuffer_test[TEST_NUM_OF_INSTANCES][OSA_MUTEX_HANDLE_SIZE];
    osa_mutex_handle_t mutexHandle_test[TEST_NUM_OF_INSTANCES];

    for (uint32_t i = 0; i < TEST_NUM_OF_INSTANCES; i++)
    {
        mutexHandle_test[i] = &mutexHandleBuffer_test[i][0];
        status              = OSA_MutexCreate(mutexHandle_test[i]);
        if (status == KOSA_StatusError)
        {
            count = i;
            break;
        }
    }

    for (uint32_t i = 0; i < count; i++)
    {
        status = OSA_MutexDestroy(mutexHandle_test[i]);
        zassert_equal(KOSA_StatusSuccess, status);
    }

    status = OSA_MutexCreate((osa_mutex_handle_t)mutexHandle);
    zassert_equal(KOSA_StatusSuccess, status);

    status = OSA_MutexUnlock((osa_mutex_handle_t)mutexHandle);
    zassert_equal(KOSA_StatusError, status);

    status = OSA_MutexLock((osa_mutex_handle_t)mutexHandle, 0);
    zassert_equal(KOSA_StatusSuccess, status);

    status = OSA_MutexUnlock((osa_mutex_handle_t)mutexHandle);
    zassert_equal(KOSA_StatusSuccess, status);

    /* Recursive Lock */
    for (uint32_t i = 0; i < 5; i++)
    {
        status = OSA_MutexLock((osa_mutex_handle_t)mutexHandle, 0);
        zassert_equal(KOSA_StatusSuccess, status);
    }

    /* Recursive Unlock */
    for (uint32_t i = 0; i < 5; i++)
    {
        status = OSA_MutexUnlock((osa_mutex_handle_t)mutexHandle);
        zassert_equal(KOSA_StatusSuccess, status);
    }

    status = OSA_MutexUnlock((osa_mutex_handle_t)mutexHandle);
    zassert_equal(KOSA_StatusError, status);

    /*timeout = osaWaitForever_c*/
    status = OSA_MutexLock((osa_mutex_handle_t)mutexHandle, osaWaitForever_c);
    zassert_equal(KOSA_StatusSuccess, status);

    status = OSA_MutexUnlock((osa_mutex_handle_t)mutexHandle);
    zassert_equal(KOSA_StatusSuccess, status);

    status = OSA_MutexDestroy((osa_mutex_handle_t)mutexHandle);
    zassert_equal(KOSA_StatusSuccess, status);

    status = OSA_MutexCreate((osa_mutex_handle_t)mutexHandle);
    zassert_equal(KOSA_StatusSuccess, status);

    status = OSA_TaskCreate((osa_task_handle_t)taskHandle1, OSA_TASK(TEST_Mutex_task), NULL);
    zassert_equal(KOSA_StatusSuccess, status);

    // Runs created task to lock a mutex
    OSA_TaskYield();
    OSA_TaskYield();

    status = OSA_MutexLock((osa_mutex_handle_t)mutexHandle, TEST_WAIT_TIME_MS);
    zassert_equal(KOSA_StatusSuccess, status);

    zassert_equal(KOSA_StatusSuccess, OSA_TaskDestroy((osa_task_handle_t)taskHandle1));
}

/*******************************************************************************
 * MSG queue test
 ******************************************************************************/

void TEST_MsgQ_task(void *param);
static OSA_TASK_DEFINE(TEST_MsgQ_task, TEST_TASK_PRIORITY, 0, 256, false);

void TEST_MsgQ_task(void *param)
{
    uint32_t msg = TEST_MSGQUEUE_MESSAGE_CHAR;

    osa_status_t retVal = OSA_MsgQPut((osa_msgq_handle_t)msgqHandle, &msg);
    zassert_equal(KOSA_StatusSuccess, retVal);

    zassert_equal(KOSA_StatusSuccess, OSA_TaskDestroy((osa_task_handle_t)taskHandle1));
}

ZTEST(maestro_tests, test_OSA_2_msgQ)
{
    osa_status_t status;
    uint32_t msgTable[TEST_MSGQUEUE_MESSAGE_CNT + 2];
    uint32_t msg;

    uint32_t i;

    for (int i = 0; i < TEST_MSGQUEUE_MESSAGE_CNT; i++)
    {
        msgTable[i] = i + 10;
    }

    /*OSA_MsgQCreate*/
    status = OSA_MsgQCreate((osa_msgq_handle_t)msgqHandle, TEST_MSGQUEUE_MESSAGE_CNT, sizeof(msgTable[0]));
    zassert_equal(KOSA_StatusSuccess, status);

    /*OSA_MsgQPut*/
    for (i = 0; i < TEST_MSGQUEUE_MESSAGE_CNT; i++)
    {
        status = OSA_MsgQPut((osa_msgq_handle_t)msgqHandle, &msgTable[i]);
        zassert_equal(KOSA_StatusSuccess, status);
    }

    /*pQueue->number >= pQueue->max*/
    status = OSA_MsgQPut((osa_msgq_handle_t)msgqHandle, &msgTable[i]);
    zassert_equal(KOSA_StatusError, status);

    /*OSA_MsgQGet*/
    for (i = 0; i < TEST_MSGQUEUE_MESSAGE_CNT; i++)
    {
        status = OSA_MsgQGet((osa_msgq_handle_t)msgqHandle, &msg, 0);
        zassert_equal(KOSA_StatusSuccess, status);
        zassert_equal(msg, msgTable[i]);
    }

    status = OSA_MsgQGet((osa_msgq_handle_t)msgqHandle, &msg, 10);
    zassert_equal(KOSA_StatusTimeout, status);

    status = OSA_MsgQPut((osa_msgq_handle_t)msgqHandle, &msgTable[0]);
    zassert_equal(KOSA_StatusSuccess, status);

    status = OSA_MsgQGet((osa_msgq_handle_t)msgqHandle, &msg, 10);
    zassert_equal(KOSA_StatusSuccess, status);

    /*OSA_MsgQDestroy*/
    status = OSA_MsgQDestroy((osa_msgq_handle_t)msgqHandle);
    zassert_equal(KOSA_StatusSuccess, status);

    status = OSA_MsgQGet((osa_msgq_handle_t)msgqHandle, &msg, 0);
    zassert_equal(KOSA_StatusError, status);

    /*Signaling from second task*/
    status = OSA_MsgQCreate((osa_msgq_handle_t)msgqHandle, 1, sizeof(uint32_t));
    zassert_equal(KOSA_StatusSuccess, status);

    status = OSA_TaskCreate((osa_task_handle_t)taskHandle1, OSA_TASK(TEST_MsgQ_task), NULL);
    zassert_equal(KOSA_StatusSuccess, status);

    status = OSA_MsgQGet((osa_msgq_handle_t)msgqHandle, &msg, TEST_WAIT_TIME_MS);
    zassert_equal(KOSA_StatusSuccess, status);
    zassert_equal(msg, TEST_MSGQUEUE_MESSAGE_CHAR);
}

/*******************************************************************************
 * Task API test
 ******************************************************************************/

#define TEST_TASK_DELAY_MS        (10u)
#define TEST_TASK_DELAY_MARGIN_MS (2u)
#define TEST_TASK_PARAMETER       (0x12345678u)

#if ((TEST_TASK_DELAY_MS - TEST_TASK_DELAY_MARGIN_MS) < 0)
#error "Adjist TEST_TASK_DELAY_MS an dTEST_TASK_DELAY_MARGIN_MS"
#endif

void TaskTest_task(void *param)
{
    zassert_equal(param, (void *)TEST_TASK_PARAMETER);
}
static OSA_TASK_DEFINE(TaskTest_task, OSA_PRIORITY_NORMAL, 1, 512, 0);

ZTEST(maestro_tests, test_OSA_1_task)
{
    osa_status_t status;
    osa_task_priority_t priority;

    OSA_DisableIRQGlobal();
    OSA_DisableIRQGlobal();
    OSA_EnableIRQGlobal();
    OSA_EnableIRQGlobal();
    OSA_EnableIRQGlobal();

    // Tests OSA_TimeGetMsec and OSA_TimeDelay
    uint32_t timeBefore_ms = OSA_TimeGetMsec();
    OSA_TimeDelay(TEST_TASK_DELAY_MS);
    uint32_t timeAfter_ms = OSA_TimeGetMsec();
    uint32_t timeDiff_ms  = timeAfter_ms - timeBefore_ms;

    bool isTimeCorrect = false;

    if (timeDiff_ms >= (TEST_TASK_DELAY_MS - TEST_TASK_DELAY_MARGIN_MS) &&
        timeDiff_ms <= (TEST_TASK_DELAY_MS + TEST_TASK_DELAY_MARGIN_MS))
    {
        isTimeCorrect = true;
    }

    zassert_true(isTimeCorrect);

    // Tests task API
    status = OSA_TaskCreate((osa_task_handle_t)taskHandle1, OSA_TASK(TaskTest_task), (void *)TEST_TASK_PARAMETER);
    zassert_equal(KOSA_StatusSuccess, status);

    priority = OSA_TaskGetPriority((osa_task_handle_t)taskHandle1);
    zassert_equal(OSA_PRIORITY_NORMAL, priority);

    status = OSA_TaskSetPriority((osa_task_handle_t)taskHandle1, OSA_PRIORITY_ABOVE_NORMAL);
    zassert_equal(KOSA_StatusSuccess, status);

    priority = OSA_TaskGetPriority((osa_task_handle_t)taskHandle1);
    zassert_equal(OSA_PRIORITY_ABOVE_NORMAL, priority);

    OSA_TimeDelay(TEST_WAIT_TIME_MS);

    status = OSA_TaskDestroy((osa_task_handle_t)taskHandle1);
    zassert_equal(KOSA_StatusSuccess, status);

    status = OSA_TaskDestroy((osa_task_handle_t)taskHandle1);
    zassert_equal(KOSA_StatusError, status);
}

/*******************************************************************************
 * OSA API - Task handle test
 ******************************************************************************/

#define TEST_TASK_SLEEP_MS (10000u)

void TaskHandle_taskHandleTest_InitiedOSA1(void *param)
{
    // Since OSA Init was not called, NULL should be returned
    osa_task_handle_t taskHandle = OSA_TaskGetCurrentHandle();
    zassert_equal((void *) taskHandle, param);

    OSA_TimeDelay(TEST_TASK_SLEEP_MS);
}

void TaskHandle_taskHandleTest_InitiedOSA2(void *param)
{
    // Since OSA Init was not called, NULL should be returned
    osa_task_handle_t taskHandle = OSA_TaskGetCurrentHandle();
    zassert_equal((void *) taskHandle, param);

    OSA_TimeDelay(TEST_TASK_SLEEP_MS);
}

static OSA_TASK_DEFINE(TaskHandle_taskHandleTest_InitiedOSA1, OSA_PRIORITY_HIGH, 1, 512, 0);
static OSA_TASK_DEFINE(TaskHandle_taskHandleTest_InitiedOSA2, OSA_PRIORITY_HIGH, 1, 512, 0);

ZTEST(maestro_tests, test_OSA_1_TaskHandle_2Init)
{
    osa_status_t status;
    osa_task_handle_t taskHandle;

    OSA_Init();

    // No task should be present in OSA list
    taskHandle = OSA_TaskGetCurrentHandle();
    zassert_equal((void *) taskHandle, NULL);

    // Create 2 tasks without calling OSA_INIT
    status = OSA_TaskCreate((osa_task_handle_t)taskHandle1, OSA_TASK(TaskHandle_taskHandleTest_InitiedOSA1), (void *) (osa_task_handle_t) taskHandle1);
    zassert_equal(KOSA_StatusSuccess, status);

    status = OSA_TaskCreate((osa_task_handle_t)taskHandle2, OSA_TASK(TaskHandle_taskHandleTest_InitiedOSA2), (void *) (osa_task_handle_t) taskHandle2);
    zassert_equal(KOSA_StatusSuccess, status);

    OSA_TimeDelay(100);

    status = OSA_TaskDestroy((osa_task_handle_t)taskHandle1);
    zassert_equal(KOSA_StatusSuccess, status);

    status = OSA_TaskDestroy((osa_task_handle_t)taskHandle2);
    zassert_equal(KOSA_StatusSuccess, status);

    // No task should be present in OSA list
    taskHandle = OSA_TaskGetCurrentHandle();
    zassert_equal((void *) taskHandle, NULL);
}

/*******************************************************************************
 * OSA API - Task Notification test
 ******************************************************************************/

void TaskHandle_taskNotificationTest(void *param)
{
    osa_status_t status;
    osa_task_handle_t taskHandle = OSA_TaskGetCurrentHandle();

    status = OSA_TaskNotifyGet(0);
    zassert_equal(KOSA_StatusTimeout, status);

    status = OSA_TaskNotifyPost(taskHandle);
    zassert_equal(KOSA_StatusSuccess, status);

    status = OSA_TaskNotifyGet(0);
    zassert_equal(KOSA_StatusSuccess, status);

    osa_status_t retVal = OSA_SemaphorePost((osa_semaphore_handle_t)semaphoreHandle);
    zassert_equal(KOSA_StatusSuccess, retVal);

    OSA_TimeDelay(1000);
}

static OSA_TASK_DEFINE(TaskHandle_taskNotificationTest, OSA_PRIORITY_HIGH, 1, 512, 0);

// Must be called after OSA_Init
ZTEST(maestro_tests, test_OSA_3_TaskNotification)
{
    osa_status_t status;

    osa_task_handle_t taskHandle = OSA_TaskGetCurrentHandle();
    zassert_equal((void *) taskHandle, NULL);

    status = OSA_TaskNotifyPost(taskHandle);
    zassert_equal(KOSA_StatusError, status);

    status = OSA_TaskNotifyGet(0);
    zassert_equal(KOSA_StatusError, status);

    status = OSA_TaskNotifyGet(10);
    zassert_equal(KOSA_StatusError, status);

    status = OSA_SemaphoreCreate((osa_semaphore_handle_t)semaphoreHandle, 0);
    zassert_equal(KOSA_StatusSuccess, status);

    status = OSA_TaskCreate((osa_task_handle_t)taskHandle1, OSA_TASK(TaskHandle_taskNotificationTest), NULL);
    zassert_equal(KOSA_StatusSuccess, status);

    status = OSA_SemaphoreWait((osa_semaphore_handle_t)semaphoreHandle, 1000);
    zassert_equal(KOSA_StatusSuccess, status);

    status = OSA_TaskDestroy((osa_task_handle_t)taskHandle1);
    zassert_equal(KOSA_StatusSuccess, status);
}