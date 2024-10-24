#ifndef MY_REAL_H__
#define MY_REAL_H__

#include "my_types.h"

#define PROCESS_DONE 0
#define READ_REAL_FAILED 1
#define OVERFLOW_REAL 2
#define EMPTY_REAL_LINE 3
#define NOT_REAL 4
#define OVERFLOW_REAL_ORDER 5

int is_real(char *str);

void normalize_real(normal_t *num, char *str);

int read_real(normal_t *num);

#endif
