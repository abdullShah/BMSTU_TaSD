#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list_stack.h"

typedef struct
{
    Node *addresses[MAX_SIZE];
    int values[MAX_SIZE];
    int len;
} FreeList;

FreeList free_list = { .len = 0 };

static void add_to_free_list(Node *node)
{
    if (!(free_list.len < MAX_SIZE))
        printf("\nСписок освобожденных узлов полон!\n\n");

    free_list.addresses[free_list.len] = node;
    free_list.values[free_list.len] = node->value;
    free_list.len++;
}

void print_free_list(void)
{
    if (free_list.len == 0)
    {
        printf("\nСписок освобожденных узлов пуст!\n\n");
        return;
    }

    printf("\nСписок освобожденных узлов:\n");
    for (int i = 0; i < free_list.len; i++)
        printf("{ %p } [%d]: %d\n", (void *) free_list.addresses[i], i, free_list.values[i]);

    printf("\n");
}

static Node *create_node(int value)
{
    Node *node = malloc(sizeof(Node));

    if (node == NULL)
        return NULL;

    node->value = value;
    node->next = NULL;

    return node;
}

list_stack *init_list_stack(void)
{
    list_stack *stack = malloc(sizeof(list_stack));
    if (!stack)
        return NULL;

    stack->ps = NULL;
    stack->len = 0;

    return stack;
}

int list_stack_is_empty(list_stack *stack)
{
    return stack->len == 0;
}

int list_stack_is_full(list_stack *stack)
{
    return stack->len == MAX_SIZE;
}

int list_stack_push(list_stack *stack, int value)
{
    Node *node = create_node(value);

    if (!node)
        return 1;

    node->next = stack->ps;
    stack->ps = node;
    stack->len++;

    return 0;
}

int list_stack_pop(list_stack *stack, int is_testing)
{
    if (!stack->ps)
        return 1;

    Node *tmp = stack->ps;
    stack->ps = stack->ps->next;

    int value = tmp->value;
    if (!is_testing)
        add_to_free_list(tmp);
    free(tmp);
    stack->len--;

    return value;
}

void list_stack_print(list_stack *stack)
{
    printf("\nСодержимое стека с использованием списка:\n");

    Node *cur = stack->ps;

    int index = 0;
    while (cur)
    {
        printf("{ %p } [%d] = %d\n", (void *) cur, stack->len - index - 1, cur->value);
        cur = cur->next;
        index++;
    }

    printf("\nРазмер стека (списка): %d\n", index);
    printf("Занятая память (байты): %lu\n\n",
           (unsigned long) (sizeof(int) + sizeof(Node *) + index * (sizeof(int) + sizeof(Node *))));
}

void list_stack_free(list_stack *stack)
{
    Node *cur = stack->ps;
    Node *next;

    while (cur)
    {
        next = cur->next;
        free(cur);
        cur = next;
    }

    free(stack);
}

void list_stack_descending_series(list_stack *stack, int v)
{
    if (v)
        printf("\nУбывающие серии:\n");

    int last_value = list_stack_pop(stack, !v);
    int current_value;

    while (!list_stack_is_empty(stack))
    {
        if (v)
            printf("%d ", last_value);

        current_value = list_stack_pop(stack, !v);

        if (!(current_value > last_value))
            if (v)
                printf("\n");

        last_value = current_value;
    }

    if (v)
    {
        printf("%d\n", last_value);
        printf("\n");
    }
}
