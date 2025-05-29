#include <iostream>
#include <cstring>
#include <string>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include<functional>
#include<wait.h>
#include<sys/wait.h>
#include<pthread.h>
#include "log.h"
#include"Thread_Pool.h"
const uint16_t gport = 1234;
using task_t=std::function<void()>;
class server
{
private:
    int _sockfd;
    uint16_t _port;
    bool _isrunning = false;
    void init()
    {
        _sockfd = socket(AF_INET, SOCK_STREAM, 0);
        struct sockaddr_in local;
        memset(&local, 0, sizeof(local));
        local.sin_family = AF_INET;
        local.sin_addr.s_addr = INADDR_ANY;
        local.sin_port = htons(_port);
        int n = bind(_sockfd, (struct sockaddr *)&local, sizeof(local));
        listen(_sockfd, 8);
    }
    void service(int fd)
    {
        while(true)
        {
            char buf[128];
            ssize_t n = read(fd, buf, sizeof(buf) - 1);
            if (n > 0)
            {
                buf[n] = 0;
                std::string echo_message = "Server Echo#";
                echo_message += buf;
                write(fd, echo_message.c_str(), echo_message.size());
            }
            else if (n == 0)
            {
                LOG(DEBUG, "client quit");
                break;
            }
        }
        close(fd);
    }
    class ThreadData
    {
        public:
        int _sockfd;
        server* _self;

        ThreadData(server* self,int sockfd)
        :_sockfd(sockfd),
        _self(self){}
    };
public:
    server(uint16_t port = gport)
        : _port(port)
    {
        init();
    }
    static void* Execute(void* arg)
    {
        pthread_detach(pthread_self());
        ThreadData* td=static_cast<ThreadData*>(arg);
        td->_self->service(td->_sockfd);
        delete td;
        return nullptr;
    }
    void loop()
    {
        _isrunning = true;
        while (_isrunning)
        {
            struct sockaddr_in client;
            socklen_t len = sizeof(client);
            int fd = accept(_sockfd, (struct sockaddr *)&client, &len);//没有收到连接时就会阻塞

            //多进程版本
            // int n=fork();
            // if(n==0)
            // {
            //     close(_sockfd);
            //     if(fork()>0)
            //         exit(0);
            //     service(fd);
            // }
            // close(fd);
            // waitpid(n,nullptr,0);

            //多线程版本
            //多线程版本不能关闭文件描述符，因为所有线程都是共享的文件描述符
            //为了能够多线程并发执行，因此将新线程设置为分离状态的
            // pthread_t tid;
            // ThreadData* td=new ThreadData(this,fd);
            // pthread_create(&tid,nullptr,Execute,(void*)td);

            //线程池版本
            task_t t=std::bind(&server::service,this,fd);
            Thread_Pool<task_t>::GetInstance()->push(t);
        }
        _isrunning=false;
    }
};