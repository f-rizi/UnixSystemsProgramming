#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUFSIZE 1024

int main(int argc, char *argv[])
{
    char buf[BUFSIZE];
    pid_t childpid = 0;
    int i, n;
    FILE *fp;

    if (argc != 3)
    {
        fprintf(stderr, "Usage: %s processes filename\n", argv[0]);
        return 1;
    }

    n = atoi(argv[1]);

    for (i = 0; i < n; i++)
    {
        if ((childpid = fork()) != 0)
        {
            break;
        }
    }

    if (childpid == -1)
    {
        perror("Failed to fork");
        return 1;
    }

    fp = fopen(argv[2], "a");
    if (fp == NULL)
    {
        perror("Failed to open file");
        return 1;
    }

    snprintf(buf, sizeof(buf),
             "i:%d process:%ld. parent:%ld child:%ld\n",
             i, (long)getpid(), (long)getppid(), (long)childpid);

    fwrite(buf, sizeof(char), strlen(buf), fp);
    fclose(fp);

    sleep(1);
    return 0;
}
