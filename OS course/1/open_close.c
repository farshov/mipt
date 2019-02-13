#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

int main()
{
    mkfifo("lol", 0666);
    char buf[7];
    int read_ = open("lol", O_RDONLY | O_NONBLOCK);
    printf("read_ = %d\n", read_);
    int write_ = open("lol", O_WRONLY | O_NONBLOCK);
    printf("write_ = %d\n", write_);
    int written = write(write_, "hello\n", 6);
    printf("written = %d\n", written);
    close(read_);
    //close(write_);
    read_ = open("lol", O_RDONLY | O_NONBLOCK);
    int read__ = read(read_, buf, 6);
    printf("read__ = %d\n", read__);
    write(STDOUT_FILENO, buf, 6);
    //unlink("lol");
}
