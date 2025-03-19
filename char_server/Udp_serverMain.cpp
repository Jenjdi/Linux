#include "Udp_server.hpp"
#include "route.hpp"

int main()
{
    int port = 10000;
    route messageroute;
    service_t message_route = bind(&route::Forward, &messageroute, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    unique_ptr<Udpserver> usvr = std::make_unique<Udpserver>(message_route,port);
    usvr->init();
    usvr->start();
}