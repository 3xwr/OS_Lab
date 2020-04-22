#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <semaphore.h>
#include <fstream>
#include <unistd.h>
#include <iostream>
#include <string.h>
#include <mqueue.h>
#define BUFFER_SIZE 256

bool thread_cancel = false;
mqd_t mqid;

static void* thread_func(void* arg)
{
    int result;
    char buf[BUFFER_SIZE];
    while(!thread_cancel)
    {
        strcpy(buf, "Hello!");
        result = mq_send(mqid, buf,BUFFER_SIZE, 0);
        if (result == -1)
        {
            perror("mq_send");
        }
        else
        {
            //ok
        }
        sleep(1);
        
    }
}

int main()
{
   /* struct mq_attr attr;
    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = 33;
    attr.mq_curmsgs = 0;*/
    pthread_t thread;
    mqid =  mq_open("/myqueue", O_CREAT | O_WRONLY | O_NONBLOCK, 0644, NULL);
    pthread_create(&thread, NULL, thread_func, NULL);
    getchar();
    thread_cancel = true;
    pthread_join(thread, NULL);
    mq_close(mqid);
    mq_unlink("/myqueue");
}
