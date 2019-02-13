#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

#define PIPE "fifo"
#define CLIENT "client"
#define SERVER "server"
#define F_GETPIPE_SZ 1032

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
        exit(EXIT_FAILURE);
    }
    return 0;
}

void server(char ** argv)
{
    mkfifo(SERVER, 0666);

    int server_read = open(SERVER, O_RDONLY | O_NONBLOCK);
    int server_write = open(SERVER, O_WRONLY | O_NONBLOCK);
    
    int pipe_size = fcntl(server_write, F_GETPIPE_SZ);
    char * full = malloc(sizeof(char) * pipe_size);
    int in_full = write(server_write, full, pipe_size);
    if(in_full == -1)
    {
        fprintf(stderr, "There is another server\n");
        exit(EXIT_FAILURE);
    }

    int file = open(argv[1], O_RDONLY);
    if(file == -1)
    {
        fprintf(stderr, "invalid file\n");
        exit(EXIT_FAILURE);
    }

    mkfifo(PIPE, 0666);
    int pipe = open(PIPE, O_WRONLY);
    char * buf = malloc(sizeof(char) * PIPE_BUF);   
    int in_buf = 1;
    while(in_buf > 0)
    {
        in_buf = read(file, buf, PIPE_BUF);
        write(pipe, buf, in_buf);
    }
    free(buf);
    free(full);
    close(file);
    close(server_read);
    unlink(SERVER);
}

void client()
{
    mkfifo(CLIENT, 0666);

    int client_read = open(CLIENT, O_RDONLY | O_NONBLOCK);
    int client_write = open(CLIENT, O_WRONLY | O_NONBLOCK);
    
    int pipe_size = fcntl(client_read, F_GETPIPE_SZ);
    char * full = malloc(sizeof(char) * pipe_size);
    int in_full = write(client_write, full, pipe_size);
    if(in_full == -1)
    {
        printf("There is another client\n");
        exit(EXIT_FAILURE);
    }    
    
    mkfifo(PIPE, 0666);
    int counter = 1; 
    int pipe = open(PIPE, O_RDONLY);
    char * buf = malloc(sizeof(char) * PIPE_BUF);   
    while(counter > 0)
    {
        counter = read(pipe, buf, PIPE_BUF);
        write(STDOUT_FILENO, buf, counter);
    }

    close(pipe);
    free(buf);
    free(full);
    close(client_read);
    unlink(CLIENT);
    unlink(PIPE);   //unlink успешно отработал, удалился файл или нет
}
