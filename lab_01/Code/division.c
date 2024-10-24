#include <stdlib.h>
#include <stdio.h> // !!
#include "division.h"

void printArray(int arr[], int size) // !!
{
    printf("ARRAY: ");

    for (int i = 0; i < size; i++)
        printf("%d ", arr[i]);

    printf("\n");
}

// Функция копирует элементы из одного массива в другой
void copy(int dest[], int src[], size_t len)
{
    for (size_t i = 0; i < len; i++)
        dest[i] = src[i];
}

// Функция проверяет, содержит ли массив только нули
int all_zeros(int arr[], int size)
{
    for (int i = 0; i < size; i++)
        if (arr[i] != 0)
            return 0;

    return 1;
}

// Функция уменьшает длину мантиссы, удаляя незначащие нули с конца
void correct_mantissa_len(normal_t *res)
{
    while (res->mantissa_len > 0 && res->mantissa[res->mantissa_len - 1] == 0)
        res->mantissa_len--;
}

// Функция выполняет сравнение двух массивов целых чисел, игнорируя ведущие нули
int compare_arrays(int arr1[], int len1, int arr2[], int len2)
{
    // Пропускаем ведущие нули в первом массиве
    int i = 0;
    while (i < len1 && arr1[i] == 0)
        i++;

    // Пропускаем ведущие нули во втором массиве
    int j = 0;
    while (j < len2 && arr2[j] == 0)
        j++;

    // Сравниваем оставшиеся длины массивов после удаления ведущих нулей
    int remaining_len1 = len1 - i;
    int remaining_len2 = len2 - j;

    if (remaining_len1 < remaining_len2)
        return -1;
    else if (remaining_len1 > remaining_len2)
        return 1;

    // Если длины совпадают, сравниваем элемент за элементом
    for (; i < len1 && j < len2; i++, j++)
    {
        if (arr1[i] < arr2[j])
            return -1;
        else if (arr1[i] > arr2[j])
            return 1;
    }

    return 0;
}

// Функция выполняет вычитание двух массивов чисел, представляющих целые числа поразрядно
void array_subtract(int res[], int *res_len, int arr1[], int len1, int arr2[], int len2)
{
    int i, j, k = 0, borrow = 0;
    int temp_res[MANTISSA_LEN] = {0};

    // Мы начинаем с конца массивов (как при вычитании "в столбик")
    i = len1 - 1;
    j = len2 - 1;

    while (i >= 0 || j >= 0)
    {
        int digit1 = (i >= 0) ? arr1[i] : 0;  // Если массив короче, используем 0
        int digit2 = (j >= 0) ? arr2[j] : 0;

        // Вычитание с учётом займа
        int sub = digit1 - digit2 - borrow;

        if (sub < 0)
        {
            sub += 10;  // Добавляем 10, если нужна "занять" десяток
            borrow = 1; // Переносим единицу в следующий разряд
        }
        else
            borrow = 0;

        temp_res[k++] = sub;
        i--;
        j--;
    }

    // Убираем лидирующие нули из результата
    while (k > 1 && temp_res[k - 1] == 0)
        k--;

    // Перевернем результат, так как мы шли от младших разрядов
    for (i = 0; i < k; i++)
        res[i] = temp_res[k - i - 1];

    *res_len = k;
}

// Функция выполняет деление, где делимое и делитель в виде массивов цифр
int quotient(int divisible[], int divisible_len, int divider[], int divider_len, int *isCompletely, int res[], int *res_len, int isFirstQuot)
{
    size_t cnt = 0;

    /*puts("["); // !
    printArray(res, *res_len);
    printArray(divider, divider_len);
    puts("]");*/

    if (!isFirstQuot && compare_arrays(res, *res_len, divider, divider_len) < 0)
        return cnt;

    // Первоначальное вычитание
    array_subtract(res, res_len, divisible, divisible_len, divider, divider_len);
    cnt++;

    /*puts("["); // !
    printArray(res, *res_len);
    printArray(divider, divider_len);
    puts("]");*/

    // Продолжаем вычитать, пока результат больше или равен делителю
    while (compare_arrays(res, *res_len, divider, divider_len) >= 0)
    {
        //printf("in while cnt = [%d]\n", (int) cnt); // !
        array_subtract(res, res_len, res, *res_len, divider, divider_len);
        cnt++;
    }

    // Проверяем, равен ли остаток нулю
    *isCompletely = (*res_len == 1 && res[0] == 0);

    // Возвращаем количество вычитаний как частное
    return cnt;
}

