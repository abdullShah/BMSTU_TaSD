#ifndef DIVISION_H__
#define DIVISION_H__

#include "my_types.h"

#define PROCESS_DONE 0
#define DIVISION_BY_ZERO 1
#define OVERFLOW_ORDER 2

int division(normal_t *res, normal_t num1, normal_t num2);

#endif
