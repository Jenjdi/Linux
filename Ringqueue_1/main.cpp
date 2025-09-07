#include<iostream>
#include<functional>
#include"ringqueue.h"
using task_t=function<void(int,int)>;
void task(int a,int b)
{
    std::cout<<a+b<<endl;
}
void* Producer(void* args)
{
    srand(time(nullptr));
    RingQueue<task_t>* rq=static_cast<RingQueue<task_t>*>(args);
    while (true)
    {
        rq->push(task);
    }
    
}
void* Consumer(void* args)
{
    RingQueue<task_t>* rq=static_cast<RingQueue<task_t>*>(args);
    while(true)
    {
        task_t t;
        rq->pop(&t);
        int x=rand()%10+1;
        usleep(100);
        int y=rand()%10+1;
        cout<<"Consumer->";
        t(x,y);
    }
}
int main()
{
    pthread_t c,p;
    RingQueue<task_t>* rq=new RingQueue<task_t>();
    pthread_create(&c,nullptr,&Producer,(void*)rq);
    pthread_create(&p,nullptr,&Consumer,(void*)rq);
    pthread_join(p,nullptr);
    pthread_join(c,nullptr);
    return 0;
}