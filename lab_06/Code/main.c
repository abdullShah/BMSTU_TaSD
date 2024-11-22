#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ui.h"
#include "tree.h"
#include "measure.h"

enum errors
{
    SUCCESS,
    NO_DATA_ERROR,
    INPUT_COMMAND_ERR,
    INPUT_FOR_FIND_ERR,
    INPUT_FOR_INSERT_ERR,
    INPUT_FOR_REMOVE_ERR,
    INPUT_FOR_APPEND_TO_FILE_ERR
};

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        printf("Не передан датасет\n");
        return NO_DATA_ERROR;
    }

    char *DATA = argv[1];
    int isRead = 0;

    Node *root = NULL;

    int rc;
    int choice = 1;
    while (choice)
    {
        menu();
        printf("Выберите команду: ");

        if (scanf("%d", &choice) != 1)
        {
            printf("Неудачный ввод команды!\n");
            freeTree(&root);
            return INPUT_COMMAND_ERR;
        }

        switch (choice)
        {
            case 0:
                freeTree(&root);
                break;
            case 1:
                if (isRead)
                {
                    puts("\nФайл уже был считан\n");
                    break;
                }

                rc = readTreeByFile(DATA, &root);

                if (rc != SUCCESS)
                {
                    freeTree(&root);
                    return rc;
                }

                isRead = 1;
                printf("\nДанные из файла прочитаны успешно!\n\n");
                break;
            case 2:
                if (root == NULL)
                {
                    puts("\nДерево пустое!\n");
                    break;
                }

                printf("\nДерево в виде графа:\n");
                printTree(root, 0, '^');
                printf("\n");
                break;
            case 3:
                if (root == NULL)
                {
                    puts("\nДерево пустое! Нечего искать.\n");
                    break;
                }

                int numberToSearch;
                printf("\nВведите число для поиска: ");
                if (scanf("%d", &numberToSearch) != 1)
                {
                    printf("Ошибка ввода числа для поиска!\n");
                    return INPUT_FOR_FIND_ERR;
                }

                if (find(root, numberToSearch))
                    printf("\nЧисло %d найдено в дереве :-)\n\n", numberToSearch);
                else
                    printf("\nЧисло %d не найдено в дереве :-(\n\n", numberToSearch);

                break;
            case 4:
                printf("\nВведите число для добавления: ");

                int numberToAdd;
                if (scanf("%d", &numberToAdd) != 1)
                {
                    printf("Ошибка ввода числа для ввода!\n");
                    return INPUT_FOR_INSERT_ERR;
                }

                if (find(root, numberToAdd))
                {
                    printf("\nЧисло %d уже в дереве!\n\n", numberToAdd);
                    break;
                }

                root = insert(root, numberToAdd);
                if (root == NULL)
                    printf("Ошибка: не удалось добавить число %d из-за недостатка памяти!\n", numberToAdd);
                else
                    printf("\nЧисло %d успешно добавлено в дерево!\n\n", numberToAdd);

                break;
            case 5:
                if (root == NULL)
                {
                    puts("\nДерево пустое! Нечего удалять.\n");
                    break;
                }

                printf("\nВведите число для удаления: ");

                int numberToDelete;
                if (scanf("%d", &numberToDelete) != 1)
                {
                    printf("Ошибка ввода числа для удаления!\n");
                    return INPUT_FOR_INSERT_ERR;
                }

                int found = 0;
                root = deleteNode(root, numberToDelete, &found);

                if (!found)
                    printf("\nЭлемент %d не найден в дереве для удаления :-(\n\n", numberToDelete);
                else
                    printf("\nЧисло %d успешно удалено из дерева :-)\n\n", numberToDelete);

                break;
            case 6:
                if (root == NULL)
                {
                    puts("\nДерево пустое!\n");
                    break;
                }

                printf("\nДерево в постордерном обходе:\n");
                preOrder(root);
                printf("\n\n");
                break;
            case 7:
                if (root == NULL)
                {
                    puts("\nДерево пустое!\n");
                    break;
                }

                printf("\nДерево в инордерном обходе:\n");
                inOrder(root);
                printf("\n\n");
                break;
            case 8:
                if (root == NULL)
                {
                    puts("\nДерево пустое!\n");
                    break;
                }

                printf("\nДерево в преордерном обходе:\n");
                postOrder(root);
                printf("\n\n");
                break;
            case 9:
                if (root == NULL)
                {
                    puts("\nДерево пустое! Нечего считать.\n");
                    break;
                }

                printNodesPerLevel(root);
                break;
            case 10:
                printf("\nВведите число для добавления в файл: ");

                int numberToAppend;
                if (scanf("%d", &numberToAppend) != 1)
                {
                    printf("Ошибка ввода числа для записи в файл!\n");
                    return INPUT_FOR_APPEND_TO_FILE_ERR;
                }

                rc = appendNumberToFile(DATA, numberToAppend);

                if (rc == SUCCESS)
                    printf("\nЧисло %d успешно добавлено в файл %s\n\n", numberToAppend, DATA);
                else
                    printf("\nНе удалось добавить число в файл %s\n\n", DATA);

                break;
            case 11:
                printf("\nЗапускается замер времени добавления в дерево и файл...\n");
                measureAverageInsertTime();
                break;
        }
    }

    freeTree(&root);
    return SUCCESS;
}
