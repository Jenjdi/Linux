#include <cstdio>
#include <fcntl.h>
#include <iostream>
#include <unistd.h>
void SetNoBlock(int fd)
{
    int fl = fcntl(fd, F_GETFL);
    if (fd < 0)
    {
        std::cerr << "Get FL failed" << std::endl;
        return;
    }
    fcntl(fd, F_SETFL, fl | O_NONBLOCK);
}
int main()
{
    char buff[128];
    SetNoBlock(0);
    while (1)
    {
        //fflush(stdout);
        int n = read(0, buff, sizeof(buff) - 1);
        if (n < 0)
        {
            if (errno == EWOULDBLOCK)
            {
                sleep(1);
                std::cout << "no input,blocking" << std::endl;
                continue;
            }
            else if(errno==EINTR)
            {
                continue;
            }
            else
            {
                std::cout<<"read error"<<std::endl;
                break;
            }
        }
        else if(n==0)
        {
            std::cout<<"read done"<<std::endl;
            break;
        }
        else
        {
            buff[n]=0;
            std::cout<<"Get"<<buff<<std::endl;
        }
    }
}