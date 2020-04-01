#include <iostream>
#include <sys/mman.h>
#include <sys/stat.h> /* For mode constants */
#include <fcntl.h> /* For O_* constants */ 
#include <semaphore.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

bool thread_close = false;
sem_t *sem_write;
sem_t *sem_read;
int shm;
int* addr;

static void* thread_func(void* arg)
{
    int value;
    while(!thread_close)
    {
        sem_wait(sem_write);
        memcpy(&value, addr, sizeof(int));
        std::cout << value << std::endl << std::flush;
        sem_post(sem_read);
        sleep(1);
    }
}

int main()
{
    srand(time(NULL));
    pthread_t thread;
    shm = shm_open("my_shared_memory", O_CREAT|O_RDWR, 0644);
    ftruncate(shm,sizeof(int));
    addr = (int*)mmap(0,sizeof(int),PROT_WRITE|PROT_READ,MAP_SHARED,shm,0);
    sem_write = sem_open("/my_named_write_semaphore",O_CREAT,0644,0);
    sem_read = sem_open("/my_named_read_semaphore",O_CREAT,0644,0);
    pthread_create(&thread, NULL,thread_func, NULL);
    getchar();
    thread_close = true;
    pthread_join(thread, NULL);
    sem_close(sem_write);
    sem_close(sem_read);
    sem_unlink("/my_named_write_semaphore");
    sem_unlink("/my_named_read_semaphore");
    munmap(addr,sizeof(int));
    close(shm);
    shm_unlink("my_shared_memory");
    return 0;
}