
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

void memokey_enqueue( redisDb *db, robj *key, mstime_t when)
{
    serverLog( LL_DEBUG, "d4n: memokey_enqueue() key=%s", (char *)key->ptr);
    if( !inited) {
        memokey_init();
    }
    /* calloc enough space all at once to hold structures:
     *    1. struct memokey_keydescriptor 
     *    2. deep clone of robj, including strlen( key->ptr)+1 
     */
    unsigned char * mem_block = (unsigned char *)zcalloc( sizeof( struct memokey_keydescriptor)
                                + sizeof( robj)
                                + strlen( key->ptr)+1);
#define PTR_KEYDESCRIPTOR (struct memokey_keydescriptor *)(mem_block)
#define PTR_ROBJ (robj *)(mem_block+sizeof( struct memokey_keydescriptor))
#define PTR_KEY (char *)(mem_block+sizeof( struct memokey_keydescriptor)+sizeof(robj))
    
    struct memokey_keydescriptor * keydescr = PTR_KEYDESCRIPTOR;
    serverLog( LL_DEBUG, "d4n: memokey_enqueue(...) keydescr = %p", (void *)keydescr);
    keydescr->db = db;
    keydescr->key = PTR_ROBJ;
    memcpy( keydescr->key, key, sizeof( robj));
        char * clonedstr = PTR_KEY;
        strcpy( clonedstr, key->ptr);
    keydescr->key->ptr = clonedstr;
    keydescr->when = when;
    serverLog( LL_DEBUG, "d4n: ENQUEUING: heap_insert( &heaproot, %p);", (void *)&keydescr->elem);
    serverLog( LL_DEBUG, "d4n: ENQUEUING: key: %s", (char *)keydescr->key->ptr);
    serverLog( LL_DEBUG, "d4n: ENQUEUING: when: %lld", keydescr->when);
    heap_insert( &heaproot, &keydescr->elem, NULL);
    // do a peek to verify what we inserted
    struct heap_elem *e = heap_peek( &heaproot);
    if( e) {
        keydescr = heap_entry( e, struct memokey_keydescriptor, elem);
        serverLog( LL_DEBUG, "d4n: ENQUEUE PEEKING: key: %s", (char *)keydescr->key->ptr);
        serverLog( LL_DEBUG, "d4n: ENQUEUE PEEKING: when: %lld", keydescr->when);
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
    serverLog( LL_DEBUG, "d4n: PEEKING: key: %s", (char *)keydescr->key->ptr);
    serverLog( LL_DEBUG, "d4n: PEEKING: when: %lld", keydescr->when);
        if( keydescr->when <= min) {
            e = heap_pop( &heaproot, NULL);
            keydescr = heap_entry( e, struct memokey_keydescriptor, elem);
    serverLog( LL_DEBUG, "d4n: POPPING: key: %s", (char *)keydescr->key->ptr);
    serverLog( LL_DEBUG, "d4n: POPPING: when: %lld", keydescr->when);
            return keydescr;
        }
    }
    return NULL;
}

void memokey_free( struct memokey_keydescriptor * keydescr)
{
    serverLog( LL_DEBUG, "d4n: memokey_free(                   %p)", (void *)keydescr);
    zfree( keydescr->key->ptr);
    zfree( keydescr);
}
