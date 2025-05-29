// #include "Thread.h"
// #include "ringqueue.h"
// const int globalthreadnum = 5;
// const int globalmaxtasknum = 10;
// namespace Pool
// {
//     template <class T>
//     class Thread_Pool
//     {
//     private:
//         ringqueue<T> _rq;
//         vector<Thread> _threads;
//         int _threadnum;
//         int _sleepnum;
//         bool _isrunning;
//         pthread_mutex_t _cmutex;
//         pthread_mutex_t _pmutex;
//         pthread_cond_t _cond;
//         void Wake()
//         {
//             pthread_cond_signal(&_cond);
//         }
//         void Sleep()
//         {
//             pthread_cond_wait(&_cond, &_cmutex);
//         }
//         bool IsEmpty()
//         {
//             return _rq.IsEmpty();
//         }
//         void Handler(const string &name)
//         {
//             while (true)
//             {
//                 pthread_mutex_lock(&_cmutex);
//                 while (IsEmpty() && _isrunning) // 需要在互斥锁保护下检查条件
//                 {
//                     _sleepnum++;
//                     Sleep(); // 必须与互斥锁配合使用
//                     _sleepnum--;
//                 }
//                 if (IsEmpty() && !_isrunning)
//                 {
//                     pthread_mutex_unlock(&_cmutex);
//                     break;
//                 }
//                 T out;
//                 pthread_mutex_unlock(&_cmutex);
//                 _rq.pop(&out); // ringqueue自己有锁保护
//                 out();
//                 cout << name << ":" << out.result()<<endl;
//             }
//         }

//     public:
//         Thread_Pool(int threadnum = globalthreadnum, int tasknum = globalmaxtasknum)
//             : _threadnum(threadnum),
//               _isrunning(false),
//               _sleepnum(0),
//               _rq(tasknum, &_cmutex, &_pmutex)
//         {
//             pthread_cond_init(&_cond, nullptr);
//         }
//         ~Thread_Pool()
//         {
//             pthread_cond_destroy(&_cond);
//         }
//         void init()
//         {
//             func_t func = bind(&Thread_Pool::Handler, this, placeholders::_1);
//             for (int i = 0; i < _threadnum; i++)
//             {
//                 string name = "Thread-" + to_string(i + 1);
//                 _threads.emplace_back(name, func);
//             }
//         }
//         void push(const T &in)
//         {
//             if (_isrunning)
//             {
//                 _rq.push(in);
//                 if (_sleepnum > 0)
//                 {
//                     Wake();
//                 }
//             }
//         }
//         void start()
//         {
//             _isrunning = true;
//             for (auto &t : _threads)
//             {
//                 t.start();
//             }
//         }
//         void stop() {
//             pthread_mutex_lock(&_cmutex);
//             _isrunning = false;
//             pthread_cond_broadcast(&_cond);
//             pthread_mutex_unlock(&_cmutex);
//         }
//         void join()
//         {
//             if (!_isrunning)
//             {
//                 for (auto &t : _threads)
//                 {
//                     t.join();
//                 }
//             }
//         }
//     };
// }
#include "Thread.h"
#include "LockGuard.hpp"
#include <iostream>
using namespace std;
const int globalnum = 5;
template <class T>
class Thread_Pool
{
private:
    vector<Thread> _threads;
    queue<T> _task_queue;
    int _thread_num;
    int _sleep_num;
    bool _isrunnning;
    pthread_mutex_t _mutex;
    pthread_cond_t _cond;
    static Thread_Pool<T> *_tp;
    static pthread_mutex_t _sig_mutex;
    bool IsEmpty()
    {
        return _task_queue.empty();
    }
    void Sleep()
    {
        pthread_cond_wait(&_cond, &_mutex);
    }
    void Wake()
    {
        pthread_cond_signal(&_cond);
    }
    void WakeUpAll()
    {
        pthread_cond_broadcast(&_cond);
    }
    void Handler(const string &name)
    {
        while (true)
        {
            pthread_mutex_lock(&_mutex);
            while (IsEmpty() && _isrunnning)
            {
                _sleep_num++;
                LOG(INFO, "%s thread sleep begin!\n", name.c_str());
                Sleep();
                LOG(INFO, "%s thread wake up!\n", name.c_str());
                _sleep_num--;
            }
            if (IsEmpty() && !_isrunnning)
            {
                pthread_mutex_unlock(&_mutex);
                LOG(INFO, "%s thread quit\n", name.c_str());
                break;
            }
            T t;
            t = _task_queue.front();
            _task_queue.pop();
            pthread_mutex_unlock(&_mutex);
            t();
            // cout << name << ":" << t.result() << endl;
            LOG(DEBUG, "handler task done, task is : %s\n", name.c_str());
        }
    }
    void init()
    {
        func_t func = bind(&Thread_Pool::Handler, this, placeholders::_1);
        for (int i = 0; i < _thread_num; i++)
        {
            string name = "Thread-" + to_string(i + 1);
            _threads.emplace_back(name, func);
            LOG(DEBUG, "construct thread %s done, init success\n", name.c_str());
        }
    }
    void start()
    {
        _isrunnning = true;
        for (auto &t : _threads)
        {
            LOG(DEBUG, "start thread %s done.\n", t.Name().c_str());
            t.start();
        }
    }

public:
    Thread_Pool(int num = globalnum)
        : _thread_num(num),
          _sleep_num(0),
          _isrunnning(false)
    {
        pthread_cond_init(&_cond, nullptr);
        pthread_mutex_init(&_mutex, nullptr);
    }
    static Thread_Pool<T> *GetInstance()
    {
        if (_tp == nullptr)//第一次创建完成后就不为空了，因此也就可以不用进来获取锁了
        {
            LockGuard lock(&_sig_mutex);
            if (_tp == nullptr)
            {

                LOG(INFO, "create threadpool\n");
                _tp = new Thread_Pool();
                _tp->init();
                _tp->start();
            }
        }
        else
        {
            LOG(INFO, "get threadpool\n");
        }
        return _tp;
    }
    void stop()
    {
        pthread_mutex_lock(&_mutex);
        _isrunnning = false;
        WakeUpAll();
        pthread_mutex_unlock(&_mutex);
        LOG(INFO, "Thread Pool Stop Success!\n");
    }
    void join()
    {
        if (!_isrunnning)
        {
            for (auto &t : _threads)
            {
                t.join();
            }
        }
    }
    void push(const T &in)
    {
        pthread_mutex_lock(&_mutex);
        if (_isrunnning)
        {
            _task_queue.push(in);
            if (_sleep_num > 0)
            {
                Wake();
            }
        }
        pthread_mutex_unlock(&_mutex);
    }
};
template <class T>
Thread_Pool<T> *Thread_Pool<T>::_tp = nullptr;
template <class T>
pthread_mutex_t Thread_Pool<T>::_sig_mutex = PTHREAD_MUTEX_INITIALIZER;