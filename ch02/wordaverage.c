#include <string.h>

#define LINE_DELIMITER "\n"
#define WORD_DELIMITER " "

static int countwords(char *line)
{
    char *last;

    if (strtok_r(line, WORD_DELIMITER, &last) == NULL)
    {
        return 0;
    }

    int counter = 1;

    while (strtok_r(NULL, WORD_DELIMITER, &last) != NULL)
    {
        counter += 1;
    }

    return counter;
}

double wordaverage(char *lines)
{
    char *last;

    char *line = strtok_r(lines, LINE_DELIMITER, &last);

    if (line == NULL)
    {
        return 0.0;
    }

    int linecounter = 1;
    int wordcounter = countwords(line);

    while ((line = strtok_r(NULL, LINE_DELIMITER, &last)) != NULL)
    {
        linecounter += 1;
        wordcounter += countwords(line);
    }

    return (double)wordcounter / linecounter;
}
