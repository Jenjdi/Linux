#pragma once
#include "Socket.h"
#include <memory>
#include <fcntl.h>
class Listen
{
private:
    std::unique_ptr<TcpSocket> _listensock;

public:
    Listen(int port)
    {
        _listensock = std::make_unique<TcpSocket>();
        _listensock->BuildListen(port);
    }
    int listenfd()
    {
        return _listensock->SockFD();
    }
    void Accept(Connection *conn)
    {
        errno = 0;
        while (true)
        {
            int code = 0;
            int newfd = _listensock->Accept(&code);
            if (newfd > 0)
            {
                conn->_reactor->AddConnecttion(newfd,EPOLLIN|EPOLLET,NormalConnection);
            }
            else
            {
                if (code == EWOULDBLOCK)
                {
                    LOG(DEBUG,"data empty");
                    break;
                }
                else if(code==EINTR)
                {
                    LOG(DEBUG,"code is EINTR");
                    continue;
                }
                else
                {
                    LOG(ERROR,"get newfd failed");
                    break;
                }
            }
        }
    }
    ~Listen() {}
};