#ifndef LIST_SORT_H
#define LIST_SORT_H

#include "list.h"

// a function to sort two values
// return 1 if a should go before b
// return 0 if b should go before a
typedef int (*sort_fn)(void *a, void *b);

list list_sort(list l, sort_fn sorter);

#endif