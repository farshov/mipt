#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <errno.h>
#include <limits.h>

#define GET_KEY "file"
#define PROJ_ID '1'

long TOTAL = 0;

void making_processes();
void print_num(long num, int queue);
void get_total(char ** argv);

struct msgp
{
    long mtype;
};

int main(int argc, char ** argv)
{
    if(argc != 2)
    {
        fprintf(stderr, "invalid number of arguments\n");
        exit(EXIT_FAILURE);
    }
    
    get_total(argv); 
    making_processes();

    return 0;
}

void get_total(char ** argv)
{
    char * endptr;
    errno = 0;
    TOTAL = strtol(argv[1], &endptr, 10);
    
    if (errno == ERANGE && (TOTAL == LONG_MAX || TOTAL == LONG_MIN)) {
        fprintf(stderr, "The number is out of range\n");
        exit(EXIT_FAILURE);
    }
    if (endptr == argv[1] || *endptr != '\0')
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

void making_processes()
{
    long i = 1;
    int pid = 0;
    int key = ftok(GET_KEY, PROJ_ID);
    int queue = msgget(key, IPC_CREAT | 0666);
    struct msgp msg;
    int sended;

    while(i <= TOTAL)
    {
        pid = fork();
        if(pid == 0)
        {
            break;
        }
        else if(pid < 0)
        {
            fprintf(stderr, "fork fail\n");
            exit(EXIT_FAILURE);
        }
        i = i + 1;
    }

    if(pid == 0)
    {
        print_num(i, queue);
    }
    else if(pid > 0)
    {
        msg.mtype = 1;
        sended = msgsnd(queue, &msg, 0, 0);
        if(sended == -1)
        {  
            fprintf(stderr, "sending_1 failure\n");
            exit(EXIT_FAILURE);
        }
    }
}

void print_num(long num, int queue)
{
    int received = 0;
    int sended = 0;
    struct msgp msg;

    received = msgrcv(queue, &msg, 0, num, 0);
    if(received == -1)
    {
        fprintf(stderr, "receiving failure\n");
        exit(EXIT_FAILURE);
    }
        
    fprintf(stdout, "%ld ", num);
    fflush(stdout);

    if(num < TOTAL)
    {
        msg.mtype = num + 1;
        sended = msgsnd(queue, &msg, 0, 0);
        if(sended == -1)
        {
            fprintf(stderr, "sending failure\n");
            exit(EXIT_FAILURE);
        }
    }
    else if(num == TOTAL)
    {
        msgctl(queue, IPC_RMID, NULL);
    }
}