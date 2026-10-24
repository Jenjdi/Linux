#include <unistd.h>
#include <pthread.h>
#include <functional>
#include <string>
#include <queue>
#include <vector>
#include"log.h"
static int _connectnum;

// 修复：线程池回调接收线程名，保持声明和调用一致。
using ThreadFunction = std::function<void(const std::string&)>;
class Thread
{
private:
    pthread_t _tid;
    std::string _name;
    ThreadFunction _func;
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
        _func(_name);
        _isrunning = false;
    }

public:
    Thread(const std::string &name, ThreadFunction func)
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
        // join 等待线程结束，不能以运行状态跳过等待。
        pthread_join(_tid, nullptr);
    }
};
