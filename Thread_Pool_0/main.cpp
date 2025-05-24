#include "thread_pool.hpp"
#include "task.hpp"
int main()
{
    // unique_ptr<thread_pool<Task>> tp=make_unique<thread_pool<Task>>();
    // tp->init();
    // tp->start();
    // int cnt=10;
    // while(cnt--)
    // {

    //     Task t(1,2);
    //     tp->push(t);
    //     cout<<"cnt:"<<cnt<<endl;
    // }
    // sleep(5);
    // tp->stop();
    int cnt = 10;
    while (cnt--)
    {
        Task t(1, 2);
        thread_pool<Task>::GetInstance()->push(t);
        sleep(1);
    }
    thread_pool<Task>::GetInstance()->stop();
}