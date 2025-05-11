#include <stdlib.h>
#include <string.h>
#include "log.h"

typedef struct list_struct
{
   data_t item;
   struct list_struct *next;
} log_t;

static log_t *headptr = NULL;
static log_t *tailptr = NULL;

int addmsg(data_t data)
{
   return 0;
}

// Releases all the storage that has been allocated for the logged messages and
// empties the list of the logged messages.
void clearlog(void)
{
}

// Allocate enough space for a string containing the entire log, copies the log
// into this string and retur na pointer to the string.
char *getlog(void)
{
   return NULL;
}

// Save the logged messages to a disk file.
int savelog(char *filename)
{
   return 0;
}