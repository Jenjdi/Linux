#include"Udp_server.h"
#include<memory>
int main()
{
    std::unique_ptr<Server> ptr=std::make_unique<Server>(12);
    ptr->init();
    ptr->start();
}