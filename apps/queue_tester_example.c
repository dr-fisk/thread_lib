#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include <queue.h>

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

/* Create */
void test_create(void) {
        fprintf(stderr, "*** TEST create ***\n");

        TEST_ASSERT(queue_create() != NULL);
}

/* Enqueue/Dequeue simple */
void test_queue_simple(void) {
        int data = 3, *ptr;
        queue_t q;

        fprintf(stderr, "*** TEST queue_simple ***\n");

        q = queue_create();
        queue_enqueue(q, &data);
        queue_dequeue(q, (void**)&ptr);
        TEST_ASSERT(ptr == &data);
}

void test_queue_delete(void) {
        int data = 3, *ptr, status;
        queue_t q;
        q = queue_create();
        queue_enqueue(q, &data);
        q = queue_create();
        queue_enqueue(q, &data);
        q = queue_create();
        queue_enqueue(q, &data);
        status = queue_delete(q, data);

        TEST_ASSERT(status == 0);

}

int main(void) {
        test_create();
        test_queue_simple();
        test_queue_delete();
        return 0;
}
