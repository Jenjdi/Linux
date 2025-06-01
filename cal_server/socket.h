#pragma once
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <functional>
#include "log.h"
#include <memory>
namespace ns_sock
{
    class Socket;
    using SockPtr = std::shared_ptr<Socket>;
    class Socket
    {
        // 模板方法模式
    public:
    
        virtual void CreateSocketOrdie() = 0;
        virtual void CreateBindOrdie(uint16_t port) = 0;
        virtual void CreateLisetenerOrdie() = 0;
        virtual SockPtr accepter() = 0;
        virtual bool connector(const std::string &peerip, uint16_t peerport) = 0;
        void BuildListenSocket(uint16_t port)
        {
            CreateSocketOrdie();
            CreateBindOrdie(port);
            CreateLisetenerOrdie();
        }
        void BuildClientSocket(const std::string &peerip, uint16_t peerport)
        {
            CreateSocketOrdie();
            connector(peerip, peerport);
        }
        virtual int Recv(std::string *out) = 0;
        virtual int Send(const std::string &in) = 0;
        virtual int sockfd() = 0;
        virtual void Close() = 0;
    };
    
    class TCPSocket : public Socket
    {
    public:
        TCPSocket() {}
        TCPSocket(int sockfd)
            : _sockfd(sockfd) {}
        ~TCPSocket()
        {
            Close();
        }
        void CreateSocketOrdie() override
        {
            _sockfd = socket(AF_INET, SOCK_STREAM, 0);
            if (_sockfd < 0)
            {
                LOG(FATAL, "socket create failed");
                exit(1);
            }
            LOG(DEBUG, "create socket success");
        }
        void CreateBindOrdie(uint16_t port) override
        {
            struct sockaddr_in local;
            local.sin_addr.s_addr = INADDR_ANY;
            local.sin_port = htons(port);
            local.sin_family = AF_INET;
            int n = bind(_sockfd, (struct sockaddr *)&local, sizeof(local));
            if (n < 0)
            {
                LOG(FATAL, "bind failed");
                exit(0);
            }
            LOG(DEBUG, "bind success");
        }
        void CreateLisetenerOrdie() override // 监听获取连接
        {
            int n = listen(_sockfd, 8);
            if (n < 0)
            {
                LOG(FATAL, "listen failed");
            }
            LOG(DEBUG, "listen success");
        }
        int Recv(std::string *out)
        {
            char buf[4096];
            LOG(DEBUG,"receiving");
            int n = recv(_sockfd, buf, sizeof(buf) - 1, 0);
            if (n > 0)
            {
                buf[n] = 0;
                *out += buf;//由于是字节流，可能读取到不完整的信息，因此可能重新回来读取，所以需要将信息追加而不是覆盖
            }
            LOG(DEBUG,"receive success");
            return n;
        }
        int Send(const std::string &in)
        {
            return send(_sockfd, in.c_str(), in.size(), 0);
        }
        SockPtr accepter() override // 获取sockfd用于数据传输
        {
            struct sockaddr_in client;
            socklen_t len = sizeof(client);
            int fd = accept(_sockfd, (struct sockaddr *)&client, &len);
            if (fd < 0)
            {
                LOG(FATAL, "accept failed");
            }
            LOG(DEBUG, "accept success");
            return std::make_shared<TCPSocket>(fd); // 构成协变
        }
        bool connector(const std::string &peerip, uint16_t peerport) override // 连接服务器
        {
            struct sockaddr_in server;
            memset(&server, 0, sizeof(server));
            server.sin_family = AF_INET;
            server.sin_port = htons(peerport);
            inet_pton(AF_INET, peerip.c_str(), &server.sin_addr);
            int n = connect(_sockfd, (struct sockaddr *)&server, sizeof(server));
            if (n < 0)
            {
                LOG(FATAL, "connect failed");
                return false;
            }
            LOG(DEBUG, "connect success");
            return true;
        }
        int sockfd()
        {
            return _sockfd;
        }
        void Close()
        {
            if (_sockfd > 0)
            {
                ::close(_sockfd);
            }
        }

    private:
        int _sockfd;
    };

}
