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
    const int num = 1;
    time_t t;
    struct heap h;
    
    /* Intializes random number generator */
    srand((unsigned) time(&t));
    
    int i;
    for( i=0; i < num; i++) {
        fprintf( stdout, "filling:\n");
        long long p = (long long)rand();
        fprintf( stderr, "inserting %lld\n", p);
        priorityqueueInsert( &h, (void *)0, p);
        fprintf( stderr, "done\n");
    }
    for( i=0; i < num; i++) {
        fprintf( stdout, "draining:\n");
        long long min = priorityqueueMin( &h);
        fprintf( stderr, "deleting %lld\n", min);
        fprintf( stdout, "%lld", min);
        priorityqueueDeleteMin( &h);
        fprintf( stderr, "done\n");
    }
}


int main( int argc, char *argv[]) {

    testInsert();
    
    return (EXIT_SUCCESS);
}
