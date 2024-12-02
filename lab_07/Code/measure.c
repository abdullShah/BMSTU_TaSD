#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <linux/time.h>
#include <x86gprintrin.h>
#include "tree.h"
#include "avl_tree.h"
#include "hash_table.h"
#include "measure.h"

#define MAX_NUMBERS 2000

unsigned long long __rdtsc(void);

extern int clock_gettime(clock_t __clock_id, struct timespec *__tp) __THROW;

static void cleanup(Node **rootTree, AVLNode **rootAVLTree, HashTable **hashTable)
{
    freeTree(rootTree);
    freeAVLTree(rootAVLTree);
    freeHashTable(hashTable);
}

static int loadNumbersFromFile(const char *filename, int *numbers, int maxSize)
{
    FILE *file = fopen(filename, "r");
    if (!file)
        return 1;

    int count = 0;
    while (count < maxSize && fscanf(file, "%d", &numbers[count]) == 1)
        count++;

    fclose(file);
    return count;
}

static int getRandomNumberFromArray(const int *numbers, int count)
{
    return numbers[rand() % count];
}

void measureAverageFindTime(int attempt)
{
    const int levels[] = { 2, 3, 5, 6, 7, 9, 10 };
    const int levelsCount = sizeof(levels) / sizeof(levels[0]);
    const int MAX_ITER = 500;

    struct timespec start, end;
    srand(time(NULL));

    char table_filename[50];
    sprintf(table_filename, "./measure/results_%d.txt", attempt);

    FILE *table_file = fopen(table_filename, "w");
    if (!table_file)
    {
        printf("Ошибка: не удалось открыть файл для записи для таблицы [создать папку ./measure]!\n\n");
        return;
    }

    for (int i = 0; i < levelsCount; i++)
    {
        int level = levels[i];

        printf("\n\nКоличество уровней: %d\n", level);
        printf("+---------------------+---------------------+---------------------+---------------------+\n");
        printf("|                     | Время поиска (нс)   | Кол. сравнений      | Память (байт)       |\n");
        printf("+---------------------+---------------------+---------------------+---------------------+\n");

        fprintf(table_file, "Количество уровней: %d\n", level);
        fprintf(table_file, "\tВремя поиска (нс)\tКол. сравнений\tПамять (байт)\n");

        double totalTimeTree = 0.0, totalTimeAVLTree = 0.0, totalTimeHashTable = 0.0;
        int cmpCntTree = 0, cmpCntAVLTree = 0, cmpCntHashTable = 0;

        char filename_in[64];
        snprintf(filename_in, sizeof(filename_in), "./trees/level_%d.txt", level);

        Node *rootTree = NULL;
        AVLNode *rootAVLTree = NULL;
        HashTable *hashTable = createHashTable(1);

        if (readTreeByFile(filename_in, &rootTree) || readAVLTreeByFile(filename_in, &rootAVLTree) ||
            readHashTableByFile(filename_in, hashTable))
        {
            printf("Ошибка: не удалось создать структуры с %d уровнями из файла!\n\n", level);
            cleanup(&rootTree, &rootAVLTree, &hashTable);
            return;
        }

        int numbers[MAX_NUMBERS];
        int numberCount = loadNumbersFromFile(filename_in, numbers, MAX_NUMBERS);
        if (numberCount == -1)
        {
            printf("Ошибка: не удалось открыть файл для считывания чисел для случайного выбора!\n\n");
            cleanup(&rootTree, &rootAVLTree, &hashTable);
            return;
        }

        for (int iter = 0; iter < MAX_ITER; iter++)
        {
            int numberToSearch = getRandomNumberFromArray(numbers, numberCount);

            clock_gettime(CLOCK_MONOTONIC, &start);
            findInTree(rootTree, numberToSearch, &cmpCntTree);
            clock_gettime(CLOCK_MONOTONIC, &end);
            totalTimeTree += (end.tv_sec * 1e9 + end.tv_nsec) - (start.tv_sec * 1e9 + start.tv_nsec);

            clock_gettime(CLOCK_MONOTONIC, &start);
            findInAVLTree(rootAVLTree, numberToSearch, &cmpCntAVLTree);
            clock_gettime(CLOCK_MONOTONIC, &end);
            totalTimeAVLTree += (end.tv_sec * 1e9 + end.tv_nsec) - (start.tv_sec * 1e9 + start.tv_nsec);

            clock_gettime(CLOCK_MONOTONIC, &start);
            findInHashTable(hashTable, numberToSearch, &cmpCntHashTable);
            clock_gettime(CLOCK_MONOTONIC, &end);
            totalTimeHashTable += (end.tv_sec * 1e9 + end.tv_nsec) - (start.tv_sec * 1e9 + start.tv_nsec);
        }

        double avgTimeTree = totalTimeTree / MAX_ITER;
        double avgTimeAVLTree = totalTimeAVLTree / MAX_ITER;
        double avgTimeHashTable = totalTimeHashTable / MAX_ITER;

        int avgCmpCntTree = (int) floor(cmpCntTree / MAX_ITER);
        int avgCmpCntAVLTree = (int) floor(cmpCntAVLTree / MAX_ITER);
        int avgCmpCntHashTable = (int) floor(cmpCntHashTable / MAX_ITER);

        size_t memoryTree = getTreeMemory(rootTree);
        size_t memoryAVLTree = getAVLTreeMemory(rootAVLTree);
        size_t memoryHashTable = getHashTableMemory(hashTable);

        printf("| Дерево              | %-19.2f | %-19d | %-19zu |\n", avgTimeTree, avgCmpCntTree, memoryTree);
        printf("| Сбаланс. дерево     | %-19.2f | %-19d | %-19zu |\n", avgTimeAVLTree, avgCmpCntAVLTree, memoryAVLTree);
        printf("| Хеш-таблица         | %-19.2f | %-19d | %-19zu |\n", avgTimeHashTable, avgCmpCntHashTable,
               memoryHashTable);
        printf("+---------------------+---------------------+---------------------+---------------------+\n");

        fprintf(table_file, "Дерево\t%.2f\t%d\t%zu\n", avgTimeTree, avgCmpCntTree, memoryTree);
        fprintf(table_file, "Сбаланс. дерево\t%.2f\t%d\t%zu\n", avgTimeAVLTree, avgCmpCntAVLTree, memoryAVLTree);
        fprintf(table_file, "Хеш-таблица\t%.2f\t%d\t%zu\n\n", avgTimeHashTable, avgCmpCntHashTable, memoryHashTable);

        cleanup(&rootTree, &rootAVLTree, &hashTable);
    }

    fclose(table_file);
    printf("\n");
}
