#include "model.h"

void len_swow(queue_t *queue)
{
    printf("Текущая длина очереди '%s': %d\n", queue->name, queue->tmp_len);
    if (queue->count_req > 0)
    {
        printf("Средняя длина очереди '%s': %lf\n", queue->name, (double) queue->sum_len / (double) queue->count_req);
    }
}
