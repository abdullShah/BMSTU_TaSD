#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <linux/time.h>
#include <x86gprintrin.h>
#include "tree.h"
#include "measure.h"

unsigned long long __rdtsc(void);

extern int clock_gettime(clock_t __clock_id, struct timespec *__tp) __THROW;

static void copyNums(char *filename_out, char *filename_in)
{
    FILE *input_file = fopen(filename_in, "rb");
    FILE *output_file = fopen(filename_out, "wb");

    if (input_file == NULL || output_file == NULL)
    {
        printf("Ошибка при открытии файла при копировании\n\n");
        if (input_file != NULL)
            fclose(input_file);
        if (output_file != NULL)
            fclose(output_file);
        return;
    }

    char buffer[1024];
    int bytes_read;

    while ((bytes_read = fread(buffer, 1, sizeof(buffer), input_file)) > 0)
        fwrite(buffer, 1, bytes_read, output_file);

    fclose(input_file);
    fclose(output_file);
}

void measureAverageInsertTime(void)
{
    const int testValue = 9999;
    const int levels[] = { 2, 3, 5, 7, 9, 10 };
    const int levelsCount = sizeof(levels) / sizeof(levels[0]);
    const int MAX_ITER = 100;

    printf("+---------------------+---------------------+---------------------+\n");
    printf("| Количество уровней  | Время в дерево (нс) |  Время в файл (нс)  |\n");
    printf("+---------------------+---------------------+---------------------+\n");

    struct timespec start, end;
    for (int i = 0; i < levelsCount; i++)
    {
        int level = levels[i];
        double totalTimeTree = 0.0, totalTimeFile = 0.0;

        char filename_in[64];
        snprintf(filename_in, sizeof(filename_in), "./trees/level_%d.txt", level);

        Node *root = NULL;
        int rc = readTreeByFile(filename_in, &root);
        if (rc != PROCESS_DONE)
        {
            printf("Ошибка: не удалось создать дерево с %d уровнями из файла [%d]!\n\n", level, rc);
            return;
        }

        char filename_out[64];
        snprintf(filename_out, sizeof(filename_out), "./out/level_%d.txt", level);
        for (int iter = 0; iter < MAX_ITER; iter++)
        {
            clock_gettime(CLOCK_MONOTONIC, &start);
            insert(root, testValue);
            clock_gettime(CLOCK_MONOTONIC, &end);
            totalTimeTree += (end.tv_sec * 1e9 + end.tv_nsec) - (start.tv_sec * 1e9 + start.tv_nsec);

            copyNums(filename_out, filename_in);
            clock_gettime(CLOCK_MONOTONIC, &start);
            appendNumberToFile(filename_out, testValue);
            clock_gettime(CLOCK_MONOTONIC, &end);
            totalTimeFile += (end.tv_sec * 1e9 + end.tv_nsec) - (start.tv_sec * 1e9 + start.tv_nsec);
        }

        double avgTimeTree = totalTimeTree / MAX_ITER;
        double avgTimeFile = totalTimeFile / MAX_ITER;

        printf("| %-19d | %-19.2f | %-19.2f |\n", level, avgTimeTree, avgTimeFile);

        freeTree(&root);
    }

    printf("+---------------------+---------------------+---------------------+\n");
}
