#include <sys/select.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <limits.h>
#include <fcntl.h>

#define CH_BUF_SZ 131072 //128 kB
#define PAR_BUF_MAX 1048576  //1 mB
#define BASIC_SZ 512 //512 B
#define RD_END 0
#define WR_END 1
#define MAX_TOTAL 6

int TOTAL = 0;
int FILE_FD = -1;
struct node * TABLE = NULL;

void get_total(char ** argv);
int get_size(int n);
void parent();
void child(int num);
void make_table();
void del_table();
int get_max();
void reading(int num);
void writing(int num);


struct node
{
    int size;
    char * buf;
    char * cur_pos;
    int in[2];
    int out[2];
    int busy;
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

    FILE_FD = open(argv[1], O_RDONLY);
    if(FILE_FD == -1)
    {
        fprintf(stderr, "Invalid file\n");
        exit(EXIT_FAILURE);
    }
    get_total(argv);
    make_table();
    
    for(i = 0; i < TOTAL; i++)
    {
        pid = fork();
        if(pid == 0)
        {
            child(i);
            break;
        }
    }
    
    if(pid > 0)
    {
        parent();
    }
    return 0;
}

void parent()
{
    int i = 0;
    fd_set rd_set, wr_set;
    
    for(i = 0; i < TOTAL + 1; i++)
    {
        TABLE[i].busy = 0;
        TABLE[i].size = get_size(TOTAL - i);
        TABLE[i].buf = malloc(TABLE[i].size * sizeof(char));
        TABLE[i].cur_pos = TABLE[i].buf;
        if(i > 0)
        {
            close(TABLE[i].in[WR_END]);
        }
        if(i != TOTAL)
        {
            close(TABLE[i].out[RD_END]);
        }
    }
    FD_ZERO(&rd_set);
    FD_ZERO(&wr_set);

    int nfds = get_max();
    int num_fds = 0;

    while(1)
    {
        FD_ZERO(&rd_set);
        FD_ZERO(&wr_set);
        int nfds = get_max();

        for(i = 0; i < TOTAL + 1; i++)
        {
            if((TABLE[i].in[RD_END] != -1) && (TABLE[i].busy == 0))
            {
                FD_SET(TABLE[i].in[RD_END], &rd_set);
                num_fds = num_fds + 1;
            }
            if(TABLE[i].out[WR_END] != -1 && TABLE[i].busy != 0)
            {
                FD_SET(TABLE[i].out[WR_END], &wr_set);
                num_fds = num_fds + 1;
            }
        }
        if(num_fds == 0)
        {
            break;
        }

        select(nfds + 1, &rd_set, &wr_set, NULL, NULL);

        for(i =0; i < TOTAL + 1; i++)
        {
            if(FD_ISSET(TABLE[i].in[RD_END], &rd_set))
            {
                reading(i);
            }
            if(FD_ISSET(TABLE[i].out[WR_END], &wr_set))
            {
                writing(i);
            }
        }
        num_fds = 0;
    }
    del_table();
}

void reading(int num)
{
    int read_ = 0;
    read_ = read(TABLE[num].in[RD_END], TABLE[num].cur_pos, TABLE[num].size - TABLE[num].busy);
    TABLE[num].busy = read_;
    if(read_ == 0)
    {
        close(TABLE[num].in[RD_END]);
        TABLE[num].in[RD_END] = -1;
        if(TABLE[num].out[WR_END] != STDOUT_FILENO && TABLE[num].out[WR_END] != TABLE[TOTAL].out[WR_END])
        {
            close(TABLE[num].out[WR_END]);
        }
        TABLE[num].out[WR_END] = -1;
    }
}

void writing(int num)
{
    int written = 0;
    written = write(TABLE[num].out[WR_END], TABLE[num].cur_pos, TABLE[num].busy);
    if(written < TABLE[num].busy)
    {
        TABLE[num].busy = TABLE[num].busy - written;
        TABLE[num].cur_pos = TABLE[num].cur_pos + written;
    }
    else
    {
        TABLE[num].cur_pos = TABLE[num].buf;
        TABLE[num].busy = 0;
    }
}

void del_table()
{
    int i = 0;
    for(i = 0; i < TOTAL + 1; i++)
    {
        free(TABLE[i].buf);
    }
    free(TABLE);
}

int get_max()
{
    int i = 0, res = 1;
    for(i = 0; i < TOTAL + 1; i ++)
    {
        if(TABLE[i].in[RD_END] > res)
        {
            res = TABLE[i].in[RD_END];
        }
        if(TABLE[i].out[WR_END] > res)
        {
            res = TABLE[i].out[WR_END];
        }
    }
    return res;
}

void make_table()
{
    int i = 0;
    TABLE = malloc((TOTAL + 1) * sizeof(struct node));
    for(i = 0; i < TOTAL + 1; i++)
    {   
        if(i != 0)
        {
            pipe(TABLE[i].in);
            fcntl(TABLE[i].in[RD_END], F_SETFL, O_NONBLOCK | O_RDONLY);
        }
        else
        {
            TABLE[i].in[RD_END] = FILE_FD;
            TABLE[i].in[WR_END] = -1;
        }
        
        if(i != TOTAL)
        {
            pipe(TABLE[i].out);
            fcntl(TABLE[i].out[WR_END], F_SETFL, O_NONBLOCK | O_WRONLY);
        }
        else
        {
            //TABLE[i].out[WR_END] = open("cars1", O_WRONLY);
            TABLE[i].out[WR_END] = STDOUT_FILENO;
            TABLE[i].out[RD_END] = -1;
        }
    }
}

void child(int num)
{
    char buf[CH_BUF_SZ] = {};
    int i = 0;
    int input = -1, output = -1; 
    
    for(i = 0; i < TOTAL; i++)
    {
        close(TABLE[i].out[WR_END]);
        close(TABLE[i + 1].in[RD_END]);
        
        if(i != num)
        {
            close(TABLE[i].out[RD_END]);
            close(TABLE[i + 1].in[WR_END]);
        }

    }
        
    input = TABLE[num].out[RD_END]; 
    output = TABLE[num + 1].in[WR_END];

    int read_ = 1, written = 1;

    while(read_)
    {
        read_ = read(input, buf, CH_BUF_SZ);
        sleep(0.01);
        written = write(output, buf, read_);
    }

    close(input);
    close(output);
    free(TABLE);
    printf("end of %d\n", num);
}

void get_total(char ** argv)
{
    char * endptr;
    errno = 0;
    TOTAL = strtol(argv[2], &endptr, 10);
    
    if (errno == ERANGE && (TOTAL == LONG_MAX || TOTAL == LONG_MIN)) 
    {
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
}

int get_size(int n)
{
    int res = 1, i = 0;
    if(n < 7)
    {
        for(i = 0; i < n; i++)
        {
            res = res * 3;
        }
        res = BASIC_SZ * res; 
    }
    else
    {
        res = PAR_BUF_MAX;
    }
    return res;
}