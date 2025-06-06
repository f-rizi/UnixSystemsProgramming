#include <errno.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

static pthread_rwlock_t listlock;
static int lockiniterror = 0;
static pthread_once_t lockisinitialized = PTHREAD_ONCE_INIT;

#define TRAV_INIT_SIZE 8

typedef struct data_struct
{
    time_t time;
    char *string;
} data_t;

typedef struct list_struct
{
    data_t item;
    struct list_struct *next;
} list_t;

static list_t endlist;

static list_t *headptr = NULL;
static list_t *tailptr = NULL;

static list_t **travptrs = NULL;

static int travptrs_size = 0;

static pthread_mutex_t listlock = PTHREAD_MUTEX_INITIALIZER;

static int accessdata(void)
{
    int i;
    list_t **newptrs;

    if (headptr == NULL)
    {
        errno = EINVAL;
        return -1;
    }

    if (travptrs_size == 0)
    {
        travptrs = (list_t **)calloc(TRAV_INIT_SIZE, sizeof(list_t *));

        if (travptrs == NULL)
        {
            return -1;
        }

        travptrs[0] = headptr;
        travptrs_size = TRAV_INIT_SIZE;
        return 0;
    }

    for (int i = 0; i < travptrs_size; i++)
    {
        if (travptrs[i] == NULL)
        {
            travptrs[i] = headptr;
            return i;
        }
    }

    newptrs = realloc(travptrs, 2 * travptrs_size * sizeof(list_t *));
    if (newptrs == NULL)
    {
        return -1;
    }

    travptrs = newptrs;
    travptrs[travptrs_size] = headptr;
    travptrs_size *= 2;
    return travptrs_size / 2;
}

static int adddata(data_t data)
{
    list_t *newnode;
    int nodesize;

    nodesize = sizeof(list_t) + strlen(data.string) + 1;

    if ((newnode = (list_t *)(malloc(nodesize))) == NULL)
    {
        return -1;
    }

    newnode->item.time = data.time;
    newnode->item.string = (char *)newnode + sizeof(list_t);
    strcpy(newnode->item.string, data.string);
    newnode->next = NULL;

    if (headptr == NULL)
    {
        headptr = newnode;
    }
    else
    {
        tailptr->next = newnode;
    }

    tailptr = newnode;
    return 0;
}

static int getdata(int key, data_t *data)
{
    list_t *t;

    if ((key < 0) || (key >= travptrs_size) || (travptrs[key] == NULL))
    {
        errno = EINVAL;
        return -1;
    }

    if (travptrs[key] == &endlist)
    {
        data->string = NULL;
        travptrs[key] = NULL;
        return 0;
    }

    t = travptrs[key];

    data->string = (char *)malloc(strlen(t->item.string) + 1);
    if (data->string == NULL)
    {
        return -1;
    }

    data->time = t->item.time;
    strcpy(data->string, t->item.string);

    if (t->next == NULL)
    {
        travptrs[key] = &endlist;
    }
    else
    {
        travptrs[key] = t->next;
    }

    return 0;
}

static int freekey(int key)
{
    if ((key < 0) || (key >= travptrs_size))
    {
        errno = EINVAL;
        return -1;
    }

    travptrs[key] = NULL;
    return 0;
}

static void ilock(void)
{
    lockiniterror = pthread_rwlock_init(&listlock, NULL);
}

int initialize_r(void)
{
    if (pthread_once(&lockisinitialized, ilock))
    {
        lockiniterror = EINVAL;
    }

    return lockiniterror;
}

int accessdata_r(void)
{
    int error;
    int errorkey = 0;
    int key;

    if (error = pthread_rwlock_wrlock(&listlock))
    {
        errno = error;
        return -1;
    }

    key = accessdata();

    if (key == -1)
    {
        error = errno;
        pthread_rwlock_unlock(&listlock);
        errno = error;
        return -1;
    }

    if (error = pthread_rwlock_unlock(&listlock))
    {
        errno = error;
        return -1;
    }

    return 0;
}

int getdata_r(int key, data_t *datap)
{
    int error;
    if (error = pthread_rwlock_rdlock(&listlock))
    {
        errno = error;
        return -1;
    }

    if (getdata(key, datap) == -1)
    {
        error = errno;
        pthread_rwlock_unlock(&listlock);
        errno = error;
        return -1;
    }

    if (error = pthread_rwlock_unlock(&listlock))
    {
        errno = error;
        return -1;
    }
    
    return 0;
}

int freekey_r(int key)
{
    int error;
    if (error = pthread_rwlock_wrlock(&listlock))
    {
        errno = error;
        return -1;
    }

    if (freekey(key) == -1)
    {
        error = errno;
        pthread_rwlock_unlock(&listlock);
        errno = error;
        return -1;
    }

    if (error = pthread_rwlock_unlock(&listlock))
    {
        errno = error;
        return -1;
    }

    return 0;
}