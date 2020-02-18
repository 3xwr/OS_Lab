#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <iostream>


int main(int argc, char* argv[],char** envp)
{
    pid_t parent_pid = getppid();
    pid_t pid = getpid();
    pid_t child_pid = fork();
    int status;
    //for(int i = 1; i < argc; i++)
    //{
      //  std::cout << argv[i] << std::flush;
        //sleep(1);
    //}
    char* arg[] = {"./lab4_ex","p","r","o","g","r","a","m","2", NULL};
    if(child_pid == 0)
    {
        std::cout << "ENTERED CHILD PROCESS\n";
        execvpe(arg[0], arg, envp);
    }
    waitpid(child_pid, &status,WEXITSTATUS(status));
    std::cout << "CHILD EXITED WITH CODE " << status;
    std::cout << std::endl;
    std::cout << "PROGRAM 2 PROCESS ID - " << pid << std::endl;
    std::cout << "PROGRAM 2 PARENT PROCESS ID - " << parent_pid << std::endl;
    std::cout << "PROGRAM 2 CHILD PROCESS ID - " << child_pid << std::endl;
    //exit(5);
    return 10;
}