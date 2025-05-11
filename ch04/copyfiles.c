#include <errno.h>
#include <unistd.h>
#define BLKSIZE 1024

int copyfile(int fromfd, int tofd)
{
    char *bp;
    char buf[BLKSIZE];
    int bytesread;
    int byteswriten = 0;
    int totalbyts = 0;

    for (;;)
    {
        while (((bytesread = read(fromfd, buf, BLKSIZE)) == -1) && (errno == EINTR))
            ;

        if (bytesread <= 0)
        {
            break;
        }

        bp = buf;

        while (bytesread > 0)
        {
            while (((byteswriten = write(tofd, bp, bytesread)) == -1) && (errno == EINTR))
                ;

            if (byteswriten < 0)
            {
                break;
            }

            totalbyts += byteswriten;
            bytesread -= byteswriten;
            bp += byteswriten;
        }

        if (byteswriten == -1)
        {
            break;
        }
    }

    return totalbyts;
}