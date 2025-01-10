#include"shm.hpp"
int main()
{
    sharedMemory shm(Creater,pathname,proj_id);
    char* message=(char*)shm.getshmAddr();
    char ch='A';
    while(ch<'Z')
    {
        message[ch-'A']=ch;
        ch++;
        sleep(1);
    }
}