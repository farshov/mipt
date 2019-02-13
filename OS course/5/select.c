#include <sys/select.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <limits.h>
#include <fcntl.h>

int main()
{
    int buf[2];
    pipe(buf);
    /*//fcntl(buf[0], F_SETFL, O_RDONLY | O_NONBLOCK);
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(buf[0], &fds);
    close(buf[1]);
    //write(buf[1], "hello", 6);
    struct timeval timer = {0, 0};
    int res_1 = select(buf[0] + 1, &fds, NULL, NULL, NULL);
    int res_2 = FD_ISSET(buf[0], &fds);
    printf("res_1 = %d, res_2 = %d\n", res_1, res_2);
    int buff[6];
    //read(buf[0], buff, 6);
    //write(STDOUT_FILENO, buff, 6);
    close(buf[0]);
    //close(buf[1]);*/

    char buff[12] = {};
    write(buf[1], "hello", 6);
    close(buf[1]);
    int read_ = read(buf[0], buff, 12);
    write(STDOUT_FILENO, buff, read_);
    printf("read_ = %d\n", read_);
    return 0;
}
