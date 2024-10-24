#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "literature.h"

#define EOF_ERROR 1
#define EMPTY_ERROR 2
#define OVERFLOW_ERROR 3

int read_line(FILE *f, char *str, int max_len)
{
    if (!fgets(str, max_len + 1, f))
        return EOF_ERROR;

    if (strlen(str) == 1 && str[0] == '\n')
        return EMPTY_ERROR;

    if (str[strlen(str) - 1] != '\n')
        return OVERFLOW_ERROR;

    str[strlen(str) - 1] = '\0';
    return PROCESS_DONE;
}

int read_books(char *filename, book_t books[], book_key_t books_key[], size_t *n)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
        return OPEN_FILE_FAILED;

    while (fgets(books[*n].lastname, LASTNAME_LEN + 1, file) &&
           fgets(books[*n].name, NAME_LEN + 1, file) &&
           fgets(books[*n].publisher, PUBLISHER_LEN + 1, file) &&
           fscanf(file, "%d %ud", &books[*n].page_cnt, &books[*n].type) == 2)
    {
        books[*n].lastname[strcspn(books[*n].lastname, "\n")] = '\0';
        books[*n].name[strcspn(books[*n].name, "\n")] = '\0';
        books[*n].publisher[strcspn(books[*n].publisher, "\n")] = '\0';

        books[*n].ind = *n;
        (*n)++;
        if (*n > MAX_BOOKS)
            return OVERFLOW_BOOKS;

        // Читаем оставшиеся поля в зависимости от типа книги
        if (books[*n - 1].type == 0)
        {
            fgetc(file);
            fgets(books[*n - 1].details.technical.branch, BRANCH_LEN + 1, file);
            books[*n - 1].details.technical.branch[strcspn(books[*n - 1].details.technical.branch, "\n")] = '\0';

            fscanf(file, "%d %d",
                   &books[*n - 1].details.technical.isDomestic,
                   &books[*n - 1].details.technical.year);

            fgetc(file);
        }
        else if (books[*n - 1].type == 1)
        {
            fscanf(file, "%ud", &books[*n - 1].details.artistic.type);
            fgetc(file);
        }
        else if (books[*n - 1].type == 2)
        {
            fscanf(file, "%d %ud", &books[*n - 1].details.childish.min_age, &books[*n - 1].details.childish.type);
            fgetc(file);
        }
    }

    fclose(file);
    if (*n == 0)
        return EMPTY_FILE;

    // После чтения данных инициализируем массив ключей
    for (size_t i = 0; i < *n; i++)
    {
        books_key[i].ind = i;
        books_key[i].page_cnt = books[i].page_cnt;
    }

    return PROCESS_DONE;
}

void print_books(book_t books[], size_t len)
{
    printf("\n");
    printf("N  |Фамилия        |Название       |Издательство   |Cтр.|Тип лит. |Отрасль        |Нац|Год |Худ.лит.|Мин.возр.|Тип дет. лит.|\n");
    printf("---+---------------+---------------+---------------+----+---------+---------------+---+----+--------+---------+-------------|\n");
    for (size_t i = 0; i < len; i++)
    {
        printf("%-3d|%-*s|%-*s|%-*s|%-4d|%-9s|",
               books[i].ind,
               LASTNAME_LEN, books[i].lastname,
               NAME_LEN, books[i].name,
               PUBLISHER_LEN, books[i].publisher,
               books[i].page_cnt,
               (books[i].type == technical) ? "Technical" :
               (books[i].type == artistic) ? "Artistic" :
               (books[i].type == childish) ? "Childish" : ""
        );

        if (books[i].type == technical)
        {
            printf("%-*s|%-3s|%-4d|",
                   BRANCH_LEN, books[i].details.technical.branch,
                   books[i].details.technical.isDomestic ? "Yes" : "No",
                   books[i].details.technical.year);
        }
        else
            printf("%-*s|%-3s|%-4s|", BRANCH_LEN, "", "", "");

        if (books[i].type == artistic)
        {
            printf("%-8s|",
                   (books[i].details.artistic.type == novel) ? "Novel" :
                   (books[i].details.artistic.type == play) ? "Play" :
                   (books[i].details.artistic.type == poetry) ? "Poetry" : "");
        }
        else
            printf("%-8s|", "");

        if (books[i].type == childish)
        {
            printf("%-9d|%-13s|",
                   books[i].details.childish.min_age,
                   (books[i].details.childish.type == poems) ? "Poems" :
                   (books[i].details.childish.type == fairytales) ? "Fairytales" : "");
        }
        else
            printf("%-9s|%-13s|", "", "");

        printf("\n");
    }
    printf("\n");
}

