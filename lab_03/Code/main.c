#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ui.h"
#include "matrix.h"
#include "measurement.h"

enum errors
{
    SUCCESS,
    NO_DATA_ERROR,
    INPUT_COMMAND_ERROR,
    READ_INPUT_CHOICE_ERROR,
    INVALID_INPUT_CHOICE_ERROR,
    OPEN_FILE_ERROR,
    ALLOC_ERROR,
    READ_ROWS_MATRIX_ERROR,
    INCORRECT_ROWS_MATRIX_ERROR,
    READ_COLS_MATRIX_ERROR,
    INCORRECT_COLS_MATRIX_ERROR,
    READ_VALUE_BY_INPUT_ERROR,
    READ_NON_ZERO_CNT_ERROR,
    INCORRECT_NON_ZERO_CNT_ERROR,
    READ_COORDS_ERROR,
    INCORRECT_COORDS_ERROR,
    INPUT_TEST_ROWS_ERROR,
    INPUT_TEST_COLS_ERROR,
    TEST_ALLOC_ERROR
};

int handle_read_matrix_by_file_errors(int error_code);

int handle_read_matrix_by_input_errors(int error_code);

int handle_read_matrix_by_coords_errors(int error_code);

int handle_compare_errors(int error_code);

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        printf("Не переданы датасеты\n");
        return NO_DATA_ERROR;
    }

    char *DATA_1 = argv[1];
    char *DATA_2 = argv[2];

    standard_matrix_t st_matr_1 = { 0, 0, NULL };
    sparse_matrix_t sp_matr_1 = { 0, 0, 0, NULL, NULL, 0, NULL };

    standard_matrix_t st_matr_2 = { 0, 0, NULL };
    sparse_matrix_t sp_matr_2 = { 0, 0, 0, NULL, NULL, 0, NULL };

    int rc;
    int isInit = 0;
    int choice = 1;
    while (choice)
    {
        menu();
        printf("Выберите команду: ");

        if (scanf("%d", &choice) != 1)
        {
            free_matrixes(st_matr_1, sp_matr_1);
            free_matrixes(st_matr_2, sp_matr_2);

            printf("Неудачный ввод команды!\n");
            return INPUT_COMMAND_ERROR;
        }

        switch (choice)
        {
            case 0:
                free_matrixes(st_matr_1, sp_matr_1);
                free_matrixes(st_matr_2, sp_matr_2);
                break;
            case 1:
                if (isInit)
                {
                    puts("Матрицы уже инициализированы\n");
                    break;
                }

                printf("\nВыберите способ ввода матриц: \n");
                printf("0 - Считать данные из файлов\n");
                printf("1 - Считать данные вручную\n");

                int input_choice;
                printf("Выбор: ");

                if (scanf("%d", &input_choice) != 1)
                {
                    printf("Ошибка чтения способа ввода матриц\n");
                    return READ_INPUT_CHOICE_ERROR;
                }

                if (input_choice != 0 && input_choice != 1)
                {
                    printf("Некорректный выбор способа ввода матриц\n");
                    return INVALID_INPUT_CHOICE_ERROR;
                }

                if (input_choice == 0)
                {
                    rc = read_matrix_by_file(DATA_1, &st_matr_1);
                    if (rc != PROCESS_DONE)
                        return handle_read_matrix_by_file_errors(rc);

                    rc = read_matrix_by_file(DATA_2, &st_matr_2);
                    if (rc != PROCESS_DONE)
                    {
                        free_standard_matrix(st_matr_1);
                        return handle_read_matrix_by_file_errors(rc);
                    }
                }
                else if (input_choice == 1)
                {
                    printf("\nВыберите количество строк матриц (до %d): ", MAX_DIMENSION);
                    int i_n = 0;
                    if (scanf("%d", &i_n) != 1)
                        return READ_ROWS_MATRIX_ERROR;

                    if (i_n <= 0 || i_n > MAX_DIMENSION)
                        return INCORRECT_ROWS_MATRIX_ERROR;

                    size_t n = (size_t) i_n;

                    printf("\nВыберите количество столбцов матриц (до %d): ", MAX_DIMENSION);
                    int i_m = 0;
                    if (scanf("%d", &i_m) != 1)
                        return READ_COLS_MATRIX_ERROR;

                    if (i_m <= 0 || i_m > MAX_DIMENSION)
                        return INCORRECT_COLS_MATRIX_ERROR;

                    size_t m = (size_t) i_m;

                    printf("\nВыберите способ ручного ввода первой матрицы: \n");
                    printf("0 - Полный ввод матрицы\n");
                    printf("1 - Координатный способ\n");
                    printf("Выбор: ");

                    if (scanf("%d", &input_choice) != 1)
                        return READ_INPUT_CHOICE_ERROR;

                    if (input_choice != 0 && input_choice != 1)
                        return INVALID_INPUT_CHOICE_ERROR;

                    if (input_choice == 0)
                    {
                        rc = read_matrixes_by_input(&st_matr_1, n, m);
                        if (rc != PROCESS_DONE)
                            return handle_read_matrix_by_input_errors(rc);
                    }
                    else if (input_choice == 1)
                    {
                        rc = read_matrixes_by_coords(&st_matr_1, n, m);
                        if (rc != PROCESS_DONE)
                            return handle_read_matrix_by_coords_errors(rc);
                    }

                    printf("\nВыберите способ ручного ввода второй матрицы: \n");
                    printf("0 - Полный ввод матрицы\n");
                    printf("1 - Координатный способ\n");
                    printf("Выбор: ");

                    if (scanf("%d", &input_choice) != 1)
                        return READ_INPUT_CHOICE_ERROR;

                    if (input_choice != 0 && input_choice != 1)
                        return INVALID_INPUT_CHOICE_ERROR;

                    if (input_choice == 0)
                    {
                        rc = read_matrixes_by_input(&st_matr_2, n, m);
                        if (rc != PROCESS_DONE)
                        {
                            free_standard_matrix(st_matr_1);
                            return handle_read_matrix_by_input_errors(rc);
                        }
                    }
                    else if (input_choice == 1)
                    {
                        rc = read_matrixes_by_coords(&st_matr_2, n, m);
                        if (rc != PROCESS_DONE)
                        {
                            free_standard_matrix(st_matr_1);
                            return handle_read_matrix_by_coords_errors(rc);
                        }
                    }
                }

                rc = standard_to_sparse_matrix(st_matr_1, &sp_matr_1);
                if (rc != PROCESS_DONE)
                {
                    printf("Ошибка выделения памяти во время перевода из стандартной матрицы в разреженную\n");
                    return ALLOC_ERROR;
                }

                rc = standard_to_sparse_matrix(st_matr_2, &sp_matr_2);
                if (rc != PROCESS_DONE)
                {
                    printf("Ошибка выделения памяти во время перевода из стандартной матрицы в разреженную\n");
                    return ALLOC_ERROR;
                }

                isInit = 1;
                printf("Массивы успешно инициализированы!\n\n");
                break;
            case 2:
                if (!isInit)
                {
                    puts("Матрицы еще не инициализированы\n");
                    break;
                }

                print_matrix(st_matr_1);
                break;
            case 3:
                if (!isInit)
                {
                    puts("Матрицы еще не инициализированы\n");
                    break;
                }

                print_property_sparse_matrix(sp_matr_1);
                break;
            case 4:
                if (!isInit)
                {
                    puts("Матрицы еще не инициализированы\n");
                    break;
                }

                print_sparse_matrix(sp_matr_1);
                break;
            case 5:
                if (!isInit)
                {
                    puts("Матрицы еще не инициализированы\n");
                    break;
                }

                print_matrix(st_matr_2);
                break;
            case 6:
                if (!isInit)
                {
                    puts("Матрицы еще не инициализированы\n");
                    break;
                }

                print_property_sparse_matrix(sp_matr_2);
                break;
            case 7:
                if (!isInit)
                {
                    puts("Матрицы еще не инициализированы\n");
                    break;
                }

                print_sparse_matrix(sp_matr_2);
                break;
            case 8:
                if (!isInit)
                {
                    puts("Матрицы еще не инициализированы\n");
                    break;
                }

                standard_matrix_t st_matr_3 = { 0, 0, NULL };

                sum_standard_matrixes(&st_matr_3, st_matr_1, st_matr_2);

                print_matrix(st_matr_3);

                free_standard_matrix(st_matr_3);
                break;
            case 9:
                if (!isInit)
                {
                    puts("Матрицы еще не инициализированы\n");
                    break;
                }

                sparse_matrix_t sp_matr_3 = { 0, 0, 0, NULL, NULL, 0, NULL };

                rc = sum_sparse_matrixes(&sp_matr_3, sp_matr_1, sp_matr_2);
                if (rc != PROCESS_DONE)
                {
                    free_matrixes(st_matr_1, sp_matr_1);
                    free_matrixes(st_matr_2, sp_matr_2);

                    printf("Ошибка выделения памяти во время суммы разреженных матриц\n");
                    return ALLOC_ERROR;
                }

                print_property_sparse_matrix(sp_matr_3);

                print_sparse_matrix(sp_matr_3);

                free_sparse_matrix(sp_matr_3);
                break;
            case 10:
                compare_matrix_operations();
                break;
            default:
                puts("Нет такой команды! Повторите попытку\n");
                break;
        }
    }

    return SUCCESS;
}

