#include"Tcpserver.h"
#include"http.h"
int main()
{
    HttpServer hsvr;
    std::unique_ptr<Server> tsvr=std::make_unique<Server>(std::bind(&HttpServer::HttpHandler,&hsvr,std::placeholders::_1));
    tsvr->loop();
}