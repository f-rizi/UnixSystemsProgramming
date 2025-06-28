#include <errno.h>
#include <pthread.h>
#include "buffer.h"
#include "../ch13/globalerror.h"
#include "../ch13/randsafe.h"

int getdone(int *flag);

static void *producer(void *arg1)
{
    int error;
    buffer_t item;
    int localdone = 0;

    while (!localdone)
    {
        if (error = randsafe(&item))
        {
            break;
        }

        if (error = putitem(item))
        {
            break;
        }

        if (error = getdone(&localdone))
        {
            break;
        }
    }

    if (error != ECANCELED)
    {
        seterror(error);
    }

    return NULL;
}

int initproducer(pthread_t *tproducer)
{
    int error;

    error = pthread_create(tproducer, NULL, producer, NULL);

    return seterror(error);
}