#include <iostream>
#include <cstdlib>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
std::string path="/";
std::string dir="dev/null";
void Daemon(bool isclose,bool ischdir)
{
    signal(SIGCHLD,SIG_IGN);
    signal(SIGPIPE,SIG_IGN);
    if(fork()>0)
        exit(1);
    setsid();
    if(ischdir)
    {
        chdir(path.c_str());
    }
    if(isclose)
    {
        ::close(0);
        ::close(1);
        ::close(2);
    }
    else
    {
        int fd=open(dir.c_str(),O_RDWR);
        dup2(fd,0);
        dup2(fd,1);
        dup2(fd,2);
        close(fd);
    }
}