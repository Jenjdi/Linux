#include"shm.h"
int main()
{
    SharedMemory shm(pathname,creater,proj_id);
    Fifo fifo(commonPath, user);
    fifo.OpenWrite();
    shm.clear();
    char* message=(char*)shm.getshm();
    char* ch="i am process A";
    while(1)
    {
        strcpy(message,ch);
        sleep(1);
    }
    cout<<"server quit"<<endl;
}