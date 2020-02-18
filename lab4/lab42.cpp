#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <iostream>
#include <sched.h>
#define _GNU_SOURCE
#define STACK_SIZE 16553

static int child(void* args)
{
    char** arg = (char** ) args;
    std::cout << "ENTERED CHILD PROCESS\n";
    execvpe(arg[0], arg, NULL);
    return 5;
}

int main(int argc, char* argv[])
{
    std::cout << "ENTERED PROCESS\n" << std::flush;
    char* stack;
    stack = (char*)malloc(STACK_SIZE);
    pid_t parent_pid = getppid();
    pid_t pid = getpid();
    char* arg[] = {"./lab4_ex","p","r","o","g","r","a","m","3", NULL};
    pid_t child_pid = clone(child,stack, SIGCHLD, arg);
    int status;
    waitpid(child_pid, &status, 0);
    std::cout << "CHILD EXITED WITH CODE " << status << std::flush;
    std::cout << std::endl;
    std::cout << "PROGRAM 2 PROCESS ID - " << pid << std::flush;
    std::cout << "\nPROGRAM 2 PARENT PROCESS ID - " << parent_pid << std::flush;
    std::cout << "\nPROGRAM 2 CHILD PROCESS ID - " << child_pid << std::flush;
    std::cout << std::endl;
    //exit(5);
    return 10;
}