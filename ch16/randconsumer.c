#include <math.h>
#include <pthread.h>
#include "buffer.h"
#include "../ch13/globalerror.h"
#include "../ch13/randsafe.h"
#include "../ch13/sharedsum.h"

static void *consumer(void *arg)
{
    int error;
    buffer_t nextitem;
    double value;

    for (;;)
    {
        if (error = getitem(&nextitem))
        {
            break;
        }

        value = sin(nextitem);

        if (error = add(value))
        {
            break;
        }
    }

    seterror(error);
    return NULL;
}

int initconsumer(pthread_t *tconsumer)
{
    int error;

    error = pthread_create(tconsumer, NULL, consumer, NULL);
    return (seterror(error));
}