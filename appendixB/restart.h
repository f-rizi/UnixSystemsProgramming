#include <fcntl.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>

#ifndef ETIME
#define ETIME ETIMEDOUT
#endif

pid_t r_wait(int *stat_loc);
ssize_t r_read(int fd, void *buf, size_t size);
ssize_t r_write(int fd, void *buf, size_t size);