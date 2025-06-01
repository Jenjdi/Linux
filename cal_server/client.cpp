#include"socket.h"
#include"protocol.h"
#include<iostream>
#include"time.h"
#include"unistd.h"
using namespace ns_sock;
int main(int argc,char* argv[])
{
    if(argc!=3)
    {
        std::cout<<"using client ip port"<<std::endl;
    }
    SockPtr sock=std::make_shared<TCPSocket>();
    
    sock->BuildClientSocket(argv[1],std::stoi(argv[2]));
    
    const std::string opers="+-*/^&";
    std::string packagestream;
    while(true)
    {
        srand(time(nullptr));
        int x=rand()%100;
        int y=rand()%100;
        char oper=opers[y%opers.size()];
        auto req=Factory::BuildRequest();
        req->SetValue(x,y,oper);
        std::string jsonstr;
        req->Serialize(&jsonstr);
        jsonstr=Encode(jsonstr);
        std::cout<<"request:"<<jsonstr<<std::endl;
        sock->Send(jsonstr);
        LOG(DEBUG,"send success");
        while(true)
        {
            ssize_t n=sock->Recv(&packagestream);
            if(n<=0)
            {
                LOG(ERROR,"receive failed");
                break;
            }
            std::string package=Decode(packagestream);
            LOG(DEBUG,"decode success");
            if(package.empty())
            {
                LOG(DEBUG,"package empty");
                continue;
            }
            std::cout << "package: \n" << package << std::endl;
            auto resp=Factory::BuildResponse();
            resp->Deserialize(package);
            resp->Print();
            break;
        }
        sleep(1);
    }
    sock->Close();
}