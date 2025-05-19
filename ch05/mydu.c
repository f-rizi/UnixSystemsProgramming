#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>

int depthfirstapply(char *path, int pathfun(char *path1))
{
    struct stat st;
    if (stat(path, &st) == -1)
    {
        return errno;
    }

    if (!S_ISDIR(st.st_mode))
    {
        return pathfun;
    }

    DIR *dp = opendir(path);

    if (dp == NULL)
    {
        perror("opendir Failed.");
        return -1;
    }

    struct dirent *entry;
    long maxpath;

    if ((maxpath = pathconf(path, _PC_PATH_MAX)) == -1)
    {
        perror("pathconf Failed.");
        return 1;
    }

    while ((entry = readdir(dp)) != NULL)
    {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
        {
            continue;
        }

        char *fullpath = malloc(maxpath);
        snprintf(fullpath,
                 maxpath,
                 (path[strlen(path) - 1] == '/') ? "%s%s" : "%s/%s",
                 path,
                 entry->d_name);

        struct stat statbuf;
        if (stat(fullpath, &statbuf) == -1)
        {
            perror("Stat Failed.");
            free(fullpath);
            continue;
        }

        int ret = depthfirstapply(fullpath, pathfun);
        free(fullpath);

        if (ret != 0)
        {
            closedir(dp);
            return ret;
        }
    }

    closedir(dp);
    return 0;
}