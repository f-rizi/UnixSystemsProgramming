/*
 * Thread-safe producer-consumer buffer with capacity tracking.
 */

#include <errno.h>
#include <pthread.h>
#include "buffer.h"

static buffer_t buffer[BUFSIZE];

static pthread_mutex_t bufferlock = PTHREAD_MUTEX_INITIALIZER;
static int bufin = 0;
static int bufout = 0;
static int totalitems = 0;

int getitem(buffer_t *itemp)
{
    int error;
    int erroritem = 0;

    if (error = pthread_mutex_lock(&bufferlock))
    {
        return error;
    }

    if (totalitems > 0)
    {
        *itemp = buffer[bufout];
        bufout = (bufout + 1) % BUFSIZE;
        totalitems--;
    }
    else
    {
        erroritem = EAGAIN;
    }

    if (error = pthread_mutex_unlock(&bufferlock))
    {
        return error;
    }

    return erroritem;
}

int putitem(buffer_t item)
{
    int error;
    int erroritem = 0;

    if (error = pthread_mutex_lock(&bufferlock))
    {
        return error;
    }

    if (totalitems < BUFSIZE)
    {
        buffer[bufin] = item;
        bufin = (bufin + 1) % BUFSIZE;
        totalitems++;
    }
    else
    {
        erroritem = EAGAIN;
    }

    if (error = pthread_mutex_unlock(&bufferlock))
    {
        return error;
    }

    return erroritem;
}