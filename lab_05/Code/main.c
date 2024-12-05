#include <stdio.h>
#include <stdlib.h>
#include "ui.h"
#include "model.h"
#include "model_array.h"
#include "model_list.h"
#include "timemem.h"
#include "queue_arr.h"
#include "queue_list.h"
#include "queue_dyn_arr.h"
#include "measure.h"

enum errors
{
    SUCCESS,
    INPUT_COMMAND_ERR,
    INPUT_FOR_INSERT_ERR,
};

int main(void)
{
    queue_arr_t q_arr;
    queue_arr_init(&q_arr);

    queue_dyn_arr_t q_dyn_arr;
    queue_dyn_arr_init(&q_dyn_arr);

    queue_list_t q_list;
    queue_list_init(&q_list);

    int n = 1000;
    int interval = 100;
    int flag = 0;

    //Начальные значения
    times_t t1 = { 1, 5 };
    times_t t2 = { 0, 3 };
    times_t t3 = { 0, 4 };
    times_t t4 = { 0, 1 };

    int choice = 1;
    while (choice)
    {
        menu();
        printf("Выберите команду: ");

        if (scanf("%d", &choice) != 1)
        {
            printf("Неудачный ввод команды!\n");
            return INPUT_COMMAND_ERR;
        }

        int rc;
        int numberToAdd;
        switch (choice)
        {
            case 0:
                queue_list_free(&q_list);
                queue_dyn_arr_free(&q_dyn_arr);
                break;
            case 1:
                printf("\nВведите значение для добавления: ");
                if (scanf("%d", &numberToAdd) != 1)
                {
                    printf("Ошибка ввода числа для добавления!\n\n");
                    return INPUT_FOR_INSERT_ERR;
                }
                if (queue_arr_is_full(&q_arr))
                {
                    printf("Очередь заполнена!\n\n");
                    break;
                }
                queue_arr_push(&q_arr, numberToAdd);
                printf("Элемент %d добавлен в очередь.\n\n", numberToAdd);
                break;
            case 2:
                if (queue_arr_is_empty(&q_arr))
                {
                    printf("Очередь пуста!\n\n");
                    break;
                }
                printf("Элемент %d удалён из очереди.\n\n", queue_arr_pop(&q_arr));
                break;
            case 3:
                queue_arr_print(&q_arr);
                break;
            case 4:
                printf("\nВведите значение для добавления: ");
                if (scanf("%d", &numberToAdd) != 1)
                {
                    printf("Ошибка ввода числа для добавления!\n\n");
                    return INPUT_FOR_INSERT_ERR;
                }
                queue_dyn_arr_push(&q_dyn_arr, numberToAdd);
                printf("Элемент %d добавлен в очередь.\n\n", numberToAdd);
                break;
            case 5:
                if (queue_dyn_arr_is_empty(&q_dyn_arr))
                {
                    printf("Очередь пуста!\n\n");
                    break;
                }
                printf("Элемент %d удалён из очереди.\n\n", queue_dyn_arr_pop(&q_dyn_arr));
                break;
            case 6:
                queue_dyn_arr_print(&q_dyn_arr);
                break;
            case 7:
                printf("\nВведите значение для добавления: ");
                if (scanf("%d", &numberToAdd) != 1)
                {
                    printf("Ошибка ввода числа для добавления!\n\n");
                    return INPUT_FOR_INSERT_ERR;
                }
                queue_list_push(&q_list, numberToAdd);
                printf("Элемент %d добавлен в очередь.\n\n", numberToAdd);
                break;
            case 8:
                if (queue_list_is_empty(&q_list))
                {
                    printf("Очередь пуста!\n\n");
                    break;
                }
                printf("Элемент %d удалён из очереди.\n\n", queue_list_pop(&q_list));
                break;
            case 9:
                queue_list_print(&q_list);
                break;
            case 10:
                // printf("Симуляция для очереди в виде массива.\n");
                sd_array(n, interval, t1, t2, t3, t4);  //, flag);
                break;
            case 11:
                // printf("Симуляция для очереди в виде списка.\n");
                printf("Выводить информация о памяти? \n1 - да, 0 - нет\nВыбор: ");
                rc = scanf("%d", &flag);
                if (rc != 1 || !(flag == 1 || flag == 0))
                {
                    clean_my();
                    printf("Некорректный выбор!\n\n");
                    break;
                }
                sd_list(n, interval, t1, t2, t3, t4, flag);
                break;
            case 12:
                printf("Изменение времени обработки. (введите цифру для изменения времени)\n");
                double l, r;
                int c;

                printf("\nВремя прибытия\n");
                printf("1 - T1: min = %lf; max = %lf\n", t1.min, t1.max);
                printf("2 - T2: min = %lf; max = %lf\n", t2.min, t2.max);
                printf("\nВремя обработки\n");
                printf("3 - T3: min = %lf; max = %lf\n", t3.min, t3.max);
                printf("4 - T4: min = %lf; max = %lf\n\n", t4.min, t4.max);
                printf("Какой интервал изменить?\n");
                if (check_number(&c, 1, 4) != OK)
                {
                    printf("Введён некорректный номер!\n\n");
                    break;
                }
                else
                {
                    printf("Введите левую и правую границы: ");
                    int rc = scanf("%lf", &l);
                    if (rc != 1 || l < 0)
                    {
                        clean_my();
                        printf("Такого интервала нет!\n\n");
                        break;
                    }
                    rc = scanf("%lf", &r);
                    if (rc != 1 || r < 0)
                    {
                        clean_my();
                        printf("Такого интервала нет!\n\n");
                        break;
                    }
                    if (l >= r)
                    {
                        printf("Левый интервал больше или равен правого!\n\n");
                        break;
                    }
                    switch (c)
                    {
                        case 1:
                            t1.min = l;
                            t1.max = r;
                            break;
                        case 2:
                            t2.min = l;
                            t2.max = r;
                            break;
                        case 3:
                            t3.min = l;
                            t3.max = r;
                            break;
                        case 4:
                            t4.min = l;
                            t4.max = r;
                            break;
                    }
                    printf("\nПосле изменений\n");
                    printf("\nВремя прибытия\n");
                    printf("1 - T1: min = %lf; max = %lf\n", t1.min, t1.max);
                    printf("2 - T2: min = %lf; max = %lf\n", t2.min, t2.max);
                    printf("\nВремя обработки\n");
                    printf("3 - T3: min = %lf; max = %lf\n", t3.min, t3.max);
                    printf("4 - T4: min = %lf; max = %lf\n\n", t4.min, t4.max);
                }
                break;
            case 13:
                // memory_io();
                measureQueue();
                break;
            default:
                puts("Нет такой команды! Повторите попытку\n");
                break;
        }
    }

    return SUCCESS;
}
