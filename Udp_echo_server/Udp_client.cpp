#include<unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
using namespace std;
int main()
{
    int sockfd=socket(AF_INET,SOCK_DGRAM,0);
    if(sockfd<0)
    {
        cout<<"Create Socket Failed"<<endl;
        exit(1);
    }
    
    close(sockfd);
    return 0;
}