void print_books_key(book_key_t books_key[], size_t len)
{
    printf("N  |Кол. страниц\n");
    printf("---+---+-------------\n");
    for (size_t i = 0; i < len; i++)
        printf("%-3d|%d\n", books_key[i].ind, books_key[i].page_cnt);

    printf("\n");
}

int add_book(book_t books[], book_key_t books_key[], size_t *len)
{
    int rc;

    book_t book;

    fgetc(stdin);

    do
    {
        printf("Введите фамилию автора (до %d символов): ", LASTNAME_LEN);
        rc = read_line(stdin, book.lastname, LASTNAME_LEN);

        if (rc == EMPTY_ERROR)
            printf("Введена пустая строка. Попробуйте еще раз\n");

    } while (rc == EMPTY_ERROR);

    if (rc != PROCESS_DONE)
        return OVERFLOW_LASTNAME;

    do
    {
        printf("Введите название книги (до %d символов): ", NAME_LEN);
        rc = read_line(stdin, book.name, NAME_LEN);

        if (rc == EMPTY_ERROR)
            printf("Введена пустая строка. Попробуйте еще раз\n");

    } while (rc == EMPTY_ERROR);

    if (rc != PROCESS_DONE)
        return OVERFLOW_NAME;

    do
    {
        printf("Введите издательство (до %d символов): ", PUBLISHER_LEN);
        rc = read_line(stdin, book.publisher, PUBLISHER_LEN);

        if (rc == EMPTY_ERROR)
            printf("Введена пустая строка. Попробуйте еще раз\n");

    } while (rc == EMPTY_ERROR);

    if (rc != PROCESS_DONE)
        return OVERFLOW_PUBLISHER;

    printf("Введите количество страниц: ");
    if (scanf("%d", &book.page_cnt) != 1)
        return READ_PAGES_FAILED;

    if (book.page_cnt <= 0 || book.page_cnt > 1800)
        return INVALID_CNT_PAGES;

    printf("Введите число типа литературы:\n"
           "0 - Техническая\n"
           "1 - Художественная\n"
           "2 - Детская\n");
    printf("Выбор: ");
    if (scanf("%ud\n", &book.type) != 1)
        return READ_TYPE_BOOK_FAILED;

    if (book.type != technical &&
            book.type != artistic &&
            book.type != childish)
        return INVALID_TYPE_BOOK;

    if (book.type == technical)
    {
        fgetc(stdin);

        do
        {
            printf("Введите отрасль (до %d символов): ", BRANCH_LEN);
            rc = read_line(stdin, book.details.technical.branch, BRANCH_LEN);

            if (rc == EMPTY_ERROR)
                printf("Введена пустая строка. Попробуйте еще раз\n");

        } while (rc == EMPTY_ERROR);

        if (rc != PROCESS_DONE)
            return OVERFLOW_BRANCH;

        printf("Введите национализированность книги:\n"
               "0 - Переводная\n"
               "1 - Отечественная\n");
        printf("Выбор: ");
        if (scanf("%d", &book.details.technical.isDomestic) != 1)
            return READ_NATIONAL_FAILED;

        if (book.details.technical.isDomestic != 0 &&
                book.details.technical.isDomestic != 1)
            return INVALID_NATIONAL;

        printf("Введите год издания: ");
        if (scanf("%d", &book.details.technical.year) != 1)
            return READ_YEAR_FAILED;

        if (book.details.technical.year < 1455 || book.details.technical.year > 2024)
            return INVALID_YEAR;
    }

    if (book.type == artistic)
    {
        printf("Введите тип худ. литературы:\n"
               "0 - Роман\n"
               "1 - Пьеса\n"
               "2 - Поэзия\n");
        printf("Выбор: ");
        if (scanf("%ud\n", &book.details.artistic.type) != 1)
            return READ_ART_TYPE_FAILED;

        if (book.details.artistic.type != 0 &&
                book.details.artistic.type != 1 &&
                book.details.artistic.type != 2)
            return INVALID_ART_TYPE;
    }

    if (book.type == childish)
    {
        printf("Введите минимальный возраст: ");
        if (scanf("%d", &book.details.childish.min_age) != 1)
            return READ_MIN_AGE_FAILED;

        if (book.details.childish.min_age < 0 || book.details.childish.min_age > 122)
            return INVALID_MIN_AGE;

        printf("Введите тип детской литературы:\n"
               "0 - стихи\n"
               "1 - сказки\n");
        printf("Выбор: ");
        if (scanf("%ud\n", &book.details.childish.type) != 1)
            return READ_CHILD_TYPE_FAILED;

        if (book.details.childish.type != 0 &&
                book.details.childish.type != 1)
            return INVALID_CHILD_TYPE;
    }

    book.ind = *len;
    books[*len] = book;
    books_key[*len].ind = *len;
    books_key[*len].page_cnt = book.page_cnt;
    (*len)++;

    puts("Книга добавлена успешно.\n");

    return PROCESS_DONE;
}

