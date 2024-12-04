#ifndef MODEL_LIST_H__
#define MODEL_LIST_H__

#include "model.h"

node_t* createelem(queuetype c);

node_t* pushfront(node_t *head, node_t *elem);

node_t* popfront(node_t **head);

node_t* popend(node_t **head);

void freeall(node_t *head);

void sd_list(int n, int interval, times_t t1, times_t t2, times_t t3, times_t t4, int flag);

node_t *queuelist_push(node_t *queue, queuetype c, node_t **user_memory, int *user_count, node_t **free_memory, int *free_count, int *second_use);

node_t *queuelist_pop(node_t **queue);

void queuelist_print(node_t *queue);

#endif
