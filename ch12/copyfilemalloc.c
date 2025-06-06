#include <stdlib.h>
#include <unistd.h>
#include "../appendixB/restart.h"

void *copyfilemalloc(void *arg)
{
    int *bytesp;
    int infd;
    int outfd;

    infd = *((int *)(arg));
    outfd = *((int *)(arg) + 1);

    if ((bytesp = (int *)malloc(sizeof(int))) == NULL)
    {
        return NULL;
    }

    *bytesp = copyfile(infd, outfd);
    r_close(infd);
    r_close(outfd);

    return bytesp;
}