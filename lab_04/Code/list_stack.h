#ifndef LIST_STACK_H__
#define LIST_STACK_H__

#include <stdlib.h>

#define MAX_SIZE 1000

typedef struct Node {
  int value;
  struct Node *next;
} Node;

typedef struct {
  Node *ps;
  int len;
} list_stack;

void print_free_list(void);

list_stack *init_list_stack(void);

int list_stack_is_empty(list_stack *stack);

int list_stack_is_full(list_stack *stack);

int list_stack_push(list_stack *stack, int value);

int list_stack_pop(list_stack *stack, int is_testing);

void list_stack_print(list_stack *stack);

void list_stack_free(list_stack *stack);

void list_stack_descending_series(list_stack *stack, int v);

#endif
