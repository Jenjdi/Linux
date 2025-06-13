#include"selectservice.h"
int main()
{
    std::unique_ptr<SelectServer> ups=std::make_unique<SelectServer>(1234);
    ups->initfd();
    ups->loop();

}