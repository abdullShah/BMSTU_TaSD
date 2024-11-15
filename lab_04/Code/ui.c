#include <stdio.h>
#include "ui.h"

void menu(void)
{
    printf("Меню управления:\n");
    printf("0 - Выйти из программы\n");
    printf("1 - Добавить элемент в стек (с использованием массива)\n");
    printf("2 - Удалить элемент из стека (с использованием массива)\n");
    printf("3 - Вывести стек (с использованием массива)\n");
    printf("4 - Добавить элемент в стек (с использованием списка)\n");
    printf("5 - Удалить элемент из стека (с использованием списка)\n");
    printf("6 - Вывести стек (с использованием списка)\n");
    printf("7 - Ввод элементов в стек (с использованием массива)\n");
    printf(
        "8 - Распечатать убывающие серии последовательности целых чисел\n    в обратном порядке (с использованием "
        "массива)\n");
    printf("9 - Ввод элементов в стек (с использованием списка)\n");
    printf(
        "10 - Распечатать убывающие серии последовательности целых чисел\n    в обратном порядке (с использованием "
        "списка)\n");
    printf("11 - Сравнить производительность стеков с использованием массива и списка\n");
    printf("12 - Вывести массив свободных областей памяти\n");
    printf("\n");
}
