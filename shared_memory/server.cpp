#include"shm.h"
int main()
{
    SharedMemory shm(pathname,creater,proj_id);
    Fifo fifo(commonPath, user);
    fifo.OpenWrite();
    shm.clear();
    char* message=(char*)shm.getshm();
    char ch='A';
    while(ch<='Z')
    {
        message[ch-'A']=ch;
        fifo.WriteFifo("wake up client");
        ch++;
        sleep(2);
    }
    cout<<"server quit"<<endl;
}