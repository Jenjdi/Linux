#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<string>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<cerrno>
using namespace std;
void childprocess(int pipefd)
{
    char buffer[128];
    ssize_t n=read(pipefd,buffer,sizeof(buffer)-1);
    if(buffer>0)
    {
        buffer[n]=0;
        cout<<buffer;
    }
    
}
int main()
{
    int pipefd[2]={0};
    int n=pipe(pipefd);
    if(n==-1)
    {
        perror("pipe failed");
    }
    pid_t processid=fork();
    if(processid==0)
    {
        close(pipefd[1]);
        childprocess(pipefd[0]);
        close(pipefd[0]);
        exit(0);
    }
    close(pipefd[0]);
    string buf="i am father\n";
    int w=write(pipefd[1],buf.c_str(),buf.size());
    close(1);
}