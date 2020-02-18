#include <sys/types.h>
#include <unistd.h>
#include <iostream>


int main(int argc, char* argv[])
{
    pid_t ppid = getppid();
    pid_t pid = getpid();
    for(int i = 1; i < argc; i++)
    {
        std::cout << argv[i] << std::flush;
        sleep(1);
    }
    std::cout << std::endl;
    std::cout << "PROGRAM 1 PROCESS ID - " << pid << std::endl;
    std::cout << "PROGRAM 1 PARENT PROCESS ID - " << ppid << std::endl;
    //exit(2);
    //разобраться с exit code'ом
    return 0;
}
