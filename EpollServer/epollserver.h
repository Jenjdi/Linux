#include"log.h"
#include"Socket.h"
#include<sys/epoll.h>
#include<memory>
class EpollServer
{
    const static int size=128;
    const static int gnum=128;
    private:
    std::unique_ptr<TcpSocket> _listensock;
    int _epfd;
    uint16_t _port;
    public:
    EpollServer(uint16_t port)
    :_port(port)
    {
        _listensock=std::make_unique<TcpSocket>();
        _epfd=epoll_create(size);
        _listensock->BuildListenSocket(_port);
    }
    void init()
    {
        epoll_event ev;
        ev.events=EPOLLIN;
        ev.data.fd=_listensock->sockfd();
        int n=epoll_ctl(_epfd,EPOLL_CTL_ADD,_listensock->sockfd(),&ev);
        if(n<0)
        {
            LOG(ERROR,"epoll_ctl error");
            exit(-1);
        }
        LOG(DEBUG,"epoll_ctl success");
    }
    void loop()
    {
        int timeout=-1;
        while(true)
        {
            epoll_event evnets[gnum];
            int n=epoll_wait(_epfd,evnets,gnum,timeout);
            switch(n)
            {
                case -1:
                LOG(ERROR,"epoll error");
                break;
                case 0:
                LOG(INFO,"epoll timeout");
                break;
                default:
                LOG(INFO,"event happened");
                break;
            }
        }
    }
};