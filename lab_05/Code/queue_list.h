#ifndef QUEUE_LIST_H__
#define QUEUE_LIST_H__

typedef struct list_node_t
{
    int data;
    struct list_node_t *next;
} list_node_t;

typedef struct
{
    list_node_t *head;
    list_node_t *tail;
} queue_list_t;

void queue_list_init(queue_list_t *queue);

int queue_list_is_empty(queue_list_t *queue);

void queue_list_push(queue_list_t *queue, int value);

int queue_list_pop(queue_list_t *queue);

void queue_list_print(queue_list_t *queue);

void queue_list_free(queue_list_t *queue);

size_t queue_list_memory(queue_list_t *q);

#endif
