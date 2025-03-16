#include"Udp_server.hpp"

int main()
{
    int port=10000;
    unique_ptr<Udpserver> usvr=std::make_unique<Udpserver>(port);
    usvr->init();
    usvr->start();
}