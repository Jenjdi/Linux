#pragma once
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string>
#include <functional>
#include <memory>
#include"log.h"
class Socket;
using SockPtr = std::shared_ptr<Socket>;
class Socket
{
    // 模板方法模式
public:
    virtual void CreateSocketOrdie() = 0;
    virtual void CreateBindOrdie(uint16_t port) = 0;
    virtual void CreateLisetenerOrdie() = 0;
    virtual SockPtr accepter() = 0;
    virtual bool connector(const std::string &peerip, uint16_t peerport) = 0;
    void BuildListenSocket(uint16_t port)
    {
        CreateSocketOrdie();
        CreateBindOrdie(port);
        CreateLisetenerOrdie();
    }
    void BuildClientSocket(const std::string &peerip, uint16_t peerport)
    {
        CreateSocketOrdie();
        connector(peerip, peerport);
    }
    virtual int Recv(std::string *out) = 0;
    virtual int Send(const std::string &in) = 0;
    virtual int sockfd() = 0;
    virtual void Close() = 0;
};
class TcpSocket:public Socket
{
    private:
    int _sockfd;
    std::string _ip;
    uint16_t _port;
    public:
    TcpSocket(){}
    TcpSocket(int sockfd)
    :_sockfd(sockfd)
    {}
    void CreateSocketOrdie() override
    {
        _sockfd=socket(AF_INET,SOCK_STREAM,0);
        if(_sockfd<0)
        {
            LOG(FATAL,"create sockfd error");

        }
        LOG(DEBUG,"create sockfd success");
    }
    void CreateBindOrdie(uint16_t port) override
    {
        struct sockaddr_in local;
        memset(&local,0,sizeof(local));
        local.sin_addr.s_addr=INADDR_ANY;
        local.sin_family=AF_INET,
        local.sin_port=htons(port);
        int n=bind(_sockfd,(struct sockaddr*)&local,sizeof(local));
        if(n<0)
        {
            LOG(FATAL,"bind failed");
            exit(1);
        }
        LOG(DEBUG,"bind success");
    }
    void CreateLisetenerOrdie()
    {
        int n=listen(_sockfd,8);
        if(n<0)
        {
            LOG(FATAL,"listen failed");
            exit(1);
        }
        LOG(DEBUG,"listening");
    }
    int Recv(std::string *out)
    {
        char buf[4096];
        ssize_t n=recv(_sockfd,buf,sizeof(buf)-1,0);
        if(n<0)
        {
            LOG(FATAL,"recv failed");
            exit(1);
        }
        *out+=buf;
        LOG(DEBUG,"receive success");
        return n;
    }
    int Send(const std::string &in)
    {
        return send(_sockfd,in.c_str(),in.size(),0);
    }
    SockPtr accepter()
    {
        struct sockaddr_in client;
        socklen_t len=sizeof(client);
        int fd=accept(_sockfd,(struct sockaddr*)&client,&len);
        if(fd<0)
        {
            LOG(FATAL,"accept failed");
            exit(1);
        }
        LOG(DEBUG,"accept success");
        return std::make_shared<TcpSocket>(fd);
    }
    bool connector(const std::string &peerip, uint16_t peerport)
    {
        struct sockaddr_in server;
        memset(&server,0,sizeof(server));
        server.sin_family=AF_INET;
        server.sin_port=peerport;
        inet_pton(AF_INET, peerip.c_str(), &server.sin_addr);
        ssize_t n =connect(_sockfd,(struct sockaddr*)&server,sizeof(server));
        if(n<0)
        {
            LOG(FATAL,"connect failed");
            return false;
        }
        LOG(DEBUG,"connect success");
        return true;
    }
    virtual int sockfd()
    {
        return _sockfd;
    }
    virtual void Close()
    {
        ::close(_sockfd);
    }
};