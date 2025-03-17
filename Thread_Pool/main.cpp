#include"thread_pool.hpp"
int main()
{
    unique_ptr<thread_pool<int>> tp=make_unique<thread_pool<int>>();
    tp->init();
    tp->start();
    while(1)
    {
        sleep(1);
    }
    
}