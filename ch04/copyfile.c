#include <unistd.h>
#include "../appendixB/restart.h"

#define BLKSIZE 1024

int copyfile(int fromfd, int tofd)
{
    char buf[BLKSIZE];
    int bytesread;
    int byteswrite;
    int totalbytes = 0;

    for (;;)
    {
        if ((bytesread = r_read(fromfd, buf, BLKSIZE)) <= 0)
        {
            break;
        }

        if ((byteswrite = r_write(tofd, buf, bytesread)) == -1)
        {
            break;
        }

        totalbytes += byteswrite;
    }

    return totalbytes;
}