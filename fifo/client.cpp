#include "fifo.hpp"
int main()
{
    Fifo f(commonPath, user);
    if (f.OpenWrite())
    {
        //int i=2;
        cout<<"client open fifo done pid: "<<getpid()<<endl;
        while (1)
        {
            cout<<"input message> ";
            string message="i am process A";
            //getline(cin,message);
            f.WriteFifo(message);
            sleep(1);
        }

        return 0;
    }
}