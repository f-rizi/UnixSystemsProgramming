#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#define BUFSIZE 10

/*
the output should be something like : 
./parentwritepipe 
[2644437]:my bufin is {Empty}, my bufout is {Hello}
[2644438]:my bufin is {Hello}, my bufout is {Hello}

However, if the write in the parent fails or the read 
in the child retrieves a partial result, then the output
will be different.
*/
int main(void)
{
    char bufin[BUFSIZ] = "Empty";
    char bufout[] = "Hello";

    int bytesin;
    pid_t childpid;
    int fd[2];

    if (pipe(fd) == -1)
    {
        perror("Failed to create the pipe");
        return 1;
    }

    bytesin = strlen(bufin);
    childpid = fork();

    if (childpid == -1)
    {
        perror("Failed to fork");
        return 1;
    }

    if (childpid)
    {
        // As parent:
        write(fd[1], bufout, strlen(bufout) + 1);
    }
    else
    {
        // As achild:
        bytesin = read(fd[0], bufin, BUFSIZ);
    }

    fprintf(stderr, "[%ld]:my bufin is {%.*s}, my bufout is {%s}\n", (long)getpid(), bytesin, bufin, bufout);

    return 0;
}