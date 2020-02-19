#include <semaphore.h>
#include <fcntl.h> 
#include <fcntl.h>      
#include <cstdio>    
#include <iostream>

using namespace std;


int main()
{
    sem_t *semaphore = sem_open("/sem1", O_CREAT | O_EXCL, 0777, 1);
    getchar();
    std::cout << "HELLO!";
    sem_unlink("/sem1");

    return 0;
}