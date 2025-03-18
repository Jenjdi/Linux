#pragma once
#include<pthread.h>
#include<bits/stdc++.h>
#include<unistd.h>
#include"thread.hpp"
#include"LockGuard.hpp"
#include<memory>
void test()
{
    cout<<"TEST MESSAGE"<<endl;
}
using namespace std;
static const int gdefaultnum=5;
template<typename T>
class thread_pool
{
    public:
    thread_pool(int threadnum=gdefaultnum)
    :_threadnum(threadnum),_isrunning(false)
    {
        pthread_cond_init(&_cond,nullptr);
        pthread_mutex_init(&_mutex,nullptr);
    }
    void init()
    {
        func_t func=bind(&thread_pool::Handler,this);
        for(int i=0;i<_threadnum;i++)
        {
            string threadname="thread-"+to_string(i+1);
            _threads.emplace_back(threadname,func);
        }   
    }
    void wake()
    {
        pthread_cond_signal(&_cond);
    }
    void sleep()
    {
        pthread_cond_wait(&_cond,&_mutex);
    }
    bool IsEmpty()
    {
        return _task_queue.empty();
    }
    void start()
    {
        _isrunning=true;
        for(auto t:_threads)
        {
            t.start();
        }
        _isrunning=false;
    }
    void Handler()
    {
        while(true)
        {
            LockGuard lock(_mutex);
            while(IsEmpty())//防止伪唤醒
            {
                _sleep_threadnum++;
                sleep();
            }
            T t=_task_queue.front();
            _task_queue.pop();
        }
    }
    void stop()
    {
            _isrunning=false;
            for(auto t:_threads)
            {
                t.stop();
            }
    }
    void push(const T& in)
    {
        LockGuard lock(_mutex);
        _task_queue.push(in);
        if(_sleep_threadnum>0)
        {
            wake();
        }
    }
    void pop(T* out)
    {

    }
    ~thread_pool()
    {
        pthread_cond_destroy(&_cond);
        pthread_mutex_destroy(&_mutex);
    }
    private:
    int _threadnum;
    vector<Thread> _threads;
    queue<T> _task_queue;
    bool _isrunning;
    int _sleep_threadnum;
    pthread_mutex_t _mutex;
    pthread_cond_t _cond;
};