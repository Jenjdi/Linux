// #include <pthread.h>
// #include <iostream>
// #include <string>
// #include <cstring>
// #include <functional>

// using namespace std;
// class ThreadData
// {
// public:
//     string _name;
//     pthread_mutex_t *_mutex;

//     ThreadData(const string &name, pthread_mutex_t *mutex)
//         : _name(name),
//           _mutex(mutex)
//     {
//     }
// };
// using func_t=function<void*(ThreadData* td)>;

// class Thread
// {
// private:
//     pthread_t _tid;
//     func_t _func;
//     string _name;
//     ThreadData *_td;
//     bool _isrunning;
//     void Execute()
//     {
//         _isrunning = true;
//         _func(_td);
//         _isrunning = false;
//     }
//     static void *ThreadRoutine(void *arg) // 类内函数默认第一个参数为this指针，因此将设置为static
//     {
//         Thread *self = static_cast<Thread *>(arg);
//         self->Execute(); // 类内的静态成员函数在获取this指针后就可以访问任意成员了
//         return nullptr;
//     }

// public:
//     bool start()
//     {
//         int n = pthread_create(&_tid, nullptr, ThreadRoutine, this);
//         if (0 != n)
//         {
//             return false;
//         }

//         return true;
//     }
//     void stop()
//     {
//         if (_isrunning)
//         {
//             int n = pthread_cancel(_tid);
//             _isrunning = false;
//         }
//     }
//     void join()
//     {
//         if (!_isrunning)
//             pthread_join(_tid, nullptr);
//     }
//     void status()
//     {
//         if (_isrunning)
//             cout << "running" << endl;
//         else
//             cout << "stop" << endl;
//     }

//     Thread(const string &name, func_t func, ThreadData *td);
//     ~Thread();
// };

// Thread::Thread(const string &name, func_t func, ThreadData *td)
//     : _name(name),
//       _func(func),
//       _td(td)
// {
//     cout << _name << " created" << endl;
// }

// Thread::~Thread()
// {
// }
#include <unistd.h>
#include <pthread.h>
#include <string>
#include <cstring>
#include <functional>

namespace My_Thread
{
    class ThreadData
    {
    public:
        std::string _name;
        pthread_mutex_t *_mutex;
        pthread_cond_t* _cond;
        ThreadData(const std::string name, pthread_mutex_t *mutex,pthread_cond_t* cond)
            : _name(name),
              _mutex(mutex),
              _cond(cond) {}
        ~ThreadData()
        {
            pthread_mutex_destroy(_mutex);
            pthread_cond_destroy(_cond);
        }
    };
    using func_t = std::function<void(ThreadData *td)>;
    class Thread
    {
    private:
        ThreadData *_td;
        func_t _func;
        std::string _name;
        pthread_t _tid;
        bool _isrunning;
        static void *ThreadRoute(void *arg)
        {
            Thread *self = static_cast<Thread *>(arg);
            self->Execute();
            return nullptr;
        }
        void Execute()
        {
            _isrunning = true;
            _func(_td);
            _isrunning = false;
        }

    public:
        Thread(const std::string &name, func_t func, ThreadData *td)
            : _name(name),
              _func(func),
              _td(td) {}
        ~Thread()
        {
        }
        ThreadData* gettd()
        {
            return _td;
        }
        void start()
        {
            pthread_create(&_tid, nullptr, ThreadRoute, this);
        }
        void stop()
        {
            _isrunning = false;
            pthread_cancel(_tid);
        }
        void join()
        {
            if (!_isrunning)
                pthread_join(_tid, nullptr);
        }
    };
}