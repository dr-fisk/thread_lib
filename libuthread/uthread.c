#include <assert.h>
#include <signal.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/time.h>
#include <stdio.h>

#include "private.h"
#include "uthread.h"
#include "queue.h"

/*All functions contain basic error handling elements
 programs exit with -1 to signify an error while expressing the functions
 the error occurred in. */
struct uthread_tcb {
        void *stack;
        uthread_ctx_t ctx;
};

static struct uthread_tcb *runningThread;
static queue_t waitingThreads, zombieThreads;

/* Returns running thread using dequeue, places running thread back into
running queue */
struct uthread_tcb *uthread_current(void) {
        return runningThread;
}

void uthread_switch(void) {
        struct uthread_tcb *nextThread;
        queue_dequeue(waitingThreads, (void **) &nextThread);
        runningThread = nextThread;
}

// Yield running thread, update thread states, and update queues
void uthread_yield(void) {
        preempt_disable();
        struct uthread_tcb *currentThread;
        currentThread = runningThread;
        uthread_switch();
        preempt_enable();
        uthread_ctx_switch(&currentThread->ctx, &runningThread->ctx);
}

// Exits finished thread, update thread states, and update the current queues
void uthread_exit(void) {
        struct uthread_tcb *finishedThread = uthread_current();
        queue_enqueue(zombieThreads, finishedThread);
        uthread_switch();
        uthread_ctx_switch(&finishedThread->ctx, &runningThread->ctx);
}

// Construct a new thread and enqueue to waiting queue
int uthread_create(uthread_func_t func, void *arg) {
        struct uthread_tcb *newThread = malloc(sizeof(struct uthread_tcb));

        // Failed to allocate memory or bad func given
        if (!newThread || !func)
                return -1;

        newThread->stack = uthread_ctx_alloc_stack();
        uthread_ctx_init(&newThread->ctx, newThread->stack, func, arg);

        // Failed to enqueue
        if (queue_enqueue(waitingThreads, newThread) != 0)
                return -1;

        return 0;
}

int uthread_start(uthread_func_t func, void *arg) {
        /* Creates a thread without context to allow context switch to 
        thread with an actual context */
        struct uthread_tcb *noCtxThread = malloc(sizeof(struct uthread_tcb));

        // Create queue which threads will be placed into
        waitingThreads = queue_create();
        zombieThreads = queue_create();

        // Failure to create thread process
        if (uthread_create(func, arg) != 0 || !waitingThreads)
                return -1;

        runningThread = noCtxThread;

        // Queue failed to enqueue
        if (!runningThread)
                return -1;

        // Loop until all threads have exited
        while (queue_length(waitingThreads) != 0)
                uthread_yield();

        if (runningThread)
                free(runningThread);

        struct uthread_tcb *zombie;

        while (queue_length(zombieThreads) != 0) {
                queue_dequeue(zombieThreads, (void **) &zombie);

                if (zombie)
                        free(zombie);
        }

        // Failure to dellocate memory
        if (queue_destroy(waitingThreads) != 0)
                return -1;

        return 0;
}

/* Blocks running thread, called from semaphore, thread is then stored in
another queue in semaphore */
void uthread_block(void) {
        struct uthread_tcb *blockedThread = uthread_current();
        uthread_switch();
        uthread_ctx_switch(&blockedThread->ctx, &runningThread->ctx);
}

// Enqueues blocked thread back into waiting queue from semapore
void uthread_unblock(struct uthread_tcb *uthread) {
        queue_enqueue(waitingThreads, uthread);
}