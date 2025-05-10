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

    for (; 
        bytestowrite > 0; 
        bufp += byteswritten, bytestowrite -= byteswritten) {

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

ssize_t readblock(int fd, void *buf, size_t size) {
    char *bufp = buf;
    size_t bytestoread = size;
    ssize_t bytesread = 0;
    size_t totalbytes;

    for (; 
        bytestoread > 0; 
        bufp += bytesread, bytestoread -= bytesread) {
        
        bytesread = read(fd, buf, bytestoread);

        if (bytesread == 0 && totalbytes == 0) {
            return 0;
        }

        if (bytesread == 0) {
            errno = EINVAL;
            return -1;
        }

        if (bytesread == -1 && errno != EINTR) {
            return -1;
        }

        if (bytesread == -1) {
            bytesread = 0;
        }

        totalbytes += bytesread;
    }

    return totalbytes;
}

int copyfile(int fromfd, int tofd) {
    int bytesread;
    int totalbytes = 0;
 
    while ((bytesread = readwrite(fromfd, tofd)) > 0){
        totalbytes += bytesread;
    }
    
    return totalbytes;
 }