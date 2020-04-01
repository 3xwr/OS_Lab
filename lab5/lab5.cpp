#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <semaphore.h>
#include <fstream>
#include <unistd.h>
#include <iostream>

bool thread_close = false;
sem_t* semaphore;
FILE *f;
//#define FILE_MODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

static void* thread_function(void* arg)
{
    char symb = '1';
    while(!thread_close)
    {
        sem_wait(semaphore);
        for(int i = 0; i < 10; i++)
        {
            fputc(symb,f);
            std::cout << symb << std::flush;
            //sleep(1);
        }
        sem_post(semaphore);
        sleep(3);
    }
}

int main()
{
    //sem_unlink("/semaphore");
    semaphore = sem_open("/semaphore", O_CREAT , 0777, 1);
    pthread_t thread;
    f = fopen("test.txt", "a+");
    pthread_create(&thread, NULL,thread_function, NULL);
    getchar();
    thread_close = true;
    pthread_join(thread, NULL);
    fclose(f);
    sem_close(semaphore);
    sem_unlink("/semaphore");
    return 0;
}