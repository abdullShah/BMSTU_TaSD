#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "matrix.h"

static void free_matrix(int **data, size_t n)
{
    for (size_t i = 0; i < n; i++)
        free(data[i]);

    free(data);
}

int** alloc_matrix(size_t n, size_t m)
{
    int **data = calloc(n, sizeof(int*));

    if (!data)
        return NULL;

    for (size_t i = 0; i < n; i++)
    {
        data[i] = calloc(m, sizeof(int));

        if (!data[i])
        {
            free_matrix(data, n);
            return NULL;
        }
    }

    return data;
}

int read_matrix_by_file(char *filename, standard_matrix_t *matrix)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
        return OPEN_FILE_FAILED;

    size_t n = 0;
    size_t m = 0;
    fscanf(file, "%zu %zu", &n, &m);

    int **values = alloc_matrix(n, m);

    if (!values)
        return ALLOC_FAILED;

    for (size_t i = 0; i < n; i++)
        for (size_t j = 0; j < m; j++)
            fscanf(file, "%d", &values[i][j]);

    matrix->rows = n;
    matrix->cols = m;
    matrix->values = values;

    fclose(file);
    return PROCESS_DONE;
}

void free_standard_matrix(standard_matrix_t matrix)
{
    free_matrix(matrix.values, matrix.rows);
}

static void free_array(int *data)
{
    free(data);
}

void free_sparse_matrix(sparse_matrix_t matrix)
{
    free_array(matrix.A);
    free_array(matrix.IA);
    free_array(matrix.JA);
}

void free_matrixes(standard_matrix_t st_matrix, sparse_matrix_t sp_matrix)
{
    free_standard_matrix(st_matrix);
    free_sparse_matrix(sp_matrix);
}

int read_matrixes_by_input(standard_matrix_t *matrix, size_t rows, size_t cols)
{
    int **values = alloc_matrix(rows, cols);

    if (!values)
        return ALLOC_FAILED;

    printf("\nВводите значения матрицы: \n");
    for (size_t i = 0; i < rows; i++)
        for (size_t j = 0; j < cols; j++)
            if (scanf("%d", &values[i][j]) != 1)
                return READ_VALUE_BY_INPUT_FAILED;

    matrix->rows = rows;
    matrix->cols = cols;
    matrix->values = values;

    return PROCESS_DONE;
}

int read_matrixes_by_coords(standard_matrix_t *matrix, size_t rows, size_t cols)
{
    int **values = alloc_matrix(rows, cols);

    if (!values)
        return ALLOC_FAILED;

    int non_zero_cnt;
    printf("\nВведите количество ненулевых элементов (до %zu): ", rows * cols);
    if (scanf("%d", &non_zero_cnt) != 1)
        return READ_NON_ZERO_CNT_FAILED;

    if (non_zero_cnt < 0 || non_zero_cnt > (int) (rows * cols))
        return INCORRECT_NON_ZERO_CNT_FAILED;

    for (size_t q = 0; q < (size_t) non_zero_cnt; q++)
    {
        int i;
        int j;
        int value;

        printf("Введите через пробел индексы строки (от 0 до %zu), столбца (от 0 до %zu) и значение:\n", rows - 1, cols - 1);
        if (scanf("%d %d %d", &i, &j, &value) != 3)
            return READ_COORDS_FAILED;

        if (i < 0 || i >= (int) rows || j < 0|| j >= (int) cols)
            return INCORRECT_COORDS;

        values[i][j] = value;
    }

    matrix->rows = rows;
    matrix->cols = cols;
    matrix->values = values;

    return PROCESS_DONE;
}

int standard_to_sparse_matrix(standard_matrix_t st_matr, sparse_matrix_t *sp_matr)
{
    size_t n = st_matr.rows;
    size_t m = st_matr.cols;

    size_t A_len = 0;

    for (size_t i = 0; i < n; i++)
        for (size_t j = 0; j < m; j++)
            if (st_matr.values[i][j] != 0)
                ++A_len;

    int *A = malloc(A_len * sizeof(int));
    if (!A)
        return ALLOC_FAILED;

    int *IA = malloc(A_len * sizeof(int));
    if (!IA)
    {
        free(A);
        return ALLOC_FAILED;
    }

    int *JA = malloc((m + 1) * sizeof(int));
    if (!JA)
    {
        free(A);
        free(IA);
        return ALLOC_FAILED;
    }

    for (size_t j = 0; j < m; j++)
        JA[j] = -1;

    size_t temp_A_len = 0;
    for (size_t j = 0; j < m; j++)
        for (size_t i = 0; i < n; i++)
        {
            int el = st_matr.values[i][j];
            if (el != 0)
            {
                if (JA[j] == -1)
                    JA[j] = temp_A_len;

                A[temp_A_len] = el;
                IA[temp_A_len] = i;
                ++temp_A_len;
            }
        }

    JA[m] = A_len;

    for (int j = m; j > 0; j--)
        if (JA[j - 1] == -1)
            JA[j - 1] = JA[j];

    sp_matr->rows = n;
    sp_matr->cols = m;
    sp_matr->A_len = A_len;
    sp_matr->A = A;
    sp_matr->IA = IA;
    sp_matr->JA_len = m + 1;
    sp_matr->JA = JA;

    return PROCESS_DONE;
}

void print_matrix(standard_matrix_t matrix)
{
    printf("\nЭлементы матрицы:\n");
    for (size_t i = 0; i < matrix.rows; i++)
    {
        for (size_t j = 0; j < matrix.cols; j++)
            printf("%d ", matrix.values[i][j]);

        printf("\n");
    }

    printf("\n");
}

