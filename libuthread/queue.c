#include <stdint.h>
#include <stdlib.h>

#include "queue.h"

// Doubly Linked List
struct node {
        void *data;
        struct node *next;
};

struct queue {
        int size;
        struct node *head;
        struct node *tail;
};

queue_t queue_create(void) {
        queue_t newQueue = malloc(sizeof(struct queue));
        newQueue->size = 0;
        newQueue->head = NULL;
        newQueue->tail = newQueue->head;
        return newQueue;
}

int queue_destroy(queue_t queue) {
        if (!queue || queue->size != 0)
                return -1;

        free(queue);

        return 0;
}

int queue_enqueue(queue_t queue, void *data) {
        if (!data || !queue)
                return -1;

        struct node *newNode = malloc(sizeof(struct node));
        newNode->data = data;
        newNode->next = NULL;

        // Tail will always have a null next ptr
        if (!queue->head) {
                queue->head = newNode;
                queue->tail = queue->head;
        } else { // When both head and tail are null
                queue->tail->next = newNode;
                queue->tail = queue->tail->next;
        }

        queue->size++;
        return 0;
}

int queue_dequeue(queue_t queue, void **data) {
        struct node *removedNode;

        // If queue is empty terminate //
        if (queue->size <= 0 || !queue)
                return -1;

        removedNode = queue->head;
        *data = removedNode->data;

        if (queue->head != queue->tail)
                queue->head = queue->head->next;
        else {
                queue->head = NULL;
                queue->tail = queue->head;
        }

        free(removedNode);
        queue->size--;

        return 0;
}

void queue_delete_head(queue_t queue) {
        struct node *current;
        current = queue->head;
        queue->head = queue->head->next;
        free(current);
        queue->size --;
}

void queue_delete_tail(queue_t queue) {
        struct node *current;
        struct node *prev;
        current = queue->head;

        while (current->next != NULL) {
                prev = current;
                current = current->next;
        }

        queue->tail = prev;
        prev->next = NULL;
        free(current);
        queue->size --;
}

int queue_delete_middle(queue_t queue, void *data) {
        struct node *current;
        struct node *prev;

        current = queue->head;

        while (current) {
                if (data == current->data) {
                        prev->next = current->next;
                        free(current);
                        queue->size--;
                        return 0;
                }

                prev = current;
                current = current->next;
        }

        return -1;
}

int queue_delete(queue_t queue, void *data) {
        if (!queue || !data)
                return -1;

        if (queue->head->data == data) {
                queue_delete_head(queue);
                return 0;
        } else if (queue->tail->data == data) {
                queue_delete_tail(queue);
                return 0;
        } else  // Return -1 if no node matches data
                return queue_delete_middle(queue, data);
}

// Go through entire list and send node data through a function call
int queue_iterate(queue_t queue, queue_func_t func) {
        if (!queue || !func)
                return -1;

        for (struct node *current = queue->head; current->next != NULL;
             current = current->next)
                func(current->data);

        return 0;
}

// Return the length of the queue
int queue_length(queue_t queue) {
        if (queue)
                return queue->size;

        return -1;
}