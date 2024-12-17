#ifndef MODEL_H__
#define MODEL_H__

#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAXLENQUEUE 1000000
#define EPS 1e-9
#define MAXLEN 10000
#define GHZ 1.9
#define ITER 10000

#define TRUE 1
#define FALSE 0

#define OK 0
#define INPUT_ERR 1
#define FILE_OPEN_ERR 2
#define FILE_READ_ERR 3
#define DONT_INT_NUM_CHOICE 4
#define INV_NUM_CHOICE 5

typedef char queuetype;

typedef struct queue {
  char name[30]; // Название очереди
  void *low;     // Адрес нижней границы
  void *up;      // Адрес верхней границы
  void *p_in;    // Указатель на последний элемент
  void *p_out; // Указатель на "первый на выход" элемент
  int count_len; // Кол-во элементов
  size_t size;   // Размер типа данных
  int count_req; // Кол-во запросов
  int sum_len;   // Суммарная длина очереди
  int tmp_len;   // Текущая длина очередь
  int sum_time;  // Общее время
  int out_req;   // Кол-во запросов на выход
  int in_req;    // Кол-во запросов на вход
} queue_t;

typedef struct node_t node_t;

struct node_t {
  char inf;     // Информационная часть
  node_t *next; // Указатель на следующий узел
};

// Структура временных рамок
typedef struct times_t {
  double min;
  double max;
} times_t;

void len_swow(queue_t *queue);

#endif