int del_book(book_t books[], book_key_t books_key[], size_t *len)
{
    int del_field;
    printf("Выберите поле для удаления записи:\n");
    printf("0 - Номер книги\n");
    printf("1 - Фамилия автора\n");
    printf("2 - Название книги\n");
    printf("3 - Издательство\n");
    printf("4 - Количество страниц\n");
    printf("5 - Вид литературы\n");
    printf("6 - Отрасль\n");
    printf("7 - Национализированность\n");
    printf("8 - Год издания\n");
    printf("9 - Тип худ. литературы\n");
    printf("10 - Минимальный возвраст\n");
    printf("11 - Тип детской литературы\n");
    printf("Выбор: ");

    if (scanf("%d", &del_field) != 1)
        return READ_DEL_FIELD_FAILED;

    if (del_field < 0 || del_field > 11)
        return INVALID_DEL_FIELD;

    int isFound = 0;
    int rc;
    switch (del_field)
    {
        case 0:
            printf("Введите номер книги (0 - %ld): ", *len - 1);

            int num = 0;
            if (scanf("%d", &num) != 1)
                return READ_NUM_BOOK_FAILED;

            if (num < 0 || num > (int) *len - 1)
                return INVALID_NUM_BOOK;

            for (size_t i = 0; i < *len; i++)
                if (books[i].ind == num)
                {
                    for (size_t j = i; j < *len - 1; j++)
                    {
                        books[j + 1].ind--;
                        books[j] = books[j + 1];
                    }

                    for (size_t j = i; j < *len - 1; j++)
                    {
                        books_key[j + 1].ind--;
                        books_key[j] = books_key[j + 1];
                    }

                    (*len)--;
                    printf("Запись, содержащая \"%d\" в поле \"Номер книги\", успешно удалена.\n\n", num);
                    isFound = 1;
                    break;
                }
            break;
        case 1:
            fgetc(stdin);
            char lastname_line[LASTNAME_LEN + 1];
            do
            {
                printf("Введите фамилию автора (до %d символов): ", LASTNAME_LEN);
                rc = read_line(stdin, lastname_line, LASTNAME_LEN);

                if (rc == EMPTY_ERROR)
                    printf("Введена пустая строка. Попробуйте еще раз\n");

            } while (rc == EMPTY_ERROR);

            if (rc != PROCESS_DONE)
                return OVERFLOW_LASTNAME;

            for (size_t i = 0; i < *len; i++)
                if (strcmp(books[i].lastname, lastname_line) == 0)
                {
                    for (size_t j = i; j < *len - 1; j++)
                    {
                        books[j + 1].ind--;
                        books[j] = books[j + 1];
                    }

                    for (size_t j = i; j < *len - 1; j++)
                    {
                        books_key[j + 1].ind--;
                        books_key[j] = books_key[j + 1];
                    }

                    (*len)--;
                    printf("Запись, содержащая \"%s\" в поле \"Фамилия автора\", успешно удалена.\n\n", lastname_line);
                    isFound = 1;
                    break;
                }
            break;
        case 2:
            fgetc(stdin);
            char name_line[NAME_LEN + 1];
            do
            {
                printf("Введите название книги (до %d символов): ", NAME_LEN);
                rc = read_line(stdin, name_line, NAME_LEN);

                if (rc == EMPTY_ERROR)
                    printf("Введена пустая строка. Попробуйте еще раз\n");

            } while (rc == EMPTY_ERROR);

            if (rc != PROCESS_DONE)
                return OVERFLOW_NAME;

            for (size_t i = 0; i < *len; i++)
                if (strcmp(books[i].name, name_line) == 0)
                {
                    for (size_t j = i; j < *len - 1; j++)
                    {
                        books[j + 1].ind--;
                        books[j] = books[j + 1];
                    }

                    for (size_t j = i; j < *len - 1; j++)
                    {
                        books_key[j + 1].ind--;
                        books_key[j] = books_key[j + 1];
                    }

                    (*len)--;
                    printf("Запись, содержащая \"%s\" в поле \"Название книги\", успешно удалена.\n\n", name_line);
                    isFound = 1;
                    break;
                }
            break;
        case 3:
            fgetc(stdin);
            char publisher_line[PUBLISHER_LEN + 1];
            do
            {
                printf("Введите издательство (до %d символов): ", PUBLISHER_LEN);
                rc = read_line(stdin, publisher_line, PUBLISHER_LEN);

                if (rc == EMPTY_ERROR)
                    printf("Введена пустая строка. Попробуйте еще раз\n");

            } while (rc == EMPTY_ERROR);

            if (rc != PROCESS_DONE)
                return OVERFLOW_PUBLISHER;

            for (size_t i = 0; i < *len; i++)
                if (strcmp(books[i].publisher, publisher_line) == 0)
                {
                    for (size_t j = i; j < *len - 1; j++)
                    {
                        books[j + 1].ind--;
                        books[j] = books[j + 1];
                    }

                    for (size_t j = i; j < *len - 1; j++)
                    {
                        books_key[j + 1].ind--;
                        books_key[j] = books_key[j + 1];
                    }

                    (*len)--;
                    printf("Запись, содержащая \"%s\" в поле \"Издательство\", успешно удалена.\n\n", publisher_line);
                    isFound = 1;
                    break;
                }
            break;
        case 4:
            printf("Введите количество страниц: ");
            int page_cnt = 0;
            if (scanf("%d", &page_cnt) != 1)
                return READ_PAGES_FAILED;

            if (page_cnt <= 0)
                return INVALID_CNT_PAGES;

            for (size_t i = 0; i < *len; i++)
                if (books[i].page_cnt == page_cnt)
                {
                    for (size_t j = i; j < *len - 1; j++)
                    {
                        books[j + 1].ind--;
                        books[j] = books[j + 1];
                    }

                    for (size_t j = i; j < *len - 1; j++)
                    {
                        books_key[j + 1].ind--;
                        books_key[j] = books_key[j + 1];
                    }

                    (*len)--;
                    printf("Запись, содержащая \"%d\" в поле \"Количество страниц\", успешно удалена.\n\n", page_cnt);
                    isFound = 1;
                    break;
                }
            break;
        case 5:
            printf("Введите число типа литературы:\n"
                   "0 - Техническая\n"
                   "1 - Художественная\n"
                   "2 - Детская\n");
            printf("Выбор: ");
            type_t type;
            if (scanf("%ud\n", &type) != 1)
                return READ_TYPE_BOOK_FAILED;

            if (type != technical &&
                    type != artistic &&
                    type != childish)
                return INVALID_TYPE_BOOK;

            for (size_t i = 0; i < *len; i++)
                if (books[i].type == type)
                {
                    for (size_t j = i; j < *len - 1; j++)
                    {
                        books[j + 1].ind--;
                        books[j] = books[j + 1];
                    }

                    for (size_t j = i; j < *len - 1; j++)
                    {
                        books_key[j + 1].ind--;
                        books_key[j] = books_key[j + 1];
                    }

                    (*len)--;
                    if (type == technical)
                        printf("Запись, содержащая \"Техническая\" в поле \"Тип литературы\", успешно удалена.\n\n");
                    else if (type == artistic)
                        printf("Запись, содержащая \"Художественная\" в поле \"Тип литературы\", успешно удалена.\n\n");
                    else if (type == childish)
                        printf("Запись, содержащая \"Детская\" в поле \"Тип литературы\", успешно удалена.\n\n");
                    isFound = 1;
                    break;
                }
            break;
        case 6:
            fgetc(stdin);
            char branch_line[BRANCH_LEN + 1];
            do
            {
                printf("Введите отрасль (до %d символов): ", BRANCH_LEN);
                rc = read_line(stdin, branch_line, BRANCH_LEN);

                if (rc == EMPTY_ERROR)
                    printf("Введена пустая строка. Попробуйте еще раз\n");

            } while (rc == EMPTY_ERROR);

            if (rc != PROCESS_DONE)
                return OVERFLOW_BRANCH;

            for (size_t i = 0; i < *len; i++)
                if (books[i].type == technical && strcmp(books[i].details.technical.branch, branch_line) == 0)
                {
                    for (size_t j = i; j < *len - 1; j++)
                    {
                        books[j + 1].ind--;
                        books[j] = books[j + 1];
                    }

                    for (size_t j = i; j < *len - 1; j++)
                    {
                        books_key[j + 1].ind--;
                        books_key[j] = books_key[j + 1];
                    }

                    (*len)--;
                    printf("Запись, содержащая \"%s\" в поле \"Отрасль\", успешно удалена.\n\n", branch_line);
                    isFound = 1;
                    break;
                }
            break;
        case 7:
            printf("Введите национализированность книги:\n"
                   "0 - Переводная\n"
                   "1 - Отечественная\n");
            printf("Выбор: ");
            int isDomestic = 0;
            if (scanf("%d", &isDomestic) != 1)
                return READ_NATIONAL_FAILED;

            if (isDomestic != 0 && isDomestic != 1)
                return INVALID_NATIONAL;

            for (size_t i = 0; i < *len; i++)
                if (books[i].type == technical && books[i].details.technical.isDomestic == isDomestic)
                {
                    for (size_t j = i; j < *len - 1; j++)
                    {
                        books[j + 1].ind--;
                        books[j] = books[j + 1];
                    }

                    for (size_t j = i; j < *len - 1; j++)
                    {
                        books_key[j + 1].ind--;
                        books_key[j] = books_key[j + 1];
                    }

                    (*len)--;
                    printf("Запись, содержащая \"%d\" в поле \"Национализированность\", успешно удалена.\n\n", isDomestic);
                    isFound = 1;
                    break;
                }
            break;
        case 8:
            printf("Введите год издания: ");
            int year = 0;
            if (scanf("%d", &year) != 1)
                return READ_YEAR_FAILED;

            if (year < 1455 || year > 2024)
                return INVALID_YEAR;

            for (size_t i = 0; i < *len; i++)
                if (books[i].type == technical && books[i].details.technical.year == year)
                {
                    for (size_t j = i; j < *len - 1; j++)
                    {
                        books[j + 1].ind--;
                        books[j] = books[j + 1];
                    }

                    for (size_t j = i; j < *len - 1; j++)
                    {
                        books_key[j + 1].ind--;
                        books_key[j] = books_key[j + 1];
                    }

                    (*len)--;
                    printf("Запись, содержащая \"%d\" в поле \"Год издания\", успешно удалена.\n\n", year);
                    isFound = 1;
                    break;
                }
            break;
        case 9:
            printf("Введите тип худ. литературы:\n"
                   "0 - Роман\n"
                   "1 - Пьеса\n"
                   "2 - Поэзия\n");
            printf("Выбор: ");
            art_type_t art_type;
            if (scanf("%ud\n", &art_type) != 1)
                return READ_ART_TYPE_FAILED;

            if (art_type != 0 && art_type != 1 && art_type != 2)
                return INVALID_ART_TYPE;

            for (size_t i = 0; i < *len; i++)
                if (books[i].type == artistic && books[i].details.artistic.type == art_type)
                {
                    for (size_t j = i; j < *len - 1; j++)
                    {
                        books[j + 1].ind--;
                        books[j] = books[j + 1];
                    }

                    for (size_t j = i; j < *len - 1; j++)
                    {
                        books_key[j + 1].ind--;
                        books_key[j] = books_key[j + 1];
                    }

                    (*len)--;
                    if (art_type == novel)
                        printf("Запись, содержащая \"Роман\" в поле \"Тип худ. литературы\", успешно удалена.\n\n");
                    else if (art_type == play)
                        printf("Запись, содержащая \"Пьеса\" в поле \"Тип худ. литературы\", успешно удалена.\n\n");
                    else if (art_type == poetry)
                        printf("Запись, содержащая \"Поэзия\" в поле \"Тип худ. литературы\", успешно удалена.\n\n");
                    isFound = 1;
                    break;
                }
            break;
        case 10:
            printf("Введите минимальный возраст: ");
            int min_age;
            if (scanf("%d", &min_age) != 1)
                return READ_MIN_AGE_FAILED;

            if (min_age < 0 || min_age > 122)
                return INVALID_MIN_AGE;

            for (size_t i = 0; i < *len; i++)
                if (books[i].type == childish && books[i].details.childish.min_age == min_age)
                {
                    for (size_t j = i; j < *len - 1; j++)
                    {
                        books[j + 1].ind--;
                        books[j] = books[j + 1];
                    }

                    for (size_t j = i; j < *len - 1; j++)
                    {
                        books_key[j + 1].ind--;
                        books_key[j] = books_key[j + 1];
                    }

                    (*len)--;
                    printf("Запись, содержащая \"%d\" в поле \"Минимальный возраст\", успешно удалена.\n\n", min_age);
                    isFound = 1;
                    break;
                }
            break;
        case 11:
            printf("Введите тип детской литературы:\n"
                   "0 - стихи\n"
                   "1 - сказки\n");
            printf("Выбор: ");
            child_type_t child_type;
            if (scanf("%ud\n", &child_type) != 1)
                return READ_CHILD_TYPE_FAILED;

            if (child_type != 0 && child_type != 1)
                return INVALID_CHILD_TYPE;

            for (size_t i = 0; i < *len; i++)
                if (books[i].type == childish && books[i].details.childish.type == child_type)
                {
                    for (size_t j = i; j < *len - 1; j++)
                    {
                        books[j + 1].ind--;
                        books[j] = books[j + 1];
                    }

                    for (size_t j = i; j < *len - 1; j++)
                    {
                        books_key[j + 1].ind--;
                        books_key[j] = books_key[j + 1];
                    }

                    (*len)--;
                    if (child_type == poems)
                        printf("Запись, содержащая \"Стихи\" в поле \"Тип детской литературы\", успешно удалена.\n\n");
                    else if (child_type == fairytales)
                        printf("Запись, содержащая \"Сказки\" в поле \"Тип детской литературы\", успешно удалена.\n\n");
                    isFound = 1;
                    break;
                }
            break;
        }

    if (!isFound)
        puts("Запись с указанным значением в поле не найдено.\n");

    return PROCESS_DONE;
}

