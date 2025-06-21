#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string>
#include <cstring>
#include <sys/epoll.h>
#include <iostream>
#include<functional>
#include<unistd.h>
#include "dict.h"
const static int gnum = 128;
using func_t=std::function<std::string(std::string)>;
class Server
{
private:
    int _sockfd;
    int _epfd;
    func_t _func;
    epoll_event _evnets[gnum];
    uint16_t _port;

public:
    Server(func_t func,uint16_t port = 1234)
        : _port(port)
        , _func(func)
    {
        _epfd = epoll_create(gnum);
        _sockfd = socket(AF_INET, SOCK_STREAM, 0);
        struct sockaddr_in local;
        memset(&local, 0, sizeof(local));
        local.sin_port = htons(_port);
        local.sin_addr.s_addr = INADDR_ANY;
        local.sin_family = AF_INET;
        int n = bind(_sockfd, (struct sockaddr *)&local, sizeof(local));
        if (n < 0)
        {
            std::cout << "Bind Failed" << std::endl;
            exit(-1);
        }
        int m = listen(_sockfd, 8);
    }
    ~Server()
    {
        close(_sockfd);
        
    }
    void Init()
    {
        epoll_event ev;
        ev.events = EPOLLIN;
        ev.data.fd = _sockfd;
        int n = epoll_ctl(_epfd, EPOLL_CTL_ADD, _sockfd, &ev);
        if (n < 0)
        {
            std::cout << "EPOLL ADD FAILED" << std::endl;
        }
    }
    void Loop()
    {
        int timeout = -1;
        while (true)
        {
            int n = epoll_wait(_epfd, _evnets, gnum, timeout);
            switch (n)
            {
            case -1:
                std::cout << "epoll error"<<std::endl;
                break;
            case 0:
                std::cout << "epoll timeout"<<std::endl;
                break;
            default:
                std::cout << "event happened"<<std::endl;
                Handler(n);
                break;
            }
        }
    }
    void Handler(int n)
    {
        for (int i = 0; i < n; i++)
        {
            if (_evnets[i].events & EPOLLIN)
            {
                if (_evnets[i].data.fd == _sockfd)
                {
                    struct sockaddr_in client;
                    socklen_t len = sizeof(client);
                    int fd = accept(_sockfd, (struct sockaddr *)&client, &len);
                    if(fd<0)
                    {
                        std::cout<<"get fd failed"<<std::endl;
                    }
                    epoll_event ev;
                    ev.data.fd=fd;
                    ev.events=EPOLLIN;
                    epoll_ctl(_epfd,EPOLL_CTL_ADD,fd,&ev);
                }
                else
                {
                    char buf[128];
                    int n=recv(_evnets[i].data.fd,buf,sizeof(buf)-1,0);
                    if(n>0)
                    {
                        buf[n]=0;
                        std::cout<<"Client#"<<buf<<std::endl;
                        std::string ret=_func(buf);
                        send(_evnets[i].data.fd,ret.c_str(),ret.size(),0);
                    }
                    else if(n==0)
                    {
                        epoll_ctl(_epfd, EPOLL_CTL_DEL, _evnets[i].data.fd, nullptr);
                        close(_evnets[i].data.fd);
                        std::cout<<"Client:"<<_evnets[i].data.fd<<" Quit"<<std::endl;
                    }
                }
            }
        }
    }
};