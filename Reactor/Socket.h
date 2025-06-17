#pragma once
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string>
#include <errno.h>
#include "log.h"
#include "Reactor.h"
class Socket
{
public:
    virtual void CreateSocketOrDie() = 0;
    virtual void CreateBindOrDie(uint16_t port) = 0;
    virtual void CreateListen() = 0;
    virtual void CreateConnect(const std::string &ip, const uint16_t port) = 0;
    virtual int Accept(int *code) = 0;
    virtual int SockFD() = 0;
    virtual void Close() = 0;
    virtual int Send(const std::string &in) = 0;
    virtual int Recv(std::string *out) = 0;
    void BuildListen(uint16_t port) // 这个sockfd用于获取新连接
    {
        CreateSocketOrDie();
        CreateBindOrDie(port);
        CreateListen();
    }
    void BulidConnection(const std::string &ip, const uint16_t port)
    {
        CreateSocketOrDie();
        CreateConnect(ip, port);
    }
};
class TcpSocket : public Socket
{
private:
    int _sockfd;
    void SetNoBlock(int fd)
    {
        int fl = fcntl(fd, F_GETFL);
        if (fd < 0)
        {
            std::cerr << "Get FL failed" << std::endl;
            return;
        }
        fcntl(fd, F_SETFL, fl | O_NONBLOCK);
    }

public:
    TcpSocket() {}
    TcpSocket(int sockfd)
        : _sockfd(sockfd) {}
    void CreateSocketOrDie() override
    {
        _sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (_sockfd < 0)
        {
            LOG(ERROR, "create sockfd failed");
            exit(-1);
        }
        SetNoBlock(_sockfd);
        LOG(INFO, "create sockfd success");
    }
    void CreateBindOrDie(uint16_t port)
    {
        struct sockaddr_in sock;
        memset(&sock, 0, sizeof(sock));
        sock.sin_port = htons(port);
        sock.sin_addr.s_addr = INADDR_ANY;
        sock.sin_family = AF_INET;
        int n = bind(_sockfd, (struct sockaddr *)&sock, sizeof(sock));
        if (n < 0)
        {
            LOG(ERROR, "bind failed");
            exit(-1);
        }
        LOG(DEBUG, "bind success");
    }
    void CreateListen() override
    {
        int n = listen(_sockfd, 8);
        if (n < 0)
        {
            LOG(ERROR, "listen failed");
            exit(-1);
        }
        LOG(DEBUG, "listen success");
    }
    int Accept(int *code) override // 返回获取到的新连接的fd
    {
        struct sockaddr_in client;
        socklen_t len = sizeof(client);
        int fd = accept(_sockfd, (struct sockaddr *)&client, &len);
        *code = errno;
        if (fd < 0)
        {
            LOG(ERROR, "accept failed");
            return -1;
        }
        SetNoBlock(fd);
        LOG(DEBUG, "accept success");
        return fd;
    }
    void CreateConnect(const std::string &ip, uint16_t port) override
    {
        struct sockaddr_in peer;
        memset(&peer, 0, sizeof(peer));
        peer.sin_family = AF_INET;
        peer.sin_port = htons(port);
        inet_pton(AF_INET, ip.c_str(), &peer.sin_addr);
        int n = connect(_sockfd, (struct sockaddr *)&peer, sizeof(peer));
        if (n < 0)
        {
            LOG(ERROR, "connect failed");
            exit(-1);
        }
        LOG(DEBUG, "connect success");
    }
    int SockFD() override
    {
        return _sockfd;
    }
    int Send(const std::string &in) override
    {
        int n = send(_sockfd, in.c_str(), in.size(), 0);
        if (n < 0)
        {
            LOG(ERROR, "send failed");
        }
        else
        {
            LOG(DEBUG, "send success");
        }
        return n;
    }
    int Recv(std::string *out) override
    {
        char buf[4096];
        int n = recv(_sockfd, buf, sizeof(buf) - 1, 0);
        if (n > 0)
        {
            buf[n] = 0;
            *out += buf;
        }
        return n;
    }
    void Close() override
    {
        close(_sockfd);
    }
};