#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/wait.h>
#define BUFSIZE 256
#define FIFO_PERM (S_IRUSR | S_IWUSR)

int dofifochild(const char *fifoname, const char *idstring);
int dofifoparent(const char *fifoname);

int main(int argc, char *argv[])
{
    pid_t childpid;

    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s pipename\n", argv[0]);
        return 1;
    }

    if (mkfifo(argv[1], FIFO_PERM) == -1)
    {
        if (errno != EEXIST)
        {
            fprintf(stderr, "[%ld]:failed to create named pipe %s: %s\n",
                    (long)getpid(), argv[1], strerror(errno));
            return 1;
        }
    }

    if ((childpid = fork()) == -1)
    {
        perror("Failed to fork");
        return 1;
    }

    if (childpid == 0)
    {
        return dofifochild(argv[1], "This was written by child");
    }
    else
    {
        return dofifoparent(argv[1]);
    }
}