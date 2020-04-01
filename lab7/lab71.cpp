
#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <semaphore.h>
#include <fstream>
#include <unistd.h>
#include <iostream>
#include <string.h>
#define BUFFER_SIZE 7

bool thread_cancel = false;
int fd;

static void* thread_func(void* arg)
{
    char buf[BUFFER_SIZE];
    while(!thread_cancel)
    {
        memset(buf, 0, sizeof(buf));
        read(fd, buf, BUFFER_SIZE);
        for(int i = 0; i < BUFFER_SIZE; i++)
        {
            std::cout << buf[i] << std::flush;
        }
        sleep(1);
    }
}

int main()
{
    pthread_t thread;
    mkfifo("/tmp/my_named_pipe", 0644);
    fd = open("/tmp/my_named_pipe", O_RDONLY|O_NONBLOCK);
    pthread_create(&thread, NULL, thread_func, NULL);
    getchar();
    thread_cancel = true;
    pthread_join(thread, NULL); 
    close(fd);
    unlink("/tmp/my_named_pipe");
    return 0;
}