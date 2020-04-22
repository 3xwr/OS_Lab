#include <sys/socket.h>
#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <semaphore.h>
#include <fstream>
#include <unistd.h>
#include <iostream>
#include <string.h>
#include <sys/vfs.h> 
#include <vector>
#include <netinet/in.h>
#include <netinet/ip.h> /* superset of previous */
#include <arpa/inet.h>
#define BUFFER_SIZE 4096

int clientSocket;
int count = 1;
bool connect_cancel;
bool response_transfer_cancel;
bool responce_accept_cancel;
struct sockaddr_in clientSockAddr;
socklen_t addrLen = (socklen_t)sizeof(clientSockAddr);
pthread_t connect_thread;
pthread_t response_transfer_thread;
pthread_t response_accept_thread;

static void* response_transfer(void* arg)
{
    while(!response_transfer_cancel)
    {
        char sndbuf[256];
        int len =sprintf(sndbuf, "request %d", count);
        int sentcount =send(clientSocket,sndbuf,len, 0);
        if(sentcount == -1)
        {
            perror("send error");
        }
        else
        {
            //send OK
            std::cout << "request #" << count << " ok" << std::endl;
        }
        count++;
        sleep(1);
    }
}

static void* response_accept(void* arg)
{
    char rcvbuf[256];
    while(1)
    {
        memset(rcvbuf,0,256);
        int reccount = recv(clientSocket, rcvbuf, 256, 0);
        if(reccount == -1)
        {
            perror("recv error");
            sleep(1);
        }
        else if(reccount == 0)
        {
            sleep(1);
        }
        else
        {
            for(int i = 0; i < 256; i++)
            {
                std::cout << rcvbuf[i];
            }
        }
        
    }
    std::cout << std::endl;
}

static void* connect(void* arg)
{
    while(!connect_cancel)
    {
        int result = connect(clientSocket,(struct sockaddr*)&clientSockAddr,sizeof(clientSockAddr));
        if(result == -1)
        {
            perror("connect error");
            sleep(1);
        }
        else
        {
            pthread_create(&response_transfer_thread, NULL, response_transfer, NULL);
            pthread_create(&response_accept_thread, NULL, response_accept, NULL);
        }
        
    }
    return arg;
}

int main()
{
    clientSocket = socket(AF_INET,SOCK_STREAM,0);
    fcntl(clientSocket,F_SETFL,O_NONBLOCK);
    clientSockAddr.sin_family = AF_INET;
    clientSockAddr.sin_port = htons(7000);
    clientSockAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    pthread_create(&connect_thread, NULL, connect, NULL);
    getchar();
    pthread_join(connect_thread, NULL);
    pthread_join(response_transfer_thread, NULL);
    pthread_join(response_accept_thread, NULL);
    shutdown(clientSocket,2);
    close(clientSocket);
    return 0;
}