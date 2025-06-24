#pragma once
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unordered_map>
#include <memory>
#include "Connection.h"
#include "Epoll.h"
#include"Thread_Pool.h"
// reactor类似一个connection的容器，用于分发
using func_t=std::function<void(int)>;
class Reactor
{
    const static int gnum = 128;

private:
    std::unordered_map<int, Connection *> _connection;
    std::unique_ptr<Epoll> _epoll;
    handler_t _recv;
    handler_t _send;
    handler_t _except;
    handler_t _accept;
    Thread_Pool<func_t>* _tp;
    
    
    struct epoll_event _revs[gnum];
    bool _isrunning;
    bool IsConnectionExist(int sockfd)
    {
        return _connection.find(sockfd) != _connection.end();
    }

public:
    Reactor()
        : _isrunning(false)
    {
        _epoll = std::make_unique<Epoll>();
        _tp=_tp->GetInstance();
    }
    ~Reactor()
    {
    }
    void AddConnecttion(int fd, uint32_t event, int type)
    {
        Connection *conn = new Connection(fd);
        conn->SetEvent(event);
        conn->SetReactor(this);
        conn->SetType(type);

        if (conn->Type() == ListenConnection)
        {
            conn->registerHandler(_accept, nullptr, nullptr);
        }
        else
        {
            conn->registerHandler(_recv, _send, _except);
        }
        _epoll->AddEvent(conn->Sockfd(), conn->Event());
        _connectnum++;
        _connection.insert(std::make_pair(fd, conn));
    }
    void SetListenHandler(handler_t listen)
    {
        _accept = listen;
    }
    void SetNormalHandler(handler_t recv, handler_t send, handler_t except)
    {
        _send = send;
        _recv = recv;
        _except = except;
    }
    void EnableConnectionReadWrite(int fd,bool readable,bool writable)
    {
        if(!IsConnectionExist(fd))
            return;
        uint32_t event=((readable?EPOLLIN:0)|(writable?EPOLLOUT:0)|EPOLLET);
        _epoll->ModEvent(fd,event);
    }
    void DelConnection(int fd)
    {
        //判断是否存在这个连接
        if(!IsConnectionExist(fd)) return;
        //内核中删除对应的sockfd
        EnableConnectionReadWrite(fd,false,false);
        _epoll->DelEvent(fd);
        //关闭fd
        _connection[fd]->Close();
        //在reactor中移除对connection的关心
        delete _connection[fd];
        _connection.erase(fd);
        _connectnum--;
        LOG(DEBUG,"sockfd quit ");
    }
    void LoopOnce(int timeout,int eventfd)
    {
        // LOG(DEBUG,"wait strat");
        uint64_t flag;
        ::read(eventfd,&flag,sizeof(flag));
        int n = _epoll->Wait(_revs, gnum, timeout);
        LOG(DEBUG, "wait success,retval=%d", n);
        for (int i = 0; i < n; i++)
        {
            int sockfd = _revs[i].data.fd;
            uint32_t event = _revs[i].events;
            LOG(DEBUG, "epoll event: fd=%d, events=%u", sockfd, event);
            if (event & EPOLLIN)
            {
                LOG(DEBUG, "handling recv");
                if (IsConnectionExist(sockfd))
                    _connection[sockfd]->_recv(_connection[sockfd]);
            }
            if (event & EPOLLOUT)
            {
                LOG(DEBUG, "handling send");
                if (IsConnectionExist(sockfd))
                    _connection[sockfd]->_send(_connection[sockfd]);
            }
            if (event & EPOLLERR)
            {
                LOG(DEBUG, "handling except");
                if (IsConnectionExist(sockfd))
                    _connection[sockfd]->_except(_connection[sockfd]);
            }
        }
    }
    void Dispatch()
    {
        _isrunning = true;
        while (true)
        {
            int _eventfd[_connectnum]={-1};
            for(int i=0;i<_connectnum;i++)
            {
                uint64_t flag;
                ::write(_eventfd[i],&flag,sizeof(flag));
                _tp->push(std::bind(Reactor::LoopOnce,this,std::placeholders::_1,_eventfd[i]));
            }
            
        }
        _isrunning = false;
    }
};