#include "nocopy.hpp"
#include <string.h>
#include <string>
#include <functional>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "dict.h"
#include "log.h"
using func_t = std::function<std::string(std::string)>;
const int gsock = -1;
class Server : public nocopy
{
private:
    int _sockfd;
    in_addr_t _ip;
    uint16_t _port;
    func_t _func;
    void InetAddr()
    {
        _port = htons(_port);
    }

public:
    Server(func_t func, uint16_t port=1234)
        : _port(port),
          _func(func),
          _ip(INADDR_ANY)
    {
        InetAddr();
    }
    ~Server()
    {
        if (_sockfd != gsock)
        {
            close(_sockfd);
        }
    }
    void init()
    {
        _sockfd = socket(AF_INET, SOCK_DGRAM, 0);
        if (_sockfd < 0)
        {
            LOG(FATAL, "socketfd create failed");
        }
        LOG(DEBUG, "socket create success");
        struct sockaddr_in local;
        memset(&local, 0, sizeof(local));
        local.sin_family = AF_INET;
        local.sin_addr.s_addr = _ip;
        local.sin_port = _port;
        int n = bind(_sockfd, (struct sockaddr *)&local, sizeof(local));
        if (n < 0)
        {
            LOG(FATAL, "bind failed");
            exit(-1);
        }
        LOG(DEBUG, "bind success");
        while (true)
        {
            char recbuff[128];
            struct sockaddr_in peer;
            socklen_t len = sizeof(peer);
            ssize_t rnum = recvfrom(_sockfd, recbuff, sizeof(recbuff) - 1, 0, (struct sockaddr *)&peer, &len);
            if (rnum < 0)
            {
                LOG(DEBUG, "receive failed");
            }
            recbuff[rnum] = 0;
            std::cout<<"client say#"<<recbuff<<std::endl;
            std::string sendbuff = _func(recbuff);
            ssize_t snum = sendto(_sockfd, sendbuff.c_str(), sendbuff.size(), 0, (struct sockaddr *)&peer, len);
            if (snum < 0)
            {
                LOG(DEBUG, "send failed");
            }
            sendbuff[snum] = 0;
        }
    }
};