#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "buffer.h"
#include "../ch13/globalerror.h"
#include "../ch13/doneflag.h"

int initconsumer(pthread_t *tid);
int initproducer(pthread_t *tid);
int showresults(void);

int main(int argc, char *argv[])
{
    int error;
    int i;
    int numberconsumers;
    int numberproducers;
    int sleeptime;
    pthread_t *tidc;
    pthread_t *tidp;

    if (argc != 4)
    {
        fprintf(stderr, "Usage: %s sleeptime producers consumers\n", argv[0]);
        return 1;
    }

    sleeptime = atoi(argv[1]);
    numberproducers = atoi(argv[2]);
    numberconsumers = atoi(argv[3]);

    tidp = calloc(numberproducers, sizeof(pthread_t));
    if (tidp == NULL)
    {
        perror("Failed to allocate space for producer IDs");
        return 1;
    }

    tidc = calloc(numberconsumers, sizeof(pthread_t));
    if (tidc == NULL)
    {
        perror("Failed to allocate space for consumer IDs");
        return 1;
    }

    for (i = 0; i < numberproducers; i++)
    {
        if (error = initproducer(tidp + i))
        {
            fprintf(stderr, "Failed to create producer %d:%s\n",
                    i, strerror(error));
            return 1;
        }
    }

    for (i = 0; i < numberconsumers; i++)
    {
        if (error = initconsumer(tidc + i))
        {
            fprintf(stderr, "Failed to create consumer %d:%s\n",
                    i, strerror(error));
            return 1;
        }
    }

    sleep(sleeptime);
    if (error = setdone())
    {
        fprintf(stderr, "Failed to set done indicator:%s\n", strerror(error));
        return 1;
    }

    for (i = 0; i < numberproducers; i++)
    {
        if (error = pthread_join(tidp[i], NULL))
        {
            fprintf(stderr, "Failed producer %d join:%s\n", i, strerror(error));
            return 1;
        }
    }

    for (i = 0; i < numberconsumers; i++)
    {
        if (error = pthread_join(tidc[i], NULL))
        {
            fprintf(stderr, "Failed consumer %d join:%s\n", i, strerror(error));
            return 1;
        }
    }

    if  (showresults()) {
        return 1;
    } 

    return 0;
}