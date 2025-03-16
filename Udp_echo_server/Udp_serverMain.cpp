#include"Udp_server.hpp"

int main()
{
    unique_ptr<Udpserver> usvr=make_uique<Udpserver>();
    usvr->init();
    usvr->start();
}