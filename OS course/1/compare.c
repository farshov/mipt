#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <sys/types.h>    //unlink, rename, mount
#include <sys/stat.h>
#include <unistd.h>

int main(int argc, char ** argv)
{
    if(argc != 3)
    {
        fprintf(stderr, "incorrect number of arguments\n");
        exit(EXIT_FAILURE);
    }
    struct stat buf_1, buf_2;
    int fd_1 = open(argv[1], O_RDONLY);

    int fd_2 = open(argv[2], O_RDONLY);
    fstat(fd_1, &buf_1);
    fstat(fd_2, &buf_2);
    printf("1 : %ld, 2 : %ld\n", buf_1.st_ino, buf_2.st_ino);
    if((buf_1.st_ino == buf_2.st_ino) && (buf_1.st_dev == buf_2.st_dev))
    {
        printf("SAME\n");
        return 1;
    }
    else
    {
        printf("DEFFERENT\n");
        return 0;
    }
}
