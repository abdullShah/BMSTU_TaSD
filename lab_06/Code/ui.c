#include <stdio.h>
#include "ui.h"

void menu(void)
{
    printf("Меню управления:\n");
    printf("0  - Выйти из программы\n");
    printf("1  - Считать двоичное дерево из чисел файла\n");
    printf("2  - Вывести двоичное дерево в виде графа\n");
    printf("3  - Проверить наличие числа в дереве\n");
    printf("4  - Добавить число в дерево\n");
    printf("5  - Удалить число из дерева\n");
    printf("6  - Вывести дерево в обратном (постордерном) обходе\n");
    printf("7  - Вывести дерево в симметричном (центрированном, инордерном) обходе\n");
    printf("8  - Вывести дерево в прямом (преордерном) обходе\n");
    printf("9  - Определить количество узлов дерева на каждом уровне\n");
    printf("10 - Добавить число в файл\n");
    printf("11 - Сравнить время добавления чисел в дерево и файл\n");
    printf("\n");
}
