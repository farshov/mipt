#include <stdio.h>
#include <sys/select.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <limits.h>
#include <fcntl.h>

int main()
{
    int * a = NULL;   //strace sh -c ./prog
    *a = 5;
    return 0;
}
