#ifndef QUEUE_DYN_ARR_H__
#define QUEUE_DYN_ARR_H__

typedef struct
{
    int *data;
    int capacity;
    int size;
    int head;
    int tail;
} queue_dyn_arr_t;

void queue_dyn_arr_init(queue_dyn_arr_t *queue);

void queue_dyn_arr_free(queue_dyn_arr_t *queue);

int queue_dyn_arr_is_empty(const queue_dyn_arr_t *queue);

int queue_dyn_arr_is_full(const queue_dyn_arr_t *queue);

void queue_dyn_arr_push(queue_dyn_arr_t *queue, int value);

void queue_dyn_arr_pop(queue_dyn_arr_t *queue);

void queue_dyn_arr_print(const queue_dyn_arr_t *queue);

#endif
