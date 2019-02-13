#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <signal.h>
#include <fcntl.h>
#include <errno.h>

//SIGUSR1  посылаем 0
//SIGUSR2  посылаем 1 

int DEATH = 0; //0 ребенок жив, 1 мертв
int BIT = 0;
int PARENT_PID = 0;
int CHILD_PID = 0;

void handler_alarm(int signum);
void child(char ** argv);
void parent();
void send(unsigned char cur, sigset_t set);
void handler_parent(int signum);
void handler_child(int signum);

int main(int argc, char ** argv)
{
    if(argc != 2)
    {
        fprintf(stderr, "invalid input\n");
        exit(EXIT_FAILURE);
    }

    sigset_t set;
    sigfillset(&set);
    sigprocmask(SIG_SETMASK, &set, NULL);

    PARENT_PID = getpid();  //segmentation fault послед событий, кто что делает, что да как
    int child_pid = fork();
    if(child_pid == 0)
    {
        child(argv);
    }
    else if(child_pid > 0)
    {
        CHILD_PID = child_pid;
        parent();
    }
    return 0;
}


void child(char ** argv)
{
    sigset_t set;
    sigfillset(&set);
    sigdelset(&set, SIGUSR1); 
    sigdelset(&set, SIGALRM); 

    struct sigaction act = {};
    act.sa_handler = &handler_child;
    act.sa_flags = 0;
    if(sigaction(SIGUSR1, &act, NULL) == -1)
    {
        fprintf(stderr, "SIGUSR1 error\n");
        exit(EXIT_FAILURE);
    }
    act.sa_handler = &handler_alarm;
    if(sigaction(SIGALRM, &act, NULL) == -1)
    {
        fprintf(stderr, "SIGALRM error\n");
        exit(EXIT_FAILURE);
    }

    int file = open(argv[1], O_RDONLY);
    if(file == -1)
    {
        fprintf(stderr, "invalid file\n");
        exit(EXIT_FAILURE);
    }
    
    int in_buf = 0;
    unsigned char cur;
    while(1)
    {
        in_buf = read(file, &cur, 1);
        if(in_buf == 0)
        {
            break;
        }
        send(cur, set);
    }
}
 
void send(unsigned char cur, sigset_t set)
{
    int i = 0;
    for(i = 0; i < 8; i++)
    {
        if(((cur >> i) & 1) == 1)
        {
            kill(PARENT_PID, SIGUSR2);
        }
        else if(((cur >> i) & 1) == 0)
        {
            kill(PARENT_PID, SIGUSR1);
        }
        alarm(1);
        sigsuspend(&set);
        alarm(0);
    }
}   

void parent()
{
    sigset_t set;
    sigfillset(&set);
    sigdelset(&set, SIGUSR1);
    sigdelset(&set, SIGUSR2);
    sigdelset(&set, SIGCHLD);

    unsigned char cur = 0;
    int i = 0;
    
    struct sigaction act = {};
    act.sa_handler = &handler_parent;
    act.sa_flags = 0;
    if(sigaction(SIGUSR1, &act, NULL) == -1)
    {
        fprintf(stderr, "SIGUSR1 error\n");
        exit(EXIT_FAILURE);
    }
    if(sigaction(SIGUSR2, &act, NULL) == -1)
    {
        fprintf(stderr, "SIGUSR2 error\n");
        exit(EXIT_FAILURE);
    }
    if(sigaction(SIGCHLD, &act, NULL) == -1)
    {
        fprintf(stderr, "SIGCHLD error\n");
        exit(EXIT_FAILURE);
    }
    
    while(DEATH != 1)
    {
        for(cur = 0, i = 0; i < 8; i++)
        {
            sigsuspend(&set);
            if(DEATH == 1)
            {
                break;
            }
            cur = cur | (BIT << i);
            kill(CHILD_PID, SIGUSR1);
        }

        if(i == 8)
        {
            printf("%c", cur);
        }
        else if(i == 0)
        {
            break;
        }
        else
        {
            fprintf(stderr, "not full letter has been received\n");
            exit(EXIT_FAILURE);
        }
        cur = 0;
    }  
}

void handler_child(int signum)
{
    
}

void handler_alarm(int signum)
{
    fprintf(stderr, "ALARM\n");
    exit(EXIT_FAILURE);
}

void handler_parent(int signum)
{
    if(signum == SIGUSR1)
    {
        BIT = 0;
    }
    else if(signum == SIGUSR2)
    {
        BIT = 1;
    }
    else if(signum == SIGCHLD)
    {
        DEATH = 1;
    }
}                                        