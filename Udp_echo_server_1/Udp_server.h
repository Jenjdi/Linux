#include"nocopy.h"
#include<sys/socket.h>
#include<string.h>
#include<string>
#include<netinet/in.h>
#include<arpa/inet.h>
#include"log.h"
const int defaultport=1234;
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
    std::string _address;
    bool _isrunning;
    public:
    Server(const std::string& address,int port=defaultport)
    :_address(address),
    _port(port){}
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
        local.sin_addr.s_addr=inet_addr(_address.c_str());
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
        _isrunning==true;
        char inbuffer[1024];
        while(_isrunning)
        {
            struct sockaddr_in peer;
            socklen_t len=sizeof(peer);
            ssize_t n=recvfrom(_sockfd,inbuffer,sizeof(inbuffer)-1,0,(sockaddr*)&peer,&len);
            if(n<0)
            {
                LOG(DEBUG,"not received");
            }
        }
    }
};