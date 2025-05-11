#include <errno.h>
#include <unistd.h>

int readline(int fd, char *buf, int nbytes)
{
    int counter = 0;
    int returnval;

    while (counter < nbytes - 1)
    {
        returnval = read(fd, buf + counter, 1);

        if (returnval == -1 && errno == EINTR)
        {
            continue;
        }
        else if ((returnval == 0) && (counter == 0))
        {
            return 0;
        }
        else if (returnval == 0)
        {
            // would break and return -1 also set errno to indicate at least
            // one byte was read end of file was reached before reading new line.
            break;
        }
        else if (returnval == -1)
        {
            return -1;
        }

        counter++;

        if (buf[counter - 1] == '\n')
        {
            buf[counter] = '\0';
            return counter;
        }
    }

    errno = EINVAL;
    return -1;
}