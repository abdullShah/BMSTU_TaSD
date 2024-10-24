#include <stdio.h>
#include "ui.h"
#include "my_int.h"
#include "my_real.h"
#include "division.h"

#define SUCCESS 0
#define READ_INT_ERROR 1
#define OVERFLOW_INT_ERROR 2
#define EMPTY_INT_ERROR 3
#define INPUT_INT_ERROR 4
#define READ_REAL_ERROR 5
#define OVERFLOW_REAL_ERROR 6
#define EMPTY_REAL_ERROR 7
#define INPUT_REAL_ERROR 8
#define OVERFLOW_REAL_ORDER_ERROR 9
#define DIVISION_BY_ZERO_ERROR 10
#define OVERFLOW_ORDER_ERROR 11

int handle_read_real_errors(int error_code);

int handle_read_int_errors(int error_code);

int handle_division_errors(int error_code);

void print_pattern(void);

int main(void)
{
    info();

    normal_t num1;
    normal_t num2;
    normal_t res;
    int rc;

    printf("\nВведите вещественное число:\n");
    ruler();

    rc = read_real(&num1);

    if (rc != PROCESS_DONE)
        return handle_read_real_errors(rc);

    print_normal(num1);

    printf("\nВведите целое число:\n");
    ruler();
    rc = read_int(&num2);

    if (rc != PROCESS_DONE)
        return handle_read_int_errors(rc);

    print_normal(num2);

    rc = division(&res, num1, num2);

    if (rc != PROCESS_DONE)
        return handle_division_errors(rc);

    printf("\nРезультат деления вещественого числа на целое:\n");
    //print_pattern();
    print_normal(res);

    return SUCCESS;
}

int handle_read_real_errors(int error_code)
{
    switch (error_code)
    {
        case READ_REAL_FAILED:  printf("Ошибка ввода вещестенного числа\n");
                                return READ_REAL_ERROR;
        case OVERFLOW_REAL: printf("Ошибка переполнения вещестенного числа\n");
                            return OVERFLOW_REAL_ERROR;
        case EMPTY_REAL_LINE:   printf("Введена пустая строка вместо вещестенного числа\n");
                                return EMPTY_REAL_ERROR;
        case NOT_REAL:  printf("Введено не вещестенное число\n");
                        return INPUT_REAL_ERROR;
        case OVERFLOW_REAL_ORDER:   printf("При привидении к нормальному виду произошло достижение машинного нуля или машинной бесконечности (переполнение порядка)\n");
                                    return OVERFLOW_REAL_ORDER_ERROR;
    }

    printf("Неизвестная ошибка считывания вещестенного числа\n");
    return error_code;
}

int handle_read_int_errors(int error_code)
{
    switch (error_code)
    {
        case READ_INT_FAILED:   printf("Ошибка ввода целого числа\n");
                                return READ_INT_ERROR;
        case OVERFLOW_INT:  printf("Ошибка переполнения целого числа\n");
                            return OVERFLOW_INT_ERROR;
        case EMPTY_INT_LINE:    printf("Введена пустая строка вместо целого числа\n");
                                return EMPTY_INT_ERROR;
        case NOT_INT:   printf("Введено не целое число\n");
                        return INPUT_INT_ERROR;
    }

    printf("Неизвестная ошибка считывания целого числа\n");
    return error_code;
}

int handle_division_errors(int error_code)
{
    switch (error_code)
    {
        case DIVISION_BY_ZERO:  printf("Деление на нуль\n");
                                return DIVISION_BY_ZERO_ERROR;
        case OVERFLOW_ORDER:  printf("Достижение машинного нуля или машинной бесконечности (переполнение порядка)\n");
                                return OVERFLOW_ORDER_ERROR;
    }

    printf("Неизвестная ошибка считывания вещестенного числа\n");
    return error_code;
}

void print_pattern(void)
{
    printf("%3s", "");
    for (int i = 0; i < 35; i++)
    {
        if (i % 5 == 4)
            printf("|");
        else
            printf("*");
    }
    printf("\n");
}
