#include <errno.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <unistd.h>

#define BUFSIZE 1024
#define TEN_MILLION 10000000L

void *threadout(void *args)
{
    char buffer[BUFSIZE];
    char *c;
    struct timespec sleeptime;
    sem_t *semlockp;

    sleeptime.tv_sec = 0;
    sleeptime.tv_nsec = TEN_MILLION;
    semlockp = (sem_t *)args;

    snprintf(buffer, BUFSIZE, "This is a thread from process %ld.\n",
             (long)getpid());
    c = buffer;

    while (sem_wait(semlockp) == -1)
    {
        if (errno != EINTR)
        {
            fprintf(stderr, "Thread failed to lock semaphore.\n");
            return NULL;
        }
    }

    while (*c != '\0')
    {
        fputc(*c, stderr);
        c++;
        nanosleep(&sleeptime, NULL);
    }

    if (sem_post(semlockp) == -1)
    {
        fprintf(stderr, "Thread failed to unlock semaphore\n");
    }

    return NULL;
}