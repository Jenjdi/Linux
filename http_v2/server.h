#pragma once
#include"socket.h"
#include"http.h"
#include<memory>
#include<pthread.h>
using service_t = std::function<std::string(std::string &)>;
class Server
{
    private:
    uint16_t _port;
    service_t _service;
    std::unique_ptr<TcpServer> _server;
    
    int _fd;
    static void *Execute(void *arg)
    {
        pthread_detach(pthread_self());
        Server* s=static_cast<Server*>(arg);
        std::string message;
        ssize_t n = s->_server->Recv(s->_fd,message);//由于TCP是面向字节流的，因此可能读取到不完整的请求
        if (n > 0)
        {
            std::string resp;
            resp = s->_service(message);
            s->_server->Send(s->_fd,resp);
        }
        return nullptr;
    }
    public:
    Server(service_t service,uint16_t port=1234)
    :_port(port),
    _service(service)
    {
        _server=std::make_unique<TcpServer>();
    }
    void init()
    {
        _server->CreateSock();
        _server->ServerBind(_port);
        _server->ServerListen(); 
    }
    
    void start()
    {
        while(true)
        {
            pthread_t _tid;
            _fd=_server->Accept();
            pthread_create(&_tid,nullptr,Execute,this);
        }
    }
    
};