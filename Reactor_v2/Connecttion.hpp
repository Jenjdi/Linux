#pragma once
#include "Socket.hpp"
// 连接示例目前只保留描述符，显式命名类型以修复未完成声明。
class Connection {
    int socketFd_ = -1;
public:
    explicit Connection(int socketFd = -1) : socketFd_(socketFd) {}
    int socketFd() const { return socketFd_; }
};
