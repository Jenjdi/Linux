#pragma once
#include <iostream>
#include <cstring>
#include <string>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "InetAddr.hpp"
#include <sys/wait.h>
#include "log.h"
const int gbacklog = 8; // 连接队列的大小，一般不要太大
const int gport=1234;
using namespace std;
enum
{
    SOCKERR = 1,
    BINDERR = 0,
    LISERR = 2,
    ACCERR = 3
};
class tcp_server
{
private:
    uint16_t _port;
    int _listensock;
    bool _isrunning = false;
    void init()
    {
        _listensock = socket(AF_INET, SOCK_STREAM, 0);
        if (_listensock < 0)
        {
            LOG(FATAL, "sock create failed");
            exit(SOCKERR);
        }
        LOG(DEBUG,"sock create success");
        struct sockaddr_in local;
        memset(&local, 0, sizeof(local));
        local.sin_addr.s_addr = INADDR_ANY;
        local.sin_port = htons(_port);
        local.sin_family = AF_INET;
        int n = bind(_listensock, (struct sockaddr *)&local, sizeof(local));
        if (n < 0)
        {
            LOG(FATAL, "bind failed");
            exit(BINDERR);
        }
        // tcp是面向连接的，因此需要建立连接，将套接字设置为listen状态
        if (listen(_listensock, gbacklog) < 0)
        {
            LOG(FATAL, "listen failed");
            exit(LISERR);
        }
    }

public:
    tcp_server(uint16_t port=gport)
    :_port(port)
    {
        init();
        LOG(DEBUG,"init success");
    }
    ~tcp_server()
    {
    }

    void loop()
    {
        _isrunning = true;
        while (_isrunning)
        {
            struct sockaddr_in client;
            socklen_t len = sizeof(client);
            int fd = accept(_listensock, (sockaddr *)&client, &len); // 这里的_listensock只进行获取新连接，不进行读写操作，真正进行网络服务的是获取到的fd
            if (fd < 0)
            {
                LOG(FATAL, "accept failed");
                continue;
            }
            InetAddr addr(client);
            LOG(DEBUG,"get new link client info:%s\n",addr.Ip().c_str());
            service(fd,addr);
        }
        _isrunning=false;
    }
    void service(int sockfd,InetAddr addr)
    {
        while(true)
        {
            char inbuffer[128];
            ssize_t n=read(sockfd,inbuffer,sizeof(inbuffer)-1);
            if(n>0)
            {
                inbuffer[n]='\0';
                string echo_string="[server echo]#";
                echo_string+=inbuffer;
                write(sockfd,echo_string.c_str(),echo_string.size());
            }
            else if(n==0)
            {
                LOG(INFO,"client quit");
                break;
            }
            else
            {
                LOG(ERROR,"read error:%s\n",addr.Ip().c_str());
                break;
            }
        }
        close(sockfd);
    }
};