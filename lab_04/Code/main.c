#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ui.h"
#include "arr_stack.h"
#include "list_stack.h"
#include "measurement.h"

enum errors
{
    SUCCESS,
    INPUT_COMMAND_ERR,
    INPUT_VALUE_ERR,
    INIT_LIST_ALLOC_ERR,
    LEN_INPUT_ERR,
    EL_INPUT_ERR
};

int main(void)
{
    arr_stack astack;
    arr_stack_init(&astack);

    list_stack *lstack = init_list_stack();
    if (!lstack)
    {
        printf("Ошибка аллокации начального списка\n");
        return INIT_LIST_ALLOC_ERR;
    }

    int choice = 1;
    while (choice)
    {
        menu();
        printf("Выберите команду: ");

        if (scanf("%d", &choice) != 1)
        {
            printf("Неудачный ввод команды!\n");
            list_stack_free(lstack);
            return INPUT_COMMAND_ERR;
        }

        int value;
        int el;
        switch (choice)
        {
            case 0:
                list_stack_free(lstack);
                break;
            case 1:
                if (arr_stack_is_full(astack))
                {
                    printf("\nСтек с использованием массива переполнен!\n\n");
                    break;
                }

                printf("\nВведите целое значение для добавления: ");

                if (scanf("%d", &value) != 1)
                {
                    printf("\nОшибка ввода значения!\n");
                    list_stack_free(lstack);
                    return INPUT_VALUE_ERR;
                }

                arr_stack_push(&astack, value);

                printf("\nУспешно добавленное значение: %d!\n\n", value);

                break;
            case 2:
                if (arr_stack_is_empty(astack))
                {
                    printf("\nСтек с использованием массива пуст!\n\n");
                    break;
                }

                printf("\nУспешно удаленное значение: %d!\n\n", arr_stack_pop(&astack));

                break;
            case 3:
                if (arr_stack_is_empty(astack))
                {
                    printf("\nСтек с использованием массива пуст!\n\n");
                    break;
                }

                arr_stack_print(&astack);

                break;
            case 4:
                if (list_stack_is_full(lstack))
                {
                    printf("\nСтек с использованием списка переполнен!\n\n");
                    break;
                }

                printf("\nВведите целое значение для добавления: ");

                if (scanf("%d", &value) != 1)
                {
                    printf("\nОшибка ввода значения!\n");
                    list_stack_free(lstack);
                    return INPUT_VALUE_ERR;
                }

                list_stack_push(lstack, value);

                printf("\nУспешно добавленное значение: %d!\n\n", value);
                break;
            case 5:
                if (list_stack_is_empty(lstack))
                {
                    printf("\nСтек с использованием списка пуст!\n\n");
                    break;
                }

                printf("\nУспешно удаленное значение: %d!\n\n", list_stack_pop(lstack, 0));

                break;
            case 6:
                if (list_stack_is_empty(lstack))
                {
                    printf("\nСтек с использованием списка пуст!\n\n");
                    break;
                }

                list_stack_print(lstack);

                break;
            case 7:
                printf("\nВведите значение длины: ");

                if (scanf("%d", &value) != 1 || !(1 <= value && value <= MAX_SIZE))
                {
                    printf("\nОшибка ввода длины!\n");
                    list_stack_free(lstack);
                    return LEN_INPUT_ERR;
                }

                printf("\nВводите значения:\n");
                for (int i = 0; i < value; i++)
                {
                    if (scanf("%d", &el) != 1)
                    {
                        printf("\nОшибка ввода значения!\n");
                        list_stack_free(lstack);
                        return EL_INPUT_ERR;
                    }

                    arr_stack_push(&astack, el);
                }

                printf("\nУспешный ввод элементов в стек (с использованием массива)\n\n");

                break;
            case 8:
                if (arr_stack_is_empty(astack))
                {
                    printf("\nСтек с использованием массива пуст!\n\n");
                    break;
                }

                arr_stack_descending_series(&astack, 1);

                break;
            case 9:
                printf("\nВведите значение длины: ");

                if (scanf("%d", &value) != 1 || !(1 <= value && value <= MAX_SIZE))
                {
                    printf("\nОшибка ввода длины!\n");
                    list_stack_free(lstack);
                    return LEN_INPUT_ERR;
                }

                printf("\nВводите значения:\n");
                for (int i = 0; i < value; i++)
                {
                    if (scanf("%d", &el) != 1)
                    {
                        printf("\nОшибка ввода значения!\n");
                        list_stack_free(lstack);
                        return EL_INPUT_ERR;
                    }

                    list_stack_push(lstack, el);
                }

                printf("\nУспешный ввод элементов в стек (с использованием списка)\n\n");

                break;
            case 10:
                if (list_stack_is_empty(lstack))
                {
                    printf("\nСтек с использованием списка пуст!\n\n");
                    break;
                }

                list_stack_descending_series(lstack, 1);

                break;
            case 11:
                compare_stacks();
                break;
            case 12:
                print_free_list();
                break;
            default:
                puts("Нет такой команды! Повторите попытку\n");
                break;
        }
    }

    return SUCCESS;
}
