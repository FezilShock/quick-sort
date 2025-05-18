#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>

void swap(int *a, int *b)
{
    int t = *a;
    *a = *b;
    *b = t;
}

int partition(int *arr, int l, int r)
{
    int pivot  = arr[r];
    int i = l - 1;
    for(int j = l; j <= r - 1; j++)
    {
        if(arr[j] < pivot)
        {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }

    swap(&arr[i + 1], &arr[r]);
    return i + 1;
}

void quicksort (int *arr, int l, int r)
{
    if (l < r)
    {
        int q = partition(arr, l, r);
        quicksort(arr, l, q - 1);
        quicksort(arr, q + 1, r);
    } 
}

int *createArr(int size)
{
    int i;
    int *arr = malloc(sizeof(int) * size);
    for(i = 0; i < size; i++)
        arr[i] = rand() % 100;
    return arr;
}

double gettime(int *arr, int size)
{
    clock_t start, end;
    double cpu_time_used;
    start = clock();
    quicksort(arr, 0, size - 1);
    free(arr);
    end = clock();
    return ((double) (end - start)) / CLOCKS_PER_SEC;
}

int main(void)
{   
    int i;
    int size;
    srand(time(NULL));
    for (i = 0; i < 45; i++)
    {   
        size = (i * 16000);
        printf("Результат теста на %d элементов\n", size);
        printf("%f\n", gettime(createArr(size), size));
    }

    return 0;
}