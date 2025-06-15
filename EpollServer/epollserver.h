#include "log.h"
#include "Socket.h"
#include <sys/epoll.h>
#include <memory>
class EpollServer
{
    const static int size = 128;
    const static int gnum = 128;

private:
    std::unique_ptr<TcpSocket> _listensock;
    int _epfd;
    uint16_t _port;
    epoll_event evnets[gnum];

public:
    EpollServer(uint16_t port)
        : _port(port)
    {
        _listensock = std::make_unique<TcpSocket>();
        _epfd = epoll_create(size);
        _listensock->BuildListenSocket(_port);
    }
    void init()
    {
        epoll_event ev;
        ev.events = EPOLLIN;
        ev.data.fd = _listensock->sockfd();
        int n = epoll_ctl(_epfd, EPOLL_CTL_ADD, _listensock->sockfd(), &ev);
        if (n < 0)
        {
            LOG(ERROR, "epoll_ctl error");
            exit(-1);
        }
        LOG(DEBUG, "epoll_ctl success");
    }
    void Accept()
    {
    }
    void Handler(int n)
    {
        for (int i = 0; i < n; i++)
        {
            if (evnets[i].events && EPOLLIN)
            {
                if (evnets[i].data.fd == _listensock->sockfd())
                {
                    int fd = _listensock->accepter();
                    if (fd < 0)
                    {
                        LOG(FATAL, "fd accept failed");
                        exit(1);
                    }
                    // 拿到fd后不能直接读取，因为此时读缓冲区还没有数据，必须等到读就绪才能开始读取
                    // 将这个fd交给epoll进行管理来得知什么时候就绪
                    epoll_event ev;
                    ev.data.fd = fd;
                    ev.events = EPOLLIN;
                    epoll_ctl(_epfd, EPOLL_CTL_ADD, fd, &ev);
                }
                else
                {
                    char buf[128];
                    int rnum = recv(evnets[i].data.fd, buf, sizeof(buf) - 1, 0);
                    if (n > 0)
                    {
                        buf[n] = 0;
                        LOG(DEBUG, "recv success");
                        // 写操作默认就是就绪的，因此不需要等待就绪
                        std::string ret = "[server]:";
                        ret += buf;
                        send(evnets[i].data.fd, ret.c_str(), ret.size(), 0);
                    }
                    else if (n == 0)
                    {
                        LOG(ERROR, "client quit");
                        epoll_ctl(_epfd, EPOLL_CTL_DEL, evnets[i].data.fd, evnets);
                        close(evnets[i].data.fd);
                    }
                    else
                    {
                        LOG(ERROR, "recv failed");
                        epoll_ctl(_epfd, EPOLL_CTL_DEL, evnets[i].data.fd, evnets);
                        close(evnets[i].data.fd);
                    }
                }
            }
        }
    }
    void loop()
    {
        int timeout = -1;
        while (true)
        {
            int n = epoll_wait(_epfd, evnets, gnum, timeout);
            switch (n)
            {
            case -1:
                LOG(ERROR, "epoll error");
                break;
            case 0:
                LOG(INFO, "epoll timeout");
                break;
            default:
                LOG(INFO, "event happened");
                Handler(n);
                break;
            }
        }
    }
};