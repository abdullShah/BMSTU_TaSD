#include <stdlib.h>
#include <stdio.h>
#include "hash_table.h"

// #define INIT_SIZE 1
// #define ALLOC_STEP 2

static unsigned int hash(int value, size_t size)
{
    return abs(value) % size;
}

static int is_prime(int n)
{
    if (n <= 1)
        return 0;

    for (int i = 2; i * i <= n; i++)

        if (n % i == 0)
            return 0;

    return 1;
}

static int next_prime(int n)
{
    n++;
    while (!is_prime(n))
        n++;

    return n;
}

HashTable *createHashTable(size_t init_size)
{
    size_t prime_size = next_prime(init_size);
    HashTable *table = (HashTable *) malloc(sizeof(HashTable));
    if (!table)
        return ALLOC_ERR_HT;

    table->size = prime_size;
    table->count = 0;
    table->buckets = (HashTableNode **) calloc(table->size, sizeof(HashTableNode *));
    if (!table->buckets)
    {
        free(table);
        return ALLOC_ERR_HT;
    }

    return table;
}

static int resizeHashTable(HashTable *table)
{
    size_t new_size = next_prime(table->size * 2);
    HashTableNode **new_buckets = (HashTableNode **) calloc(new_size, sizeof(HashTableNode *));
    if (!new_buckets)
        return 1;

    for (size_t i = 0; i < table->size; i++)
    {
        HashTableNode *current = table->buckets[i];
        while (current)
        {
            unsigned int new_index = hash(current->value, new_size);
            HashTableNode *next = current->next;
            current->next = new_buckets[new_index];
            new_buckets[new_index] = current;
            current = next;
        }
    }

    free(table->buckets);
    table->buckets = new_buckets;
    table->size = new_size;

    return 0;
}

int insertInHashTable(HashTable *table, int value)
{
    unsigned int index = hash(value, table->size);
    HashTableNode *current = table->buckets[index];
    size_t chain_length = 0;

    while (current)
    {
        chain_length++;
        current = current->next;
    }

    if (chain_length >= 3)
    {
        if (resizeHashTable(table) != 0)
            return ALLOC_ERR_HT;
        index = hash(value, table->size);
    }

    HashTableNode *new_node = (HashTableNode *) malloc(sizeof(HashTableNode));
    if (!new_node)
        return ALLOC_ERR_HT;

    new_node->value = value;
    new_node->next = table->buckets[index];
    table->buckets[index] = new_node;
    table->count++;

    return 0;
}

int findInHashTable(HashTable *table, int value, int *cmpCnt)
{
    unsigned int index = hash(value, table->size);
    HashTableNode *current = table->buckets[index];

    while (current)
    {
        (*cmpCnt)++;

        if (current->value == value)
            return 1;

        current = current->next;
    }
    return 0;
}

void deleteInHashTable(HashTable *table, int value, int *found)
{
    unsigned int index = hash(value, table->size);
    HashTableNode *current = table->buckets[index];
    HashTableNode *prev = NULL;

    *found = 0;
    while (current)
    {
        if (current->value == value)
        {
            *found = 1;

            if (prev)
                prev->next = current->next;
            else
                table->buckets[index] = current->next;

            free(current);
            table->count--;
            return;
        }

        prev = current;
        current = current->next;
    }
}

void freeHashTable(HashTable **table)
{
    if (table == NULL || *table == NULL)
        return;

    for (size_t i = 0; i < (*table)->size; i++)
    {
        HashTableNode *current = (*table)->buckets[i];
        while (current)
        {
            HashTableNode *temp = current;
            current = current->next;
            free(temp);
        }
    }

    free((*table)->buckets);
    (*table)->buckets = NULL;

    free(*table);
    *table = NULL;
}

void printHashTable(HashTable *table)
{
    for (size_t i = 0; i < table->size; i++)
    {
        printf("Контейнер %zu: ", i);
        HashTableNode *current = table->buckets[i];
        while (current)
        {
            printf("%d -> ", current->value);
            current = current->next;
        }
        printf("NULL\n");
    }
}

int readHashTableByFile(char *filename, HashTable *table)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("Ошибка открытия файла для чтения хэш-таблицы\n");
        return OPEN_FILE_FAILED_HT;
    }

    int number;
    while (fscanf(file, "%d", &number) == 1)
    {
        if (insertInHashTable(table, number) != 0)
        {
            printf("Ошибка аллокации памяти для узла хэш-таблицы\n");
            fclose(file);
            return ALLOC_ERR_HT;
        }
    }

    fclose(file);
    return 0;
}

size_t getHashTableMemory(HashTable *table)
{
    if (!table)
        return 0;

    // size_t memory = sizeof(HashTable) + table->size * sizeof(HashTableNode *);

    size_t memory = table->size * sizeof(HashTableNode) + sizeof(table->size) + sizeof(table->count);

    for (size_t i = 0; i < table->size; i++)
    {
        HashTableNode *current = table->buckets[i];
        while (current)
        {
            memory += sizeof(HashTableNode);
            current = current->next;
        }
    }

    return memory;
}
