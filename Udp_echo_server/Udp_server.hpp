#pragma once
#include<iostream>
#include<memory>
#include<string>
#include<cstring>
#include<sys/socket>
#include<sys/types.h>
#include<netinet/in.h>
#include<arpa/inet.h>
using namespace std;
static const int gsocket=-1;
const uint_16 defaultport=10000;
enum
{
    _SOCKET_ERROR=-1,
    _BIND_ERROR=-2,
};
class nocpy
{
    nocpy(){}
    nocpy(const nocpy &)=delete;
    const nocpy& operator=(const nocpy&)=delete;
    ~nocpy(){}
};
class Udpserver:public nocpy
{
    public:
    Udpserver(const string& localIp,uint_16 _localport=defaultport)
    :_socketfd(gsocket),_localport(defaultport),_localIp(localIp)
    {}
    void init()
    {
        _socketfd=socket(AF_INET,SOCK_DGRAM,0);
        if(_socketfd===0)
        {

            exit(_SOCKET_ERROR);
        }
        struct sockaddr_in local;
        memset(&local,0,sizeof(local));
        local.sin_family=AF_INET;
        local.sin_port=htons(_localport);
        local.sin_addr.s_addr=inet_addr(_localIp.c_str());
        int n=bind(_socketfd,(struct sockaddr*)&local,sizeof(local));
        if(n<0){
            exit(_BIND_ERROR);
        }
        cout<<"bind success"<<endl;
    }
    void start()
    {
        while(true)
        {
            recvfrom(_socketfd,)
        }
    }
    ~Udpserver()
    {

    }
    private:
    int _socketfd;
    uint_16 _localport;
    string _localIp;
};