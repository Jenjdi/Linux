#include"nocopy.h"
#include<string.h>
#include<string>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include"log.h"
const int defaultport=1234;
static const int gsocket = -1;
enum
{
    SOCK_ERR=1,
    BIND_ERR=2
};
class Server:public nocopy
{
    private:
    int _sockfd;
    int _port;
    bool _isrunning;
    public:
    Server(int port=defaultport)
    :_port(port){}
    ~Server()
    {
        if(_sockfd>gsocket) close(_sockfd);
    }
    void init()
    {
        _sockfd=socket(AF_INET,SOCK_DGRAM,0);//创建套接字
        if(_sockfd<0)
        {
            LOG(FATAL,"socket error");
            exit(SOCK_ERR);
        }
        LOG(DEBUG,"socket created");
        struct sockaddr_in local;
        memset(&local,0,sizeof(local));
        local.sin_family=AF_INET;
        local.sin_port=htons(_port);
        //local.sin_addr.s_addr=inet_addr(_address.c_str());
        local.sin_addr.s_addr=INADDR_ANY;//INADDR_ANY表示任意字段，和0.0.0.0类似
        int n=bind(_sockfd, (sockaddr*)&local,sizeof(local));
        if(n<0)
        {
            LOG(FATAL,"bind error");
            exit(BIND_ERR);
        }
        LOG(DEBUG,"bind success");
    }
    void start()
    {
        _isrunning=true;
        char inbuffer[1024];
        while(_isrunning)
        {
            struct sockaddr_in peer;
            socklen_t len=sizeof(peer);
            ssize_t n=recvfrom(_sockfd,inbuffer,sizeof(inbuffer)-1,0,(struct sockaddr*)&peer,&len);
            if(n>0)
            {
                inbuffer[n]=0;
                std::string sendmessage="[Server Echo]:";
                sendmessage+=inbuffer;
                std::string clientip=inet_ntoa(peer.sin_addr);
                uint16_t clientport=ntohs(peer.sin_port);
                sendto(_sockfd,sendmessage.c_str(),sendmessage.size(),0,(struct sockaddr*)&peer,len);
                std::cout<<"["<<clientip<<":"<<clientport<<"] #"<<inbuffer<<std::endl;
            }
        }
    }
};