#include"shm.h"
int main()
{
    SharedMemory shm(pathname,user,proj_id);
    Fifo fifo(commonPath, creater);
    fifo.OpenRead();
    char* getmess=(char*)shm.getshm();
    
    while(1)
    {
        string tmp;
        cout<<"get message:"<<getmess<<endl;
        sleep(1);
    }
}