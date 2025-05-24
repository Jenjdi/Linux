// #include"Thread.h"
// #include<unistd.h>
// int cnt=1000;
// void* Print(const string &name)
// {

//     while(1)
//     {
//         if(cnt>0)
//         {
//             cout<<name<<": "<<cnt<<endl;
//             cnt--;
//         }
//         else
//         {
//             break;
//         }
//     }
//     return nullptr;
// }
// int main()
// {
//     vector<Thread> tids;
//     for(int i=0;i<10;i++)
//     {
//         string name="Thread-"+to_string(i+1);
//         tids.emplace_back(name,Print);

//     }
//     for(auto& t:tids)
//     //如果不加引用，当循环结束后，拷贝的对象就被销毁了，但是线程还在运行，此时此时线程内部访问的 _name 指针或数据已经失效，导致所有线程输出的 name 都变成了同一个
//     {
//         t.start();
//     }
//     sleep(5);
//     for(auto& t:tids)
//     {
//         t.stop();
//     }
//     for(auto& t:tids)
//     {
//         t.join();
//     }
// }
// // #include<pthread.h>
// // #include<iostream>
// // #include<string>
// // #include<unistd.h>
// // #include"Thread.h"
// // int cnt=1000;
// // void* route(const string& name)
// // {
// //     while(1)
// //     {
// //         if(cnt>0)
// //         {
// //             //usleep(1000);
// //             cout<<name<<": "<<cnt<<endl;
// //             cnt--;
// //         }
// //         else
// //         {
// //             break;
// //         }
// //     }
// //     return nullptr;
// // }
// // int main()
// // {
// //     Thread t1("Thread-1",route);
// //     Thread t2("Thread-2",route);
// //     Thread t3("Thread-3",route);
// //     t1.start();
// //     t2.start();
// //     t3.start();
// //     t1.join();
// //     t2.join();
// //     t3.join();
// // }
#include <unistd.h>
#include <iostream>
#include "Thread.h"
#include<vector>
#include "LockGuard.h"
int num = 10;
int cnt = 100000;
void *func(My_Thread::ThreadData *td)
{
    while (1)
    {
        LockGuard lockguard(td->_mutex);
        if (cnt > 0)
        {
            pthread_cond_wait(td->_cond,td->_mutex);
            //usleep(1000);
            std::cout << td->_name << ": " << cnt << std::endl;
            cnt--;
            //pthread_cond_signal(td->_cond);
        }
        else
        {
            break;
        }
    }
    return nullptr;
}
int main()
{
    pthread_cond_t cond;
    pthread_mutex_t mutex;
    pthread_cond_init(&cond,nullptr);
    pthread_mutex_init(&mutex, nullptr);
    std::vector<My_Thread::Thread> threads;
    for (int i = 0; i < num; i++)
    {
        std::string name = "Thread-" + std::to_string(i + 1);
        My_Thread::ThreadData *td = new My_Thread::ThreadData(name, &mutex,&cond);
        threads.emplace_back(name, func, td);
    }
    for (auto &t : threads)
    {
        t.start();
    }
    
    //sleep(5);
    // // 如果不sleep的话，可能存在线程运行时被关闭导致死锁的发生，或者不进行stop，而是直接join
    // for (auto &t : threads)
    // {
    //     t.stop();
    // }
    for (auto &t : threads)
    {
        t.join();
    }
}