// Основная функция деления
int division(normal_t *res, normal_t num1, normal_t num2)
{
    if (all_zeros(num2.mantissa, MANTISSA_LEN))
        return DIVISION_BY_ZERO;

    if (abs(num1.order - num2.order) > LIMIT_ORDER)
        return OVERFLOW_ORDER;

    if (all_zeros(num1.mantissa, MANTISSA_LEN))
    {
        res->mantissa_sign = 0;
        res->mantissa[0] = 0;
        res->mantissa_len = 1;
        res->order = 0;

        return PROCESS_DONE;
    }

    res->mantissa_sign = num1.mantissa_sign ^ num2.mantissa_sign;
    res->order = num1.order - num2.order;

    // 1)
    size_t divisible_cur_len = 0;
    size_t q = 0;

    while (num1.mantissa[q] == num2.mantissa[q] && q < MANTISSA_LEN)
        q++;

    if (num1.mantissa[q] > num2.mantissa[q])
    {
        res->order++;
        divisible_cur_len = num2.mantissa_len;
    }
    else if (num1.mantissa[q] < num2.mantissa[q])
        divisible_cur_len = num2.mantissa_len + 1;
    else
    {
        res->order++;
        divisible_cur_len = num2.mantissa_len;
    }

    if (abs(res->order) > LIMIT_ORDER)
        return OVERFLOW_ORDER;

    // 2)
    int s_divisible[MANTISSA_LEN];
    copy(s_divisible, num1.mantissa, divisible_cur_len);

    int t;
    size_t res_ind = 0;
    int isCompletely = 0;
    int isLenEqual = 0;

    int q_res[MANTISSA_LEN];
    int q_res_len = 0;
    int isFirstQuot = 1;
    t = quotient(s_divisible, divisible_cur_len, num2.mantissa, num2.mantissa_len, &isCompletely, q_res, &q_res_len, isFirstQuot);
    isFirstQuot = 0;
    res->mantissa[res_ind] = t;
    //printf("t = [%d] q_res_len = [%d]\n", t, q_res_len); // !

    //printArray(q_res, q_res_len); // !
    //printf("isCompletely = %d res_ind = %ld mant_len = %ld\n", isCompletely, res_ind, num1.mantissa_len); // !
    if (isCompletely && res_ind + 1 == num1.mantissa_len)
        isLenEqual = 1;  // для 4 / 2

    //puts("----------------"); // !
    if (!isLenEqual)
    {
        res_ind++;
        while (res_ind < MANTISSA_LEN)
        {
            //puts("----------------");
            //printf("!!! divisible_cur_len = %ld\n", divisible_cur_len);
            //printf("num1.mantissa[] = [%d] len = [%ld]\n", num1.mantissa[divisible_cur_len - 1 + res_ind], divisible_cur_len - 1 + res_ind);

            //printf("?? q_res_len = [%d]\n", q_res_len); // !
            //printf("?? q_res[q_res_len - 1] = %d\n", q_res[q_res_len - 1]);
            q_res_len++;

            if (divisible_cur_len - 1 + res_ind < MANTISSA_LEN)
                q_res[q_res_len - 1] = num1.mantissa[divisible_cur_len - 1 + res_ind];
            else
            {
                //puts("Закончился"); // !
                q_res[q_res_len - 1] = 0;
            }

            //printf("q_res_len = [%d]\n", q_res_len); // !
            //printArray(q_res, q_res_len); // !

            if (all_zeros(q_res, q_res_len))
            {
                res->mantissa[res_ind++] = 0;
                q_res_len--;
                continue;
            }

            t = quotient(q_res, q_res_len, num2.mantissa, num2.mantissa_len, &isCompletely, q_res, &q_res_len, isFirstQuot);
            res->mantissa[res_ind++] = t;

            //printArray(q_res, q_res_len); // !
            //printf("t = [%d] q_res_len = [%d]\n", t, q_res_len); // !
        }
    }
    //printf("res_ind = %ld\n", res_ind); // !
    res->mantissa_len = res_ind + 1;

    if (!isLenEqual && res_ind == MANTISSA_LEN)
    {
        //puts("Еabs(num1.order - num2.order)ще нужно t"); // !
        q_res_len++;
        q_res[q_res_len - 1] = 0;

        if (!all_zeros(q_res, q_res_len))
        {
            t = quotient(q_res, q_res_len, num2.mantissa, num2.mantissa_len, &isCompletely, q_res, &q_res_len, isFirstQuot);

            //printf("new t = %d\n", t); // !
            if (t >= 5)
            {
                while (res->mantissa[res_ind - 1] + 1 == 10)
                {
                    res->mantissa[res_ind - 1] = 0;
                    res_ind--;
                }
                res->mantissa[res_ind - 1]++;
            }
        }

        res->mantissa_len = res_ind;
    }

    //puts("All in All"); // !
    //printf("= %ld\n", res_ind); // !
    //printArray(res->mantissa, res_ind); // !

    correct_mantissa_len(res);

    return PROCESS_DONE;
}
