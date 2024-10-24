#ifndef SORT_H__
#define SORT_H__

#include <stdlib.h>
#include "literature.h"

void quick_sort_by_page_cnt(book_t books[], int left, int right);

void slow_sort_by_page_cnt(book_t books[], size_t len);

void quick_sort_by_key(int left, int right, book_key_t books_key[]);

void slow_sort_by_key(book_key_t books_key[], size_t len);

void print_sort_efficiency_table(book_t books[], book_key_t books_key[], size_t n, int cnt_iter);

#endif
