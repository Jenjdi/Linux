#include "nocopy.h"
#include "log.h"
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <string>
#include <cstring>
#include "InetAddr.h"
#include "cond.h"
#include "routine.h"
uint16_t gport = 1234;
int gsockfd = -1;

class server : public nocopy
{
private:
    int _sockfd = gsockfd;
    uint16_t _port;
    struct sockaddr_in _local;
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

        _local.sin_family = AF_INET;
        _local.sin_addr.s_addr = INADDR_ANY;
        _local.sin_port = htons(_port);
        int n = bind(_sockfd, (sockaddr *)&_local, len);
        if (n < 0)
        {
            LOG(FATAL, "bind failed");
            exit(BINDERR);
        }
        LOG(DEBUG, "bind success");
    }

public:
    server(uint16_t port = gport)
        : _port(port)
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

        char rbuf[128];
        while (1)
        {
            struct sockaddr_in peer;
            socklen_t plen=sizeof(peer);
            int rnum = recvfrom(_sockfd, rbuf, sizeof(rbuf) - 1, 0, (sockaddr *)&peer, &plen);
            if (rnum < 0)
            {
                LOG(ERROR, "receive failed");
            }
            InetAddr addr(peer);
            rbuf[rnum] = 0;
            std::cout << "[" << inet_ntoa(peer.sin_addr) << ":" << _port << "]# " << rbuf << std::endl;
            char sbuff[256];
            snprintf(sbuff, sizeof(sbuff) - 1, "[%s:%d]#", inet_ntoa(peer.sin_addr), _port);
            routine r(addr);
            r.forward(_sockfd, addr, sbuff);
        }
    }
};