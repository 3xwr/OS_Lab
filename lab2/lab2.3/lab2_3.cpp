#include <pthread.h>
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>
#include <time.h>

using namespace std;

bool thr1_cancel = false;
bool thr2_cancel = false;

sem_t semaphore;

struct timespec time_passed;

static void* thread1_start(void* args)
{
    std::cout << "THREAD 1 STARTING\n" << std::flush;
    while(!thr1_cancel)
    {
	if(sem_timedwait(&semaphore, &time_passed) == 0)
	{
        time_passed.tv_sec+=1;
		//sem_wait(&semaphore);
		for(int i = 0; i<5; i++)
		{
		    std::cout << '1' << std::flush;
		    sleep(0.1);
		}
		sem_post(&semaphore);
		sleep(1);
	}
    }   
    std::cout << "THREAD 1 EXITING\n" << std::flush;
}

static void* thread2_start(void* args)
{
    std::cout << "THREAD 2 STARTING\n" << std::flush;
    while(!thr2_cancel)
    {
	if(sem_timedwait(&semaphore, &time_passed) == 0)
	{
        time_passed.tv_sec+=1;
		//sem_wait(&semaphore);
		for(int i = 0; i<5; i++)
		{
		    std::cout << '2' << std::flush;
		    sleep(0.1);
		}
		sem_post(&semaphore);
		sleep(1);
	}
    }   
    std::cout << "THREAD 2 EXITING\n" << std::flush;
}

int main()
{
    std::cout << "MAIN STARTING\n" << std::flush;
    clock_gettime(CLOCK_REALTIME, &time_passed);
    pthread_t thread1;
    pthread_t thread2;
    sem_init(&semaphore,0,1);
    pthread_create(&thread1, NULL,thread1_start, NULL);
    pthread_create(&thread2, NULL,thread2_start, NULL);
    getchar();
    thr1_cancel = true;
    thr2_cancel = true;
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    sem_destroy(&semaphore);
    std::cout << "MAIN EXITING\n" << std::flush;
    return 0;
}
