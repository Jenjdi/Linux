#pragma once
#include<netinet/in.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<arpa/inet.h>
#include<iostream>
#include<string>
using namespace std;
class InetAddr
{
    private:
    void to_host(const struct sockaddr_in &addr)
    {
        
        _port=ntohs(addr.sin_port);
        char ip_buf(32);
        inet_ntop(AF_INET,&addr.sin_addr,&ip_buf,sizeof(addr));
        _ip=ip_buf;
    }
    public:
    InetAddr(const struct sockaddr_in& addr)
    :_addr(addr)
    {
        to_host(addr);
    }
    bool operator == (InetAddr& addr)
    {
        return this->_ip==addr._ip&&this->_port==addr._port;
    }
    sockaddr_in Addr()
    {
        return _addr;
    }
    string Ip()
    {
        return _ip;
    }
    uint16_t Port()
    {
        return _port;
    }
    ~InetAddr()
    {
        
    }
    private:
    string _ip;
    uint16_t _port;
    struct sockaddr_in _addr;
    int _listensockfd;
};