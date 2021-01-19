#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <queue.h>

#define TEST_ASSERT(assert)                             \
do {                                                                    \
        printf("ASSERT: " #assert " ... ");     \
        if (assert) {                                           \
                printf("PASS\n");                               \
        } else        {                                                       \
                printf("FAIL\n");                               \
                exit(1);                                                \
        }                                                                       \
} while(0)

void test_create(void) {
        queue_t q;
        q = queue_create();
        TEST_ASSERT(q != NULL);
        printf("Queue is created\n");
        TEST_ASSERT(queue_destroy(q) == 0);
        q = NULL;
        printf("Queue is destroyed\n");
        printf("Next queue destroy should fail\n");
        TEST_ASSERT(queue_destroy(q) == 0);
}

void test_empty_enqueue() {
        queue_t q;
        int data = 3, *ptr;
        q = queue_create();

        for (int i = 0; i < 10; i++) {
                queue_enqueue(q, &data);
        }

        for (int j = 0; j < 12; j++)
                queue_dequeue(q, (void **) &ptr);
}

void test_empty_dequeue() {
        queue_t q;
        int *ptr;
        q = queue_create();
        TEST_ASSERT(queue_dequeue(q, (void **) &ptr) == 0);
}

void test_int() {
        queue_t q;
        int data = 3, *ptr;
        q = queue_create();
        queue_enqueue(q, &data);
        queue_dequeue(q, (void **) &ptr);
        TEST_ASSERT(ptr == &data);
}

void test_char() {
        queue_t q;
        q = queue_create();
        char *jimbo = malloc(sizeof(int));
        char *kimbo = malloc(sizeof(int));
        *jimbo = 'F';
        queue_enqueue(q, jimbo);
        queue_dequeue(q, (void **) &kimbo);

        printf("%s\n", kimbo);
}

void test_queue_delete(void) {
        int data = 3, data2 = 4, data3 = 5;
        queue_t q;
        q = queue_create();
        printf("First enqueue\n");
        queue_enqueue(q, &data);
        printf("Second enqueue\n");
        queue_enqueue(q, &data2);
        printf("Multiple deletes\n");
        queue_enqueue(q, &data3);
        TEST_ASSERT(queue_delete(q, &data2) == 0);
        TEST_ASSERT(queue_delete(q, &data3) == 0);
        TEST_ASSERT(queue_delete(q, &data) == 0);
        queue_destroy(q);
}

int main(void) {
        test_queue_delete();
}
