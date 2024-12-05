#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "avl_tree.h"

static int getHeight(AVLNode *node)
{
    if (node == NULL)
        return 0;
    return node->height;
}

static int getBalance(AVLNode *node)
{
    if (node == NULL)
        return 0;
    return getHeight(node->left) - getHeight(node->right);
}

static AVLNode *createAVLTreeNode(int value)
{
    AVLNode *newNode = (AVLNode *) malloc(sizeof(AVLNode));
    if (newNode == NULL)
        return NULL;

    newNode->value = value;
    newNode->left = NULL;
    newNode->right = NULL;
    newNode->height = 1;
    return newNode;
}

static AVLNode *rotateRight(AVLNode *y)
{
    AVLNode *x = y->left;
    AVLNode *T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = 1 + (getHeight(y->left) > getHeight(y->right) ? getHeight(y->left) : getHeight(y->right));
    x->height = 1 + (getHeight(x->left) > getHeight(x->right) ? getHeight(x->left) : getHeight(x->right));

    return x;
}

static AVLNode *rotateLeft(AVLNode *x)
{
    AVLNode *y = x->right;
    AVLNode *T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = 1 + (getHeight(x->left) > getHeight(x->right) ? getHeight(x->left) : getHeight(x->right));
    y->height = 1 + (getHeight(y->left) > getHeight(y->right) ? getHeight(y->left) : getHeight(y->right));

    return y;
}

AVLNode *insertInAVLTree(AVLNode *root, int value)
{
    if (root == NULL)
        return createAVLTreeNode(value);

    if (value < root->value)
        root->left = insertInAVLTree(root->left, value);
    else if (value > root->value)
        root->right = insertInAVLTree(root->right, value);
    else
        return root;

    root->height =
        1 + (getHeight(root->left) > getHeight(root->right) ? getHeight(root->left) : getHeight(root->right));

    int balance = getBalance(root);

    if (balance > 1 && value < root->left->value)
        return rotateRight(root);

    if (balance < -1 && value > root->right->value)
        return rotateLeft(root);

    if (balance > 1 && value > root->left->value)
    {
        root->left = rotateLeft(root->left);
        return rotateRight(root);
    }

    if (balance < -1 && value < root->right->value)
    {
        root->right = rotateRight(root->right);
        return rotateLeft(root);
    }

    return root;
}

static AVLNode *findMinAVLTreeNode(AVLNode *root)
{
    while (root->left != NULL)
        root = root->left;
    return root;
}

AVLNode *deleteInAVLTree(AVLNode *root, int value, int *found)
{
    if (root == NULL)
    {
        *found = 0;
        return NULL;
    }

    if (value < root->value)
        root->left = deleteInAVLTree(root->left, value, found);
    else if (value > root->value)
        root->right = deleteInAVLTree(root->right, value, found);
    else
    {
        *found = 1;

        if (root->left == NULL || root->right == NULL)
        {
            AVLNode *temp = root->left ? root->left : root->right;

            if (temp == NULL)
            {
                temp = root;
                root = NULL;
            }
            else
                *root = *temp;

            free(temp);
        }
        else
        {
            AVLNode *temp = findMinAVLTreeNode(root->right);
            root->value = temp->value;
            root->right = deleteInAVLTree(root->right, temp->value, found);
        }
    }

    if (root == NULL)
        return root;

    root->height =
        1 + (getHeight(root->left) > getHeight(root->right) ? getHeight(root->left) : getHeight(root->right));

    int balance = getBalance(root);

    if (balance > 1 && getBalance(root->left) >= 0)
        return rotateRight(root);

    if (balance > 1 && getBalance(root->left) < 0)
    {
        root->left = rotateLeft(root->left);
        return rotateRight(root);
    }

    if (balance < -1 && getBalance(root->right) <= 0)
        return rotateLeft(root);

    if (balance < -1 && getBalance(root->right) > 0)
    {
        root->right = rotateRight(root->right);
        return rotateLeft(root);
    }

    return root;
}

int findInAVLTree(AVLNode *root, int value, int *cmpCnt)
{
    if (root == NULL)
        return 0;

    (*cmpCnt)++;

    if (value == root->value)
        return 1;

    if (value < root->value)
        return findInAVLTree(root->left, value, cmpCnt);
    else
        return findInAVLTree(root->right, value, cmpCnt);
}

void freeAVLTree(AVLNode **root)
{
    if (root && *root)
    {
        freeAVLTree(&((*root)->left));
        freeAVLTree(&((*root)->right));
        free(*root);
        *root = NULL;
    }
}

#define DIST 5

void printAVLTree(AVLNode *root, size_t n, char dir)
{
    if (root)
    {
        printAVLTree(root->right, n + DIST, '/');

        for (size_t i = 0; i < n; i++)
        {
            if (i % DIST == 0)
                printf("\x1b[90m|\x1b[0m");
            else
                printf(" ");
        }

        printf("%c%d\n", dir, root->value);

        printAVLTree(root->left, n + DIST, '\\');
    }
}

int readAVLTreeByFile(char *filename, AVLNode **root)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("Ошибка открытия файла для чтения для сбалансированного дерева\n");
        return OPEN_FILE_FAILED_AVL_TREE;
    }

    int number;
    while (fscanf(file, "%d", &number) == 1)
    {
        *root = insertInAVLTree(*root, number);
        if (*root == NULL)
        {
            printf("Ошибка аллокации памяти для узла сбалансированного дерева\n");
            fclose(file);
            return ALLOC_ERR_AVL_TREE;
        }
    }

    fclose(file);
    return 0;
}

size_t getAVLTreeMemory(AVLNode *root)
{
    if (!root)
        return 0;

    return sizeof(int) + sizeof(int) + sizeof(AVLNode) + sizeof(AVLNode) + getAVLTreeMemory(root->left) +
           getAVLTreeMemory(root->right);
}
