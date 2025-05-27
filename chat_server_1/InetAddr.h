#pragma once
#include<netinet/in.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<arpa/inet.h>
#include<string>
class InetAddr
{
    void to_host(const struct sockaddr_in& addr)
    {
        _port=ntohs(addr.sin_port);
        _ip=inet_ntoa(addr.sin_addr);
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
    std::string Ip()
    {
        return _ip;
    }
    uint16_t Port()
    {
        return _port;
    }
    
    private:
    std::string _ip;
    uint16_t _port;
    struct sockaddr_in _addr;
};