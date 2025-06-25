#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

#define BUFSIZE 1024
#define TEN_MILLION 10000000L

void *threadout(void *args)
{
    char buffer[BUFSIZE];
    char *c;
    struct timespec sleeptime;

    sleeptime.tv_sec = 0;
    sleeptime.tv_nsec = TEN_MILLION;

    snprintf(buffer, BUFSIZE, "This is a thread from process %ld\n",
             (long)getpid());
    c = buffer;

    while (*c != '\0')
    {
        fputc(*c, stderr);
        c++;
        nanosleep(&sleeptime, NULL);
    }

    return NULL;
}