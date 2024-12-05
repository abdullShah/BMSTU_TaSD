#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <linux/time.h>
#include "measure.h"
#include "queue_arr.h"
#include "queue_list.h"
#include "queue_dyn_arr.h"

extern int clock_gettime(clock_t __clock_id, struct timespec *__tp) __THROW;

#define MAX_ITER 500

void measureQueue(void)
{
    FILE *output_file = fopen("results.txt", "w");
    if (!output_file)
    {
        puts("Не удалось открыть файл для записи");
        return;
    }

    queue_arr_t q_arr;
    queue_dyn_arr_t q_dyn_arr;
    queue_list_t q_list;

    printf("Добавление\n");
    printf("| Размер  | Стат. массив             | Дин. массив              | Список                   |\n");
    printf("| очереди | Время, нс | Память, байт | Время, нс | Память, байт | Время, нс | Память, байт |\n");
    printf("|---------+-----------+--------------+-----------+--------------+-----------+--------------|\n");

    int sizes[] = { 10, 50, 100, 500, 1000 };

    for (int i = 0; i < (int) (sizeof(sizes) / sizeof(sizes[0])); i++)
    {
        int size = sizes[i];
        long total_time_arr = 0, total_time_dyn = 0, total_time_list = 0;
        size_t memory_arr = 0, memory_dyn = 0, memory_list = 0;

        for (int iter = 0; iter < MAX_ITER; iter++)
        {
            // Инициализация очередей
            queue_arr_init(&q_arr);
            queue_dyn_arr_init(&q_dyn_arr);
            queue_list_init(&q_list);

            struct timespec start, end;

            // Измерение для статического массива
            clock_gettime(CLOCK_MONOTONIC, &start);
            for (int j = 0; j < size; j++)
            {
                queue_arr_push(&q_arr, j);
            }
            clock_gettime(CLOCK_MONOTONIC, &end);
            total_time_arr += (int) ((end.tv_sec - start.tv_sec) * 1e9L + (end.tv_nsec - start.tv_nsec));
            memory_arr = queue_arr_memory(&q_arr);  // Память одинакова для всех итераций

            // Измерение для динамического массива
            clock_gettime(CLOCK_MONOTONIC, &start);
            for (int j = 0; j < size; j++)
            {
                queue_dyn_arr_push(&q_dyn_arr, j);
            }
            clock_gettime(CLOCK_MONOTONIC, &end);
            total_time_dyn += (int) ((end.tv_sec - start.tv_sec) * 1e9L + (end.tv_nsec - start.tv_nsec));
            memory_dyn = queue_dyn_arr_memory(&q_dyn_arr);

            // Измерение для списка
            clock_gettime(CLOCK_MONOTONIC, &start);
            for (int j = 0; j < size; j++)
            {
                queue_list_push(&q_list, j);
            }
            clock_gettime(CLOCK_MONOTONIC, &end);
            total_time_list += (int) ((end.tv_sec - start.tv_sec) * 1e9L + (end.tv_nsec - start.tv_nsec));
            memory_list = queue_list_memory(&q_list);

            // Очистка очередей
            queue_list_free(&q_list);
            queue_dyn_arr_free(&q_dyn_arr);
        }

        // Вывод среднего времени и памяти
        printf("| %7d | %9ld | %12zu | %9ld | %12zu | %9ld | %12zu |\n", size, total_time_arr / MAX_ITER, memory_arr,
               total_time_dyn / MAX_ITER, memory_dyn, total_time_list / MAX_ITER, memory_list);

        fprintf(output_file, "%d\t%ld\t%zu\t%ld\t%zu\t%ld\t%zu\n", size, total_time_arr / MAX_ITER, memory_arr,
                total_time_dyn / MAX_ITER, memory_dyn, total_time_list / MAX_ITER, memory_list);
    }

    printf("|---------+-----------+--------------+-----------+--------------+-----------+--------------|\n\n");

    printf("Удаление\n");
    printf("| Размер  | Стат. массив             | Дин. массив              | Список                   |\n");
    printf("| очереди | Время, нс | Память, байт | Время, нс | Память, байт | Время, нс | Память, байт |\n");
    printf("|---------+-----------+--------------+-----------+--------------+-----------+--------------|\n");

    for (int i = 0; i < (int) (sizeof(sizes) / sizeof(sizes[0])); i++)
    {
        int size = sizes[i];
        long total_time_arr = 0, total_time_dyn = 0, total_time_list = 0;
        size_t memory_arr = 0, memory_dyn = 0, memory_list = 0;

        for (int iter = 0; iter < MAX_ITER; iter++)
        {
            // Инициализация очередей
            queue_arr_init(&q_arr);
            queue_dyn_arr_init(&q_dyn_arr);
            queue_list_init(&q_list);

            struct timespec start, end;

            // Измерение для статического массива
            for (int j = 0; j < size; j++)
            {
                queue_arr_push(&q_arr, j);
            }
            clock_gettime(CLOCK_MONOTONIC, &start);
            for (int j = 0; j < size; j++)
            {
                queue_arr_pop(&q_arr);
            }
            clock_gettime(CLOCK_MONOTONIC, &end);
            total_time_arr += (int) ((end.tv_sec - start.tv_sec) * 1e9L + (end.tv_nsec - start.tv_nsec));
            memory_arr = queue_arr_memory(&q_arr);  // Память одинакова для всех итераций

            // Измерение для динамического массива
            for (int j = 0; j < size; j++)
            {
                queue_dyn_arr_push(&q_dyn_arr, j);
            }
            clock_gettime(CLOCK_MONOTONIC, &start);
            for (int j = 0; j < size; j++)
            {
                queue_dyn_arr_pop(&q_dyn_arr);
            }
            clock_gettime(CLOCK_MONOTONIC, &end);
            total_time_dyn += (int) ((end.tv_sec - start.tv_sec) * 1e9L + (end.tv_nsec - start.tv_nsec));
            memory_dyn = queue_dyn_arr_memory(&q_dyn_arr);

            // Измерение для списка
            for (int j = 0; j < size; j++)
            {
                queue_list_push(&q_list, j);
            }
            memory_list = queue_list_memory(&q_list);
            clock_gettime(CLOCK_MONOTONIC, &start);
            for (int j = 0; j < size; j++)
            {
                queue_list_pop(&q_list);
            }
            clock_gettime(CLOCK_MONOTONIC, &end);
            total_time_list += (int) ((end.tv_sec - start.tv_sec) * 1e9L + (end.tv_nsec - start.tv_nsec));

            // Очистка очередей
            queue_list_free(&q_list);
            queue_dyn_arr_free(&q_dyn_arr);
        }

        // Вывод среднего времени и памяти
        printf("| %7d | %9ld | %12zu | %9ld | %12zu | %9ld | %12zu |\n", size, total_time_arr / MAX_ITER, memory_arr,
               total_time_dyn / MAX_ITER, memory_dyn, total_time_list / MAX_ITER, memory_list);

        fprintf(output_file, "%d\t%ld\t%zu\t%ld\t%zu\t%ld\t%zu\n", size, total_time_arr / MAX_ITER, memory_arr,
                total_time_dyn / MAX_ITER, memory_dyn, total_time_list / MAX_ITER, memory_list);
    }

    printf("|---------+-----------+--------------+-----------+--------------+-----------+--------------|\n\n");

    fclose(output_file);
    queue_list_free(&q_list);
    queue_dyn_arr_free(&q_dyn_arr);
}
