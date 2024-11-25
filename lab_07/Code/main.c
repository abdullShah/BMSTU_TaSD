#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ui.h"
#include "tree.h"
#include "avl_tree.h"
#include "hash_table.h"
#include "measure.h"

enum errors
{
    SUCCESS,
    NO_DATA_ERROR,
    INPUT_COMMAND_ERR,
    INPUT_FOR_FIND_ERR,
    INPUT_FOR_INSERT_ERR,
    INPUT_FOR_REMOVE_ERR,
    INPUT_FOR_APPEND_TO_FILE_ERR,
};

static void cleanup(Node **rootTree, AVLNode **rootAVLTree, HashTable **hashTable)
{
    freeTree(rootTree);
    freeAVLTree(rootAVLTree);
    freeHashTable(hashTable);
}

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        printf("Не передан датасет\n");
        return NO_DATA_ERROR;
    }

    char *DATA = argv[1];

    Node *rootTree = NULL;
    AVLNode *rootAVLTree = NULL;
    HashTable *hashTable = createHashTable();

    int choice = 1;
    int attempt = 1;
    while (choice)
    {
        menu();
        printf("Выберите команду: ");

        if (scanf("%d", &choice) != 1)
        {
            printf("Неудачный ввод команды!\n");
            cleanup(&rootTree, &rootAVLTree, &hashTable);
            return INPUT_COMMAND_ERR;
        }

        int rc;
        int numberToSearch;
        int numberToAdd;
        int numberToDelete;
        int found = 0;
        int cmpCnt;
        switch (choice)
        {
            case 0:
                cleanup(&rootTree, &rootAVLTree, &hashTable);
                break;
            case 1:
                rc = readTreeByFile(DATA, &rootTree);

                if (rc != SUCCESS)
                {
                    cleanup(&rootTree, &rootAVLTree, &hashTable);
                    return rc;
                }

                printf("\nДанные из файла прочитаны успешно в дерево!\n\n");
                break;
            case 2:
                if (rootTree == NULL)
                {
                    puts("\nДерево пустое!\n");
                    break;
                }

                printf("\nДерево в виде графа:\n");
                printTree(rootTree, 0, '^');
                printf("\n");
                break;
            case 3:
                if (rootTree == NULL)
                {
                    puts("\nДерево пустое! Нечего искать.\n");
                    break;
                }

                printf("\nВведите число для поиска: ");
                if (scanf("%d", &numberToSearch) != 1)
                {
                    printf("Ошибка ввода числа для поиска!\n");
                    cleanup(&rootTree, &rootAVLTree, &hashTable);
                    return INPUT_FOR_FIND_ERR;
                }

                if (findInTree(rootTree, numberToSearch, &cmpCnt))
                    printf("\nЧисло %d найдено в дереве :-)\n\n", numberToSearch);
                else
                    printf("\nЧисло %d не найдено в дереве :-(\n\n", numberToSearch);

                break;
            case 4:
                printf("\nВведите число для добавления: ");

                if (scanf("%d", &numberToAdd) != 1)
                {
                    printf("Ошибка ввода числа для ввода!\n");
                    cleanup(&rootTree, &rootAVLTree, &hashTable);
                    return INPUT_FOR_INSERT_ERR;
                }

                if (findInTree(rootTree, numberToAdd, &cmpCnt))
                {
                    printf("\nЧисло %d уже в дереве!\n\n", numberToAdd);
                    break;
                }

                rootTree = insertInTree(rootTree, numberToAdd);
                if (rootTree == NULL)
                    printf("Ошибка: не удалось добавить число %d из-за недостатка памяти!\n", numberToAdd);
                else
                    printf("\nЧисло %d успешно добавлено в дерево!\n\n", numberToAdd);

                break;
            case 5:
                if (rootTree == NULL)
                {
                    puts("\nДерево пустое! Нечего удалять.\n");
                    break;
                }

                printf("\nВведите число для удаления: ");

                if (scanf("%d", &numberToDelete) != 1)
                {
                    printf("Ошибка ввода числа для удаления!\n");
                    cleanup(&rootTree, &rootAVLTree, &hashTable);
                    return INPUT_FOR_INSERT_ERR;
                }

                rootTree = deleteInTree(rootTree, numberToDelete, &found);

                if (!found)
                    printf("\nЭлемент %d не найден в дереве для удаления :-(\n\n", numberToDelete);
                else
                    printf("\nЧисло %d успешно удалено из дерева :-)\n\n", numberToDelete);

                break;
            case 6:
                rc = readAVLTreeByFile(DATA, &rootAVLTree);

                if (rc != SUCCESS)
                {
                    cleanup(&rootTree, &rootAVLTree, &hashTable);
                    return rc;
                }

                printf("\nДанные из файла прочитаны успешно в слабанс. дерево!\n\n");
                break;
            case 7:
                if (rootAVLTree == NULL)
                {
                    puts("\nСбаланс. дерево пустое!\n");
                    break;
                }

                printf("\nСбаланс. дерево в виде графа:\n");
                printAVLTree(rootAVLTree, 0, '^');
                printf("\n");
                break;
            case 8:
                if (rootAVLTree == NULL)
                {
                    puts("\nСбаланс. дерево пустое! Нечего искать.\n");
                    break;
                }

                printf("\nВведите число для поиска: ");
                if (scanf("%d", &numberToSearch) != 1)
                {
                    printf("Ошибка ввода числа для поиска!\n");
                    cleanup(&rootTree, &rootAVLTree, &hashTable);
                    return INPUT_FOR_FIND_ERR;
                }

                if (findInAVLTree(rootAVLTree, numberToSearch, &cmpCnt))
                    printf("\nЧисло %d найдено в сбаланс. дереве :-)\n\n", numberToSearch);
                else
                    printf("\nЧисло %d не найдено в сбаланс. дереве :-(\n\n", numberToSearch);

                break;
            case 9:
                printf("\nВведите число для добавления: ");

                if (scanf("%d", &numberToAdd) != 1)
                {
                    printf("Ошибка ввода числа для ввода!\n");
                    cleanup(&rootTree, &rootAVLTree, &hashTable);
                    return INPUT_FOR_INSERT_ERR;
                }

                if (findInAVLTree(rootAVLTree, numberToAdd, &cmpCnt))
                {
                    printf("\nЧисло %d уже в сбаланс. дереве!\n\n", numberToAdd);
                    break;
                }

                rootAVLTree = insertInAVLTree(rootAVLTree, numberToAdd);
                if (rootAVLTree == NULL)
                    printf("Ошибка: не удалось добавить число %d из-за недостатка памяти!\n", numberToAdd);
                else
                    printf("\nЧисло %d успешно добавлено в сбаланс. дерево!\n\n", numberToAdd);

                break;
            case 10:
                if (rootAVLTree == NULL)
                {
                    puts("\nСбаланс. дерево пустое! Нечего удалять.\n");
                    break;
                }

                printf("\nВведите число для удаления: ");

                if (scanf("%d", &numberToDelete) != 1)
                {
                    printf("Ошибка ввода числа для удаления!\n");
                    cleanup(&rootTree, &rootAVLTree, &hashTable);
                    return INPUT_FOR_INSERT_ERR;
                }

                rootAVLTree = deleteInAVLTree(rootAVLTree, numberToDelete, &found);

                if (!found)
                    printf("\nЭлемент %d не найден в сбаланс. дереве для удаления :-(\n\n", numberToDelete);
                else
                    printf("\nЧисло %d успешно удалено из сбаланс. дерева :-)\n\n", numberToDelete);

                break;
            case 11:
                rc = readHashTableByFile(DATA, hashTable);

                if (rc != SUCCESS)
                {
                    cleanup(&rootTree, &rootAVLTree, &hashTable);
                    return rc;
                }

                printf("\nДанные из файла прочитаны успешно в хеш-таблицу!\n\n");
                break;
            case 12:
                if (hashTable->count == 0)
                {
                    puts("\nХеш-таблица пуста!\n");
                    break;
                }

                printf("\nХеш-таблица:\n");
                printHashTable(hashTable);
                printf("\n");
                break;
            case 13:
                if (hashTable->count == 0)
                {
                    puts("\nХеш-таблица пуста! Нечего искать.\n");
                    break;
                }

                printf("\nВведите число для поиска: ");
                if (scanf("%d", &numberToSearch) != 1)
                {
                    printf("Ошибка ввода числа для поиска!\n");
                    cleanup(&rootTree, &rootAVLTree, &hashTable);
                    return INPUT_FOR_FIND_ERR;
                }

                if (findInHashTable(hashTable, numberToSearch, &cmpCnt))
                    printf("\nЧисло %d найдено в хеш-таблице :-)\n\n", numberToSearch);
                else
                    printf("\nЧисло %d не найдено в хеш-таблице :-(\n\n", numberToSearch);

                break;
            case 14:
                printf("\nВведите число для добавления: ");

                if (scanf("%d", &numberToAdd) != 1)
                {
                    printf("Ошибка ввода числа для ввода!\n");
                    cleanup(&rootTree, &rootAVLTree, &hashTable);
                    return INPUT_FOR_INSERT_ERR;
                }

                if (findInHashTable(hashTable, numberToAdd, &cmpCnt))
                {
                    printf("\nЧисло %d уже в хеш-таблице!\n\n", numberToAdd);
                    break;
                }

                if (insertInHashTable(hashTable, numberToAdd) != 0)
                    printf("Ошибка: не удалось добавить число %d из-за недостатка памяти!\n", numberToAdd);
                else
                    printf("\nЧисло %d успешно добавлено в хеш-таблицу!\n\n", numberToAdd);

                break;
            case 15:
                if (hashTable->count == 0)
                {
                    puts("\nХеш-таблица пуста! Нечего удалять.\n");
                    break;
                }

                printf("\nВведите число для удаления: ");

                if (scanf("%d", &numberToDelete) != 1)
                {
                    printf("Ошибка ввода числа для удаления!\n");
                    cleanup(&rootTree, &rootAVLTree, &hashTable);
                    return INPUT_FOR_INSERT_ERR;
                }

                deleteInHashTable(hashTable, numberToDelete, &found);

                if (!found)
                    printf("\nЭлемент %d не найден в хеш-таблице для удаления :-(\n\n", numberToDelete);
                else
                    printf("\nЧисло %d успешно удалено из хеш-таблицы :-)\n\n", numberToDelete);

                break;
            case 16:
                measureAverageFindTime(attempt++);
                break;
            default:
                puts("Нет такой команды! Повторите попытку\n");
                break;
        }
    }

    return SUCCESS;
}
