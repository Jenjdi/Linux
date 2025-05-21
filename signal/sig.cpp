#include<signal.h>
#include<iostream>
using namespace std;

void ring(int sig)
{
    cout<<"ring bells"<<endl;
    exit(1);
}
int main()
{
    signal(SIGALRM,ring);
    //signal(9,handler);
    alarm(3);
    int n=alarm(5);
    //int m=alarm(0);
    cout<<n<<endl;
    while(1)
    {
        cout<<"waiting for signal:"<<getpid()<<endl;
        //raise(2);
        //kill(getpid(),9);
        sleep(1);
    }
}