#include"Tcpserver.h"
#include"http.h"
HttpResponse login(HttpRequest& req)
{
    HttpResponse resp;
    resp.addCode(200,"OK");
    resp.addBodytext("<html><h1>login success</h1></html>");
    return resp;
}
int main()
{
    HttpServer hsvr;
    hsvr.InsertService("/login",login);
    std::unique_ptr<Server> tsvr=std::make_unique<Server>(std::bind(&HttpServer::HttpHandler,&hsvr,std::placeholders::_1));
    tsvr->loop();
}
