
#ifndef __PRIORITYQUEUE_H
#define __PRIORITYQUEUE_H

#include "heap.h"

struct item_wrapper
{
    long long priority;
    void * item;
    struct heap_elem elem;
};

void priorityqueueInsert( 
    struct heap  *h, 
    void *item, 
    long long itemPriority
  );

void * priorityqueueDeleteMin(
    struct heap  *h
  );

long long priorityqueueMin(
    struct heap  *h
  );

#endif
