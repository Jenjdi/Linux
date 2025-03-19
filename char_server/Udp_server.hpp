#pragma once
#include <iostream>
#include <memory>
#include <string>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <functional>
#include "InetAddr.hpp"
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
    nocopy() {}
    ~nocopy() {}
    nocopy(const nocopy &) = delete;
    const nocopy &operator=(const nocopy &) = delete;
};
using service_t = std::function<void(int, const std::string &message, InetAddr &who)>;
class Udpserver : public nocopy
{
public:
    Udpserver(service_t func,uint16_t localport = defaultport)//如果一个参数有默认值，那么它之后的所有参数都必须有默认值，因此若将service_t放在后面，会导致报错
        : _socketfd(gsocket), _localport(localport), _isrunning(false), _func(func)
    {
    }
    void init()
    {
        _socketfd = socket(AF_INET, SOCK_DGRAM, 0);
        if (_socketfd == 0)
        {
            cout << "Socket Create Failed" << endl;
            exit(_SOCKET_ERROR);
        }
        cout << "Socket Create Success" << endl;
        struct sockaddr_in local;
        memset(&local, 0, sizeof(local));
        local.sin_family = AF_INET;
        local.sin_port = htons(_localport);
        // local.sin_addr.s_addr = inet_addr(_localIp.c_str());
        local.sin_addr.s_addr = INADDR_ANY; // 接收来着所有IP的消息
        int n = bind(_socketfd, (struct sockaddr *)&local, sizeof(local));
        if (n < 0)
        {
            cout << "Bind Failed" << endl;
            exit(_BIND_ERROR);
        }
        cout << "Bind Success" << endl;
    }
    void start()
    {
        _isrunning = true;

        char message[1024];
        while (_isrunning)
        {
            struct sockaddr_in remote;
            socklen_t len = sizeof(remote);
            ssize_t n = recvfrom(_socketfd, message, sizeof(message) - 1, 0, (struct sockaddr *)&remote, &len);
            if (n > 0)
            {
                InetAddr addr(remote);
                message[n] = 0;
                cout << "[" << addr.Ip() << ":" << addr.Port() << "]" << message << endl;
                // string msg="[UDP Server Received]";
                // sendto(_socketfd,msg.c_str(),msg.size()-1,0,(struct sockaddr*)&remote,len);
                _func(_socketfd, message, addr);
            }
            else
            {
                cout << "receive error" << endl;
                break;
            }
        }
    }
    ~Udpserver()
    {
        if (_socketfd > gsocket)
            close(_socketfd);
    }

private:
    int _socketfd;
    uint16_t _localport;
    // string _localIp;
    bool _isrunning;
    service_t _func;
};