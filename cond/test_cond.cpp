#include<pthread.h>
#include<unistd.h>
#include<string>
#include<iostream>
using namespace std;
const int num=5;
pthread_mutex_t gmutex=PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t gcond=PTHREAD_COND_INITIALIZER;
void* wait(void* args)
{
    string name=static_cast<char*>(args);
    while(true)
    {
        pthread_mutex_lock(&gmutex);
        pthread_cond_wait(&gcond,&gmutex);
        cout<<"name="<<name<<endl;
        pthread_mutex_unlock(&gmutex);
        usleep(1000000);
    }
}
int main()
{
    pthread_t tid[num];
    for(int i=0;i<num;i++)
    {
        char* name=new char[1024];
        snprintf(name,1024,"thread_%d",i+1);
        pthread_create(tid+i,nullptr,wait,(void*)name);
    }
    while(true)
    {
        pthread_cond_signal(&gcond);
        cout<<"awake a thread"<<endl;
        sleep(2);
    }
    for(int i=0;i<num;i++)
    {
        pthread_join(tid[i],nullptr);
    }
}