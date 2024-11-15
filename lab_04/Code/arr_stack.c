#include <stdio.h>
#include <stdlib.h>
#include "arr_stack.h"

void arr_stack_init(arr_stack *stack)
{
    stack->ps = 0;
}

int arr_stack_is_empty(arr_stack stack)
{
    return stack.ps == 0;
}

int arr_stack_is_full(arr_stack stack)
{
    return stack.ps == MAX_SIZE;
}

void arr_stack_push(arr_stack *stack, int value)
{
    stack->data[(stack->ps)++] = value;
}

int arr_stack_pop(arr_stack *stack)
{
    return stack->data[--(stack->ps)];
}

void arr_stack_print(arr_stack *stack)
{
    printf("\nСодержимое стека с использованием массива:\n");

    for (int i = 0; i < stack->ps; i++)
        printf("{ %p } [%d] = %d\n", (void *) &stack->data[i], i, stack->data[i]);

    printf("\nРазмер стека (массива): %d\n", stack->ps);
    printf("Занятая память (байты): %lu\n\n", (unsigned long) (stack->ps * sizeof(int) + sizeof(int)));
}

void arr_stack_descending_series(arr_stack *stack, int v)
{
    if (v)
        printf("\nУбывающие серии:\n");

    int last_value = arr_stack_pop(stack);
    int current_value;

    while (!arr_stack_is_empty(*stack))
    {
        if (v)
            printf("%d ", last_value);

        current_value = arr_stack_pop(stack);

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
