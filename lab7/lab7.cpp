
#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <semaphore.h>
#include <fstream>
#include <unistd.h>
#include <iostream>
#include <string.h>
#include <signal.h>
#define BUFFER_SIZE 7

bool thread_cancel = false;
int fd;

struct arg_struct
{
    pthread_t t_thread;
    pthread_t o_thread;
};

static void* transfer_thread_func(void* arg)
{
    char buf[BUFFER_SIZE];
    while(!thread_cancel)
    {
        strcpy(buf, "Hello!");
        write(fd, buf, BUFFER_SIZE);
        sleep(1);
    }
}

static void* open_thread_func(void* arg)
{
    struct arg_struct *args = (struct arg_struct *) arg;
    while(!thread_cancel)
    {
        fd = open("/tmp/my_named_pipe", O_WRONLY|O_NONBLOCK);
        if(fd == -1)
        {
            std::cout << "error opening the file" << std::endl;
            sleep(2);
        }
        else
        {
            pthread_create(&args->t_thread, NULL, transfer_thread_func, NULL);
            //pthread_join(args->o_thread, NULL); 
            std::cout << "file opened successfully" << std::endl;
            pthread_exit(arg);
        }
    }
}

int main()
{
    signal(SIGPIPE, SIG_IGN);
    struct arg_struct args;
    pthread_t open_thread;
    pthread_t transfer_thread;
    args.t_thread = transfer_thread;
    args.o_thread = open_thread;
    mkfifo("/tmp/my_named_pipe", 0644);
    pthread_create(&open_thread, NULL, open_thread_func, (void*)&args);
    getchar();
    thread_cancel = true;
    pthread_join(open_thread, NULL); 
    pthread_join(transfer_thread, NULL); 
    close(fd);
    unlink("/tmp/my_named_pipe");
    return 0;
}