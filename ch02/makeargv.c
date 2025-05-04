#include <errno.h>
#include <stdlib.h>
#include <string.h>

int makeargv(const char *str, const char *delimiters, char ***argvp) {
    if (str == NULL || delimiters == NULL || argvp == NULL) {
        errno = EINVAL;
        return -1;
    }

    int error;
    int numtokens;
    const char *strnew;
    char *strcp;
    *argvp = NULL;

    strnew = str + strspn(str, delimiters);

    if ((strcp = malloc(strlen(strnew) + 1)) == NULL) {
        return -1;
    }

    strcpy(strcp, strnew);
    numtokens = 0;

    if (strtok(strcp, delimiters) != NULL) {
        numtokens += 1;
        while (strtok(NULL, delimiters) != NULL)
        {
            numtokens += 1;
        }        
    }

    if ((*argvp = malloc((numtokens + 1) * sizeof(char *))) == NULL) {
        error = errno;
        free(strcp);
        errno = error;
        return -1;
    }

    if (numtokens == 0) {
        free(strcp);
        *(*(argvp) + numtokens) = NULL;
        return 0;
    }

    strcpy(strcp, strnew);
    **argvp = strtok(strcp, delimiters);

    for (int i = 1; i < numtokens; i++) {
        *((*argvp) + i) = strtok(NULL, delimiters);
    }

    *(*(argvp) + numtokens) = NULL;
    return numtokens;
}