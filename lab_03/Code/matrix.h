#ifndef MATRIX_H__
#define MATRIX_H__

#include <stdlib.h>

#define MAX_DIMENSION 1000

enum matirx_statuses
{
    PROCESS_DONE,
    OPEN_FILE_FAILED,  //
    ALLOC_FAILED,
    READ_VALUE_BY_INPUT_FAILED,  //
    READ_NON_ZERO_CNT_FAILED,    //
    INCORRECT_NON_ZERO_CNT_FAILED,
    READ_COORDS_FAILED,
    INCORRECT_COORDS
};

typedef struct
{
    size_t rows;
    size_t cols;
    int **values;
} standard_matrix_t;

typedef struct
{
    size_t rows;
    size_t cols;
    size_t A_len;
    int *A;
    int *IA;
    size_t JA_len;
    int *JA;
} sparse_matrix_t;

int** alloc_matrix(size_t n, size_t m);

int read_matrix_by_file(char *filename, standard_matrix_t *matrix);

void free_standard_matrix(standard_matrix_t matrix);

void free_sparse_matrix(sparse_matrix_t matrix);

void free_matrixes(standard_matrix_t st_matrix, sparse_matrix_t sp_matrix);

int read_matrixes_by_input(standard_matrix_t *matrix, size_t rows, size_t cols);

int read_matrixes_by_coords(standard_matrix_t *matrix, size_t rows, size_t cols);

void print_matrix(standard_matrix_t matrix);

int standard_to_sparse_matrix(standard_matrix_t st_matr, sparse_matrix_t *sp_matr);

void print_property_sparse_matrix(sparse_matrix_t matrix);

void print_sparse_matrix(sparse_matrix_t sp_matr);

void sum_standard_matrixes(standard_matrix_t *matr_3, standard_matrix_t matr_1, standard_matrix_t matr_2);

int sum_sparse_matrixes(sparse_matrix_t *matr_3, sparse_matrix_t matr_1, sparse_matrix_t matr_2);

#endif
