#include<unistd.h>
#include<pthread.h>
#include<iostream>
#include<queue>
#include<string>
using namespace std;
const static int defaut=10;
template<typename T>
class BlockQueue
{
    public:
    BlockQueue(int cap=defaut)
    :_max_cap(cap)
    {
        pthread_mutex_init(&_consumer_mutex,nullptr);
        pthread_mutex_init(&_produce_mutex,nullptr);
        pthread_cond_init(&_popcond,nullptr);
        pthread_cond_init(&_pushcond,nullptr);
    }
    void push(T in)
    {
        pthread_mutex_lock(&_produce_mutex);
        while(_block_queue.size()==_max_cap)
        {
            pthread_cond_wait(&_pushcond,&_produce_mutex);
        }
        _block_queue.push(in);
        pthread_mutex_unlock(&_produce_mutex);
        pthread_cond_signal(&_popcond);
    }
    void pop(T* out)
    {
        pthread_mutex_lock(&_consumer_mutex);
        while(_block_queue.empty())
        {
            pthread_cond_wait(&_popcond,&_consumer_mutex);
            
        }
        *out=_block_queue.front();
        _block_queue.pop();
        pthread_mutex_unlock(&_consumer_mutex);
        pthread_cond_signal(&_pushcond);
    }
    ~BlockQueue()
    {
        pthread_mutex_destroy(&_produce_mutex);
        pthread_mutex_destroy(&_consumer_mutex);
        pthread_cond_destroy(&_pushcond);
        pthread_cond_destroy(&_popcond);
    }
    private:
    int _max_cap;
    queue<T> _block_queue;
    pthread_mutex_t _produce_mutex;
    pthread_mutex_t _consumer_mutex;
    pthread_cond_t _pushcond;
    pthread_cond_t _popcond;
    
};