#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    pid_t childpid = 0;
    int n;
    int i;

    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s processes\n", argv[0]);
        return 1;
    }

    n = atoi(argv[1]);
    for (i = 0; i < n; i++)
    {
        if ((childpid = fork() <= 0))
        { // original process creats n children
            break;
        }
    }

    fprintf(stderr, "i:%d  process ID:%ld  parent ID:%ld  child ID:%ld\n",
            i, (long)getpid(), (long)getppid(), (long)childpid);

    return 0;
}