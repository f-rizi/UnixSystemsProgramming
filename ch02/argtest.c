#include <stdio.h>
#include <stdlib.h>

int makeargv(const char *str, const char *delimiters, char ***argvp);

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s string\n", argv[0]);
        return 1;
    } 

    char delimiters[] = " \t";
    char **myargv;
    int numtokens;

    if ((numtokens = makeargv(argv[1], delimiters, &myargv)) == -1) {
        fprintf(stderr, "Failed to construct an argument array for %s\n", argv[1]);
        return 1;
    }

    printf("The argument array contains:\n");

    for (int i = 0; i < numtokens; i++) {
        printf("%d:%s\n", i, myargv[i]);
    }

    free(*myargv);
    free(myargv);

    return 0;
}