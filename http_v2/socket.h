#pragma once
#include<arpa/inet.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<string>
#include"log.h"
class TcpServer
{
    private:
    int _listenfd;
    public:
    TcpServer()
    {}
    void CreateSock()
    {
        _listenfd=socket(AF_INET,SOCK_STREAM,0);
        int opt = 1;
        setsockopt(_listenfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
        if(_listenfd<0)
        {
            LOG(FATAL,"create listenfd failed");
        }
    }
    void ServerBind(uint16_t port)
    {
        struct sockaddr_in local;
        local.sin_addr.s_addr=INADDR_ANY;
        local.sin_family=AF_INET;
        local.sin_port=htons(port);
        int n=bind(_listenfd,(struct sockaddr*)&local,sizeof(local));
        if(n<0)
        {
            LOG(FATAL,"Bind Failed");
        }
    }
    void ServerListen()
    {
        int n=listen(_listenfd,8);
        if(n<0)
        {
            LOG(FATAL,"Listen failed");
        }
    }
    int Accept()
    {
        struct sockaddr_in sock;
        socklen_t len=sizeof(sock);
        int fd=accept(_listenfd,(struct sockaddr*)&sock,&len);
        if(fd<0)
        {
            LOG(FATAL,"accept fd failed");
        }
        return fd;
    }
    int Recv(int recfd,std::string& rec)
    {
        char buf[4096];
        int n=recv(recfd,buf,sizeof(buf)-1,0);
        if(n<0)
        {
            LOG(ERROR,"recv failed");
            exit(-1);
        }
        buf[n]=0;
        rec+=buf;
        return n;
    }
    int Send(int senfd,const std::string& message)
    {
        return send(senfd,message.c_str(),message.size(),0);
    }
    int SockFd()
    {
        return _listenfd;
    }
    int Close()
    {
        return close(_listenfd);
    }
};