#include <pthread.h>
#include <queue>
#include <unistd.h>
using namespace std;
template <class T>
class Block_Queue
{
private:
    pthread_cond_t _p_cond;
    pthread_cond_t _c_cond;
    pthread_mutex_t _mutex;
    queue<T> _block_queue;
    int _max_cap = 10;

public:
    bool IsFull()
    {
        return _max_cap == _block_queue.size();
    }
    bool IsEmpty()
    {
        return _block_queue.size() == 0;
    }
    Block_Queue()
    {
        pthread_cond_init(&_p_cond, nullptr);
        pthread_mutex_init(&_mutex, nullptr);
        pthread_cond_init(&_c_cond, nullptr);
    }
    ~Block_Queue()
    {
        pthread_mutex_destroy(&_mutex);
        pthread_cond_destroy(&_p_cond);
        pthread_cond_destroy(&_c_cond);
    }
    void pop(T *out)
    {
        pthread_mutex_lock(&_mutex);
        // if(IsEmpty())
        while (IsEmpty())
        {
            // 如果条件还没有满足，但是线程被异常唤醒了，这种情况叫做伪唤醒
            // 出现伪唤醒时，这个被伪唤醒的线程会先竞争锁，竞争成功后开始获取队列
            // 但是此时队列中已经没有数据了，因此会出问题
            // 所以这里的判断条件要用while，线程唤醒后重新判断一遍是否为空，如果真的为空则执行下面的代码
            pthread_cond_wait(&_c_cond, &_mutex);
        }
        *out = _block_queue.front();
        _block_queue.pop();

        // 在释放锁之前或者之后唤醒都可以，因为唤醒使另一个线程还必须要重新竞争获得锁才能进行后续操作
        // 因此不会出问题
        pthread_mutex_unlock(&_mutex);
        pthread_cond_signal(&_p_cond);
    }
    void push(const T &in)
    {
        pthread_mutex_lock(&_mutex);
        while (IsFull())
        {
            pthread_cond_wait(&_p_cond, &_mutex);
            // 被调用的时候，除了让自己排队等待，还会释放传入的锁
            // 由于返回时，还在临界区中，因此必须参与锁的竞争，重新加上锁，函数才能成功返回
        }
        _block_queue.push(in);

        pthread_mutex_unlock(&_mutex);
        pthread_cond_signal(&_c_cond);
    }
};