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
#define BUFFER_SIZE 16000


int listenSocket;
int serverSocket;
std::vector<std::string> msglist;
bool request_accept_cancel = false;
bool response_transfer_cancel = false;
bool wait_cancel = false;  
pthread_t request_accept_thread;
pthread_t request_processing_thread;
pthread_t response_transfer_thread;
pthread_t wait_thread;
pthread_mutex_t mutex;
struct sockaddr_in serverSockAddr;
socklen_t addrLen = (socklen_t)sizeof(serverSockAddr);

static void* request_accept(void* arg)
{
    char rcvbuf[BUFFER_SIZE];
    while(!request_accept_cancel)
    {
        int reccount = recv(serverSocket, rcvbuf, 256, 0);
        if(reccount == -1)
        {
            perror("recv error");
            sleep(1);
        }
        else if(reccount == 0)
        {
            //disconnect
            sleep(1);
        }
        else
        {
            pthread_mutex_lock(&mutex);
            msglist.push_back(std::string(rcvbuf));
            pthread_mutex_unlock(&mutex);
            //захватить мьютекс
            //поместить запрос в очередь
            //мьютекс освободить

        }
        
        //принять запрос из сокета
        //положить запрос в очередь на обработку
    }
}

static void* response_transfer(void* arg)
{
    char sndbuf[BUFFER_SIZE];
    struct statfs statfs_s;
    while(!response_transfer_cancel)
    {
        pthread_mutex_lock(&mutex);
        if(!msglist.empty())
        {
            std::string S = msglist.back();
            msglist.pop_back();
            pthread_mutex_unlock(&mutex);
            statfs("file.txt", &statfs_s);
            //strcpy(sndbuf, (char*)statfs_s.f_blocks);
            strcpy(sndbuf, "TEST SUCCESSFUL");
            int sentcount = send(serverSocket,sndbuf,BUFFER_SIZE, 0);
            if(sentcount == -1)
            {
                perror("send error");
            }
            else
            {
                //send ok
                std::cout << "send OK" << std::endl;
            }
        }
        else
        {
            pthread_mutex_unlock(&mutex);
            sleep(1);
        }
        
            
        }
       /* прочитать ответ из очереди на передачу;
        передать ответ в сокет;*/
}

static void* wait(void* arg)
{

    while(!wait_cancel)
    {
        serverSocket = accept(listenSocket,(struct sockaddr*)&serverSockAddr,&addrLen);
        //fcntl(serverSocket, F_SETFL, O_NONBLOCK);
        if(serverSocket == -1)
        {
            perror("accept error");
            sleep(1);
        }
        else
        {
            pthread_create(&request_accept_thread, NULL, request_accept, NULL);
            pthread_create(&response_transfer_thread, NULL, response_transfer, NULL);
            return arg;
        }
        
       /* прием соединения от клиента;
            если соединение принято
            {
                создать поток приема запросов;
                создать поток обработки запросов;
                создать поток передачи ответов;
                завершить работу потока ожидания соединений;
            }*/
    }
}

int main()
{
    pthread_mutex_init(&mutex, NULL);
    int optval = 1;
    listenSocket = socket(AF_INET, SOCK_STREAM,0);
    fcntl(listenSocket, F_SETFL, O_NONBLOCK);
    struct sockaddr_in listenSockAddr;
    listenSockAddr.sin_family = AF_INET;
    listenSockAddr.sin_port = htons(7000);
    listenSockAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    setsockopt(listenSocket, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
    bind(listenSocket,(struct sockaddr*)&listenSockAddr,sizeof(listenSockAddr));
    listen(listenSocket,SOMAXCONN);
    pthread_create(&wait_thread, NULL, wait, NULL);
    getchar();
    request_accept_cancel = true;
    response_transfer_cancel = true;
    wait_cancel = true;   
    pthread_join(wait_thread, NULL);
    pthread_join(response_transfer_thread, NULL);
    pthread_join(request_accept_thread, NULL);
    shutdown(serverSocket,2);
    close(serverSocket);
    close(listenSocket);
    pthread_mutex_destroy(&mutex);
}