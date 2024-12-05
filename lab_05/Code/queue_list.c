#include <stdio.h>
#include <stdlib.h>
#include "queue_list.h"

void queue_list_init(queue_list_t *queue)
{
    queue->head = NULL;
    queue->tail = NULL;
}

int queue_list_is_empty(queue_list_t *queue)
{
    return queue->head == NULL;
}

void queue_list_push(queue_list_t *queue, int value)
{
    list_node_t *new_node = (list_node_t *) malloc(sizeof(list_node_t));
    if (!new_node)
    {
        printf("Ошибка: не удалось выделить память.\n\n");
        return;
    }

    new_node->data = value;
    new_node->next = NULL;

    if (queue_list_is_empty(queue))
        queue->head = new_node;
    else
        queue->tail->next = new_node;

    queue->tail = new_node;
}

int queue_list_pop(queue_list_t *queue)
{
    list_node_t *temp = queue->head;
    int value = temp->data;
    queue->head = queue->head->next;
    if (queue->head == NULL)
        queue->tail = NULL;  // Если очередь стала пустой

    free(temp);
    return value;
}

static void print_reverse(list_node_t *node)
{
    if (node == NULL)
        return;

    print_reverse(node->next);
    printf("%d ", node->data);
}

void queue_list_print(queue_list_t *queue)
{
    if (queue_list_is_empty(queue))
    {
        printf("Очередь пуста!\n\n");
        return;
    }

    printf("Состояние очереди:\n-->\t");
    print_reverse(queue->head);
    printf("\t-->\n\n");
}

void queue_list_free(queue_list_t *queue)
{
    while (!queue_list_is_empty(queue))
        queue_list_pop(queue);
}

size_t queue_list_memory(queue_list_t *q)
{
    size_t memory = sizeof(q->head) + sizeof(q->tail);  // Память на указатели головы и хвоста
    list_node_t *current = q->head;

    // Считаем память, занимаемую каждым узлом списка
    while (current)
    {
        memory += sizeof(list_node_t);  // Каждый узел занимает память под data и next
        current = current->next;
    }

    return memory;
}
