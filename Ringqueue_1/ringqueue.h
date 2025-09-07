#include<vector>
#include<pthread.h>
#include<semaphore.h>
#include<string>
#include<unistd.h>
const int DefaultCapacity=10;
using namespace std;
template<class T>
class RingQueue
{
    private:
    int max_capacity;
    int consumer_step;
    int producer_step;
    sem_t data_sem;
    sem_t space_sem;
    pthread_mutex_t consumer_mutex;
    pthread_mutex_t producer_mutex;
    vector<T> Queue;
    public:
    RingQueue(int MaxCapacity=DefaultCapacity)
    :max_capacity(MaxCapacity),Queue(MaxCapacity)
    {
        pthread_mutex_init(&consumer_mutex,nullptr);
        pthread_mutex_init(&producer_mutex,nullptr);
        sem_init(&data_sem,0,0);
        sem_init(&space_sem,0,max_capacity);
        consumer_step=0;
        producer_step=0;
    }
    ~RingQueue()
    {
        pthread_mutex_destroy(&consumer_mutex);
        pthread_mutex_destroy(&producer_mutex);
        sem_destroy(&data_sem);
        sem_destroy(&space_sem);
    }
    void P(sem_t& sem)
    {
        sem_wait(&sem);
    }
    void V(sem_t& sem)
    {
        sem_post(&sem);
    }
    void push(const T& val)
    {
        pthread_mutex_lock(&producer_mutex);
        P(space_sem);
        Queue[producer_step]=val;
        producer_step++;
        producer_step%=max_capacity;
        V(data_sem);
        pthread_mutex_unlock(&producer_mutex);
    }
    void pop(T* val)
    {
        pthread_mutex_lock(&consumer_mutex);
        P(data_sem);
        *val=Queue[consumer_step];
        consumer_step++;
        consumer_step%=max_capacity;
        V(space_sem);
        pthread_mutex_unlock(&consumer_mutex);
    }
};