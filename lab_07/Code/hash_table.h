#ifndef HASH_TABLE__
#define HASH_TABLE__

#include <stdlib.h>

enum hash_table_statuses
{
    ALLOC_ERR_HT,
    OPEN_FILE_FAILED_HT,
};

typedef struct HashTableNode
{
    int value;
    struct HashTableNode *next;
} HashTableNode;

typedef struct HashTable
{
    HashTableNode **buckets;
    size_t size;
    size_t count;
} HashTable;

HashTable *createHashTable(void);

int insertInHashTable(HashTable *table, int value);

int findInHashTable(HashTable *table, int value, int *cmpCnt);

void deleteInHashTable(HashTable *table, int value, int *found);

void freeHashTable(HashTable **table);

void printHashTable(HashTable *table);

int readHashTableByFile(char *filename, HashTable *table);

int getHashTableMemory(HashTable *table);

#endif
