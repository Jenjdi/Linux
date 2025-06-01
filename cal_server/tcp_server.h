#pragma once
#include <iostream>
#include <cstring>
#include <string>
#include <memory>

#include <sys/types.h>
#include <sys/wait.h>

#include <unistd.h>
#include <pthread.h>

#include "socket.h"

using task_t = std::function<void(ns_sock::SockPtr)>;
class server
{
private:
    ns_sock::SockPtr _listensock;
    uint16_t _port;
    bool _isrunning = false;
    task_t _task;
    class ThreadData
    {
    public:
    
        ns_sock::SockPtr _sockfd;
        server *_self;
        ThreadData(server *self, ns_sock::SockPtr sockfd)
            : _sockfd(sockfd),
              _self(self) {}
    };
    static void *Execute(void *arg)
    {
        pthread_detach(pthread_self());
        ThreadData *td = static_cast<ThreadData *>(arg);
        LOG(DEBUG,"execute");
        td->_self->_task(td->_sockfd);
        td->_sockfd->Close();
        delete td;
        return nullptr;
    }
public:
    server(task_t task,uint16_t port=1234)
        : _port(port),
          _listensock(std::make_shared<ns_sock::TCPSocket>()), _isrunning(false),
          _task(task)
    {
        _listensock->BuildListenSocket(_port);
    }
    
    void loop()
    {
        _isrunning = true;
        while (_isrunning)
        {
            ns_sock::SockPtr newsock = _listensock->accepter();
            pthread_t tid;
            ThreadData *td = new ThreadData(this, newsock);
            pthread_create(&tid, nullptr, Execute, (void *)td);
        }
        _isrunning = false;
    }
};