static void print_arr(int *arr, size_t n)
{
    for (size_t i = 0; i < n; i++)
        printf("%d ", arr[i]);

    printf("\n");
}

void print_property_sparse_matrix(sparse_matrix_t matrix)
{
    printf("\nВектор A содержит значения ненулевых элементов\nA: ");
    print_arr(matrix.A, matrix.A_len);

    printf("Вектор IA содержит номера строк для элементов вектора A\nIA: ");
    print_arr(matrix.IA, matrix.A_len);

    printf("Вектор JA, в элементе Nk которого находится номер компонент в A и IA, с "
         "которых начинается описание столбца Nk матрицы A\nJA: ");
    print_arr(matrix.JA, matrix.JA_len);

    printf("\n");
}

void print_sparse_matrix(sparse_matrix_t matrix)
{
    int **temp_matr = alloc_matrix(matrix.rows, matrix.cols);
    standard_matrix_t temp_standard_matr =
    {
        matrix.rows,
        matrix.cols,
        temp_matr
    };

    if (matrix.A_len != 0)
    {
        int A_pointer = 0;
        for (size_t j = 1; j < matrix.JA_len; j++)
        {
            int n = matrix.JA[j] - matrix.JA[j - 1];
            while ((n--) > 0)
            {
                temp_matr[ matrix.IA[A_pointer] ][j - 1] = matrix.A[A_pointer];
                A_pointer++;
            }

        }
    }

    print_matrix(temp_standard_matr);

    free_matrix(temp_matr, matrix.rows);
}

void sum_standard_matrixes(standard_matrix_t *matr_3, standard_matrix_t matr_1, standard_matrix_t matr_2)
{
    matr_3->rows = matr_1.rows;
    matr_3->cols = matr_1.cols;
    matr_3->values = alloc_matrix(matr_1.rows, matr_1.cols);

    for (size_t i = 0; i < matr_1.rows; i++)
        for (size_t j = 0; j < matr_1.cols; j++)
            matr_3->values[i][j] =  matr_1.values[i][j] + matr_2.values[i][j];
}

int sum_sparse_matrixes(sparse_matrix_t *matr_3, sparse_matrix_t matr_1, sparse_matrix_t matr_2)
{
    size_t max_A_len = matr_1.A_len + matr_2.A_len;

    int *temp_A = malloc(max_A_len * sizeof(int));
    int *temp_IA = malloc(max_A_len * sizeof(int));
    int *temp_JA = malloc(matr_1.JA_len * sizeof(int));

    if (!temp_A || !temp_IA || !temp_JA)
    {
        free(temp_A);
        free(temp_IA);
        free(temp_JA);
        return ALLOC_FAILED;
    }

    size_t A1_pointer = 0, A2_pointer = 0, A3_pointer = 0;
    size_t JA3_pointer = 0;

    // Инициализация для первой позиции JA
    temp_JA[0] = 0;

    // Проходим по каждому столбцу
    for (size_t j = 1; j < matr_1.JA_len; j++)
    {
        // Количество ненулевых элементов в столбце j
        int n1 = matr_1.JA[j] - matr_1.JA[j - 1];
        int n2 = matr_2.JA[j] - matr_2.JA[j - 1];

        while (n1 > 0 || n2 > 0)
        {
            int row_1 = (n1 > 0) ? matr_1.IA[A1_pointer] : -1;
            int row_2 = (n2 > 0) ? matr_2.IA[A2_pointer] : -1;

            if (row_1 == row_2)
            {
                // Складываем элементы
                temp_IA[A3_pointer] = row_1;
                temp_A[A3_pointer] = matr_1.A[A1_pointer] + matr_2.A[A2_pointer];
                A1_pointer++;
                A2_pointer++;
                n1--;
                n2--;
            }
            else if (row_1 < row_2 || n2 == 0)
            {
                // Копируем элемент из первой матрицы
                temp_IA[A3_pointer] = row_1;
                temp_A[A3_pointer] = matr_1.A[A1_pointer];
                A1_pointer++;
                n1--;
            }
            else
            {
                // Копируем элемент из второй матрицы
                temp_IA[A3_pointer] = row_2;
                temp_A[A3_pointer] = matr_2.A[A2_pointer];
                A2_pointer++;
                n2--;
            }

            A3_pointer++;
        }

        // Устанавливаем новое значение JA для результирующей матрицы
        temp_JA[++JA3_pointer] = A3_pointer;
    }

    matr_3->A = malloc(A3_pointer * sizeof(int));
    matr_3->IA = malloc(A3_pointer * sizeof(int));
    matr_3->JA = malloc(matr_1.JA_len * sizeof(int));

    if (!matr_3->A || !matr_3->IA || !matr_3->JA)
    {
        free(temp_A);
        free(temp_IA);
        free(temp_JA);
        free(matr_3->A);
        free(matr_3->IA);
        free(matr_3->JA);
        return ALLOC_FAILED;
    }

    // Копируем результаты из временных массивов в результирующие
    for (size_t i = 0; i < A3_pointer; i++)
    {
        matr_3->A[i] = temp_A[i];
        matr_3->IA[i] = temp_IA[i];
    }

    for (size_t i = 0; i < matr_1.JA_len; i++)
        matr_3->JA[i] = temp_JA[i];

    // Устанавливаем размеры для matr_3
    matr_3->A_len = A3_pointer;
    matr_3->JA_len = matr_1.JA_len;
    matr_3->rows = matr_1.rows;
    matr_3->cols = matr_1.cols;

    // Очистка временных массивов
    free(temp_A);
    free(temp_IA);
    free(temp_JA);

    return PROCESS_DONE;
}
