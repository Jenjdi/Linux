#include"tcp_server.h"
#include"service.h"
#include"calculator.h"

int main()
{
    Calculator cal;
    service s(std::bind(&Calculator::calculator,&cal,std::placeholders::_1));
    std::unique_ptr<server> sptr=std::make_unique<server>(std::bind(&service::ServiceExecute,&s,std::placeholders::_1));
    sptr->loop();
}