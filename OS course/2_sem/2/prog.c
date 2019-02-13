#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <errno.h>
#include <limits.h>
#include <pthread.h>

#define GET_KEY "file"
#define PROJ_ID '1'

long TOTAL = 0;
int NUM = 'a';

void making_threads();
static void * print_num(void * num);

struct msgp
{
    long mtype;
};

int main(int argc, char ** argv)
{
    making_threads();
    return 0;
}


void making_threads()
{
    int i = 0;
    int res = 0;
    pthread_t tid[26] = {};
    for(i = 0; i < 26; i++)
    {
        res = pthread_create(&(tid[i]), NULL, &print_num, &i);
        if(res != 0)
        {
            fprintf(stderr, "error in pthread create\n");
        }
    }
}

static void * print_num(void * num)
{
    int new = *(int *)num;
    if((new + 'a') == NUM)
    {
        printf("%c ", new + 'a');
        if(new == 25)
        {
            NUM = 0;
        }
        NUM++;
    }
}