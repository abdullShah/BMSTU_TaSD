#ifndef MY_INT_H__
#define MY_INT_H__

#include "my_types.h"

#define PROCESS_DONE 0
#define READ_INT_FAILED 1
#define OVERFLOW_INT 2
#define EMPTY_INT_LINE 3
#define NOT_INT 4

int is_int(char *str);

void normalize_int(normal_t *num, char *str);

int read_int(normal_t *num);

#endif
