#include<unistd.h>
#include<sys/wait.h>
#include<iostream>
int main()
{
    pid_t pid =fork();
    if(pid==0)
    {
        execl("/usr/bin/ls","ls","-l","-a",NULL);
    }
    else
    {
        int status=0;
        pid_t rid=waitpid(pid,&status,0);
        if(rid>0)
        {
            std::cout<<"wait success,status="<<WEXITSTATUS(status)<<std::endl;
        }
    }
}