void print_books_by_key(book_t books[], book_key_t books_key[], size_t len)
{
    printf("\n");
    printf("N  |Фамилия        |Название       |Издательство   |Cтр.|Тип лит. |Отрасль        |Нац|Год |Худ.лит.|Мин.возр.|Тип дет. лит.|\n");
    printf("---+---------------+---------------+---------------+----+---------+---------------+---+----+--------+---------+-------------|\n");
    for (size_t q = 0; q < len; q++)
    {
        size_t index = books_key[q].ind;

        printf("%-3d|%-*s|%-*s|%-*s|%-4d|%-9s|",
               books[index].ind,
               LASTNAME_LEN, books[index].lastname,
               NAME_LEN, books[index].name,
               PUBLISHER_LEN, books[index].publisher,
               books[index].page_cnt,
               (books[index].type == technical) ? "Technical" :
               (books[index].type == artistic) ? "Artistic" :
               (books[index].type == childish) ? "Childish" : ""
        );

        if (books[index].type == technical)
        {
            printf("%-*s|%-3s|%-4d|",
                   BRANCH_LEN, books[index].details.technical.branch,
                   books[index].details.technical.isDomestic ? "Yes" : "No",
                   books[index].details.technical.year);
        }
        else
            printf("%-*s|%-3s|%-4s|", BRANCH_LEN, "", "", "");

        if (books[index].type == artistic)
        {
            printf("%-8s|",
                   (books[index].details.artistic.type == novel) ? "Novel" :
                   (books[index].details.artistic.type == play) ? "Play" :
                   (books[index].details.artistic.type == poetry) ? "Poetry" : "");
        }
        else
            printf("%-8s|", "");

        if (books[index].type == childish)
        {
            printf("%-9d|%-13s|",
                   books[index].details.childish.min_age,
                   (books[index].details.childish.type == poems) ? "Poems" :
                   (books[index].details.childish.type == fairytales) ? "Fairytales" : "");
        }
        else
            printf("%-9s|%-13s|", "", "");

        printf("\n");
    }
    printf("\n");
}

