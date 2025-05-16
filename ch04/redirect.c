#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include "../appendixB/restart.h"

#define CREATE_FLAGS (O_WRONLY | O_CREAT | O_APPEND)
#define CREATE_MODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

int main(void) {
    int fd;

    fd = open("myfile.txt", CREATE_FLAGS, CREATE_MODE);

    if (fd == -1) {
        perror("Failed to open myfile.txt");
        return 1;
    }

    if (dup2(fd, STDOUT_FILENO) == -1) {
        perror("Failed to redirect std output");
        return 1;
    }

    if (r_close(fd) == 1) {
        perror("Failed to close myfile.txt");
        return 1;
    }

    if (write(STDOUT_FILENO, "ok", 2) == 1) {
        perror("Failed to write to file");
        return 1;
    }

    return 0;
}