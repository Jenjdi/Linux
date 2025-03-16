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
using namespace std;
static const int gsocket = -1;
const uint16_t defaultport = 10000;
enum
{
    _SOCKET_ERROR = -1,
    _BIND_ERROR = -2,
};
class nocopy
{
public:
    nocopy(){}
    ~nocopy(){}
    nocopy(const nocopy&) = delete;
    const nocopy& operator=(const nocopy&) = delete;
};
class Udpserver : public nocopy
{
public:
    Udpserver(const string &localIp, uint16_t _localport = defaultport)
        : _socketfd(gsocket), _localport(defaultport), _localIp(localIp),_isrunning(false)
    {}
    void init()
    {
        _socketfd = socket(AF_INET, SOCK_DGRAM, 0);
        if (_socketfd == 0)
        {
            cout<<"Socket Create Success"<<endl;
            exit(_SOCKET_ERROR);
        }
        cout<<"Socket Create Success"<<endl;
        struct sockaddr_in local;
        memset(&local, 0, sizeof(local));
        local.sin_family = AF_INET;
        local.sin_port = htons(_localport);
        local.sin_addr.s_addr = inet_addr(_localIp.c_str());
        int n = bind(_socketfd, (struct sockaddr *)&local, sizeof(local));
        if (n < 0)
        {
            cout<<"Bind Failed"<<endl;
            exit(_BIND_ERROR);
        }
        cout << "Bind Success" << endl;
    }
    void start()
    {
        _isrunning=true;
        
        char inbuffer[1024];
        while (_isrunning)
        {
            struct sockaddr_in remote;
            socklen_t len=sizeof(remote);
            ssize_t n=recvfrom(_socketfd,inbuffer,sizeof(inbuffer)-1,0,(struct sockaddr*)&remote,&len);
            if(n>0)
            {
                inbuffer[n]=0;
                string msg="[UDP Server Echo]";
                msg+=inbuffer;
                sendto(_socketfd,msg.c_str(),msg.size(),0,(struct sockaddr*)&remote,len);

            }
        }
    }
    ~Udpserver()
    {
        if(_socketfd>gsocket) close(_socketfd);
    }

private:
    int _socketfd;
    uint16_t _localport;
    string _localIp;
    bool _isrunning;
};