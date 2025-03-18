#include"thread_pool.hpp"
#include"task.hpp"
int main()
{
    unique_ptr<thread_pool<Task>> tp=make_unique<thread_pool<Task>>();
    tp->init();
    tp->start();
    while(1)
    {
        sleep(1);
        Task t(1,2);
        tp->push(t);
        
    }
    
}