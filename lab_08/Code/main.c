#include <stdio.h>
#include <stdlib.h>
#include "ui.h"
#include "graph.h"

enum errors
{
    SUCCESS,
    NO_DATA_ERROR,
    INPUT_COMMAND_ERR,
};

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        printf("Не передан датасет\n");
        return NO_DATA_ERROR;
    }

    char *DATA = argv[1];

    int n = 0;  // Количество городов
    graph_t graph;
    graph_t dist;

    int choice = 1;
    int isRead = 0;
    while (choice)
    {
        menu();
        printf("Выберите команду: ");

        if (scanf("%d", &choice) != 1)
        {
            printf("Неудачный ввод команды!\n");
            return INPUT_COMMAND_ERR;
        }

        switch (choice)
        {
            case 0:
                break;
            case 1:
                if (isRead)
                {
                    printf("\nДанные уже считаны\n\n");
                    break;
                }
                printf("Считывание из файла...\n\n");
                FILE *file_data = fopen(DATA, "r");
                if (!file_data)
                {
                    printf("Ошибка при открытии переданного файла для чтения\n\n");
                    break;
                }

                inputMatrixFromFile(&n, graph, file_data);
                fclose(file_data);

                isRead = 1;
                printf("Успешно считаны %d городов!\n\n", n);
                break;
            case 2:
                if (isRead)
                {
                    printf("\nДанные уже считаны\n\n");
                    break;
                }
                isRead = inputMatrixManually(&n, graph);
                if (isRead)
                    printf("Успешно считаны %d городов!\n\n", n);
                break;
            case 3:
                if (isRead)
                {
                    printf("\nДанные уже считаны\n\n");
                    break;
                }
                isRead = inputGraphByCoordinates(&n, graph);
                if (isRead)
                    printf("Успешно считаны %d городов!\n\n", n);
                break;
            case 4:
                if (!isRead)
                {
                    printf("\nДанные еще не считаны\n\n");
                    break;
                }

                FILE *file_dot = fopen("./graph.dot", "w");

                if (!file_dot)
                {
                    printf("Ошибка при открытии файла graph.dot для записи\n\n");
                    break;
                }

                graphExportToDot(file_dot, graph, n);
                fclose(file_dot);

                printf("\nГраф экспортирован в файл graph.dot\n\n");
                break;
            case 5:
                if (!isRead)
                {
                    printf("\nДанные еще не считаны\n\n");
                    break;
                }

                floydWarshall(n, graph, dist);
                int optimalCity = findOptimalCity(n, dist);

                printf("\nСуммы расстояний до остальных городов:\n");
                for (int i = 0; i < n; i++)
                {
                    int sum = 0;
                    for (int j = 0; j < n; j++)
                        if (dist[i][j] != INF)
                            sum += dist[i][j];

                    printf("Город %d: %d\n", i, sum);
                }

                printf("\nГород с минимальной суммой расстояний: %d\n\n", optimalCity);
                break;
            default:
                puts("Нет такой команды! Повторите попытку\n\n");
                break;
        }
    }

    return SUCCESS;
}
