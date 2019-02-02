
#include <stdio.h>
#include <malloc.h>
#include <sys/time.h>

#include "priorityqueue.h"
#include "heap.h"

bool wrapper_less( 
    const struct heap_elem *a,
    const struct heap_elem *b,
    void * aux
  )
{
    if( aux != aux) {
      ;
    }
    return heap_entry( a, struct item_wrapper, elem)->priority
        < heap_entry( b, struct item_wrapper, elem)->priority;
}


void priorityqueueInit( struct heap *h)
{
    heap_init( h, wrapper_less);
}

/**
 * Insert pointer to data into the priority queue within h
 * at priority itemPriority
 */
void priorityqueueInsert( 
    struct heap *h, 
    void *item, 
    long long itemPriority
  )
{
    struct item_wrapper * wrapper = malloc( sizeof( struct item_wrapper));
    wrapper->priority = itemPriority;
    wrapper->item = item;
    
    heap_insert( h, &wrapper->elem, NULL);
}

void *priorityqueueDeleteMin(
    struct heap *h
  )
{
    struct heap_elem *e = heap_pop( h, NULL);
    struct item_wrapper *wrapper = heap_entry( e, struct item_wrapper, elem);
    void * item = wrapper->item;
    free( wrapper);
    return item;
}

long long priorityqueueMin(
    struct heap *h
  )
{
    struct heap_elem *e = heap_peek( h);
    if( e) {
        struct item_wrapper *wrapper = heap_entry( e, struct item_wrapper, elem);
        return wrapper->priority;
    } else {
        return -1;
    }
}
