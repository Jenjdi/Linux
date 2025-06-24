#include<sys/epoll.h>
#include<unistd.h>
#include<memory>
#include"socket.h"
#include"log.h"
const static int gnum=128;
class Epoll
{
    private:
    int _epfd;
    
    public:
    Epoll()
    {
        _epfd=epoll_create(128);
        if(_epfd<0)
        {
            LOG(FATAL,"create epoll failed");
        }
    }
    void AddEvent(int fd,uint32_t event)
    {
        epoll_event ev;
        ev.data.fd=fd;
        ev.events=event;
        int n=epoll_ctl(_epfd,EPOLL_CTL_ADD,fd,&ev);
        if(n<0)
        {
            LOG(FATAL,"Add Event Failed");
        }
    }
    void DelEvent(int fd,uint32_t event)
    {
        epoll_event ev;
        ev.data.fd=fd;
        ev.events=event;
        int n=epoll_ctl(_epfd,EPOLL_CTL_DEL,fd,&ev);
    }
    int Wait(struct epoll_event ev[],int maxnum,int timeout)
    {
        LOG(DEBUG,"Epoll is waiting");
        return epoll_wait(_epfd,ev,maxnum,timeout);
    }

};