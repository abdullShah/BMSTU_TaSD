#include <stdio.h>
#include <stdlib.h>
#include "queue_dyn_arr.h"

#define INIT_CAPACITY 1
#define ALLOC_SIZE 2

void queue_dyn_arr_init(queue_dyn_arr_t *queue)
{
    queue->capacity = INIT_CAPACITY;
    queue->size = 0;
    queue->head = 0;
    queue->tail = -1;
    queue->data = (int *) malloc(queue->capacity * sizeof(int));
    if (!queue->data)
    {
        printf("Ошибка выделения памяти\n");
        return;
    }
}

void queue_dyn_arr_free(queue_dyn_arr_t *queue)
{
    free(queue->data);
    queue->data = NULL;
    queue->capacity = 0;
    queue->size = 0;
    queue->head = 0;
    queue->tail = -1;
}

int queue_dyn_arr_is_empty(const queue_dyn_arr_t *queue)
{
    return queue->size == 0;
}

int queue_dyn_arr_is_full(const queue_dyn_arr_t *queue)
{
    return queue->size == queue->capacity;
}

void queue_dyn_arr_push(queue_dyn_arr_t *queue, int value)
{
    if (queue_dyn_arr_is_full(queue))
    {
        queue->capacity *= ALLOC_SIZE;
        queue->data = (int *) realloc(queue->data, queue->capacity * sizeof(int));
        if (!queue->data)
        {
            printf("Ошибка увеличения памяти\n");
            return;
        }
        // Обновить индексы для циклической очереди
        if (queue->tail < queue->head)
        {
            for (int i = 0; i < queue->tail + 1; i++)
                queue->data[queue->size + i] = queue->data[i];

            queue->tail += queue->size;
        }
    }

    queue->tail = (queue->tail + 1) % queue->capacity;
    queue->data[queue->tail] = value;
    queue->size++;
}

int queue_dyn_arr_pop(queue_dyn_arr_t *queue)
{
    int value = queue->data[queue->head];
    queue->head = (queue->head + 1) % queue->capacity;
    queue->size--;
    return value;
}

void queue_dyn_arr_print(const queue_dyn_arr_t *queue)
{
    if (queue_dyn_arr_is_empty(queue))
    {
        printf("Очередь пуста!\n\n");
        return;
    }

    printf("Состояние очереди:\n-->\t");
    for (int i = 0; i < queue->size; i++)
    {
        int idx = (queue->tail - i + queue->capacity) % queue->capacity;
        printf("%d ", queue->data[idx]);
    }
    printf("\t-->\n\n");
}

size_t queue_dyn_arr_memory(queue_dyn_arr_t *q)
{
    // Память под данные (динамический массив) + память под метаданные
    return (q->capacity * sizeof(int)) + sizeof(q->capacity) + sizeof(q->size) + sizeof(q->head) + sizeof(q->tail);
}
