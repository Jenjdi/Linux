#pragma once
#include <pthread.h>
#include <bits/stdc++.h>
#include <unistd.h>
#include "thread.hpp"
#include "LockGuard.hpp"
#include <memory>
void test()
{
    cout << "TEST MESSAGE" << endl;
}
using namespace std;
static const int gdefaultnum = 5;
template <typename T>
class thread_pool
{
private:
    void LockQueue()
    {
        pthread_mutex_lock(&_mutex);
    }
    void UnlockQueue()
    {
        pthread_mutex_unlock(&_mutex);
    }
    void wake()
    {
        pthread_cond_signal(&_cond);
    }
    void WakeupAll()
    {
        pthread_cond_broadcast(&_cond);
    }
    void Sleep()
    {
        pthread_cond_wait(&_cond, &_mutex);
    }
    bool IsEmpty()
    {
        return _task_queue.empty();
    }
    void Handler()
    {
        while (true)
        {
            LockQueue();
            while (IsEmpty() && _isrunning) // 防止伪唤醒
            {
                _sleep_threadnum++;
                Sleep();
                _sleep_threadnum--;
            }
            if (IsEmpty() && !_isrunning)
            {
                UnlockQueue();
                break;
            }
            T t = _task_queue.front();
            _task_queue.pop();
            UnlockQueue();
            t();
        }
    }
    void init()
    {
        func_t func = bind(&thread_pool::Handler, this);
        for (int i = 0; i < _threadnum; i++)
        {
            string threadname = "thread-" + to_string(i + 1);
            _threads.emplace_back(threadname, func);
        }
    }
    void start()
    {
        _isrunning = true;
        for (auto& t : _threads)
        {
            t.start();
        }
    }
    thread_pool(const thread_pool<T> &) = delete;
    void operator=(const thread_pool<T> &) = delete;
public:
    thread_pool(int threadnum = gdefaultnum)
        : _threadnum(threadnum), _isrunning(false)
    {
        pthread_cond_init(&_cond, nullptr);
        pthread_mutex_init(&_mutex, nullptr);
    }
    
    void stop()
    {
        LockQueue();
        _isrunning = false;
        WakeupAll();
        UnlockQueue();
    }
    void push(const T &in)
    {
        LockQueue();
        if (_isrunning)
        {
            _task_queue.push(in);
            if (_sleep_threadnum > 0)
            {
                wake();
            }
        }
        UnlockQueue();
    }
    static thread_pool<T>* GetInstance()
    {
        if(_tp==nullptr)
        {
            LockGuard lockguard(&_sig_mutex);
            if(_tp==nullptr)
            {
                _tp=new thread_pool();
                _tp->init();
                _tp->start();
            }
        }
        return _tp;
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
    static pthread_mutex_t _sig_mutex;
    static thread_pool<T>* _tp;
};
template <typename T>
thread_pool<T>* thread_pool<T>::_tp=nullptr;
template <typename T>
pthread_mutex_t thread_pool<T>::_sig_mutex=PTHREAD_MUTEX_INITIALIZER;
