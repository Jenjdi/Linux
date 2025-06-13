#include"PollServer.h"
int main()
{
    std::unique_ptr<PollServer> up=std::make_unique<PollServer>(1234);
    up->initfd();
    up->loop();
}