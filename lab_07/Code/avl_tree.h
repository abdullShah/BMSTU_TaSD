#ifndef AVL_TREE_H__
#define AVL_TREE_H__

#include <stdio.h>

enum avl_tree_statuses
{
    ALLOC_ERR_AVL_TREE,
    OPEN_FILE_FAILED_AVL_TREE,
};

typedef struct AVLNode
{
    int value;
    int height;
    struct AVLNode *left;
    struct AVLNode *right;
} AVLNode;

AVLNode *insertInAVLTree(AVLNode *root, int value);

AVLNode *deleteInAVLTree(AVLNode *root, int value, int *found);

int findInAVLTree(AVLNode *root, int value, int *cmpCnt);

void freeAVLTree(AVLNode **root);

void printAVLTree(AVLNode *root, size_t n, char dir);

int readAVLTreeByFile(char *filename, AVLNode **root);

size_t getAVLTreeMemory(AVLNode *root);

#endif