int find_and_printf_novels_by_author(book_t books[], size_t len)
{
    int rc;
    int isFound = 0;

    fgetc(stdin);
    char lastname[LASTNAME_LEN + 1];
    do
    {
        printf("Введите фамилию автора (до %d символов): ", LASTNAME_LEN);
        rc = read_line(stdin, lastname, LASTNAME_LEN);

        if (rc == EMPTY_ERROR)
            printf("Введена пустая строка. Попробуйте еще раз\n");

    } while (rc == EMPTY_ERROR);

    if (rc != PROCESS_DONE)
        return OVERFLOW_LASTNAME;

    printf("\n");
    printf("N  |Фамилия        |Название       |Издательство   |Cтр.|Тип лит. |Худ.лит.|\n");
    printf("---+---------------+---------------+---------------+----+---------+--------|\n");
    for (size_t i = 0; i < len; i++)
        if (books[i].type == artistic && books[i].details.artistic.type == novel &&
                strcmp(books[i].lastname, lastname) == 0)
        {
            printf("%-3d|%-*s|%-*s|%-*s|%-4d|%-9s|%-8s|\n",
                   books[i].ind,
                   LASTNAME_LEN, books[i].lastname,
                   NAME_LEN, books[i].name,
                   PUBLISHER_LEN, books[i].publisher,
                   books[i].page_cnt,
                   (books[i].type == technical) ? "Technical" :
                   (books[i].type == artistic) ? "Artistic" :
                   (books[i].type == childish) ? "Childish" : "",
                   (books[i].details.artistic.type == novel) ? "Novel" :
                   (books[i].details.artistic.type == play) ? "Play" :
                   (books[i].details.artistic.type == poetry) ? "Poetry" : ""
            );
            isFound = 1;
        }

    printf("\n");
    if (!isFound)
        puts("Ничего не найдено\n");

    return PROCESS_DONE;
}
