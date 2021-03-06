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

#include "It_posix_pthread.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cpluscplus */
#endif /* __cpluscplus */

static VOID *PthreadF01(void *argument)
{
    g_testCount++;

    return argument;
}

static UINT32 Testcase(VOID)
{
    pthread_t newTh;
    pthread_attr_t attr;
    UINT32 ret;
    UINT32 uwint = 8;
    char cc = 'a';
    char str[5] = "abcd";
    _pthread_data *joinee = NULL;
    UINTPTR temp;

    g_testCount = 0;

    ret = pthread_attr_init(&attr);
    ICUNIT_ASSERT_EQUAL(ret, 0, ret);

    /* Create a new thread. */
    ret = pthread_create(NULL, &attr, PthreadF01, NULL);
    ICUNIT_ASSERT_EQUAL(ret, EINVAL, ret);

    ret = pthread_create(&newTh, NULL, PthreadF01, (void *)&uwint);
    ICUNIT_ASSERT_EQUAL(ret, 0, ret);
    LOS_TaskDelay(1);
    ICUNIT_ASSERT_EQUAL(g_testCount, 1, g_testCount);

    ret = pthread_join(newTh, (void *)&temp);
    ICUNIT_ASSERT_EQUAL(ret, 0, ret);
    ICUNIT_ASSERT_EQUAL(*((UINT32 *)temp), 8, *((UINT32 *)temp)); // 8, here assert the result.

    ret = pthread_create(&newTh, &attr, NULL, (void *)8); // 8, test for invalid param.
    ICUNIT_ASSERT_EQUAL(ret, EINVAL, ret);

    ret = pthread_create(&newTh, &attr, PthreadF01, NULL);
    ICUNIT_ASSERT_EQUAL(ret, 0, ret);

    LOS_TaskDelay(1);
    ICUNIT_ASSERT_EQUAL(g_testCount, 2, g_testCount); // 2, here assert the result.

    ret = pthread_join(newTh, (void *)&temp);
    ICUNIT_ASSERT_EQUAL(ret, 0, ret);
    ICUNIT_ASSERT_EQUAL((UINT32)temp, 0, (UINT32)temp);

    ret = pthread_create(&newTh, &attr, PthreadF01, (void *)&cc);
    ICUNIT_ASSERT_EQUAL(ret, 0, ret);

    LOS_TaskDelay(1);
    ICUNIT_ASSERT_EQUAL(g_testCount, 3, g_testCount); // 3, here assert the result.
    ret = pthread_join(newTh, (void *)&temp);
    ICUNIT_ASSERT_EQUAL(ret, 0, ret);
    ICUNIT_ASSERT_EQUAL(*((char *)temp), 97, *((char *)temp)); // 97, here assert the result.

    ret = pthread_create(&newTh, &attr, PthreadF01, (void *)str);
    ICUNIT_ASSERT_EQUAL(ret, 0, ret);

    LOS_TaskDelay(1);
    ICUNIT_ASSERT_EQUAL(g_testCount, 4, g_testCount); // 4, here assert the result.

    ret = pthread_join(newTh, (void *)&temp);
    ICUNIT_ASSERT_EQUAL(ret, 0, ret);
    ICUNIT_ASSERT_STRING_EQUAL((char *)temp, "abcd", (char *)temp);

    ret = pthread_attr_destroy(&attr);
    ICUNIT_ASSERT_EQUAL(ret, 0, ret);

    return PTHREAD_NO_ERROR;
}

VOID ItPosixPthread007(VOID) // IT_Layer_ModuleORFeature_No
{
    TEST_ADD_CASE("ItPosixPthread007", Testcase, TEST_POSIX, TEST_PTHREAD, TEST_LEVEL2, TEST_FUNCTION);
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */
