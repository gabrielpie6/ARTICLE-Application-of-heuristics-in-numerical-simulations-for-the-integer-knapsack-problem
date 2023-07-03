#include "sort.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void QuickSort(Array array, Compare compare, int ini, int end)
{
    int i, j, pivotIndex;
    pivotIndex = floor((ini + end) / 2);
    Item pivot  = array[pivotIndex];

    for (i = ini, j = end; i < j;)
    {
        while (compare(array[i], pivot))
            i++;
        while (compare(pivot, array[j]))
            j--;
        if (i <= j)
        {
            Item aux = array[i];
            array[i] = array[j];
            array[j] = aux;
            i++;
            j--;
        }
    }
    if (ini < j)
        QuickSort(array, compare, ini, j);
    if (i < end)
        QuickSort(array, compare, i, end);
}