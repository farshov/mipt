#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>


int main()
{
    errno = 0;
    mkfifo("fifo", 0666);
    //int rd_fd = open("fifo", O_RDONLY | O_NONBLOCK);
    int wr_fd = open("fifo", O_WRONLY | O_NONBLOCK);
    printf("wr_fd = %d\n", wr_fd);
    char buf[6] = {};

    //close(rd_fd);
    int written = write(wr_fd, "hello\n", 6);
    if(errno == EPIPE)
    {
        printf("EPIPE\n");
    }
    /*int read_ = read(rd_fd, buf, 6);
    printf("wrtitten = %d, read_ = %d\n", written, read_);
    write(STDOUT_FILENO, buf, 6);
    
    close(wr_fd);
    close(rd_fd);*/
    unlink("fifo");
}