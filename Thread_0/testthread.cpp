#include <pthread.h>
#include <unistd.h>
#include <iostream>
#include <vector>
#include<cstring>
using namespace std;
const int nums = 10;
void *func(void *arg)
{
    while (1)
    {
        cout << static_cast<const char *>(arg) << " is running" << endl;
        sleep(1);
        return arg;
    }
}
int main()
{
    vector<pthread_t> tids;
    for (int i = 0; i < nums; i++)
    {
        pthread_t tid;
        char *name = new char[64]; // 每一个进程都需要获取不同的name，因此不能使用main函数栈空间中的变量，需要在堆上开辟空间
        snprintf(name, 64, "thread-%d", i + 1);
        pthread_create(&tid, nullptr, func, name);
        tids.push_back(tid);
    }
    for (auto e:tids)
    {
        void *retv=nullptr;
        pthread_join(e, &retv);
        cout << (const char *)retv << " quited"<<endl;
        delete retv; // 将分配在堆上的内容传入新线程后，线程退出时又传回到主线程
    }
}