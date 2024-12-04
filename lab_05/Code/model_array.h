#ifndef MODEL_ARRAY_H__
#define MODEL_ARRAY_H__

#include "model.h"

void sd_array(int n, int interval, times_t t1, times_t t2, times_t t3, times_t t4);  //, int flag);

void queuearray_push(queue_t *queue, char c_elem);  //,char *elem, )

char queuearray_pop(queue_t *gueue);  //, char *elem);

void queuearray_print(queue_t *queue);  //, char *elem);

#endif
