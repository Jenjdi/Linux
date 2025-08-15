#include<thread>
#include<mutex>
#include<functional>
#include<condition_variable>
#include<queue>
#include<vector>
template<typename T>
class BlockQueue
{
    private:
    std::mutex _producer;
    std::mutex _consumer;
    std::condition_variable _not_empty;
    bool _nonblock;
    std::queue<T> _consumer_queue;
    std::queue<T> _producer_queue;
    int swap_queue()
    {
        std::unique_lock<std::mutex> lock(_producer);
        _not_empty.wait(lock,[this]{return !_producer_queue.empty()||_nonblock;});
        std::swap(_consumer_queue,_producer_queue);
        return _consumer_queue.size();//因为已经完成了交换，所以consumer队列中存放的实际上是生产者队列中的内容
    }
    public:
    BlockQueue()
    :_nonblock(false){}
    void push(const T& value)
    {
        std::lock_guard<std::mutex> lock(_producer);
        _producer_queue.emplace(value);
        _not_empty.notify_one();//随便选一个正在等待的线程进行调度
    }
    bool pop(T& value)
    {
        std::unique_lock<std::mutex> lock(_consumer);
        if(_consumer_queue.empty()&&swap_queue()==0)
            return false;
        value=_consumer_queue.front();
        _consumer_queue.pop();
        return true;
    }
    void Cancel()
    {
        std::unique_lock<std::mutex> lock(_producer);
        _nonblock=true;
        _not_empty.notify_all();
    }
};
template<typename T>
class ThreadPool
{
    private:
    BlockQueue<T> _queue;
    std::vector<std::thread> _worker;
    void Worker()
    {
        while(true)
        {
            std::function<void()> task;
            if(!_queue.pop(task))
                break;
            task();
        }
    }
    public:
    ThreadPool(int num)
    {
        for(int i=0;i<num;i++)
        {
            _worker.emplace_back([this]->void{Worker();});
        }
    }
    template<typename F,typename... Args>
    void Post(F&& f,Args &&...args)
    {
        auto task=std::bind(std::forward<F>(f),std::forward<Args>(args)...);
        _queue.push(task);
    }
    ~ThreadPool()
    {
        _queue.Cancel();
        for(auto& worker:_worker)
        {
            if(worker.joinable())
            {
                worker.join();
            }
        }
    }
};