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

int NUM = 'a';

void making_processes();
void print_num(int num, int queue);

struct msgp
{
    long mtype;
};

int main()
{
    making_processes();
    return 0;
}

void making_processes()
{
    int i = 0;
    int pid = 0;
    int key = ftok(GET_KEY, PROJ_ID);
    int queue = msgget(key, IPC_CREAT | 0666);
    struct msgp msg;
    int sended;
    int k = 0;

    for(k = 0; k < 26; k++)
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

void print_num(int num, int queue)
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
        
    fprintf(stdout, "%c ", num + 'a');
    fflush(stdout);

    if(num < 25)
    {
        msg.mtype = num + 1;
        sended = msgsnd(queue, &msg, 0, 0);
        if(sended == -1)
        {
            fprintf(stderr, "sending failure\n");
            exit(EXIT_FAILURE);
        }
    }
    else if(num == 25)
    {
        msgctl(queue, IPC_RMID, NULL);
    }
}