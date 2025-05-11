#include <stdio.h>
#include <stdlib.h>

double wordaverage(const char *str);

int main(int argc, char *argv[])
{

    char str[] = "Hello, World!\n\nThis is a string with multiple newlines\nand spaces.";
    printf("word average is %f\n", wordaverage(str));

    return 0;
}