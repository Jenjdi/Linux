#include "fifo.hpp"
int main()
{
    Fifo f(commonPath, customer);
    if (f.OpenWrite())
    {
        int i=2
        while (i--)
        {
            string message = "client pid=:";
            message += to_string(getpid());
            f.WritePipe(message);
        }

        return 0;
    }
}