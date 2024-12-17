#include <stdio.h>
#include <stdlib.h>
#include "queue_arr.h"

void queue_arr_init(queue_arr_t *q)
{
    q->head = 0;
    q->tail = -1;
    q->size = 0;
}

int queue_arr_is_empty(queue_arr_t *q)
{
    return q->size == 0;
}

int queue_arr_is_full(queue_arr_t *q)
{
    return q->size == MAX_QUEUE_SIZE;
}

void queue_arr_push(queue_arr_t *q, int value)
{
    q->tail = (q->tail + 1) % MAX_QUEUE_SIZE;  // Циклический переход
    q->data[q->tail] = value;
    q->size++;
}

int queue_arr_pop(queue_arr_t *q)
{
    int value = q->data[q->head];
    q->head = (q->head + 1) % MAX_QUEUE_SIZE;  // Циклический переход
    q->size--;
    return value;
}

void queue_arr_print(queue_arr_t *q)
{
    if (queue_arr_is_empty(q))
    {
        printf("Очередь пуста.\n\n");
        return;
    }

    printf("Состояние очереди:\n-->\t");
    for (int i = 0; i < q->size; i++)
    {
        int idx = (q->tail - i + MAX_QUEUE_SIZE) % MAX_QUEUE_SIZE;  // Обратный проход
        printf("%d ", q->data[idx]);
    }
    printf("\t-->\n\n");
}

size_t queue_arr_memory(queue_arr_t *q)
{
    // Размер массива + размер метаданных (голова, хвост, размер)
    return sizeof(q->data) + sizeof(q->head) + sizeof(q->tail) + sizeof(q->size);
}
