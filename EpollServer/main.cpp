#include"epollserver.h"
int main()
{
    std::unique_ptr<EpollServer> upes=std::make_unique<EpollServer>(1234);
    upes->init();
    upes->loop();
}