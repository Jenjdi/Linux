#include"shm.hpp"
int main()
{
    sharedMemory shm(customer,pathname,proj_id);
    char* getmess=(char*)shm.getshmAddr();
    while(1)
    {
        cout<<"get message:"<<getmess<<endl;
        sleep(1);
    }
}