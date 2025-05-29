#include<iostream>
#include<cstring>
#include<string>
#include<sys/types.h>
#include<unistd.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include"InetAddr.hpp"

int main(int argc,char* argv[])
{
    if(argc!=3)
    {
        cout<<"Usage: server_ip server_port"<<endl;
        exit(0);
    }
    string serverip=argv[1];
    uint16_t serverport=stoi(argv[2]);
    int sockfd=socket(AF_INET,SOCK_STREAM,0);
    if(sockfd<0)
    {
        cout<<"client create socket failed"<<endl;
        exit(0);
    }
    //也需要bind绑定，但是不需要显式的绑定IP和port，所以需要隐式绑定，OS会用自己的IP和随机端口号自动bind sockfd，
    //进行自动bind 的时机：当完成connect或binding成功时
    struct sockaddr_in addr;
    memset(&addr,0,sizeof(addr));
    addr.sin_family=AF_INET;
    addr.sin_port=htons(serverport);
    inet_pton(AF_INET,serverip.c_str(),&addr.sin_addr);
    
    int n=connect(sockfd,(struct sockaddr*)&addr,sizeof(addr));
    if(n<0)
    {
        cout<<"Connect Error"<<endl;
        exit(0);
    }
    while(true)
    {
        string msg;
        cout<<"Enter Message#";
        getline(cin,msg);
        write(sockfd,msg.c_str(),msg.size());
        char echobuffer[1024];
        n=read(sockfd,echobuffer,sizeof(echobuffer));
        if(n>0)
        {
            echobuffer[n]='\0';
            cout<<echobuffer<<endl;
        }
        else
        {
            break;
        }
    }
}