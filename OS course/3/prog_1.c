#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

#define GET_KEY "file"
#define PROJ_ID 'F'
#define N 4
#define BUF_SIZE 4096

#define SEND 0
#define RECEIVE 1
#define MUTEX 2
#define FULL 3

void server(char ** argv);
void client();

int main(int argc, char ** argv)
{
    if(argc == 1)
    {
        client();
    }
    else if(argc == 2)
    {
        server(argv);
    }
    else
    {
        fprintf(stderr, "invalid input\n");
        exit(-1);
    }
    return 0;
}

void server(char ** argv)
{   
    int key = ftok(GET_KEY, PROJ_ID);
    int sem_id = semget(key, N, IPC_CREAT | 0666);
    if(sem_id == -1)
    {
        fprintf(stderr, "semget in server failed\n");
        exit(EXIT_FAILURE);
    }

    int shm_id = shmget(key, BUF_SIZE, IPC_CREAT | 0666);
    char * mem = (char *) shmat(shm_id, NULL, 0666);

    struct sembuf sem_1[2] = { 
        {SEND, 0, IPC_NOWAIT},
        {SEND, 1, SEM_UNDO}
    };

    if(semop(sem_id, sem_1, 2) == -1)
    {
        fprintf(stderr, "There is another server\n");
        exit(EXIT_FAILURE);
    }

    int file = open(argv[1], O_RDONLY | 0666);
    if(file == -1)
    {
        fprintf(stderr, "file fail\n");
        exit(EXIT_FAILURE);
    }

    char buf[BUF_SIZE];
    int in_buf = 0;
    int i = 0;
    int semop_res = 0;
    while(1)
    {
        struct sembuf sem_2[5] = { 
            {RECEIVE, -1, IPC_NOWAIT},
            {RECEIVE, 1, 0},
            {FULL, 0, 0},
            {MUTEX, 0, 0},
            {MUTEX, 1, SEM_UNDO}
        };
        if(i = 0)
        {
            semop_res = semop(sem_id, sem_2 + 2, 3);
        }
        else if(i > 0)
        {
            semop_res = semop(sem_id, sem_2, 5);
        }
        if(semop_res == -1)
        {
            fprintf(stderr, "semop 2 failed\n");
            exit(EXIT_FAILURE);
        }
  
        in_buf = read(file, buf, BUF_SIZE - 1);
        *mem = in_buf;
        strncpy(mem + 1, buf, in_buf);
        struct sembuf sem_3[2] = {  
            {MUTEX, -1, 0},
            {FULL, 1, 0}
        };
        if(semop(sem_id, sem_3, 2) == -1)
        {
            fprintf(stderr, "semop 3 failed\n");
            exit(EXIT_FAILURE);
        }
        if(in_buf == 0)
        {
            break;
        }
        i = i + 1;
    }
    struct sembuf sem_4[2] = {
        {RECEIVE, 0, 0},
        {SEND, -1, 0}
    };
    if(semop(sem_id, sem_4, 2) == -1)
    {
        fprintf(stderr, "semop 4 failed\n");
        exit(EXIT_FAILURE);
    }

    if(shmdt(mem) == -1)
    {
        fprintf(stderr, "shmdt fail\n");
        exit(-1);
    }
    close(file);
}

void client()
{
    int key = ftok(GET_KEY, PROJ_ID);
    int sem_id = semget(key, N, IPC_CREAT | 0666);
    if(sem_id == -1)
    {
        fprintf(stderr, "semget failure\n");
        exit(EXIT_FAILURE);
    }

    struct sembuf sem_5[2] = { 
        {RECEIVE, 0, IPC_NOWAIT},
        {RECEIVE, 1, SEM_UNDO}
    };
    if(semop(sem_id, sem_5, 2) == -1)
    {
        fprintf(stderr, "There is another client\n");
        exit(EXIT_FAILURE);
    }

    int shm_id = shmget(key, BUF_SIZE, IPC_CREAT | 0666);
    char * mem = (char *) shmat(shm_id, NULL, SHM_RDONLY | 0666);
    

    char buf[BUF_SIZE];
    int in_buf = 1;
    while(1)
    {
        struct sembuf sem_6[6] = {
            {SEND, -1, 0},
            {SEND, 1, 0},
            {FULL, -1, 0},
            {FULL, 1, 0},
            {MUTEX, 0, 0},
            {MUTEX, 1, SEM_UNDO}
        };
        if(semop(sem_id, sem_6, 6) == -1)
        {
            fprintf(stderr, "semop 6 fail\n");
            exit(EXIT_FAILURE);
        }

        in_buf = * mem;
        strncpy(buf, mem + 1, in_buf);
        write(STDOUT_FILENO, buf, in_buf);
        
        struct sembuf sem_7[2] = {  
            {MUTEX, -1, 0},
            {FULL, -1, 0}
        };
        if(semop(sem_id, sem_7, 2) == -1)
        {
            fprintf(stderr, "semop 7 fail\n");
            exit(EXIT_FAILURE);
        }
        if(in_buf == 0)
        {
            break;
        }
    }
    struct sembuf sem_8 = {RECEIVE, -1, 0};
    if(semop(sem_id, &sem_8, 1) == -1)
    {
        fprintf(stderr, "semop 8 fail\n");
        exit(EXIT_FAILURE);
    }

    if(shmdt(mem) == -1)
    {
        fprintf(stderr, "shmdt fail\n");
        exit(-1);
    }

    shmctl(shm_id, IPC_RMID, NULL);
    semctl(sem_id, 4, IPC_RMID);
}