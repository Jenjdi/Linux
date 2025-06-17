#include"Connection.h"
#include"Listen.h"
#include"Reactor.h"
#include"Epoll.h"
#include"Handler.h"
#include"packageparse.h"

int main()
{
    std::unique_ptr<Reactor> revr=std::make_unique<Reactor>();
    Listen listenfd(1234);
    PackageParse parse;
    Handler handler(std::bind(&PackageParse::Execute,&parse,std::placeholders::_1));
    revr->SetListenHandler(std::bind(&Listen::Accept,&listenfd,std::placeholders::_1));
    revr->SetNormalHandler(
        std::bind(&Handler::HandleRecv,&handler,std::placeholders::_1),
        std::bind(&Handler::HandleSend,&handler,std::placeholders::_1),
        std::bind(&Handler::HandleExcept,&handler,std::placeholders::_1)
    );
    revr->AddConnecttion(listenfd.listenfd(),EPOLLIN|EPOLLET,ListenConnection);
    revr->Dispatch();
}