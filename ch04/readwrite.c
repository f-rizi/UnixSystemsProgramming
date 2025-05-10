#include <limits.h>
#include "../appendixB/restart.h"
#define BLKSIZE PIPE_BUF

int readwrite(int fromfd, int tofd) {
    //Any write of ≤ PIPE_BUF bytes to a pipe is atomic — i.e., 
    // it won’t be interleaved with writes from other processes.
    char buf[BLKSIZE];
    int bytesread;

    if ((bytesread = r_read(fromfd, buf, BLKSIZE)) == -1) {
        return -1;
    }

    if (bytesread == 0) {
        return 0;
    }

    if (r_write(tofd, buf, bytesread) == -1) {
        return -1;
    }

    return bytesread;
}