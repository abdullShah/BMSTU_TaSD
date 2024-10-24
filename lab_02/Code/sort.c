#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <linux/time.h>
#include "literature.h"
#include "sort.h"

extern int clock_gettime (clock_t __clock_id, struct timespec *__tp) __THROW;

void quick_sort_by_page_cnt(book_t books[], int left, int right)
{
    if (left < right)
    {
        int i = left, j = right;
        int page_cnt = books[left].page_cnt;

        while (i <= j)
        {
            while (books[i].page_cnt < page_cnt)
                i++;

            while (books[j].page_cnt > page_cnt)
                j--;

            if (i <= j)
            {
                book_t temp = books[i];
                books[i] = books[j];
                books[j] = temp;
                i++;
                j--;
            }
        }

        quick_sort_by_page_cnt(books, left, j);
        quick_sort_by_page_cnt(books, i, right);
    }
}

void slow_sort_by_page_cnt(book_t books[], size_t len)
{
    for (size_t i = 0; i < len - 1; i++)
        for (size_t j = 0; j < len - i - 1; j++)
            if (books[j].page_cnt > books[j + 1].page_cnt)
            {
                book_t temp = books[j];
                books[j] = books[j + 1];
                books[j + 1] = temp;
            }
}

void quick_sort_by_key(int left, int right, book_key_t books_key[])
{
    if (left < right)
    {
        int i = left, j = right;
        int page_cnt = books_key[left].page_cnt;

        while (i <= j)
        {
            // Ищем элементы, которые больше или равны опорному элементу
            while (books_key[i].page_cnt < page_cnt)
                i++;

            // Ищем элементы, которые меньше или равны опорному элементу
            while (books_key[j].page_cnt > page_cnt)
                j--;

            if (i <= j)
            {
                book_key_t temp = books_key[i];
                books_key[i] = books_key[j];
                books_key[j] = temp;
                i++;
                j--;
            }
        }

        // Рекурсивно сортируем две подгруппы
        if (left < j)
            quick_sort_by_key(left, j, books_key);

        if (i < right)
            quick_sort_by_key(i, right, books_key);
    }
}

void slow_sort_by_key(book_key_t books_key[], size_t len)
{
    for (size_t i = 0; i < len - 1; i++)
        for (size_t j = 0; j < len - i - 1; j++)
            if (books_key[j].page_cnt > books_key[j + 1].page_cnt)
            {
                book_key_t temp = books_key[j];
                books_key[j] = books_key[j + 1];
                books_key[j + 1] = temp;
            }
}


void print_sort_efficiency_table(book_t books[], book_key_t books_key[], size_t n, int cnt_iter)
{
    struct timespec quick_sort_data_start, quick_sort_data_end;
    struct timespec slow_sort_data_start, slow_sort_data_end;
    struct timespec quick_sort_key_start, quick_sort_key_end;
    struct timespec slow_sort_key_start, slow_sort_key_end;

    long long quick_sort_data_time_sum = 0;
    long long slow_sort_data_time_sum = 0;
    long long quick_sort_key_time_sum = 0;
    long long slow_sort_key_time_sum = 0;

    for (int i = 0; i < cnt_iter; i++)
    {
        // Измерение времени выполнения быстрой сортировки данных
        clock_gettime(CLOCK_MONOTONIC, &quick_sort_data_start);
        quick_sort_by_page_cnt(books, 0, n - 1);
        clock_gettime(CLOCK_MONOTONIC, &quick_sort_data_end);
        quick_sort_data_time_sum += (quick_sort_data_end.tv_sec - quick_sort_data_start.tv_sec) * 1000000000LL +
                                (quick_sort_data_end.tv_nsec - quick_sort_data_start.tv_nsec);

        // Измерение времени выполнения медленной сортировки данных
        clock_gettime(CLOCK_MONOTONIC, &slow_sort_data_start);
        slow_sort_by_page_cnt(books, n);
        clock_gettime(CLOCK_MONOTONIC, &slow_sort_data_end);
        slow_sort_data_time_sum += (slow_sort_data_end.tv_sec - slow_sort_data_start.tv_sec) * 1000000000LL +
                               (slow_sort_data_end.tv_nsec - slow_sort_data_start.tv_nsec);

        // Измерение времени выполнения быстрой сортировки ключей
        clock_gettime(CLOCK_MONOTONIC, &quick_sort_key_start);
        quick_sort_by_key(0, n - 1, books_key);
        clock_gettime(CLOCK_MONOTONIC, &quick_sort_key_end);
        quick_sort_key_time_sum += (quick_sort_key_end.tv_sec - quick_sort_key_start.tv_sec) * 1000000000LL +
                               (quick_sort_key_end.tv_nsec - quick_sort_key_start.tv_nsec);

        // Измерение времени выполнения медленной сортировки ключей
        clock_gettime(CLOCK_MONOTONIC, &slow_sort_key_start);
        slow_sort_by_key(books_key, n);
        clock_gettime(CLOCK_MONOTONIC, &slow_sort_key_end);
        slow_sort_key_time_sum += (slow_sort_key_end.tv_sec - slow_sort_key_start.tv_sec) * 1000000000LL +
                              (slow_sort_key_end.tv_nsec - slow_sort_key_start.tv_nsec);
    }

    // Рассчитываем среднее время выполнения
    long long average_quick_sort_data_time = quick_sort_data_time_sum / cnt_iter;
    long long average_slow_sort_data_time = slow_sort_data_time_sum / cnt_iter;
    long long average_quick_sort_key_time = quick_sort_key_time_sum / cnt_iter;
    long long average_slow_sort_key_time = slow_sort_key_time_sum / cnt_iter;


    // Выводим средние значения в таблицу
    printf("\nСреднее время выполнения сортировок\n");
    printf("      Сортировка           |    Время, нс   |\n");
    printf("---------------------------+----------------|\n");
    printf("Быстрая сортировка         | %-14lld |\n", average_quick_sort_data_time);
    printf("Медленная сортировка       | %-14lld |\n", average_slow_sort_data_time);
    printf("Быстрая сортировка ключей  | %-14lld |\n", average_quick_sort_key_time);
    printf("Медленная сортировка ключей| %-14lld |\n", average_slow_sort_key_time);

    // Оценка использования оперативной памяти
    size_t memory_used = n * sizeof(book_t);  // Размер данных
    size_t memory_used_keys = n * sizeof(book_key_t);  // Размер таблицы ключей
    printf("\nИспользование оперативной памяти:\n");
    printf("Размер данных: %zu байт\n", memory_used);
    printf("Размер таблицы ключей: %zu байт\n", memory_used_keys);
    printf("Общий объем используемой памяти: %zu байт\n\n", memory_used + memory_used_keys);
}
