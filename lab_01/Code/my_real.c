#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include "my_real.h"

// Функция проверяет, является ли строка вещественным числом
int is_real(char *str)
{
    int has_digit = 0;
    int has_dot = 0;
    int has_exp = 0;

    if (*str == '+' || *str == '-')
        str++;

    while (*str != '\0')
    {
        if (isdigit(*str))
            has_digit = 1;
        else if (*str == '.' && !has_dot)
            has_dot = 1;
        else if ((*str == 'e' || *str == 'E') && !has_exp && has_digit)
        {
            has_exp = 1;
            has_digit = 0;  // Сбросим has_digit для проверки цифр после экспоненты
            str++;
            if (*str == '+' || *str == '-')
                str++;

            continue;
        }
        else
            return 0;

        str++;
    }

    return has_digit;
}

// Функция нормализует вещественное число и записывает его в структуру normal_t
void normalize_real(normal_t *num, char *str)
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

    int ind = 0;
    int order = 0;
    int non_zero_found = 0;
    int is_whole_part = 0;

    while (*str != '.' && *str != '\0' && *str != 'e' && *str != 'E')
    {
        if (*str != '0')
            non_zero_found = 1;

        if (non_zero_found)
        {
            is_whole_part = 1;
            num->mantissa[ind++] = *str - '0';
            num->mantissa_len++;
            order++;
        }

        str++;
    }

    if (*str == '.')
    {
        str++;
        while (*str != '\0' && *str != 'e' && *str != 'E')
        {
            if (!is_whole_part && *str == '0')
                order--;
            else
            {
                num->mantissa[ind++] = *str - '0';
                num->mantissa_len++;
            }
            str++;
        }
    }

    if (*str == 'e' || *str == 'E')
    {
        str++;
        order += atoi(str);
    }

    while (num->mantissa[num->mantissa_len - 1] == 0)
        num->mantissa_len--;

    num->order = order;
}

// Функция считывает строку с вещественным числом
int read_real(normal_t *num)
{
    char str[NUM_LEN + 1];
    int rc;

    rc = read_line(str, NUM_LEN);

    switch (rc)
    {
        case EOF_ERROR: return READ_REAL_FAILED;
        case OVERFLOW_ERROR:    return OVERFLOW_REAL;
    }

    char *trim_str = strip(str);

    if (strlen(trim_str) == 0)
        return EMPTY_REAL_LINE;

    if (!is_real(trim_str))
        return NOT_REAL;

    if (is_zero_string(trim_str))
    {
        normalize_zero(num);
        return PROCESS_DONE;
    }

    char no_zero_str[NUM_LEN + 1];

    remove_insignificant_zeros(no_zero_str, trim_str);

    normalize_real(num, no_zero_str);

    /*if (abs(num->order) > LIMIT_ORDER)
        return OVERFLOW_REAL_ORDER;*/

    return PROCESS_DONE;
}
