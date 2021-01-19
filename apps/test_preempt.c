#include <stdio.h>
#include <stdlib.h>

#include <uthread.h>

int exitWhile = 0;

void thread2(void *arg) {
        printf("thread2\n");
        exitWhile = 1;
}

void thread1(void *arg) {
        uthread_create(thread2, NULL);

        while (exitWhile == 0)
                printf("thread1\n");
}

int main() {
        uthread_start(thread1, NULL);
        return 0;
}
