/* 
 * File:   priorityqueue_test.c
 * Author: daniel
 *
 * Created on January 29, 2019, 10:52 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "priorityqueue.h"

void testInsert()
{
    const int num = 10000000;
    time_t t;
    struct heap h;
    
    /* Intializes random number generator */
    srand((unsigned) time(&t));
    
    priorityqueueInit( &h);
    int i;
    for( i=0; i < num; i++) {
        long long p = (long long)rand();
//        fprintf( stderr, "-> %lld\n", p);
        priorityqueueInsert( &h, (void *)0, p);
    }
    fprintf( stderr, "done inserts\n");
    while( 1) {
        long long min = priorityqueueMin( &h);
        if( min == -1)
            break;
//        fprintf( stdout, "%lld\n", min);
        priorityqueueDeleteMin( &h);
    }
    fprintf( stderr, "done deletes\n");
}


int main( int argc, char *argv[]) {

    testInsert();
    
    return (EXIT_SUCCESS);
}
