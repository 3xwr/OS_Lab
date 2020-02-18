#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <semaphore.h>
#include <fstream>
#include <unistd.h>
#include <iostream> /*debug*/
#include <pthread.h>

bool thread_close = false;
sem_t* semaphore;
std::ofstream text_file;

static void* thread_function(void* arg)
{

    char symb = '2';
    while(!thread_close)
    {
        sem_wait(semaphore);
        for(int i = 0; i < 5; i++)
        {
            text_file << symb;
            sleep(0.1);
        }
        sem_post(semaphore);
        sleep(1);
    }
}

int main()
{
    semaphore = sem_open("/semaphore", O_CREAT | O_EXCL ,0777, 0);
    pthread_t thread;
    text_file.open("file.txt");
    pthread_create(&thread, NULL,thread_function, NULL);
    getchar();
    thread_close = true;
    pthread_join(thread, NULL);
    text_file.close();
    sem_close(semaphore);
    sem_unlink("/semaphore");
    return 0;
}