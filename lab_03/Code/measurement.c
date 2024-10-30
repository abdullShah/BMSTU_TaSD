#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <linux/time.h>
#include "matrix.h"
#include "measurement.h"

#define RAND_LIMIT 100
#define CNT_ITER 25

extern int clock_gettime(clock_t __clock_id, struct timespec *__tp) __THROW;

static void fill_standard_matrix(int **matrix, int n, int m, int filling)
{
    srand(time(NULL));

    filling = 100 - filling;

    size_t zeros_needed = (n * m * filling) / 100;

    for (size_t i = 0; i < (size_t) n; i++)
        for (size_t j = 0; j < (size_t) m; j++)
            matrix[i][j] = rand() % (RAND_LIMIT + 1);

    while (zeros_needed > 0)
    {
        size_t i = rand() % n;
        size_t j = rand() % m;
        if (matrix[i][j] != 0)
        {
            matrix[i][j] = 0;
            zeros_needed--;
        }
    }
}

int compare_matrix_operations(void)
{
    int n, m;
    printf("\nВведите количество строк матриц: ");
    if (scanf("%d", &n) != 1 || n <= 0)
        return INPUT_ROWS_FAILED;

    printf("Введите количество столбцов матриц: ");
    if (scanf("%d", &m) != 1 || m <= 0)
        return INPUT_COLS_FAILED;

    printf("\n");
    printf("                     |       Время, нс       |  Объем памяти, байт   |\n");
    printf("Процент заполнения, %%|разреженная|стандартная|разреженная|стандартная|\n");
    printf("---------------------+-----------+-----------+-----------+-----------|\n");

    for (int filling = 10; filling <= 100; filling += 20)
    {
        if (filling == 90)
            filling = 100;

        size_t sparse_memory = 0;
        size_t standard_memory = sizeof(int) * n * m;
        int sparse_time_total = 0;
        int standard_time_total = 0;

        for (int iter = 0; iter < CNT_ITER; iter++)
        {
            standard_matrix_t st_matr_1 = { 0, 0, NULL };
            sparse_matrix_t sp_matr_1 = { 0, 0, 0, NULL, NULL, 0, NULL };

            int **values_1 = alloc_matrix(n, m);
            if (!values_1)
                return ALLOC_FAILED;

            fill_standard_matrix(values_1, n, m, filling);
            st_matr_1.rows = n;
            st_matr_1.cols = m;
            st_matr_1.values = values_1;

            if (standard_to_sparse_matrix(st_matr_1, &sp_matr_1) != PROCESS_DONE)
                return ALLOC_FAILED;

            standard_matrix_t st_matr_2 = { 0, 0, NULL };
            sparse_matrix_t sp_matr_2 = { 0, 0, 0, NULL, NULL, 0, NULL };

            int **values_2 = alloc_matrix(n, m);
            if (!values_2)
                return ALLOC_FAILED;

            fill_standard_matrix(values_2, n, m, filling);
            st_matr_2.rows = n;
            st_matr_2.cols = m;
            st_matr_2.values = values_2;

            if (standard_to_sparse_matrix(st_matr_2, &sp_matr_2) != PROCESS_DONE)
                return ALLOC_FAILED;

            struct timespec start, end;

            // Операция сложения стандартных матриц
            standard_matrix_t st_matr_3 = { 0, 0, NULL };
            clock_gettime(CLOCK_REALTIME, &start);
            sum_standard_matrixes(&st_matr_3, st_matr_1, st_matr_2);
            clock_gettime(CLOCK_REALTIME, &end);
            int standard_time = (int) ((end.tv_sec - start.tv_sec) * 1e9 + (end.tv_nsec - start.tv_nsec));
            standard_time_total += standard_time;

            // Операция сложения разреженных матриц
            sparse_matrix_t sp_matr_3 = { 0, 0, 0, NULL, NULL, 0, NULL };
            clock_gettime(CLOCK_REALTIME, &start);
            sum_sparse_matrixes(&sp_matr_3, sp_matr_1, sp_matr_2);
            clock_gettime(CLOCK_REALTIME, &end);
            int sparse_time = (int) ((end.tv_sec - start.tv_sec) * 1e9 + (end.tv_nsec - start.tv_nsec));
            sparse_time_total += sparse_time;

            sparse_memory = sp_matr_1.A_len * sizeof(int) * 2;
            sparse_memory += sp_matr_1.JA_len * sizeof(int);

            /*printf("==========\n");
            print_matrix(st_matr_1);
            printf("==========\n");
            print_matrix(st_matr_2);
            printf("==========\n");*/

            free_matrixes(st_matr_1, sp_matr_1);
            free_matrixes(st_matr_2, sp_matr_2);
            free_standard_matrix(st_matr_3);
            free_sparse_matrix(sp_matr_3);
        }

        // Выводим усредненное время
        int avg_standard_time = standard_time_total / CNT_ITER;
        int avg_sparse_time = sparse_time_total / CNT_ITER;

        printf("%-21d|%-11d|%-11d|%-11zu|%-11zu|\n", filling, avg_sparse_time, avg_standard_time, sparse_memory,
               standard_memory);
    }

    printf("\n");
    return PROCESS_DONE;
}
