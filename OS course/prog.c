#include <sys/select.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>     //double pow (double x, double y);
#include <errno.h>
#include <limits.h>
#include <fcntl.h>

#define CH_BUF_SZ 131072
#define PB_MAX 1048576
#define BASIC_SZ 512
#define RD_END 0
#define WR_END 1

int TOTAL = 0;
int BUF_SIZES[] = 
{
    729 * BASIC_SZ,
    243 * BASIC_SZ,
    81 * BASIC_SZ,
    27 * BASIC_SZ,
    9 * BASIC_SZ,
    3 * BASIC_SZ,
    BASIC_SZ
};
int ** TABLE = NULL;
char * FILENAME = NULL;
int TABLE_SIZE = 0;

void get_total(char ** argv);
void child(int num);
void parent();

struct par_buf
{
    int size;
    char * start;
    int cur_start;
    int cur_size;
    int rfd;
    int wfd;
};

struct ch_buf
{
    int num;
    char * start;
    int cur_start;
    int cur_size;
    int rfd;
    int wfd;
};


int main(int argc, char ** argv)   // сначала имя файла, далее число детей
{
    long i = 0;
    int pid = 0;
    if(argc != 3)
    {
        fprintf(stderr, "invalid number of arguments\n");
        exit(EXIT_FAILURE);
    }

    FILENAME = argv[1];
    get_total(argv);
    // все данный получены и обработаны
    // подготовка ресурсов

    TABLE_SIZE = (2 * TOTAL - 1);
    TABLE = malloc(TABLE_SIZE * sizeof(int *));
    for(i = 0; i < TABLE_SIZE; i++)
    {
        TABLE[i] = malloc(2 * sizeof(int));
        pipe(TABLE[i]);   // блокирующий флаг по дефолту
    }
    for(i = 0; i < TOTAL; i++)
    {
        pid = fork();
        if(pid == 0)
        {
            child(i);
        }
    }
    //создали таблиц с пайпами
    if(pid > 0)
    {
        parent();
    }
    
    for(i = 0; i < TABLE_SIZE; i++)
    {
        free(TABLE[i]);
    }
    free(TABLE);
    return 0;
}

void parent()
{
    // создаем массив структу
    int i = 0;
    struct par_buf * buf_list = malloc(TOTAL * sizeof(struct par_buf));
    for(i = 0; i < TOTAL; i++)
    {
        buf_list[i].size = BUF_SIZES[i];
        buf_list[i].start = malloc(buf_list[i].size * sizeof(char));
        buf_list[i].cur_start = 0;
        buf_list[i].cur_size = buf_list[i].size;
        if(i == TOTAL - 1)
        {
            buf_list[i].wfd = STDOUT_FILENO;
        }
        else
        {
            buf_list[i].wfd = TABLE[2 * i + 1][WR_END];
        }
        buf_list[i].rfd = TABLE[2 * i][RD_END];
    }
    //инизиализировали все буфера родителя
    //получаем максимальный дескриптор
    int max_fd = -1;
    for(i = 0; i < TOTAL; i++)
    {
        if(buf_list[i].wfd > max_fd && i != (TOTAL -1))
        {
            max_fd = buf_list[i].wfd;
        }
        if(buf_list[i].rfd > max_fd)
        {
            max_fd = buf_list[i].rfd;
        }
    }

    fd_set full_rfd;
    FD_ZERO(&full_rfd);
    for(i = 0; i < TOTAL; i++)
    {
        FD_SET(buf_list[i].rfd, &full_rfd);
    }

    //выставляем все файловые дескрипторы в неблокинующий режим
    for(i = 0; i < TOTAL; i++)
    {
        fcntl(buf_list[i].rfd, F_SETFD, 0666 | O_NONBLOCK);
        if (i == TOTAL - 1)
        {
            fcntl(buf_list[i].wfd, F_SETFD, 0666 | O_NONBLOCK);
        }
    }
    fd_set read_fds = 
    int written = 1, read_ = 1;
    while(written || read_)
    {
        printf("parent\n");
        
        read_ = read(buf_list[0].rfd, buf_list[0].start, buf_list[0].size);
        printf("parent read_ = %d\n", read_);
        written = write(buf_list[0].wfd, buf_list[0].start, read_);    //????????
    }

    for(i = 0; i < TOTAL; i ++)
    {
        free(buf_list[i].start);
        close(buf_list[i].rfd);
        if(i != TOTAL -1)
        {
            close(buf_list[i].wfd);
        }
    }
    free(buf_list);
}


void child(int num)
{
    struct ch_buf buf = {};
    
    buf.num = num;
    buf.start = malloc(CH_BUF_SZ * sizeof(char));
    buf.cur_start = 0;
    buf.cur_size = CH_BUF_SZ;
    if(num == 0)
    {
        buf.rfd = open(FILENAME, O_RDONLY);   //???????????????????
        if(buf.rfd == -1)
        { 
            fprintf(stderr, "Invalid file\n");
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        buf.rfd = TABLE[2 * num - 1][RD_END];
    }
    buf.wfd = TABLE[2 * num][WR_END];
    
    int read_ = 1, written = 0;
    while(read_)
    {
        read_ = read(buf.rfd, buf.start, CH_BUF_SZ);
        printf("child read_ = %d\n", read_);
        written = write(buf.wfd, buf.start, read_);
        printf("child written_ = %d\n", written);
    }

    free(buf.start);
    close(buf.rfd);
    close(buf.wfd);
}


void get_total(char ** argv)
{
    char * endptr;
    errno = 0;
    TOTAL = strtol(argv[2], &endptr, 10);
    
    if (errno == ERANGE && (TOTAL == LONG_MAX || TOTAL == LONG_MIN)) {
        fprintf(stderr, "The number is out of range\n");
        exit(EXIT_FAILURE);
    }
    if (endptr == argv[2] || *endptr != '\0')
    {
        fprintf(stderr, "The argument is not a number\n");
        exit(EXIT_FAILURE);
    }
    if(TOTAL <= 0)
    {
        fprintf(stderr, "The number of processes can not be non positive\n");
        exit(EXIT_FAILURE);
    }
    if(TOTAL >= 8)//проверка на размер
    {
        fprintf(stderr, "The number of buffers is too big");
        exit(EXIT_FAILURE);
    }
}