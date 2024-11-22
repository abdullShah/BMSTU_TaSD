#ifndef TREE_H__
#define TREE_H__

#include <stdio.h>

enum statuses
{
    PROCESS_DONE,
    ALLOC_ERR,
    OPEN_FILE_FAILED,
};

typedef struct Node
{
    int value;
    struct Node *left;
    struct Node *right;
} Node;

void freeTree(Node **root);

Node *createNode(int value);

int find(Node *root, int value);

Node *insert(Node *root, int value);

Node *deleteNode(Node *root, int value, int *found);

void printTree(Node *root, size_t space, char dir);

void preOrder(Node *root);

void inOrder(Node *root);

void postOrder(Node *root);

void printNodesPerLevel(Node *root);

int readTreeByFile(char *filename, Node **root);

int appendNumberToFile(char *filename, int number);

#endif
