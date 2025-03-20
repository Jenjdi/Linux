#pragma once
#include<iostream>
#include<cstring>
#include<string>
#include<sys/types.h>
#include<unistd.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include"InetAddr.hpp"
#include<sys/wait.h>
using namespace std;
const static uint16_t gport=8888;
const static int gsock=-1;
const static int backlog=8;
class tcpserver
{
    
    public:
    tcpserver(uint16_t port=gport)
    :_port(port),_listensockfd(gsock),_isrunning(false)
    {

    }
    void init()
    {
        _listensockfd=socket(AF_INET,SOCK_STREAM,0);//创建socket
        if(_listensockfd<0)
        {
            cout<<"sockfd init failed"<<endl;
            exit(1);
        }
        cout<<"sockfd successfully created"<<endl;
        struct sockaddr_in local;
        memset(&local,0,sizeof(local));
        local.sin_family=AF_INET;
        local.sin_port=htons(_port);
        local.sin_addr.s_addr=INADDR_ANY;
        socklen_t len=sizeof(local);
        int n=bind(_listensockfd,(struct sockaddr*)&local,len);//绑定文件描述符和地址
        if(n<0)
        {
            cout<<"Bind Failed"<<endl;
            exit(-1);
        }
        cout<<"Bind Success"<<endl;
        //tcp是面向连接的，因此需要做的能够获取连接，需要将套接字设置为listen状态
        if(listen(_listensockfd,backlog)<0)
        {
            cout<<"Listen Failed"<<endl;
            exit(-1);
        }
        cout<<"Listen Success"<<endl;
    }
    void loop()
    {
        _isrunning=true;
        while(true)
        {
            struct sockaddr_in client;
            socklen_t len=sizeof(client);
            int sockfd=accept(_listensockfd,(struct sockaddr*)&client,&len);
            if(sockfd<0)
            {
                cout<<"accept error"<<endl;
                continue;
            }
            InetAddr addr(client);
            //service(sockfd,addr);
            //多进程版本：
            pid_t id=fork();
            if(id==0)
            {
                close(_listensockfd);
                if(fork()>0)//创建一个孙子进程来执行下面的操作，然后自己退出，当孙子进程执行完成后，将会变成孤儿进程，操作系统将会自己回收
                {
                    exit(0);
                }
                service(sockfd,addr);
                exit(0);
            }
            close(sockfd);
            int n=waitpid(id,nullptr,0);
            if(n>0)
            {
                cout<<"child wait success"<<endl;
            }
        }
        _isrunning=false;
    }
    void service(int sockfd,InetAddr& addr)
    {
        while(true)
        {
            char inbuffer[1024];
            ssize_t n=read(_listensockfd,inbuffer,sizeof(inbuffer)-1);
            if(n>0)
            {
                string echo_string="[server echo]#";
                echo_string+=inbuffer;
                write(_listensockfd,echo_string.c_str(),echo_string.size());
            }
            else if(n==0)
            {
                cout<<"client quit"<<endl;
                break;
            }
            else
            {
                cout<<"read error"<<endl;
            }
        }
        close(_listensockfd);
    }
    private:
    uint16_t _port;
    int _listensockfd;
    bool _isrunning;
};