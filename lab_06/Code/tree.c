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

Node *createNode(int value)
{
    Node *newNode = (Node *) malloc(sizeof(Node));

    if (newNode == NULL)
        return NULL;

    newNode->value = value;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

static Node *findMinNode(Node *root)
{
    while (root->left != NULL)
        root = root->left;

    return root;
}

int find(Node *root, int value)
{
    if (root == NULL)
        return 0;

    if (root->value == value)
        return 1;

    if (value < root->value)
        return find(root->left, value);
    else
        return find(root->right, value);
}

Node *insert(Node *root, int value)
{
    if (root == NULL)
    {
        root = createNode(value);
        if (root == NULL)
            return NULL;

        return root;
    }

    if (value > root->value)
        root->right = insert(root->right, value);
    else if (value < root->value)
        root->left = insert(root->left, value);

    return root;
}

Node *deleteNode(Node *root, int value, int *found)
{
    if (root == NULL)
    {
        *found = 0;
        return NULL;
    }

    if (value < root->value)
        root->left = deleteNode(root->left, value, found);
    else if (value > root->value)
        root->right = deleteNode(root->right, value, found);
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

        Node *temp = findMinNode(root->right);
        root->value = temp->value;
        root->right = deleteNode(root->right, temp->value, found);
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

void preOrder(Node *root)
{
    if (root != NULL)
    {
        printf(" %d ", root->value);
        preOrder(root->left);
        preOrder(root->right);
    }
}

void inOrder(Node *root)
{
    if (root != NULL)
    {
        inOrder(root->left);
        printf(" %d ", root->value);
        inOrder(root->right);
    }
}

void postOrder(Node *root)
{
    if (root != NULL)
    {
        postOrder(root->left);
        postOrder(root->right);
        printf(" %d ", root->value);
    }
}

static void countNodesPerLevel(Node *root, int level, int *levels, int *maxLevel)
{
    if (root == NULL)
        return;

    levels[level]++;

    if (level > *maxLevel)
        *maxLevel = level;

    countNodesPerLevel(root->left, level + 1, levels, maxLevel);
    countNodesPerLevel(root->right, level + 1, levels, maxLevel);
}

void printNodesPerLevel(Node *root)
{
    int levels[100] = { 0 };
    int maxLevel = 0;

    countNodesPerLevel(root, 0, levels, &maxLevel);

    printf("\nКоличество узлов на каждом уровне:\n");
    for (int i = 0; i <= maxLevel; i++)
        printf("Уровень %d: %d узлов\n", i, levels[i]);

    printf("\n");
}

int readTreeByFile(char *filename, Node **root)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("Ошибка открытия файла для чтения\n");
        return OPEN_FILE_FAILED;
    }

    int number;
    while (fscanf(file, "%d", &number) == 1)
    {
        *root = insert(*root, number);
        if (*root == NULL)
        {
            printf("Ошибка аллокации памяти для узла\n");
            fclose(file);
            return ALLOC_ERR;
        }
    }

    fclose(file);
    return PROCESS_DONE;
}

// Функция записи дерева в файл (прямой обход)
static void writeTreeToFile(FILE *file, Node *root)
{
    if (root == NULL)
        return;

    fprintf(file, "%d\n", root->value);
    writeTreeToFile(file, root->left);
    writeTreeToFile(file, root->right);
}

static Node *loadTreeFromFile(FILE *file)
{
    Node *root = NULL;
    int value;

    while (fscanf(file, "%d", &value) == 1)
        root = insert(root, value);

    return root;
}

int appendNumberToFile(char *filename, int number)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("Ошибка открытия файля для записи!\n");
        return OPEN_FILE_FAILED;
    }

    Node *root = loadTreeFromFile(file);
    fclose(file);

    root = insert(root, number);

    file = fopen(filename, "w");
    if (file == NULL)
    {
        perror("Не удалось открыть файл для записи");
        freeTree(&root);
        return OPEN_FILE_FAILED;
    }

    writeTreeToFile(file, root);
    fclose(file);

    freeTree(&root);
    return PROCESS_DONE;
}
