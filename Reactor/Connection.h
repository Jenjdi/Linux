#pragma once
#include<iostream>
#include<functional>
#include"log.h"
#include<netinet/in.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unordered_map>
#include<string>
class Connection;
class Reactor;
using handler_t=std::function<void(Connection*)>;
#define ListenConnection 1
#define NormalConnection 0
class Connection
{
    private:
    int _sockfd;//连接建立完成后的fd
    std::string _inbuff;//接收缓冲区
    std::string _outbuff;//发送缓冲区
    
    uint32_t _event;
    int _type;
    public:
    Reactor* _reactor;
    handler_t _recv;//处理接收
    handler_t _send;//处理发送
    handler_t _except;//处理异常
    Connection(int fd)
    :_sockfd(fd){}
    ~Connection(){}
    void registerHandler(handler_t recv,handler_t send,handler_t except)
    {
        _recv=recv;
        _send=send;
        _except=except;
        LOG(DEBUG,"handler registered");
    }
    void HandleRecv()
    {
        if(_recv==nullptr)
            LOG(ERROR,"recv is null");
        _recv(this);
    }
    void AppendInBuff(const std::string& inbuf)
    {
        _inbuff+=inbuf;
    }
    void AppendOutBuff(const std::string& outbuf)
    {
        _outbuff+=outbuf;
    }
    void HandleSend()
    {
        if(_send==nullptr)
            LOG(ERROR,"send is null");
        _send(this);
    }
    void DiscardOutBuff(int n)
    {
        _outbuff.erase(0,n);
    }
    void Close()
    {
        if(_sockfd>0) close(_sockfd);
    }
    std::string& Inbuff()
    {
        return _inbuff;
    }
    std::string& OutBuff()
    {
        return _outbuff;
    }
    void HandleExcept()
    {
        if(_except==nullptr)
            LOG(ERROR,"except is null");
        _except(this);
    }
    int Type()
    {
        return _type;
    }
    void SetType(int type)
    {
        _type=type;
    }
    void SetReactor(Reactor* react)
    {
        _reactor=react;
    }
    void SetEvent(uint32_t event)
    {
        _event=event;
    }
    int Sockfd()
    {
        return _sockfd;
    }
    uint32_t Event()
    {
        return _event;
    }
    
};