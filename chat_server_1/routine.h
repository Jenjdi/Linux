#pragma once
#include<netinet/in.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<arpa/inet.h>
#include<functional>
#include<string>
#include<vector>
#include"InetAddr.h"
#include"Thread_Pool.h"

using task_t=std::function<void()>;
class routine
{
    private:
    std::vector<InetAddr> _userlist;
    pthread_mutex_t _mutex;
    public:
    routine()
    {
        pthread_mutex_init(&_mutex,nullptr);
    }
    ~routine()
    {
        pthread_mutex_destroy(&_mutex);
    }
    void checkuer(InetAddr& who)
    {
        LockGuard lockguard(&_mutex);
        for(auto e:_userlist)
        {
            if(e==who)
            {
                return;
            }
        }
        _userlist.push_back(who);
    }
    void offline(InetAddr& who)
    {
        LockGuard lockguard(&_mutex);
        auto it=_userlist.begin();
        for(;it<_userlist.end();it++)
        {
            if(*it==who)
            {
                _userlist.erase(it);
                break;
            }
        }
    }
    void forwardhelper(int sockfd,const std::string& message)
    {
        LockGuard lockguard(&_mutex);
        for(auto e:_userlist)
        {
            struct sockaddr_in addr=e.Addr();
            socklen_t len = sizeof(addr);
            int n=sendto(sockfd,message.c_str(),message.size(),0,(sockaddr*)&addr,len);
        }
    }
    void forward(int sockfd,InetAddr& who,const std::string& message)
    {
        checkuer(who);
        if(message=="q"||message=="quit")
        {
            offline(who);
        }
        //forwardhelper(sockfd,message);
        task_t t=std::bind(&routine::forwardhelper,this,sockfd,message);
        Thread_Pool<task_t>::GetInstance()->push(t);
    }
};