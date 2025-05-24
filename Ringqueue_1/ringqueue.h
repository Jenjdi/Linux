#include<pthread.h>
#include<iostream>
#include<vector>
#include<semaphore.h>
using namespace std;
template<class T>
class ringqueue
{
    private:
    int _maxcap;
    vector<T> _ringqueue;
    int _cstep;
    int _pstep;
    sem_t _data_sem;
    sem_t _space_sem;
    pthread_mutex_t _cmutex;
    pthread_mutex_t _pmutex;
    public:
    ringqueue(int maxcap)
    :_maxcap(maxcap),
    _ringqueue(_maxcap),
    _cstep(0),
    _pstep(0)
    {
        sem_init(&_data_sem,0,0);
        sem_init(&_space_sem,0,_maxcap);//线程间共享
        pthread_mutex_init(&_cmutex);
        pthread_mutex_init(&_pmutex);
    }
    ~ringqueue()
    {
        sem_destroy(&_data_sem);
        sem_destroy(&_space_sem);
        pthread_mutexattr_destroy(&_pmutex);
        pthread_mutexattr_destroy(&_cmutex);
    }
    bool P(sem_t* sem)
    {
        int n=sem_wait(sem);
        if(n==0)
        {
            return true;
        }
        return false;
    }
    bool V(sem_t* sem)
    {
        sem_post(sem);
        if(n==0)
        {
            return true;
        }
        return false;
    }
    
    void push(const T& in)
    {
        P(&_space_sem);
        pthread_mutex_lock(&_pmutex);
        _ringqueue[_pstep]=in;
        _pstep++;
        _pstep%=_maxcap;
        pthread_mutex_unlock(&_pmutex);
        V(&_data_sem);
    }
    void pop(const T& out)
    {
        P(&_data_sem);
        pthread_mutex_lock(&_cmutex);
        out=_ringqueue[_cstep];
        _cstep++;
        _cstep%=_maxcap;
        pthread_mutex_unlock(&_cmutex);
        V(&_space_sem);
    }
};