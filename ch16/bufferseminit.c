// Thread-safe bounded buffer using mutex and semaphores for synchronization.
// Prevents busy waiting and ensures safe producer-consumer coordination.

#include <errno.h>
#include <pthread.h>
#include <semaphore.h>
#include "buffer.h"

static buffer_t buffer[BUFSIZE];

static pthread_mutex_t bufferlock = PTHREAD_MUTEX_INITIALIZER;
static int bufin = 0;
static int bufout = 0;
static sem_t semitems;
static sem_t semslots;

int bufferinit(void)
{
    int error;

    if (sem_init(&semitems, 0, 0))
    {
        return errno;
    }

    if (sem_init(&semslots, 0, BUFSIZE))
    {
        error = errno;
        sem_destroy(&semitems);
        return error;
    }

    return 0;
}

int getitem(buffer_t *itemp)
{
    int error;

    while (((error = sem_wait(&semitems)) == -1) && (errno == EINTR))
        ;

    if (error)
    {
        return errno;
    }

    if (error = pthread_mutex_lock(&bufferlock))
    {
        return error;
    }

    *itemp = buffer[bufout];
    bufout = (bufout + 1) % BUFSIZE;

    if (error = pthread_mutex_unlock(&bufferlock))
    {
        return error;
    }

    if (sem_post(&semslots) == -1)
    {
        return errno;
    }

    return 0;
}

int putitem(buffer_t item)
{
    int error;

    while (((error = sem_wait(&semslots)) == -1) && (errno == EINTR))
        ;

    if (error)
    {
        return errno;
    }

    if (error = pthread_mutex_lock(&bufferlock))
    {
        return error;
    }

    buffer[bufin] = item;
    bufin = (bufin + 1) % BUFSIZE;

    if (error = pthread_mutex_unlock(&bufferlock))
    {
        return error;
    }

    if (sem_post(&semitems) == -1)
    {
        return errno;
    }

    return 0;
}
