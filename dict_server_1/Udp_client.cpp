#include <string.h>
#include <string>
#include <functional>
#include <iostream>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "log.h"
int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        std::cout << "use client ip port" << std::endl;
    }
    char *ip = argv[1];
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0)
    {
        LOG(FATAL, "create socket failed");
    }
    struct sockaddr_in remote;
    memset(&remote, 0, sizeof(remote));
    remote.sin_family = AF_INET;
    remote.sin_addr.s_addr = inet_addr(ip);
    remote.sin_port = htons(std::stoi(argv[2]));
    while (1)
    {
        std::string line;
        getline(std::cin, line);
        ssize_t snum = sendto(sockfd, line.c_str(), line.size(), 0, (struct sockaddr *)&remote, sizeof(remote));
        if (snum < 0)
        {
            LOG(ERROR, "send failed");
            exit(1);
        }
        struct sockaddr temp;
        socklen_t len=sizeof(temp);
        char recvbuf[128];
        ssize_t rnum=recvfrom(sockfd,recvbuf,sizeof(recvbuf)-1,0,&temp,&len);
        recvbuf[rnum]=0;
        std::cout<<recvbuf<<std::endl;
    }
}