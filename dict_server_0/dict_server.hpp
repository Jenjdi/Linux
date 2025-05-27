#pragma once
#include <iostream>
#include <memory>
#include <string>
#include <cstring>
#include<unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include<functional>
#include"InetAddr.hpp"
#include"nocopy.hpp"
using namespace std;
static const uint16_t defaultport=10000; 
const int gsocket=-1;
using func_t=function<string(string)>;
class dict_server:public nocopy
{
    public:
    dict_server(func_t func,uint16_t port=defaultport)
    :_port(port),_isrunning(false),_func(func)
    {}
    void init()
    {
        _socketfd=socket(AF_INET,SOCK_DGRAM,0);
        if(_socketfd==0)
        {
            cout<<"Socket Create Failed"<<endl;
        }
        cout<<"Socket Create Success"<<endl;
        struct sockaddr_in local;
        memset(&local,0,sizeof(local));
        local.sin_addr.s_addr=INADDR_ANY;
        local.sin_family=AF_INET;
        local.sin_port=htons(_port);
        int m=bind(_socketfd,(struct sockaddr*)&local,sizeof(local));
        if(m<0)
        {
            cout<<"Bind Failed"<<endl;
            exit(-1);
        }
        cout<<"Bind Success"<<endl;
    }
    void start()
    {
        _isrunning=true;
        struct sockaddr_in remote;
        socklen_t len=sizeof(remote);
        while (_isrunning)
        {
            char buffer[1024];
            ssize_t n=recvfrom(_socketfd,buffer,sizeof(buffer)-1,0,(struct sockaddr*)&remote,&len);
            if(n>0)
            {
                InetAddr addr(remote);
                buffer[n]=0;
                cout<<"["<<addr.Ip()<<":"<<addr.Port()<<"]#"<<buffer<<endl;;
                string result=_func(buffer);
                sendto(_socketfd,result.c_str(),sizeof(result)-1,0,(struct sockaddr*)&remote,len);
            }
        }
        
    }
    ~dict_server()
    {
        if(_socketfd>gsocket)
        {
            close(_socketfd);
        }
    }
    private:
    int _socketfd;
    uint16_t _port;
    bool _isrunning;
    func_t _func;
};