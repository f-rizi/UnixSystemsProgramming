/*
 * This file implements a simple linked list data structure with functions to:
 * - Add data to the list (`adddata`).
 * - Access data from the list via traversal key (`getdata`).
 * - Manage multiple independent traversers using an array of pointers (`travptrs`).
 * - Maintain a sentinel node (`endlist`) to mark the end of the list during traversal.
 * - Dynamically allocate and manage memory for list nodes and traversal pointers.
 * 
 * Functions:
 * - `adddata()`: Adds a new node to the end of the list.
 * - `getdata()`: Retrieves data from the list, advancing the traversal pointer.
 * - `freekey()`: Frees a traversal pointer at a given index.
 * - `accessdata()`: Initializes or adds a new traverser for the list.
 * 
 * The list supports multiple traversers, each represented by a pointer in `travptrs`, allowing independent traversal of the same list.
 * The sentinel node `endlist` indicates the end of the list and prevents accessing invalid memory during traversal.
 * 
 * **Important**: This implementation is not thread-safe. 
 * To make it thread-safe, synchronization mechanisms should be added to protect shared resources.
 */

#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include "listlib.h"

#define TRAV_INIT_SIZE 8

typedef struct list_struct {
    data_t item;
    struct list_struct *next;
} list_t;

static list_t endlist;

static list_t *headptr = NULL;
static list_t *tailptr = NULL;

static list_t **travptrs = NULL;

static int travptrs_size = 0;

int accessdata(void) {
    int i;
    list_t **newptrs;

    if (headptr == NULL) {
        errno = EINVAL;
        return -1;
    }

    if (travptrs_size == 0) {
        travptrs = (list_t **) calloc(TRAV_INIT_SIZE, sizeof(list_t *));

        if (travptrs == NULL) {
            return -1;
        }

        travptrs[0] = headptr;
        travptrs_size = TRAV_INIT_SIZE;
        return 0;
    }

    for (int i = 0; i < travptrs_size; i++) {
        if (travptrs[i] == NULL) {
            travptrs[i] = headptr;
            return i;
        }
    }

    newptrs = realloc(travptrs, 2 * travptrs_size * sizeof(list_t *));
    if (newptrs == NULL) {
        return -1;
    }

    travptrs = newptrs;
    travptrs[travptrs_size] = headptr;
    travptrs_size *= 2;
    return travptrs_size / 2;
}

int adddata(data_t data) {
    list_t *newnode;
    int nodesize;

    nodesize = sizeof(list_t) + strlen(data.string) + 1;

    if ((newnode = (list_t *)(malloc(nodesize))) == NULL) {
        return -1;
    }

    newnode->item.time = data.time;
    newnode->item.string = (char *) newnode + sizeof(list_t);
    strcpy(newnode->item.string, data.string);
    newnode->next = NULL;

    if (headptr == NULL) {
        headptr = newnode;
    } else {
        tailptr->next = newnode;
    }

    tailptr = newnode;
    return 0;
}

int getdata(int key, data_t *data) {
    list_t *t;

    if ((key < 0) || (key >= travptrs_size) || (travptrs[key] == NULL)) {
        errno = EINVAL;
        return -1;
    }

    if (travptrs[key] == &endlist) {
        data->string = NULL;
        travptrs[key] = NULL;
        return 0;
    }

    t = travptrs[key];

    data->string = (char *) malloc(strlen(t->item.string) + 1);
    if (data->string == NULL) {
        return -1;
    }

    data->time = t->item.time;
    strcpy(data->string, t->item.string);

    if (t->next == NULL) {
        travptrs[key] = &endlist;
    } else {
        travptrs[key] = t->next;
    }

    return 0;
}

int freekey(int key) {
    if ((key < 0) || (key >= travptrs_size)) {
        errno = EINVAL;
        return -1;
    }

    travptrs[key] = NULL;
    return 0;
}