#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "tree.h"

void freeTree(Node **root)
{
    if (root && *root)
    {
        freeTree(&((*root)->left));
        freeTree(&((*root)->right));
        free(*root);
        *root = NULL;
    }
}

static Node *createTreeNode(int value)
{
    Node *newNode = (Node *) malloc(sizeof(Node));

    if (newNode == NULL)
        return NULL;

    newNode->value = value;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

static Node *findMinTreeNode(Node *root)
{
    while (root->left != NULL)
        root = root->left;

    return root;
}

int findInTree(Node *root, int value, int *cmpCnt)
{
    if (root == NULL)
        return 0;

    (*cmpCnt)++;

    if (root->value == value)
        return 1;

    if (value < root->value)
        return findInTree(root->left, value, cmpCnt);
    else
        return findInTree(root->right, value, cmpCnt);
}

Node *insertInTree(Node *root, int value)
{
    if (root == NULL)
    {
        root = createTreeNode(value);
        if (root == NULL)
            return NULL;

        return root;
    }

    if (value > root->value)
        root->right = insertInTree(root->right, value);
    else if (value < root->value)
        root->left = insertInTree(root->left, value);

    return root;
}

Node *deleteInTree(Node *root, int value, int *found)
{
    if (root == NULL)
    {
        *found = 0;
        return NULL;
    }

    if (value < root->value)
        root->left = deleteInTree(root->left, value, found);
    else if (value > root->value)
        root->right = deleteInTree(root->right, value, found);
    else
    {
        *found = 1;

        if (root->left == NULL)
        {
            Node *temp = root->right;
            free(root);
            return temp;
        }
        else if (root->right == NULL)
        {
            Node *temp = root->left;
            free(root);
            return temp;
        }

        Node *temp = findMinTreeNode(root->right);
        root->value = temp->value;
        root->right = deleteInTree(root->right, temp->value, found);
    }

    return root;
}

#define DIST 5

void printTree(Node *root, size_t n, char dir)
{
    if (root)
    {
        printTree(root->right, n + DIST, '/');

        for (size_t i = 0; i < n; i++)
        {
            if (i % DIST == 0)
                printf("\x1b[90m|\x1b[0m");
            else
                printf(" ");
        }

        printf("%c%d\n", dir, root->value);

        printTree(root->left, n + DIST, '\\');
    }
}

int readTreeByFile(char *filename, Node **root)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("Ошибка открытия файла для чтения для дерева\n");
        return OPEN_FILE_FAILED_TREE;
    }

    int number;
    while (fscanf(file, "%d", &number) == 1)
    {
        *root = insertInTree(*root, number);
        if (*root == NULL)
        {
            printf("Ошибка аллокации памяти для узла дерева\n");
            fclose(file);
            return ALLOC_ERR_TREE;
        }
    }

    fclose(file);
    return 0;
}

int getTreeMemory(Node *root)
{
    if (!root)
        return 0;

    return sizeof(root->value) + sizeof(root->left) + sizeof(root->right) + getTreeMemory(root->left) +
           getTreeMemory(root->right);
}
