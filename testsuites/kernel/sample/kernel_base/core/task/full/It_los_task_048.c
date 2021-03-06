/*
 * Copyright (c) 2013-2019 Huawei Technologies Co., Ltd. All rights reserved.
 * Copyright (c) 2020-2021 Huawei Device Co., Ltd. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this list of
 * conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list
 * of conditions and the following disclaimer in the documentation and/or other materials
 * provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its contributors may be used
 * to endorse or promote products derived from this software without specific prior written
 * permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "It_los_task.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cpluscplus */
#endif /* __cpluscplus */

static void TaskF01(void)
{
    UINT32 ret;
    ICUNIT_GOTO_EQUAL(g_testCount, 0, g_testCount, EXIT);
    g_testCount++;

EXIT:
    LOS_TaskDelete(g_testTaskID01);
}

static void TaskF02(void)
{
    UINT32 ret;
    ICUNIT_GOTO_EQUAL(g_testCount, 1, g_testCount, EXIT);
    g_testCount++;

EXIT:
    LOS_TaskDelete(g_testTaskID02);
}

static UINT32 Testcase(void)
{
    UINT32 ret;
    UINT32 taskCnt;
    TSK_INIT_PARAM_S task1 = { 0 };
    task1.pfnTaskEntry = (TSK_ENTRY_FUNC)TaskF01;
    task1.uwStackSize = TASK_STACK_SIZE_TEST;
    task1.pcName = "Tsk048A";
    task1.uwResved = LOS_TASK_STATUS_DETACHED;

    taskCnt = LOSCFG_BASE_CORE_TSK_CONFIG - OsShellCmdTaskCntGet();
    // 4, Possible range of parameters
    if (taskCnt >= 4)
        // 2, Set the priority according to the task purpose
        task1.usTaskPrio = 2;
    // 2, 3, Possible range of parameters
    else if ((taskCnt == 3) || (taskCnt == 2))
        // 1, Set the priority according to the task purpose
        task1.usTaskPrio = 1;
    else
        task1.usTaskPrio = 0;

    g_testCount = 0;

    ret = LOS_TaskCreate(&g_testTaskID01, &task1);
    ICUNIT_ASSERT_EQUAL(ret, LOS_OK, ret);
    TestBusyTaskDelay(2); // 2, set delay time

    ICUNIT_GOTO_EQUAL(g_testCount, 1, g_testCount, EXIT);

    task1.pfnTaskEntry = (TSK_ENTRY_FUNC)TaskF02;
    task1.pcName = "Tsk048B";
    task1.usTaskPrio = OS_TASK_PRIORITY_LOWEST - 1;
    task1.uwResved = LOS_TASK_STATUS_DETACHED;
    ret = LOS_TaskCreate(&g_testTaskID02, &task1);
    ICUNIT_ASSERT_EQUAL(ret, LOS_OK, ret);

    ret = LOS_TaskDelay(4); // 4, set delay time.
    ICUNIT_GOTO_EQUAL(ret, LOS_OK, ret, EXIT1);

    ICUNIT_GOTO_EQUAL(g_testCount, 2, g_testCount, EXIT1); // 2, assert that g_testCount is equal to this.

EXIT1:
    LOS_TaskDelete(g_testTaskID02);
EXIT:
    LOS_TaskDelete(g_testTaskID01);
    return LOS_OK;
}

void ItLosTask048(void) // IT_Layer_ModuleORFeature_No
{
    TEST_ADD_CASE("ItLosTask048", Testcase, TEST_LOS, TEST_TASK, TEST_LEVEL2, TEST_FUNCTION);
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */
