#ifndef SORTS_ALGORITHMS_H
#define SORTS_ALGORITHMS_H

#include <stdbool.h>

typedef void * Item;
typedef bool (*Compare)(Item A, Item B);
typedef Item * Array;

void QuickSort(Array array, Compare compare, int ini, int end);

#endif