#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include "my_int.h"

// Функция проверяет, является ли строка целым числом
int is_int(char *str)
{
    if (strchr(str, '.') != NULL)
        return 0;

    if (!isdigit(*str) && (*str) != '+' && (*str) != '-')
        return 0;

    if ((*str) == '+' || (*str) == '-')
        str++;

    for (; *str != '\0'; str++)
        if (!isdigit(*str))
            return 0;

    return 1;
}

// Функция нормализует целое число и записывает его в структуру normal_t
void normalize_int(normal_t *num, char *str)
{
   if (str[0] == '-')
   {
       num->mantissa_sign = 1;
       str++;
   }
   else if (str[0] == '+')
   {
       num->mantissa_sign = 0;
       str++;
   }
   else
       num->mantissa_sign = 0;

    memset(num->mantissa, 0, sizeof(num->mantissa));

    int order = 0;
    for (int i = strlen(str) - 1; i >= 0 && str[i] == '0'; i--)
        order++;

    int c_order = order;
    int mantissa_len = 0;
    for (size_t i = 0; i < strlen(str) - c_order; i++)
    {
        num->mantissa[i] = str[i] - '0';
        order++;
        mantissa_len++;
    }

    num->order = order;
    num->mantissa_len = mantissa_len;
}

// Функция считывает строку с целым числом
int read_int(normal_t *num)
{
    char str[NUM_LEN + 1];
    int rc;

    rc = read_line(str, NUM_LEN);

    switch (rc)
    {
        case EOF_ERROR: return READ_INT_FAILED;
        case OVERFLOW_ERROR:    return OVERFLOW_INT;
    }

    char *trim_str = strip(str);

    if (strlen(trim_str) == 0)
        return EMPTY_INT_LINE;

    if (!is_int(trim_str))
        return NOT_INT;

    if (is_zero_string(trim_str))
    {
        normalize_zero(num);
        return PROCESS_DONE;
    }

    char no_zero_str[NUM_LEN + 1];

    remove_insignificant_zeros(no_zero_str, trim_str);

    normalize_int(num, no_zero_str);

    return PROCESS_DONE;
}
