#pragma once
#include"socket.h"
#include"tcp_server.h"
#include"protocol.h"
using process_t=std::function<std::shared_ptr<Response>(std::shared_ptr<Request>)>;
class service
{
    private:
    process_t _process;
    public:
    service(process_t process)
    :_process(process)
    {}
    void ServiceExecute(ns_sock::SockPtr sock)
    {
        std::string message;
        while(true)
        {
            ssize_t n=sock->Recv(&message);
            if(n<0)
            {
                LOG(ERROR,"Receive failed");
                break;
            }
            std::string package=Decode(message);
            LOG(DEBUG,"decode success");
            if(package.empty())
            {
                LOG(DEBUG,"package empty");
                continue;//读取到不完整的报文就重新读取
            }
            auto req=Factory::BuildRequest();
            req->Deserialize(package);
            LOG(DEBUG,"deserialize success");
            auto res=_process(req);
            std::string respmessage;
            res->Serialize(&respmessage);
            LOG(DEBUG,"serialize response message success");
            respmessage=Encode(respmessage);
            if(sock->Send(respmessage)<0)
            {
                LOG(ERROR,"send failed");
            }
            else
            {
                LOG(DEBUG,"send success");
            }
            
        }
        
        
    }
    ~service(){}
};