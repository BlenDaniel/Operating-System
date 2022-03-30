/*
 * h2o/h2o.c --
 *
 *      A simple program to create water molecules by bonding two
 *      hydrogen and one oxygen atom.
 */

#define _POSIX_C_SOURCE 200809L

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <pthread.h>

typedef struct h2o {
    unsigned int hydrogen;
    unsigned int oxygen;
    unsigned int bonds;
    pthread_mutex_t mutex;
pthread_cond_t cond;
    /* add whatever you may need here */
} h2o_t;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"




/*
 * Wrapper functions that catch and report errors.
 */

static void mutex_lock(pthread_mutex_t *mutex)
{
    int rc = pthread_mutex_lock(mutex);
    if (rc) {
        fprintf(stderr, "pthread_mutex_lock(): %s\n", strerror(rc));
        exit(EXIT_FAILURE);
    }
}

static void mutex_unlock(pthread_mutex_t *mutex)
{
    int rc = pthread_mutex_unlock(mutex);
    if (rc) {
        fprintf(stderr, "pthread_mutex_unlock(): %s\n", strerror(rc));
        exit(EXIT_FAILURE);
    }
}

static void cond_wait(pthread_cond_t *cond, pthread_mutex_t *mutex)
{
    int rc = pthread_cond_wait(cond, mutex);
    if (rc) {
        fprintf(stderr, "pthread_cond_wait(): %s\n", strerror(rc));
        exit(EXIT_FAILURE);
    }
}

static void cond_broadcast(pthread_cond_t *cond)
{
    int rc = pthread_cond_broadcast(cond);
    if (rc) {
        fprintf(stderr, "pthread_cond_broadcast(): %s\n", strerror(rc));
        exit(EXIT_FAILURE);
    }
}
#pragma GCC diagnostic pop

/*
 * The oxygen thread function.
 */

static void* oxygen(void *data)
{
    
    h2o_t *h2o = (h2o_t *) data;
    mutex_lock(&h2o -> mutex);
    while(cond){
        (void) h2o;
        /*
        */

    }
    (void) h2o;         /* do something more useful here */
    mutex_unlock(&h2o -> mutex);
    return NULL;
}

/*
 * The hydrogen threat function.
 */

static void* hydrogen(void *data)
{
    h2o_t *h2o = (h2o_t *) data;

    mutex_lock(&h2o ->mutex);
    while(cond){
       (void) h2o; 
       /*
        */

    }
             /* do something more useful here */
    mutex_unlock(&h2o -> mutex);
    return NULL;
}

int main(int argc, char *argv[])
{
    int rc;
    int num = 3 * (argc-1);
    pthread_t tids[num];
    h2o_t h2o = {
        .hydrogen = 0,
        .oxygen = 0,
        .bonds = 0,
    };

    (void) argv;
    for (int i = 0; i < num; i++) {
        rc = pthread_create(&tids[i], NULL,
                            (i % 3 == 0) ? oxygen : hydrogen, &h2o);
        if (rc) {
            fprintf(stderr, "pthread_create(): %s\n", strerror(rc));
        }
    }

    for (int i = 0; i < num; i++) {
        if (tids[i]) {
            rc = pthread_join(tids[i], NULL);
            if (rc) {
                fprintf(stderr, "pthread_join(): %s\n", strerror(rc));
            }
        }
    }

    assert(h2o.bonds == num / 3);
    return EXIT_SUCCESS;
}
