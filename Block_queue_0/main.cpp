#include"block_queue.hpp"
#include"task.hpp"
void* Consumer(void* args)
{
    BlockQueue<task_t>* bq=static_cast<BlockQueue<task_t>*>(args);
    while(true)
    {
        usleep(10000);
        task_t t;
        bq->pop(&t);
        t();
        //cout<<"Consumer->"<<t.execute()<<endl;
    }
}
void* Producer(void* args)
{
    srand(time(nullptr));
    BlockQueue<task_t>* bq =  static_cast<BlockQueue<task_t>*>(args);
    
    while(true)
    {
        sleep(1);
        //int x=rand()%10+1;
        //int y=rand()%10+1;
        //task t(x,y);
        bq->push(mission);
        cout<<"Producer->"<<"mission"<<endl;
    }
}
int main()
{
    BlockQueue<task_t>* bq=new BlockQueue<task_t>(10);
    pthread_t c,p;
    pthread_create(&p,nullptr,Producer,(void*)bq);
    pthread_create(&c,nullptr,Consumer,(void*)bq);
    pthread_join(c,nullptr);
    pthread_join(p,nullptr);
}