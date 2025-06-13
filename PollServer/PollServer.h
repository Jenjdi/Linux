#pragma once
#include "log.h"
#include "Socket.h"
#include <poll.h>
class PollServer
{
    const static int gnum = sizeof(fd_set) * 8;
    const static int defaultfd = -1;

private:
    uint16_t _port;
    struct pollfd fds[gnum];
    std::unique_ptr<TcpSocket> _listenfd;

public:
    PollServer(uint16_t port)
        : _port(port)
    {
        _listenfd = std::make_unique<TcpSocket>();
    }
    ~PollServer() {}
    void initfd()
    {
        for (int i = 0; i < gnum; i++)
        {
            fds[i].fd = defaultfd;
            fds[i].events = 0;
            fds[i].revents = 0;
        }
        fds[0].fd = _listenfd->sockfd();
        fds[0].events = POLLIN;
    }
    void Acceptor()
    {
        int sockfd=_listenfd->accepter();
        if(sockfd>0)
        {
            bool flag=false;
            for(int i=1;i<gnum;i++)
            {
                if(fds[i].fd==defaultfd)
                {
                    flag=true;
                    fds[i].fd=sockfd;
                    fds[i].events=POLLIN;
                    break;
                }
            }
            if(!flag)
            {
                //扩容，添加
                LOG(ERROR,"fds is full");
            }
        }
    }
    void HandleIO(int pos)
    {
        char buf[128];
        int n=recv(fds[pos].fd,buf,sizeof(buf)-1,0);
        if(n>0)
        {
            buf[n]=0;
            std::string echo="[server]:";
            echo+=buf;
            send(fds[pos].fd,echo.c_str(),echo.size(),0);
        }
    }
    void Handler()
    {
        for (int pos = 0; pos < gnum; pos++)
        {
            if (fds[pos].fd == defaultfd)
                continue;
            if (fds[pos].revents & POLLIN)
            {
                if (_listenfd->sockfd() == fds[pos].fd)
                    Acceptor();
                else
                    HandleIO(pos);
            }
        }
    }
    void loop()
    {
        int timeout = 10;
        while (true)
        {
            int n = poll(fds, gnum, timeout);
            switch (n)
            {
            case -1:
                LOG(ERROR, "poll failed");
                break;
            case 0:
                LOG(ERROR, "time out");
                break;
            default:
                Handler();
                break;
            }
        }
    }
};