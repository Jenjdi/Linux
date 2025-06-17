#pragma once
#include<sys/epoll.h>
#include"log.h"
class MutiPlex
{
    public:
    virtual bool AddEvent(int fd,uint32_t event)=0;
    virtual int Wait(struct epoll_event* evs,int num,int timeout)=0;
    virtual bool ModEvent(int fd,uint32_t event)=0;
    virtual bool DelEvent(int fd)=0;
};
class Epoll:public MutiPlex
{
    private:
    int _epollfd;
    bool EpollModHelper(int fd,uint32_t event,int oper)
    {
        struct epoll_event ev;
        ev.data.fd=fd;
        ev.events=event;
        return epoll_ctl(_epollfd,oper,fd,&ev);
    }
    public:
    Epoll()
    {
        _epollfd=epoll_create(128);
        if(_epollfd<0)
        {
            LOG(ERROR,"epollfd create failed");
        }
        LOG(DEBUG,"epollfd create success");
    }
    bool ModEvent(int fd,uint32_t event)
    {
        return EpollModHelper(fd,event,EPOLL_CTL_MOD);
    }
    bool AddEvent(int fd,uint32_t event) override
    {
        return EpollModHelper(fd,event,EPOLL_CTL_ADD);
    }
    bool DelEvent(int fd) override
    {
        return 0==epoll_ctl(_epollfd,EPOLL_CTL_DEL,fd,nullptr);
    }
    int Wait(struct epoll_event evs[],int num,int timeout) override
    {
        LOG(DEBUG,"ready to wait");
        return ::epoll_wait(_epollfd, evs, num, timeout);
    }
};