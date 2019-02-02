
#include "server.h"
#include <heap.h>
#include "zmalloc.h"

bool inited = 0;
struct heap heaproot;

bool key_less_fn( 
    const struct heap_elem *a,
    const struct heap_elem *b,
    void * aux
  )
{
    return heap_entry( a, struct key_expiry_memo, elem)->when
        < heap_entry( b, struct key_expiry_memo, elem)->when;
}

void initExpirationScheduler()
{
    heap_init( &heaproot, key_less_fn);
    inited = 1;
}

void enqueueKeyExpiration( redisDb *db, robj *key, mstime_t when)
{
    if( !inited) {
        initExpirationScheduler();
    }
    struct key_expiry_memo * memo = zmalloc( sizeof( struct key_expiry_memo));
    memo->db = db;
    memo->key = key;
    memo->when = when;
    heap_insert( &heaproot, &memo->elem, NULL);
}

/**
 * if the next element in queue is less than or equal to min, then it is ready to expire, 
 * pop it off.
 * @param min
 * @return 
 */
struct key_expiry_memo *dequeueNextExpired( mstime_t min)
{
    if( !inited) {
        initExpirationScheduler();
    }
    struct heap_elem *e = heap_peek( &heaproot);
    if( e) {
        struct key_expiry_memo * memo = heap_entry( e, struct key_expiry_memo, elem);
        if( memo->when <= min) {
            e = heap_pop( &heaproot, NULL);
            memo = heap_entry( e, struct key_expiry_memo, elem);
            return memo;
        }
    }
    return NULL;
}
