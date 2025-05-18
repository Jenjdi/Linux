#include "fifo.hpp"

int main()
{
    Fifo fifo(commonPath, creater);
    if (fifo.OpenRead())
    {
        string message;
        while (true)
        {
            int n = fifo.ReadFifo(message);
            if (n > 0)
            {
                cout << "server read> " << message << endl;
            }
            else if(n==0)
            {
                cout<<"client process quit,server quit"<<endl;
                break;
            }
        }
    }
}