#pragma once
#include "Socket.h"
#include "log.h"
#include <fcntl.h>
#include <unistd.h>
class SelectServer
{
    const static int gnum = sizeof(fd_set) * 8;
    const static int defaultfd = -1;

private:
    uint16_t _port;
    std::unique_ptr<TcpSocket> _listensock;
    int fd_arr[gnum];

public:
    SelectServer(uint16_t port)
        : _port(port),
          _listensock(std::make_unique<TcpSocket>())
    {
        int opt = 1;
        setsockopt(_listensock->sockfd(), SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
        _listensock->BuildListenSocket(_port);
    }
    ~SelectServer() {}
    void Acceptor()
    {
        int sockfd = _listensock->accepter();
        if (sockfd > 0)
        {
            LOG(DEBUG, "accept success");
            // 成功获取到一个sockfd了，但是此时由于条件不一定满足，因此不能马上读取，所以需要交给select
            // 将新获得的sockfd交付给select
            bool flag = false;
            for (int i = 1; i < gnum; i++)
            {
                if (fd_arr[i] == defaultfd)
                {
                    flag = true;
                    fd_arr[i] = sockfd; // 找到可用的位置，添加进数组中用于管理
                    break;
                }
            }
            // 没有空间进行添加了，只能将新获取的fd关闭
            if (!flag)
            {
                close(sockfd);
                LOG(ERROR, "fd_arr is full");
            }
        }
    }
    void HandleIO(int pos)
    {
        char buf[1024];
        int n = recv(fd_arr[pos], buf, sizeof(buf) - 1, 0);
        if (n > 0)
        {
            buf[n] = 0;
            std::string echo_str = "[server]:";
            echo_str += buf;
            send(fd_arr[pos], echo_str.c_str(), echo_str.size(), 0);
        }
    }
    void Handler(fd_set *fs)
    {
        // 区分普通fd和listenfd
        for (int pos = 0; pos < gnum; pos++)
        {
            if (fd_arr[pos] == defaultfd) // 判断fd是否存在
                continue;
            if (FD_ISSET(fd_arr[pos], fs)) // 判断fd是否就绪，读事件由于缓冲区为空不能读取，因此需要等待就绪
            {
                if (_listensock->sockfd() == fd_arr[pos])
                {
                    Acceptor();
                }
                else // 写事件默认缓冲区为空，因此默认就是就绪的，可以直接写
                {
                    // 普通文件描述符，正常读写
                    HandleIO(pos);
                }
            }
        }
    }
    void print()
    {
        for (int i = 0; i < gnum; i++)
        {
            if (fd_arr[i] == defaultfd)
                continue;
            std::cout << fd_arr[i] << " ";
        }
        std::cout << std::endl;
    }
    void initfd()
    {
        for (int i = 1; i < gnum; i++)
        {
            fd_arr[i] = defaultfd;
        }
        fd_arr[0] = _listensock->sockfd();
    }
    void loop()
    {
        while (true)
        {
            fd_set fs;              // 由于select中的rfds是输入输出型参数，因此每次都需要重置来保证拿到的都是合法的参数
            int max_fd = defaultfd; // 获取最大的文件描述符
            for (int i = 0; i < gnum; i++)
            {
                if (fd_arr[i] == defaultfd)
                    continue;
                FD_SET(fd_arr[i], &fs); // 找到文件描述符，将其添加进fd_set中，让select进行检查是否就绪
                if (max_fd < fd_arr[i])
                {
                    max_fd = fd_arr[i];
                }
            }
            timeval timeout = {1, 0};
            int n = select(max_fd + 1, &fs, nullptr, nullptr, &timeout);
            switch (n)
            {
            case 0:
                LOG(DEBUG, "timeout");
                break;
            case -1:
                LOG(ERROR, "select error");
            default:
                LOG(INFO, "Get success");
                Handler(&fs);
                print();
                break;
            }
        }
    }
};