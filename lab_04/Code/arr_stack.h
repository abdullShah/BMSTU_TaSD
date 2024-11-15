#ifndef ARR_STACK_H__
#define ARR_STACK_H__

#include <stdlib.h>

#define MAX_SIZE 1000

typedef struct {
  int data[MAX_SIZE];
  int ps;
} arr_stack;

void arr_stack_init(arr_stack *stack);

int arr_stack_is_empty(arr_stack stack);

int arr_stack_is_full(arr_stack stack);

void arr_stack_push(arr_stack *stack, int value);

int arr_stack_pop(arr_stack *stack);

void arr_stack_print(arr_stack *stack);

void arr_stack_descending_series(arr_stack *stack, int v);

#endif
