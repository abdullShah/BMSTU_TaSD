#ifndef LITERATURE_H__
#define LITERATURE_H__

#include <stdlib.h>

enum statuses
{
    PROCESS_DONE,
    OPEN_FILE_FAILED,  //
    OVERFLOW_BOOKS,
    EMPTY_FILE,
    READ_NUM_BOOK_FAILED,  //
    OVERFLOW_NAME,
    OVERFLOW_PUBLISHER,
    READ_PAGES_FAILED,
    INVALID_CNT_PAGES,
    READ_TYPE_BOOK_FAILED,
    INVALID_TYPE_BOOK,
    OVERFLOW_BRANCH,
    READ_NATIONAL_FAILED,
    INVALID_NATIONAL,
    READ_YEAR_FAILED,
    INVALID_YEAR,
    READ_ART_TYPE_FAILED,
    INVALID_ART_TYPE,
    READ_MIN_AGE_FAILED,
    INVALID_MIN_AGE,
    READ_CHILD_TYPE_FAILED,
    INVALID_CHILD_TYPE,
    READ_DEL_FIELD_FAILED,  //
    INVALID_DEL_FIELD,
    INVALID_NUM_BOOK,
    OVERFLOW_LASTNAME,
};

#define MAX_BOOKS 1000
#define LASTNAME_LEN 15
#define NAME_LEN 15
#define PUBLISHER_LEN 15
#define BRANCH_LEN 15

typedef enum
{
    technical,
    artistic,
    childish
} type_t;

typedef enum
{
    novel,
    play,
    poetry
} art_type_t;

typedef enum
{
    poems,
    fairytales
} child_type_t;

typedef struct
{
    int ind;
    char lastname[LASTNAME_LEN + 1];
    char name[NAME_LEN + 1];
    char publisher[PUBLISHER_LEN + 1];
    int page_cnt;
    type_t type;
    union
    {
        struct
        {
            char branch[BRANCH_LEN + 1];
            int isDomestic;
            int year;
        } technical;

        struct
        {
            art_type_t type;
        } artistic;

        struct
        {
            int min_age;
            child_type_t type;
        } childish;
    } details;
} book_t;

typedef struct
{
    int ind;
    int page_cnt;
} book_key_t;

int read_books(char *filename, book_t books[], book_key_t books_key[], size_t *n);

void print_books(book_t books[], size_t len);

void print_books_key(book_key_t books_key[], size_t len);

int add_book(book_t books[], book_key_t books_key[], size_t *len);

int del_book(book_t books[], book_key_t books_key[], size_t *len);

void print_books_by_key(book_t books[], book_key_t books_key[], size_t len);

int find_and_printf_novels_by_author(book_t books[], size_t len);

#endif
