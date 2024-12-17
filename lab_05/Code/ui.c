#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include "ui.h"
#include "model.h"

void menu(void)
{
    printf("Меню управления:\n");
    printf("0 - Выйти из программы\n");
    printf("------------------------\n");
    printf("Очередь как статический массив\n");
    printf("1  - Добавить элемент\n");
    printf("2  - Удалить элемент\n");
    printf("3  - Вывод текущего состояния очереди\n");
    printf("------------------------\n");
    printf("Очередь как динамический массив\n");
    printf("4  - Добавить элемент\n");
    printf("5  - Удалить элемент\n");
    printf("6  - Вывод текущего состояния очереди\n");
    printf("------------------------\n");
    printf("Очередь как список\n");
    printf("7  - Добавить элемент\n");
    printf("8  - Удалить элемент\n");
    printf("9  - Вывод текущего состояния очереди\n");
    printf("------------------------\n");
    printf("10 - Моделирование и характеристика для очереди в виде массива\n");
    printf("11 - Моделирование и характеристика для очереди в виде списка\n");
    printf("12 - Изменить время обработки заявки\n");
    printf("13 - Вывод сравнения эффективности при выполнении операций\n");
    printf("\n");
}

void clean_my(void)
{
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF)
    {
    }
}

int check_float(double *const number)
{
    char num[4];
    char *n;
    fflush(stdin);
    n = fgets(num, 4, stdin);
    if (n == NULL || num[0] == '\n')
        return DONT_INT_NUM_CHOICE;
    num[strlen(num) - 1] = '\0';
    *number = (double) atof(num);

    return OK;
}

int check_number(int *number, const int l, const int r)
{
    printf("Выбор: ");
    char num[126];

    // Читаем ввод как строку
    if (scanf("%125s", num) != 1)
        return INV_NUM_CHOICE;

    // Проверяем, является ли строка числом
    for (size_t i = 0; i < strlen(num); i++)
        if (!isdigit(num[i]))
            return INV_NUM_CHOICE;

    *number = atoi(num);
    if (*number < l || *number > r)
        return INV_NUM_CHOICE;

    return OK;
}
