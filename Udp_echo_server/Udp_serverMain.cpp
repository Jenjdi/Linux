#include"Udp_server.hpp"

int main()
{
    string ip="127.0.0.1";
    int port=10000;
    unique_ptr<Udpserver> usvr=std::make_unique<Udpserver>(ip,port);
    usvr->init();
    usvr->start();
}