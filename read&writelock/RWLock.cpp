#include<condition_variable>
#include<mutex>
using namespace std;
class Lock
{
    private:
    mutex _mtx;
    public:
    void lock()
    {
        _mtx.lock();
    }
    void unlock()
    {
        _mtx.unlock();
    }
    mutex& get_mutex()
    {
        return _mtx;
    }
};
class RWlock
{
    private:
    Lock _base_lock;
    condition_variable _read_cv;
    condition_variable _write_cv;
    int _active_read=0;
    int _waiting_write=0;
    bool _active_write=false;
    public:
    void RLock()
    {
        unique_lock<mutex> lock(_base_lock.get_mutex());//这个锁用于保护_active_read
        _read_cv.wait(lock,[this]{
            return _waiting_write==0&&!_active_write;
        });
        _active_read++;
    }
    void RUnlock()
    {
        unique_lock<mutex> lock(_base_lock.get_mutex());//这个锁用于保护_active_read
        _active_read--;
        if(_active_read==0&&_active_write==false)
            _write_cv.notify_one();
    }
    void WLock()
    {
        unique_lock<mutex> lock(_base_lock.get_mutex());//保护下面三个临界资源
        _waiting_write++;
        _write_cv.wait(lock,[this]{
            return !_active_write&&_active_read==0;
        });
        _waiting_write--;
        _active_write=true;
    }
    void WUnlock()
    {
        unique_lock<mutex> lock(_base_lock.get_mutex());
        _active_write=false;
        if(_waiting_write>0)
        {
            _write_cv.notify_one();
        }
        else
        {
            _read_cv.notify_all();
        }
    }
};
class ReadGuard//使用RAII对读者进行管理
{
    private:
    RWlock& _rwlock;
    public:
    explicit ReadGuard(RWlock& rwlock):_rwlock(rwlock)
    {
        _rwlock.RLock();
    }
    ~ReadGuard()
    {
        _rwlock.RUnlock();
    }
    ReadGuard(const ReadGuard&)=delete;
    ReadGuard operator=(const ReadGuard&)=delete;
};
class WriteGuard//使用RAII对写者进行管理
{
    private:
    RWlock& _rwlock;
    public:
    explicit WriteGuard(RWlock& rwlock):_rwlock(rwlock)
    {
        _rwlock.WLock();
    }
    ~WriteGuard()
    {
        _rwlock.WUnlock();
    }
    WriteGuard(const WriteGuard&)=delete;
    WriteGuard operator=(const WriteGuard&)=delete;
};