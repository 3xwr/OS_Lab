#include <sys/types.h>
#include <unistd.h>
#include <iostream>


int main(int argc, char* argv[])
{
    pid_t pid;
    for(int i = 1; i < argc; i++)
    {
        std::cout << argv[i] << std::flush;
        sleep(1);
    }
    std::cout << std::endl;
    std::cout << pid << std::endl;
    return 0;
}
