#include<sys/epoll.h>
class Epoll
{
    private:
    int _size=8;
    int _epfd;
    
    public:
    Epoll()
    :_epfd(epoll_create(_size)){}
    void AddEvent(int sockfd,uint32_t event)
    {
        struct epoll_event ev;
        ev.data.fd=sockfd;
        ev.events=event;
        epoll_ctl(_epfd,EPOLL_CTL_ADD,sockfd,&ev);
    }
};