
/*
 * Basic producer-consumer buffer using a mutex for thread safety, 
 * but it lacks checks for full (in putitem) or empty (in getitem) buffer 
 * — may cause data loss or invalid reads.
 */

#include <pthread.h>
#include "buffer.h"

static buffer_t buffer[BUFSIZE];
static pthread_mutex_t bufferlock = PTHREAD_MUTEX_INITIALIZER;
static int bufin = 0;
static int bufout = 0;

int getitem(buffer_t *itemp)
{
    int error;

    if ((error = pthread_mutex_lock(&bufferlock)))
    {
        return error;
    }

    *itemp = buffer[bufout];
    bufout = (bufout + 1) % BUFSIZE;

    return pthread_mutex_unlock(&bufferlock);
}

int putitem(buffer_t item)
{
    int error;

    if ((error = pthread_mutex_lock(&bufferlock)))
    {
        return error;
    }

    buffer[bufin] = item;
    bufin = (bufin + 1) % BUFSIZE;

    return pthread_mutex_unlock(&bufferlock);
}