#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ui.h"
#include "literature.h"
#include "sort.h"

#define CNT_CHANGES 500

enum errors
{
    SUCCESS,
    NO_DATA_ERROR,
    INPUT_COMMAND_ERR,
    OPEN_FILE_ERROR,
    OVERFLOW_BOOKS_ERROR,
    EMPTY_FILE_ERROR,
    READ_NUM_BOOK_ERR,
    INVALID_NUM_BOOK_ERR,
    OVERFLOW_LASTNAME_ERR,
    OVERFLOW_NAME_ERR,
    OVERFLOW_PUBLISHER_ERR,
    READ_PAGES_ERR,
    INVALID_CNT_PAGES_ERR,
    READ_TYPE_BOOK_ERR,
    INVALID_TYPE_BOOK_ERR,
    OVERFLOW_BRANCH_ERR,
    READ_NATIONAL_ERR,
    INVALID_NATIONAL_ERR,
    READ_YEAR_ERR,
    INVALID_YEAR_ERR,
    READ_ART_TYPE_ERR,
    INVALID_ART_TYPE_ERR,
    READ_MIN_AGE_ERR,
    INVALID_MIN_AGE_ERR,
    READ_CHILD_TYPE_ERR,
    INVALID_CHILD_TYPE_ERR,
    READ_DEL_FIELD_ERR,
    INVALID_DEL_ERR
};

int handle_read_books_errors(int error_code);

int handle_add_book_errors(int error_code);

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        printf("Не передан датасет\n");
        return NO_DATA_ERROR;
    }

    char *DATA = argv[1];
    int isRead = 0;

    book_t books[MAX_BOOKS];
    size_t n = 0;
    book_key_t books_key[MAX_BOOKS];

    int rc;
    int choice = 1;
    while (choice)
    {
        menu();
        printf("Выберите команду: ");

        if (scanf("%d", &choice) != 1)
        {
            printf("Неудачный ввод команды!\n");
            return INPUT_COMMAND_ERR;
        }

        switch (choice)
        {
            case 0:
                break;
            case 1:
                if (isRead)
                {
                    puts("Файл уже был считан\n");
                    break;
                }

                rc = read_books(DATA, books, books_key, &n);
                if (rc != PROCESS_DONE)
                    return handle_read_books_errors(rc);

                isRead = 1;
                printf("Данные из файла прочитаны успешно: %ld шт.\n\n", n);
                break;
            case 2:
                if (!isRead)
                {
                    puts("Файл еще не был считан!\n");
                    break;
                }

                if (n == 0)
                {
                    printf("Нет данных для вывода. Выберите другую команду\n");
                    break;
                }

                print_books(books, n);
                break;
            case 3:
                if (!isRead)
                {
                    puts("Файл еще не был считан!\n");
                    break;
                }

                if (n == 0)
                {
                    printf("Нет данных для вывода. Выберите другую команду\n");
                    break;
                }

                print_books_key(books_key, n);
                break;
            case 4:
                if (!isRead)
                {
                    puts("Файл еще не был считан!\n");
                    break;
                }

                if (n + 1 >= MAX_BOOKS)
                {
                    printf("Достигнуто максимальное количество записей. Выберите другую команду\n");
                    break;
                }

                rc = add_book(books, books_key, &n);
                if (rc != PROCESS_DONE)
                    return handle_add_book_errors(rc);

                break;
            case 5:
                if (!isRead)
                {
                    puts("Файл еще не был считан!\n");
                    break;
                }

                if (n == 0)
                {
                    printf("Отсутствуют записи. Выберите другую команду\n");
                    break;
                }

                rc = del_book(books, books_key, &n);

                if (rc == READ_DEL_FIELD_FAILED)
                {
                    printf("Ошибка считывания поля для удаления записи\n");
                    return READ_DEL_FIELD_ERR;
                }

                if (rc == INVALID_DEL_FIELD)
                {
                    printf("Некорректное поле для удаления записи\n");
                    return INVALID_DEL_ERR;
                }

                if (rc != PROCESS_DONE)
                    return handle_add_book_errors(rc);

                break;
            case 6:
                if (!isRead)
                {
                    puts("Файл еще не был считан!\n");
                    break;
                }

                if (n == 0)
                {
                    printf("Нет данных для быстрой сортировки по кол. страниц. Выберите другую команду\n");
                    break;
                }

                quick_sort_by_page_cnt(books, 0, n - 1);

                puts("Данные быстро отсортированы по кол. страниц в книге.\n");
                break;
            case 7:
                if (!isRead)
                {
                    puts("Файл еще не был считан!\n");
                    break;
                }

                if (n == 0)
                {
                    printf("Нет данных для медленной сортировки по кол. страниц. Выберите другую команду\n");
                    break;
                }

                slow_sort_by_page_cnt(books, n);

                puts("Данные медленно отсортированы по кол. страниц в книге.\n");
                break;
            case 8:
                if (!isRead)
                {
                    puts("Файл еще не был считан!\n");
                    break;
                }

                if (n == 0)
                {
                    printf("Нет данных для быстрой сортировки ключей. Выберите другую команду\n");
                    break;
                }

                quick_sort_by_key(0, n - 1, books_key);

                puts("Таблица ключей быстро отсортирована по кол. страниц.\n");
                break;
            case 9:
                if (!isRead)
                {
                    puts("Файл еще не был считан!\n");
                    break;
                }

                if (n == 0)
                {
                    printf("Нет данных для медленной сортировки ключей. Выберите другую команду\n");
                    break;
                }

                slow_sort_by_key(books_key, n);
                puts("Таблица ключей медленно отсортирована по кол. страниц.\n");
                break;
            case 10:
                if (!isRead)
                {
                    puts("Файл еще не был считан!\n");
                    break;
                }

                if (n == 0)
                {
                    printf("Нет данных для вывыда по таблице ключей. Выберите другую команду\n");
                    break;
                }

                print_books_by_key(books, books_key, n);
                break;
            case 11:
                if (!isRead)
                {
                    puts("Файл еще не был считан!\n");
                    break;
                }

                if (n == 0)
                {
                    printf("Нет данных для вывода таблицы эффективности. Выберите другую команду\n");
                    break;
                }

                printf("Измерение времени выполнения сортировок для данных и таблицы ключей...\n");
                print_sort_efficiency_table(books, books_key, n, CNT_CHANGES);
                break;
            case 12:
                if (!isRead)
                {
                    puts("Файл еще не был считан!\n");
                    break;
                }

                if (n == 0)
                {
                    printf("Нет данных для вывода списока всех романов указанного автора. Выберите другую команду\n");
                    break;
                }

                rc = find_and_printf_novels_by_author(books, n);
                if (rc != PROCESS_DONE)
                    handle_add_book_errors(rc);

                break;
            default:
                puts("Нет такой команды! Повторите попытку\n");
                break;
        }
    }

    return SUCCESS;
}

