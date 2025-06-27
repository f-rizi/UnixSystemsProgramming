#include <pthread.h>
#include "buffer.h"
#include "../ch13/globalerror.h"
#include "../ch13/randsafe.h"

static void *producer(void *arg1)
{
    int error;
    buffer_t item;

    for (;;)
    {
        if (error = randsafe(&item))
        {
            break;
        }

        if (error = putitem(item))
        {
            break;
        }
    }

    seterror(error);
    return NULL;
}

int initproducer(pthread_t *tproducer)
{
    int error;

    error = pthread_create(tproducer, NULL, producer, NULL);
    return (seterror(error));
}
