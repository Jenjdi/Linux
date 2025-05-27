#include"udp_server.h"
#include<memory>
int main()
{
    routine messageroutine;
    service_t func=std::bind(&routine::forward,&messageroutine,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3);
    unique_ptr<server> usvr=std::make_unique<server>(func,1234);
    usvr->start();
}