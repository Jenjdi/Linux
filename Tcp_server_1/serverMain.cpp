#include"Tcp_server.h"
#include"memory"
int main()
{
    std::unique_ptr<server> usvr=std::make_unique<server>();
    usvr->loop()
;}