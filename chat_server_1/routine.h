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
using service_t = std::function<void(int, const std::string &message, InetAddr &who)>;
class routine
{
    private:
    std::vector<InetAddr> _userlist;
    public:
    routine(const InetAddr& who)
    {
        _userlist.push_back(who);
    }
    void checkuer(InetAddr& who)
    {
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
        for(auto e:_userlist)
        {
            struct sockaddr_in addr=e.Addr();
            int len = sizeof(addr);
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
        func_t t=std::bind(&routine::forwardhelper,this,sockfd,message);
        Thread_Pool<func_t>::GetInstance()->push(t);
    }
};