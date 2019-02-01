
#include <heap.h>
#include "server.h"

struct item_wrapper
{
    long long priority;
    redisDb * db;
    robj * key;
    struct heap_elem elem;
};

struct heap heaproot;

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

void initExpirationScheduler()
{
    heap_init( &heaproot, wrapper_less);
}

void enqueueExpiration( redisDb *db, robj *key, long long when)
{
    struct item_wrapper * wrapper = malloc( sizeof( struct item_wrapper));
    wrapper->db = db;
    wrapper->key = key;
    heap_insert( &heaproot, &wrapper->elem, NULL);
}

/**
 * if root element of queue is greater than min, pop it off
 * @param min
 * @return 
 */
dequeueNextExpired( long long min)
{
    struct heap_elem *e = heap_peek( &heaproot);
    if( e) {
        struct item_wrapper * wrapper = heap_entry( e, struct item_wrapper, elem);
        if( wrapper->priority <= min) {
            e = heap_pop( &heaproot, NULL);
            wrapper = heap_entry( e, struct item_wrapper, elem);
            return wrapper;
        }
    }
    return NULL;
}
