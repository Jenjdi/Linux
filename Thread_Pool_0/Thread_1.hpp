#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <string>
#include <functional>
using namespace std;
class ThreadData
{
private:
    string _name;
    pthread_mutex_t *_mutex;

public:
    ThreadData(const string &name, pthread_mutex_t *mutex)
        : _name(name), _mutex(mutex) {}
};
using func_t = function<void *(ThreadData *)>;
class Thread
{
private:
    string _name;
    pthread_t _tid;
    ThreadData *_td;
    func_t _func;
    bool _is_running = false;
    static void *ThreadRoutine(void *args)
    {
        Thread *self = static_cast<Thread *>(args);
        self->execute();
    }

public:
    Thread(const string &name, func_t func, ThreadData *td)
        : _name(name), _func(func), _td(td) {}
    void execute()
    {
        _is_running = true;
        _func(_td);
        _is_running = false;
    }
    bool start()
    {
        int n = pthread_create(&_tid, nullptr, ThreadRoutine, this);
        if (n == 0)
            return true;
        return false;
    }
    bool stop()
    {
        if (_is_running)
        {
            _is_running = false;
            pthread_cancel(_tid);
        }
    }
    bool status()
    {
        return _is_running;
    }
    const string name()
    {
        return _name;
    }
    void join()
    {
        if (!_is_running)
            pthread_join(_tid, nullptr);
    }
};