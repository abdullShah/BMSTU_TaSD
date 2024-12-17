#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "graph.h"

void inputMatrixFromFile(int *n, graph_t graph, FILE *file)
{
    fscanf(file, "%d", n);
    for (int i = 0; i < *n; i++)
        for (int j = 0; j < *n; j++)
        {
            fscanf(file, "%d", &graph[i][j]);
            if (graph[i][j] == 0 && i != j)
                graph[i][j] = INF;
        }
}

int inputMatrixManually(int *n, graph_t graph)
{
    printf("Введите количество городов: ");
    if (scanf("%d", n) != 1 || *n <= 0)
    {
        printf("Ошибка ввода количества городов\n");
        return 0;
    }

    printf("Введите матрицу расстояний (%d x %d):\n", *n, *n);
    for (int i = 0; i < *n; i++)
        for (int j = 0; j < *n; j++)
        {
            if (scanf("%d", &graph[i][j]) != 1 || graph[i][j] < 0)
            {
                printf("Ошибка ввода расстояния\n");
                return 0;
            }

            if (graph[i][j] == 0 && i != j)
                graph[i][j] = INF;  // Отсутствие пути между разными городами
        }

    return 1;
}

int inputGraphByCoordinates(int *n, graph_t graph)
{
    printf("Введите количество городов: ");
    if (scanf("%d", n) != 1 || *n <= 0)
    {
        printf("Ошибка ввода количества городов\n");
        return 0;
    }

    for (int i = 0; i < *n; i++)
        for (int j = 0; j < *n; j++)
            graph[i][j] = ((i == j) ? 0 : INF);

    printf("Введите количество рёбер (1 - %d): ", *n);
    int edges;
    if (scanf("%d", &edges) != 1 || edges <= 0 || edges > *n)
    {
        printf("Ошибка ввода количества рёбер\n");
        return 0;
    }

    printf("Введите рёбра в формате: начальная_вершина (0 - %d) конечная_вершина (0 - %d) вес\n", *n, *n);
    for (int i = 0; i < edges; i++)
    {
        int u, v, weight;
        if (scanf("%d %d %d", &u, &v, &weight) != 3 || u < 0 || u >= *n || v < 0 || v >= *n || weight <= 0)
        {
            printf("Ошибка значений рёбер\n");
            return 0;
        }
        graph[u][v] = weight;
    }

    return 1;
}

void floydWarshall(int n, graph_t graph, graph_t dist)
{
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            dist[i][j] = graph[i][j];

    for (int k = 0; k < n; k++)
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                if (dist[i][k] != INF && dist[k][j] != INF && dist[i][j] > dist[i][k] + dist[k][j])
                    dist[i][j] = dist[i][k] + dist[k][j];
}

int findOptimalCity(int n, graph_t dist)
{
    int minSum = INF;
    int optimalCity = -1;

    for (int i = 0; i < n; i++)
    {
        int sum = 0;
        for (int j = 0; j < n; j++)
            if (dist[i][j] != INF)
                sum += dist[i][j];

        if (sum < minSum)
        {
            minSum = sum;
            optimalCity = i;
        }
    }

    return optimalCity;
}

void graphExportToDot(FILE *f, graph_t graph, int n)
{
    fprintf(f, "digraph G {\n");
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (graph[i][j] != INF && graph[i][j] != 0)
            {
                fprintf(f, "    %d -> %d [label=\"%d\"];\n", i, j, graph[i][j]);
            }
        }
    }
    fprintf(f, "}\n");
}
