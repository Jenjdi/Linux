#include <unistd.h>
#include <pthread.h>
#include <functional>
#include <string>
#include <queue>
#include <vector>
#include"log.h"
static int _connectnum;

using func_t = std::function<void(int,int)>;
class Thread
{
private:
    pthread_t _tid;
    std::string _name;
    func_t _func;
    bool _isrunning;
    static void *ThreadRoutine(void *arg)
    {
        Thread *self = static_cast<Thread *>(arg);
        self->Execute();
        return nullptr;
    }
    void Execute()
    {
        int timeout=-1;
        _isrunning = true;
        _func(timeout);
        _isrunning = false;
    }

public:
    Thread(const std::string &name, func_t func)
        : _name(name),
          _func(func) {}
    bool start()
    {
        int n = pthread_create(&_tid, nullptr, ThreadRoutine, this);
        if (n != 0)
        {
            return false;
        }
        return true;
    }
    const std::string& Name()
    {
        return _name;
    }
    void stop()
    {
        pthread_cancel(_tid);
        _isrunning = false;
    }
    void join()
    {
        if (!_isrunning)
            pthread_join(_tid, nullptr);
    }
};
