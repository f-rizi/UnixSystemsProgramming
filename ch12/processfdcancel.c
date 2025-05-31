#include <pthread.h>
#include "../appendixB/restart.h"

#define BUFSIZE 1024

void docommand(char *cmd, int cmdsize);

void *processfdcancel(void *arg)
{
    char buff[BUFSIZE];
    int fd;
    ssize_t nbytes;
    int newstate, oldstate;

    fd = *((int *)(arg));

    for (;;)
    {
        if ((nbytes = r_read(fd, buff, BUFSIZE)) <= 0)
        {
            break;
        }

        if (pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, &oldstate))
        {
            return arg;
        }

        docommand(fd, nbytes);

        if (pthread_setcancelstate(oldstate, &newstate))
        {
            return arg;
        }
    }

    return NULL;
}