int handle_read_books_errors(int error_code)
{
    switch (error_code)
    {
        case OPEN_FILE_FAILED:
            printf("Ошибка открытия файла для чтения данных\n");
            return OPEN_FILE_ERROR;
        case OVERFLOW_BOOKS:
            printf("Достигнуто максимальное количество записей\n");
            return OVERFLOW_BOOKS_ERROR;
        case EMPTY_FILE:
            printf("Передан пустой файл\n");
            return EMPTY_FILE_ERROR;
    }

    printf("Неизвестная ошибка\n");
    return error_code;
}

int handle_add_book_errors(int error_code)
{
    switch (error_code)
    {
        case READ_NUM_BOOK_FAILED:
            printf("Ошибка при чтении номера книги\n");
            return READ_NUM_BOOK_ERR;

        case INVALID_NUM_BOOK:
            printf("Некорректный номер книги\n");
            return INVALID_NUM_BOOK_ERR;

        case OVERFLOW_LASTNAME:
            printf("Фамилия автора большей длины\n");
            return OVERFLOW_LASTNAME_ERR;

        case OVERFLOW_NAME:
            printf("Имя автора большей длины\n");
            return OVERFLOW_NAME_ERR;

        case OVERFLOW_PUBLISHER:
            printf("Издатель имеет большую длину\n");
            return OVERFLOW_PUBLISHER_ERR;

        case READ_PAGES_FAILED:
            printf("Ошибка при чтении количества страниц\n");
            return READ_PAGES_ERR;

        case INVALID_CNT_PAGES:
            printf("Некорректное количество страниц\n");
            return INVALID_CNT_PAGES_ERR;

        case READ_TYPE_BOOK_FAILED:
            printf("Ошибка при чтении типа книги\n");
            return READ_TYPE_BOOK_ERR;

        case INVALID_TYPE_BOOK:
            printf("Некорректный тип книги\n");
            return INVALID_TYPE_BOOK_ERR;

        case OVERFLOW_BRANCH:
            printf("Название отросли превышает допустимую длину\n");
            return OVERFLOW_BRANCH_ERR;

        case READ_NATIONAL_FAILED:
            printf("Ошибка при чтении национализированности\n");
            return READ_NATIONAL_ERR;

        case INVALID_NATIONAL:
            printf("Некорректная национализированность\n");
            return INVALID_NATIONAL_ERR;

        case READ_YEAR_FAILED:
            printf("Ошибка при чтении года\n");
            return READ_YEAR_ERR;

        case INVALID_YEAR:
            printf("Некорректный год\n");
            return INVALID_YEAR_ERR;

        case READ_ART_TYPE_FAILED:
            printf("Ошибка при чтении типа худ. литературы\n");
            return READ_ART_TYPE_ERR;

        case INVALID_ART_TYPE:
            printf("Некорректный тип худ. литературы\n");
            return INVALID_ART_TYPE_ERR;

        case READ_MIN_AGE_FAILED:
            printf("Ошибка при чтении минимального возраста\n");
            return READ_MIN_AGE_ERR;

        case INVALID_MIN_AGE:
            printf("Некорректный минимальный возраст\n");
            return INVALID_MIN_AGE_ERR;

        case READ_CHILD_TYPE_FAILED:
            printf("Ошибка при чтении типа детской литературы\n");
            return READ_CHILD_TYPE_ERR;

        case INVALID_CHILD_TYPE:
            printf("Некорректный тип детской литературы\n");
            return INVALID_CHILD_TYPE_ERR;
    }

    printf("Неизвестная ошибка\n");
    return error_code;
}
