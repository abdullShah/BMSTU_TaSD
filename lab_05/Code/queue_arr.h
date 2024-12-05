#ifndef QUEUE_ARR_H__
#define QUEUE_ARR_H__

#define MAX_QUEUE_SIZE 1000

typedef struct
{
    int data[MAX_QUEUE_SIZE];
    int head;
    int tail;
    int size;
} queue_arr_t;

void queue_arr_init(queue_arr_t *q);

int queue_arr_is_empty(queue_arr_t *q);

int queue_arr_is_full(queue_arr_t *q);

void queue_arr_push(queue_arr_t *q, int value);

int queue_arr_pop(queue_arr_t *q);

void queue_arr_print(queue_arr_t *q);

size_t queue_arr_memory(queue_arr_t *q);

#endif