int handle_read_matrix_by_file_errors(int error_code)
{
    switch (error_code)
    {
        case OPEN_FILE_FAILED:
            printf("Ошибка открытия файла для чтения данных\n");
            return OPEN_FILE_ERROR;
        case ALLOC_FAILED:
            printf("Ошибка выделения памяти для матрицы\n");
            return ALLOC_ERROR;
    }

    printf("Неизвестная ошибка\n");
    return error_code;
}

int handle_read_matrix_by_input_errors(int error_code)
{
    switch (error_code)
    {
        case ALLOC_FAILED:
            printf("Ошибка выделения памяти для матрицы\n");
            return ALLOC_ERROR;
        case READ_VALUE_BY_INPUT_FAILED:
            printf("Ошибка чтения значения для матрицы\n");
            return READ_VALUE_BY_INPUT_ERROR;
    }

    printf("Неизвестная ошибка\n");
    return error_code;
}

int handle_read_matrix_by_coords_errors(int error_code)
{
    switch (error_code)
    {
        case ALLOC_FAILED:
            printf("Ошибка выделения памяти для матрицы\n");
            return ALLOC_ERROR;
        case READ_NON_ZERO_CNT_FAILED:
            printf("Ошибка ввода количества ненулевых элементов\n");
            return READ_NON_ZERO_CNT_ERROR;
        case INCORRECT_NON_ZERO_CNT_FAILED:
            printf("Невалидное значение количества ненулевых элементов\n");
            return INCORRECT_NON_ZERO_CNT_ERROR;
        case READ_COORDS_FAILED:
            printf("Невалидное значение количества ненулевых элементов\n");
            return READ_COORDS_ERROR;
        case INCORRECT_COORDS:
            printf("Невалидное значение координат числа\n");
            return INCORRECT_COORDS_ERROR;
    }

    printf("Неизвестная ошибка\n");
    return error_code;
}

int handle_compare_errors(int error_code)
{
    switch (error_code)
    {
        case INPUT_ROWS_FAILED:
            printf("Некорректный ввод для количества строк.\n");
            return INPUT_TEST_ROWS_ERROR;
        case INPUT_COLS_FAILED:
            printf("Ошибка ввода количества ненулевых элементов\n");
            return INPUT_TEST_COLS_ERROR;
        case ALLOC_FAILED:
            printf("Ошибка выделения памяти для сравнения операций\n");
            return TEST_ALLOC_ERROR;
    }

    printf("Неизвестная ошибка\n");
    return error_code;
}
