#pragma once
#include<iostream>
#include<string>
#include<vector>
#include<functional>
#include"LockGuard.hpp"
#include"thread_pool.hpp"
#include<sys/socket.h>
#include<sys/types.h>
#include"InetAddr.hpp"

using task_t=function<void()>;
using namespace std;
class route
{
    public:
    route()
    {
        pthread_mutex_init(&_routemutex, nullptr);
    }
    void check_user_online(InetAddr& who)
    {
        LockGuard lockguard(&_routemutex);
        for(auto& user:_online_user)
        {
            if(user == who)
            {
                return;
            }
        }
        _online_user.push_back(who);
    }
    void offline(InetAddr& who)
    {
        LockGuard lockguard(&_routemutex);
        auto it=_online_user.begin();
        for(;it!=_online_user.end();it++)
        {
            if(*it==who)
            {
                _online_user.erase(it);
                break;
            }
        }
    }
    void forwardhelper(int socketfd,const string& message,InetAddr who)
    {
        LockGuard lockguard(&_routemutex);
        for(auto user:_online_user)
        {
            struct sockaddr_in peer=user.Addr();
            ssize_t n=sendto(socketfd,message.c_str(),message.size(),0,(struct sockaddr*)&peer,sizeof(peer));
            if(n < 0) {
                perror("sendto error");
            }
        }
        
    }
    void Forward(int socketfd,const std::string& message,InetAddr& addr)
    {
        check_user_online(addr);
        if(message=="Q"||message=="QUIT")
        {
            offline(addr);
        }
        task_t t=bind(&route::forwardhelper,this,socketfd,string(message),addr);
        thread_pool<task_t>::GetInstance()->push(t);
    }
    
    ~route()
    {
        pthread_mutex_destroy(&_routemutex);
    }
    private:
    std::vector<InetAddr> _online_user;
    pthread_mutex_t _routemutex;
};