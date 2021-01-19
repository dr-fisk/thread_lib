/*
 * Thread creation and yielding test
 *
 * Tests the creation of multiples threads and the fact that a parent thread
 * should get returned to before its child is executed. The way the printing,
 * thread creation and yielding is done, the program should output:
 *
 * thread1
 * thread2
 * thread3
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include <uthread.h>

#define TEST_ASSERT(assert)                             \
do {                                                                    \
        printf("ASSERT: " #assert " ... ");     \
        if (assert) {                                           \
                printf("PASS\n");                               \
        } else	{                                                       \
                printf("FAIL\n");                               \
                /*exit(1);*/                                                \
        }                                                                       \
} while(0)

void thread3(void *arg) {
        uthread_yield();
        printf("thread3\n");
}

void thread2(void *arg) {
        uthread_create(thread3, NULL);
        uthread_yield();
        printf("thread2\n");
}

void thread1(void *arg) {
        uthread_create(thread2, NULL);
        uthread_yield();
        printf("thread1\n");
        uthread_yield();
}

void threadStart(void) {
        int status;
        status = uthread_start(thread1, NULL);
        TEST_ASSERT(status == 0);
        printf("Thread start(thread1, NULL) executed correctly\n");
}

void testStartFailure(void) {
        TEST_ASSERT(uthread_start(NULL, NULL) == 0);
}

void testCreateFailure(void) {
        TEST_ASSERT(uthread_create(NULL, NULL) == 0);
}

int main(void) {
        threadStart();
        testStartFailure();
        testCreateFailure();
        return 0;
}
