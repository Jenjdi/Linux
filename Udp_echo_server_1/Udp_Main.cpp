#include"Udp_server.h"
#include<memory>
int main()
{
    std::unique_ptr<Server> ptr=std::make_unique<Server>();//服务器要接收所有地址发送来的消息
    ptr->init();
    ptr->start();
}