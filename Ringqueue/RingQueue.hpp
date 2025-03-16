#pragma once
#include<iostream>
#include<semaphore.h>
#include<pthread.h>
#include<unistd.h>
#include<string>
#include<functional>
#include<vector>
using namespace std;
const static int defaultnum=5;
using task_t=function<void(int,int)>;
void task(int a,int b)
{
    cout<<a+b<<endl;
}
template<typename T>
class RingQueue
{
    private:
    void P(sem_t& sem)
    {
        sem_wait(&sem);
    }
    void V(sem_t& sem)
    {
        sem_post(&sem);
    }
    public:
    RingQueue(int cap=defaultnum)
    :_max_cap(cap),_ringqueue(cap),_c_step(0),_p_step(0)
    {
        pthread_mutex_init(&_c_mutex,nullptr);
        pthread_mutex_init(&_p_mutex,nullptr);
        sem_init(&_data_sem,0,0);
        sem_init(&_space_sem,0,cap);
    }
    void push(const T& in)
    {
        pthread_mutex_lock(&_p_mutex);
        P(_space_sem);
        _ringqueue[_p_step]=in;
        _p_step++;
        _p_step%=_max_cap;
        V(_data_sem);
        pthread_mutex_unlock(&_p_mutex);
    }
    void pop(T* out)
    {
        pthread_mutex_lock(&_c_mutex);
        P(_data_sem);
        *out=_ringqueue[_c_step];
        _c_step++;
        _c_step%=_max_cap;
        V(_space_sem);
        pthread_mutex_unlock(&_c_mutex);
    }
    ~RingQueue()
    {
        pthread_mutex_destroy(&_c_mutex);
        pthread_mutex_destroy(&_p_mutex);
        sem_destroy(&_data_sem);
        sem_destroy(&_space_sem);
    }
    private:
    vector<T> _ringqueue;
    pthread_mutex_t _c_mutex;
    pthread_mutex_t _p_mutex;
    sem_t _data_sem;
    sem_t _space_sem;
    int _c_step;
    int _p_step;
    int _max_cap;
};