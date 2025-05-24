#include"RingQueue.hpp"
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
void* Producer(void* args)
{
    srand(time(nullptr));
    RingQueue<task_t>* rq=static_cast<RingQueue<task_t>*>(args);
    while(true)
    {
        
        rq->push(task);
        cout<<"Producer->"<<endl;
    }
}
int main()
{
    pthread_t c,p;
    RingQueue<task_t> *rq=new RingQueue<task_t>();
    pthread_create(&c,nullptr,Consumer,(void*)rq);
    pthread_create(&p,nullptr,Producer,(void*)rq);
    pthread_join(c,nullptr);
    pthread_join(p,nullptr);
}