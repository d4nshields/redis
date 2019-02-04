
#include "server.h"
#include <heap.h>
#include <string.h>
#include "zmalloc.h"

bool inited = 0;
struct heap heaproot;

bool key_less_fn( 
    const struct heap_elem *a,
    const struct heap_elem *b,
    void * aux
  )
{
    return heap_entry( a, struct memokey_keydescriptor, elem)->when
        < heap_entry( b, struct memokey_keydescriptor, elem)->when;
}

void memokey_init()
{
    heap_init( &heaproot, key_less_fn);
    inited = 1;
}

void memokey_enqueue( redisDb *db, void *key, mstime_t when)
{
    if( !inited) {
        memokey_init();
    }
    
    struct memokey_keydescriptor * keydescr = (struct memokey_keydescriptor *)zcalloc( sizeof( struct memokey_keydescriptor));
    keydescr->db = db;
    keydescr->key = key;
    keydescr->when = when;
    heap_insert( &heaproot, &keydescr->elem, NULL);
    // do a peek to verify what we inserted
    struct heap_elem *e = heap_peek( &heaproot);
    if( e) {
        keydescr = heap_entry( e, struct memokey_keydescriptor, elem);
    }
}

/**
 * if the next element in queue is less than or equal to min, then it is ready to expire, 
 * pop it off.
 * @param min
 * @return 
 */
struct memokey_keydescriptor *memokey_dequeue( mstime_t min)
{
    if( !inited) {
        memokey_init();
    }
    struct heap_elem *e = heap_peek( &heaproot);
    if( e) {
        struct memokey_keydescriptor * keydescr = heap_entry( e, struct memokey_keydescriptor, elem);
        if( keydescr->when <= min) {
            e = heap_pop( &heaproot, NULL);
            keydescr = heap_entry( e, struct memokey_keydescriptor, elem);
            return keydescr;
        }
    }
    return NULL;
}

void memokey_free( struct memokey_keydescriptor * keydescr)
{
    zfree( keydescr);
}
