#include <pthread.h>
static int doneflag;

static pthread_mutex_t donelock = PTHREAD_MUTEX_INITIALIZER;

int getdone(int *flag)
{
    int error;

    if (error = pthread_mutex_lock(&doneflag))
    {
        return error;
    }

    *flag = doneflag;

    return pthread_mutex_unlock(&doneflag);
}

int setdone(void)
{
    int error;

    if (error = pthread_mutex_lock(&doneflag))
    {
        return error;
    }

    doneflag = 1;

    return pthread_mutex_unlock(&doneflag);
}
