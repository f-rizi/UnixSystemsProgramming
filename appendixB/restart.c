#include <errno.h>
#include <sys/wait.h>
#include "restart.h"


pid_t r_wait(int *stat_loc) {
    pid_t retval;

    while (((retval = wait(stat_loc)) == -1) && (errno == EINTR));

    return retval;
}

ssize_t r_read(int fd, void *buf, size_t size) {
    ssize_t retval;

    while (((retval = read(fd, buf, size)) == -1) && (errno == EINTR));

    return retval;
}

ssize_t r_write(int fd, void *buf, size_t size) {
    char *bufp = buf;
    size_t bytestowrite = size;
    ssize_t byteswritten = 0;
    size_t totalbytes = 0;

    for (; bytestowrite > 0; bufp += byteswritten, bytestowrite -= byteswritten) {
        byteswritten = write(fd, bufp, bytestowrite);

        if (byteswritten == -1 && errno != EINTR) {
            return -1;
        }

        if (byteswritten == -1) {
            byteswritten = 0;
        }

        totalbytes += byteswritten;
    }

    return totalbytes;
}