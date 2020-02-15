#include <pthread.h>
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#define _GNU_SOURCE
#define BUFFER_SIZE 7

bool thr1_cancel = false;
bool thr2_cancel = false;

int filedes[2];

static void* thread1_start(void* args)
{
    std::cout << "THREAD 1 STARTING\n" << std::flush;
    char buf[BUFFER_SIZE];
    while(!thr1_cancel)
    {
        strcpy(buf, "Hello!");
        write(filedes[1],buf,BUFFER_SIZE);
        sleep(1);
    }   
    std::cout << "THREAD 1 EXITING\n" << std::flush;
}

static void* thread2_start(void* args)
{
    std::cout << "THREAD 2 STARTING\n" << std::flush;
    char buf[BUFFER_SIZE];
    while(!thr2_cancel)
    {
        memset(buf, 0, sizeof(buf));
        read(filedes[0],buf,BUFFER_SIZE);
        for(int i = 0; i < BUFFER_SIZE; i++)
        {
            std::cout << buf[i] << std::flush;
        }
        sleep(1);
    }   
    std::cout << "THREAD 2 EXITING\n" << std::flush;
}

int main()
{
    std::cout << "MAIN STARTING\n" << std::flush;
    pthread_t thread1;
    pthread_t thread2;
    pipe2(filedes, O_NONBLOCK);
    pthread_create(&thread1, NULL,thread1_start, NULL);
    pthread_create(&thread2, NULL,thread2_start, NULL);
    getchar();
    thr1_cancel = true;
    thr2_cancel = true;
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    close(filedes[0]);
    close(filedes[1]);
    std::cout << "MAIN EXITING\n" << std::flush;
    return 0;
}