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
#include <errno.h>

#define GET_KEY "file"
#define PROJ_ID 'Z'
#define N 3
#define BUF_SIZE 100

#define SERVER 0
#define CLIENT 1
#define FULL 2

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

    struct sembuf sem_1[2] = 
    { 
        {SERVER, 0, IPC_NOWAIT},
        {SERVER, 1, SEM_UNDO}
    };

    if(semop(sem_id, sem_1, 2) == -1)
    {
        fprintf(stderr, "There is another server\n");
        exit(EXIT_FAILURE);
    }

    int shm_id = shmget(key, BUF_SIZE, IPC_CREAT | 0666);
    char * mem = (char *) shmat(shm_id, NULL, 0);

    int file = open(argv[1], O_RDONLY);
    if(file == -1)
    {
        fprintf(stderr, "invalid file\n");
        exit(EXIT_FAILURE);
    }

    char buf[BUF_SIZE];
    int in_buf = read(file, buf, BUF_SIZE - 1);
    int i = 0, sem_res = 0;
    while(1)
    {
        struct sembuf sem_2[7] = 
        { 
            {CLIENT, -1, IPC_NOWAIT},
            {CLIENT, 1, IPC_NOWAIT},
            {FULL, 0, 0},
            {FULL, 1, 0},
            {FULL, -1, SEM_UNDO},
            {CLIENT, -1, IPC_NOWAIT},
            {CLIENT, 1, IPC_NOWAIT}
        };

        if(i == 1)
        {
            sem_res = semop(sem_id, sem_2 + 2, 5);
        }
        else if(i > 1)
        {
            sem_res = semop(sem_id, sem_2, 5);
        }
        
        if(sem_res == -1)
        {  
            fprintf(stderr, "semop 2 failed\n");
            exit(EXIT_FAILURE);
        }

        *mem = in_buf;
        strncpy(mem + 1, buf, in_buf);
        
        struct sembuf sem_3[4] = 
        {  
            {FULL, 1, SEM_UNDO},
            {CLIENT, -1, IPC_NOWAIT},
            {CLIENT, 1, IPC_NOWAIT}
        };
        if(i == 0)
        {
            sem_res = semop(sem_id, sem_3, 1);
        }
        else if(i > 0)
        {
            sem_res = semop(sem_id, sem_3, 3);
        }
        if(sem_res == -1)
        {
            fprintf(stderr, "semop 3 failed\n");
            exit(EXIT_FAILURE);
        }

        if(in_buf == 0)
        {
            break;
        }
        if(i < 2)
        {
            i = i + 1;
        }
        in_buf = read(file, buf, BUF_SIZE - 1);
    }

    shmdt(mem);
    close(file);
}

void client()
{
    int key = ftok(GET_KEY, PROJ_ID);
    int sem_id = semget(key, N, IPC_CREAT | 0666);
    if(sem_id == -1)
    {
        fprintf(stderr, "semget in client failure\n");
        exit(EXIT_FAILURE);
    }

    struct sembuf sem_4[4] = 
    {
        {CLIENT, 0, IPC_NOWAIT}, //если есть другой клиент, то данный закрывается
        {CLIENT, 1, SEM_UNDO},
        {SERVER, -1, 0},  // для дальнейших действий ждем запуск сервера
        {SERVER, 1, 0}
    };
    if(semop(sem_id, sem_4, 2) == -1)
    {
        fprintf(stderr, "There is another client\n");
        exit(EXIT_FAILURE);
    }
    semop(sem_id, sem_4 + 2, 2);

    int shm_id = shmget(key, BUF_SIZE, IPC_CREAT | 0666);
    char * mem = (char *) shmat(shm_id, NULL, SHM_RDONLY);
    
    char buf[BUF_SIZE];
    int in_buf = 1;
    int sem_res = 0;
    while(1)
    {
        struct sembuf sem_5[4] = 
        { 
            {SERVER, -1, IPC_NOWAIT},
            {SERVER, 1, IPC_NOWAIT},
            {FULL, -1, 0},
            {FULL, 1, SEM_UNDO}
        };
        
        sem_res = semop(sem_id, sem_5, 4);
        if(sem_res == -1)
        {
            fprintf(stderr, "semop 5 fail\n");
            exit(EXIT_FAILURE);
        }

        in_buf = * mem;
        strncpy(buf, mem + 1, in_buf);
        write(STDOUT_FILENO, buf, in_buf);

        struct sembuf sem_6[3] = 
        {  
            {FULL, -1, SEM_UNDO},
            {SERVER, -1, IPC_NOWAIT},
            {SERVER, 1, IPC_NOWAIT}
        };

        if(semop(sem_id, sem_6, 3) == -1)
        {
            fprintf(stderr, "semop 6 fail\n");
            exit(EXIT_FAILURE);
        }
        if(in_buf == 0)
        {
            break;
        }
    }

    shmdt(mem);
    shmctl(shm_id, IPC_RMID, NULL);
    semctl(sem_id, 3, IPC_RMID);
}