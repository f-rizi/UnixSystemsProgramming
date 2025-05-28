#include <unistd.h>

int main(void)
{
    // After 10 seconds, default action for SIGALRM (termination)
    // will be done on the proccess.
    alarm(10);

    for (;;)
        ;
}