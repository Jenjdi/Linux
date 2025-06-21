#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string>
#include <cstring>
#include<iostream>
int main()
{
    int sockfd=socket(AF_INET,SOCK_STREAM,0);
    std::string ip="127.0.0.1";
    struct sockaddr_in remote;
    memset(&remote,0,sizeof(remote));
    remote.sin_port=htons(1234);
    remote.sin_family=AF_INET;
    inet_pton(AF_INET,ip.c_str(),&remote.sin_addr);
    connect(sockfd,(struct sockaddr*)&remote,sizeof(remote));
    while (1)
    {
        std::string line;
        std::getline(std::cin, line);
        ssize_t snum = send(sockfd,line.c_str(),line.size(),0);
        if (snum < 0)
        {
            exit(1);
        }
        struct sockaddr temp;
        socklen_t len=sizeof(temp);
        char recvbuf[128];
        ssize_t rnum=recv(sockfd,recvbuf,sizeof(recvbuf)-1,0);
        if (rnum <= 0) break;
        recvbuf[rnum]=0;
        std::cout<<recvbuf<<std::endl;
    }
}