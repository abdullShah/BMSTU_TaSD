#ifndef MY_TYPES_H__
#define MY_TYPES_H__

#include <stdlib.h>

#define PROCESS_DONE 0
#define EOF_ERROR 1
#define OVERFLOW_ERROR 2

#define MANTISSA_LEN 35
#define LIMIT_ORDER 99999

#define NUM_LEN (2 + MANTISSA_LEN)

typedef struct
{
    int mantissa_sign;
    int mantissa[MANTISSA_LEN];
    size_t mantissa_len;
    int order;
} normal_t;

int read_line(char *str, int max_len);

char *strip(char *str);

void remove_insignificant_zeros(char *new_str, char *str);

int is_zero_string(char *str);

void normalize_zero(normal_t *num);

void print_normal(normal_t num);

#endif
