#include"Tcp_server.h"
#include"memory"
#include"command.h"
int main()
{
    command cmd;

    std::unique_ptr<server> usvr=std::make_unique<server>(std::bind(&command::Handler,&cmd,std::placeholders::_1));
    usvr->loop();
}