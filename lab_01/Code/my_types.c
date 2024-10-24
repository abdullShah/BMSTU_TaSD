#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include "my_types.h"

// Функция считывает строку из стандартного ввода
int read_line(char *str, int max_len)
{
    if (!fgets(str, max_len + 1, stdin))
        return EOF_ERROR;

    if (str[strlen(str) - 1] != '\n')
        return OVERFLOW_ERROR;

    str[strlen(str) - 1] = '\0';

    return PROCESS_DONE;
}

// Функция удаляет пробелы в начале и в конце строки
char *strip(char *str)
{
    char *end;

    while (isspace(*str))
        str++;

    if (*str == '\0')
        return str;

    end = str + strlen(str) - 1;
    while (end > str && isspace(*end))
        end--;

    *(end + 1) = '\0';

    return str;
}

// Функция удаляет незначащие нули из строки числа
void remove_insignificant_zeros(char *new_str, char *str)
{
    size_t i = 0, j = 0;
    size_t dot_pos = 0;
    size_t last_non_zero = 0;
    size_t exp_pos = 0;

    if (str[i] == '+' || str[i] == '-')
        new_str[j++] = str[i++];

    while (str[i] == '0')
        i++;

    while (str[i] != '\0' && str[i] != 'e' && str[i] != 'E')
    {
        if (str[i] == '.')
            dot_pos = j;
        else if (str[i] != '0')
            last_non_zero = j;

        new_str[j++] = str[i++];
    }

    if (dot_pos > 0)
    {
        if (last_non_zero < dot_pos)
            j = dot_pos;
        else
            j = last_non_zero + 1;

        if (new_str[last_non_zero] == '.')
            j = dot_pos;
    }

    if (str[i] == 'e' || str[i] == 'E')
    {
        new_str[j++] = str[i++];

        if (str[i] == '+' || str[i] == '-')
            new_str[j++] = str[i++];

        while (str[i] == '0')
            i++;

        exp_pos = j;

        while (isdigit(str[i]))
            new_str[j++] = str[i++];

        if (exp_pos == j)
            new_str[j++] = '0';
    }

    new_str[j] = '\0';
}

// Функция проверяет, представляет ли строка число ноль
int is_zero_string(char *str)
{
    int has_non_zero_digit = 0;
    char *ptr = str;

    // Пропускаем начальные пробелы и знак
    while (*ptr == ' ' || *ptr == '+' || *ptr == '-')
        ptr++;

    // Проверяем цифры до точки
    while (isdigit(*ptr))
    {
        if (*ptr != '0')
            has_non_zero_digit = 1;

        ptr++;
    }

    // Если есть точка, проверяем цифры после точки
    if (*ptr == '.')
    {
        ptr++;
        while (isdigit(*ptr))
        {
            if (*ptr != '0')
                has_non_zero_digit = 1;

            ptr++;
        }
    }

    // Проверяем, есть ли экспонента
    if (*ptr == 'e' || *ptr == 'E')
    {
        ptr++;

        // Пропускаем знак экспоненты
        if (*ptr == '+' || *ptr == '-')
            ptr++;

        // Проверяем цифры экспоненты
        while (isdigit(*ptr))
            ptr++;
    }

    // Возвращаем 1 (истина), если строка соответствует нулю
    return (*ptr == '\0' && !has_non_zero_digit);
}

// Функция нормализует представление числа ноль в структуре normal_t
void normalize_zero(normal_t *num)
{
    num->mantissa_sign = 0;
    memset(num->mantissa, 0, sizeof(num->mantissa));
    num->mantissa_len = 1;
    num->order = 0;
}

// Функция выводит нормализованное число в определенном формате
void print_normal(normal_t num)
{
    if (num.mantissa_sign)
        printf("-");
    else
        printf("+");

    printf("0.");

    for (size_t i = 0; i < num.mantissa_len; i++)
        printf("%d", num.mantissa[i]);

    printf("e%d", num.order);

    printf("\n");
}
