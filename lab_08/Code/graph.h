#ifndef GRAPH_H__
#define GRAPH_H__

#include <limits.h>

#define MAX_CITIES 100
#define INF INT_MAX

typedef int graph_t[MAX_CITIES][MAX_CITIES];

void inputMatrixFromFile(int *n, graph_t graph, FILE *file);

int inputMatrixManually(int *n, graph_t graph);

int inputGraphByCoordinates(int *n, graph_t graph);

void floydWarshall(int n, graph_t graph, graph_t dist);

int findOptimalCity(int n, graph_t dist);

void graphExportToDot(FILE *f, graph_t graph, int n);

#endif
