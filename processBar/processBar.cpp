#include"process.hpp"

void download(double total,double bandwidth)
{
    double current=0;
    while(current<=total)
    {
        processBar(total,current);
        current+=bandwidth;
        usleep(100000);
    }
    printf("\n");
    
}
int main()
{
    download(1024*1024*60,1024*1024);
}