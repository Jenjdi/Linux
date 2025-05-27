#include "nocopy.h"
#include "log.h"
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include<functional>
#include <string>
#include <cstring>
#include "InetAddr.h"
#include "cond.h"
#include "routine.h"
uint16_t gport = 1234;
int gsockfd = -1;
using service_t=std::function<void(int, InetAddr&, const std::string&)>;
class server : public nocopy
{
private:
    int _sockfd = gsockfd;
    uint16_t _port;
    struct sockaddr_in _local;
    service_t _func;
    socklen_t len = sizeof(_local);
    void init()
    {
        _sockfd = socket(AF_INET, SOCK_DGRAM, 0);
        if (_sockfd < 0)
        {
            LOG(FATAL, "create sockfd failed");
            exit(SOCKERR);
        }
        LOG(DEBUG, "create sockfd success");
        memset(&_local,0,sizeof(_local));
        _local.sin_family = AF_INET;
        _local.sin_addr.s_addr = INADDR_ANY;
        _local.sin_port = htons(_port);
        int n = bind(_sockfd, (struct sockaddr *)&_local, len);
        if (n < 0)
        {
            LOG(FATAL, "bind failed");
            exit(BINDERR);
        }
        LOG(DEBUG, "bind success");
    }

public:
    server(service_t func,uint16_t port = gport)
        : _port(port),
        _func(func)
    {
        memset(&_local, 0, len);
        init();
    }
    ~server()
    {
        close(_sockfd);
    }

    void start()
    {
        
        char message[1024];
        while (1)
        {
            struct sockaddr_in peer;
            socklen_t plen=sizeof(peer);
            int rnum = recvfrom(_sockfd, message, sizeof(message) - 1, 0, (struct sockaddr *)&peer, &plen);
            if (rnum < 0)
            {
                LOG(ERROR, "receive failed");
            }
            InetAddr addr(peer);
            message[rnum] = 0;
            cout << "[" << addr.Ip() << ":" << addr.Port() << "]" << message << endl;
            _func(_sockfd,addr,message);
        }
    }
};