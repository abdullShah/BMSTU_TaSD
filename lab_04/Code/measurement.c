#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <linux/time.h>
#include "arr_stack.h"
#include "list_stack.h"
#include "measurement.h"
#include <x86gprintrin.h>

unsigned long long __rdtsc(void);

#define MAX_ITER 1

extern int clock_gettime(clock_t __clock_id, struct timespec *__tp) __THROW;

static void arr_stack_rand_fill(arr_stack *stack, int size)
{
    srand((unsigned int) time(NULL));

    for (int i = 0; i < size; i++)
        arr_stack_push(stack, rand() % 100);
}

static int list_stack_rand_fill(list_stack *stack, int size)
{
    srand((unsigned int) time(NULL));

    for (int i = 0; i < size; i++)
        if (list_stack_push(stack, rand() % 100) != 0)
            return 1;

    return 0;
}

int compare_stacks(void)
{
    int sizes[] = { 10, 50, 100, 500, 1000 };

    printf(" Размер |      Время, нс      |    Память, байты    |\n");
    printf(" стека  |  массив  |  список  |  массив  |  список  |\n");
    printf("--------+----------+----------+----------+----------|\n");

    for (size_t i = 0; i < sizeof(sizes) / sizeof(int); i++)
    {
        int size = sizes[i];

        int total_time_arr = 0;
        int total_time_list = 0;
        /*unsigned long long work_time_1 = 0;
        unsigned long long work_time_2 = 0;*/
        size_t memory_usage_arr = 0;
        size_t memory_usage_list = 0;

        for (int iter = 0; iter < MAX_ITER; iter++)
        {
            arr_stack astack;
            arr_stack_init(&astack);
            arr_stack_rand_fill(&astack, size);

            struct timespec start, end;
            clock_gettime(CLOCK_MONOTONIC, &start);
            /*arr_stack_descending_series(&astack, 0);*/
            for (int q = 0; q < size; q++)
            {
                arr_stack_push(&astack, q);
            }
            for (int q = 0; q < size; q++)
            {
                arr_stack_pop(&astack);
            }
            clock_gettime(CLOCK_MONOTONIC, &end);
            int arr_time = (int) ((end.tv_sec - start.tv_sec) * 1e9 + (end.tv_nsec - start.tv_nsec));

            total_time_arr += arr_time;

            /*unsigned long long t_start_1 = __rdtsc();
            arr_stack_descending_series(&astack, 0);
            unsigned long long t_end_1 = __rdtsc();
            work_time_1 += t_end_1 - t_start_1;*/
        }

        for (int iter = 0; iter < MAX_ITER; iter++)
        {
            list_stack *lstack = init_list_stack();
            if (!lstack)
                return 1;

            if (list_stack_rand_fill(lstack, size) != 0)
            {
                list_stack_free(lstack);
                return 2;
            }
            /*printf("[] = %d\n", size);
            list_stack_print(lstack);*/

            struct timespec start, end;
            clock_gettime(CLOCK_MONOTONIC, &start);
            /*list_stack_descending_series(lstack, 0);*/
            for (int q = 0; q < size; q++)
            {
                list_stack_push(lstack, q);
            }
            for (int q = 0; q < size; q++)
            {
                list_stack_pop(lstack, 1);
            }
            clock_gettime(CLOCK_MONOTONIC, &end);
            int list_time = (int) ((end.tv_sec - start.tv_sec) * 1e9 + (end.tv_nsec - start.tv_nsec));

            total_time_list += list_time;

            /*unsigned long long t_start_1 = __rdtsc();
            list_stack_descending_series(lstack, 0);
            unsigned long long t_end_1 = __rdtsc();
            work_time_2 += t_end_1 - t_start_1;*/

            list_stack_free(lstack);
        }

        int avg_time_arr = total_time_arr / MAX_ITER;
        int avg_time_list = total_time_list / MAX_ITER;
        /*work_time_1 /= MAX_ITER;
        work_time_2 /= MAX_ITER;*/
        memory_usage_arr = (size * sizeof(int) + sizeof(int));
        memory_usage_list = (sizeof(int) + sizeof(Node *) + size * (sizeof(int) + sizeof(Node *)));

        printf("%-8d|%-10d|%-10d|%-10zu|%-10zu|\n", size, avg_time_arr, avg_time_list, memory_usage_arr,
               memory_usage_list);
        /*printf("%-8d|%-10llu|%-10llu|%-10zu|%-10zu|\n", size, work_time_1, work_time_2, memory_usage_arr,
               memory_usage_list);*/
    }

    printf("\n");
    return 0;
}
