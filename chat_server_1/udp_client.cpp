#include "log.h"
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <string>
#include <cstring>
#include "InetAddr.h"
#include "cond.h"
int main(int argc,char* argv[])
{
    if(argc!=3)
    {
        std::cout<<"use"<<argv[0]<< "server-ip server-port"<<std::endl;
        exit(0);

    }
    char* serverip=argv[1];
    uint16_t serverport=std::stoi(argv[2]);
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0)
    {
        LOG(FATAL, "client sockfd create failed");
        exit(-1);
    }
    struct sockaddr_in remote;
    remote.sin_family = AF_INET;
    remote.sin_port = htons(serverport);
    remote.sin_addr.s_addr = inet_addr(serverip);
    socklen_t len=sizeof(remote);
    //client的端口号一般不让用户设定，而是让client随机选择
    //client需要bind自己的ip和端口号，但是client不需要显示bind自己的IP和端口号
    //client在第一次向服务器发送数据时，OS会自动给client bind自己的IP和端口号
    //int n = bind(sockfd, (struct sockaddr *)&local, sizeof(local));
    while(1)
    {
        struct sockaddr temp;
        socklen_t templen=sizeof(temp);
        std::string line;
        char recbuffer[64];
        std::getline(std::cin,line);
        sendto(sockfd,line.c_str(),line.size(),0,(struct sockaddr*)&remote,len);
        int m=recvfrom(sockfd,recbuffer,sizeof(recbuffer)-1,0,&temp,&templen);
        if(m>0)
        {
            recbuffer[m]=0;
            std::cout<<recbuffer<<std::endl;
        }
        
    }
    
    
    close(sockfd);
}