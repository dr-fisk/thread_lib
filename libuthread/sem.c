#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

#include "queue.h"
#include "sem.h"
#include "private.h"

//Semaphore up and down inspired by slide 21 on lecture 8 (sync)
//test the corner case
struct semaphore {
        size_t resourceCount;
        queue_t blockedThreads;
};

// Create and allocate semaphore
sem_t sem_create(size_t count) {
        struct semaphore *newSem = malloc(sizeof(struct semaphore));
        newSem->resourceCount = count;
        newSem->blockedThreads = queue_create();
        return newSem;
}

// Deallocate semaphore //
int sem_destroy(sem_t sem) {
        if (!sem)
                return -1;

        if (queue_destroy(sem->blockedThreads) == -1)
                return -1;

        free(sem);
        return 0;
}

int sem_down(sem_t sem) {
        if (!sem)
                return -1;

        /* Trap thread until semaphore has available resource */
        while (sem->resourceCount == 0) {
                if (queue_enqueue(sem->blockedThreads, uthread_current()) == -1)
                        return -1;

                uthread_block();
        }

        sem->resourceCount --;
        return 0;
}

int sem_up(sem_t sem) {
        if (!sem)
                return -1;

        sem->resourceCount ++;

        /* Only enter if queue has an item, unblock thread, and dequeue 
        blocked queue */
        if (queue_length(sem->blockedThreads) > 0) {
                struct uthread_tcb *unblockThread;

                if (queue_dequeue(sem->blockedThreads, (void **) &unblockThread) == -1)
                        return -1;

                uthread_unblock(unblockThread);
        }

        return 0;
}



