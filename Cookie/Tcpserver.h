#pragma once
#include "Socket.h"
#include <pthread.h>
#include<memory>
using service_t = std::function<std::string(std::string &)>;
class Server
{
private:
    SockPtr _sock;
    uint16_t _port;
    service_t _service;
    class ThreadData
    {
    public:
        SockPtr _sockfd;
        Server *_self;
        ThreadData(Server *self, SockPtr sockfd)
            : _sockfd(sockfd),
              _self(self) {}
    };
    static void *Execute(void *arg)
    {
        pthread_detach(pthread_self());
        ThreadData *td = static_cast<ThreadData *>(arg);
        std::string message;
        ssize_t n = td->_sockfd->Recv(&message);
        if (n > 0)
        {
            std::string resp;
            resp = td->_self->_service(message);
            td->_sockfd->Send(resp);
        }
        td->_sockfd->Close();
        delete td;
        return nullptr;
    }

public:
    Server(service_t service, uint16_t port = 1234)
        : _service(service),
          _port(port),
          _sock(std::make_shared<TcpSocket>())
    {
        _sock->BuildListenSocket(_port);
    }
    void loop()
    {
        while (true)
        {
            SockPtr newsock = _sock->accepter();
            pthread_t tid;
            ThreadData *td = new ThreadData(this, newsock);
            pthread_create(&tid, nullptr, Execute, (void *)td);
        }
    }
};