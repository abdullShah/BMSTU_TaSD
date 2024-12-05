#ifndef TREE_H__
#define TREE_H__

#include <stdio.h>

enum tree_statuses
{
    ALLOC_ERR_TREE,
    OPEN_FILE_FAILED_TREE,
};

typedef struct Node
{
    int value;
    struct Node *left;
    struct Node *right;
} Node;

void freeTree(Node **root);

int findInTree(Node *root, int value, int *cmpCnt);

Node *insertInTree(Node *root, int value);

Node *deleteInTree(Node *root, int value, int *found);

void printTree(Node *root, size_t space, char dir);

int readTreeByFile(char *filename, Node **root);

size_t getTreeMemory(Node *root);

#endif
