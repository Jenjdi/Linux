#include"thread.hpp"
#include"LockGuard.hpp"
using namespace std;

void* route(ThreadData* td)
{
    LockGuard lock(td->_mutex);
    while(true)
    {
        cout<<td->_name<<":mutex address="<<td->_mutex<<endl;
        sleep(1);
    }
    
}
// int main()
// {

//     Thread t("thread_1",Print);
//     t.start();
//     cout<<t.Name()<<":status"<<t.status()<<endl;
//     sleep(5);
//     t.stop();
//     cout<<t.Name()<<":status"<<t.status()<<endl;
//     sleep(5);
//     t.join();
//     cout<<t.Name()<<":status"<<t.status()<<endl;
//     sleep(5);

// }
static int threadnum=5;
int main()
{
    pthread_mutex_t mutex;
    pthread_mutex_init(&mutex,nullptr);
    vector<Thread> threads;
    for(int i=0;i<threadnum;i++)
    {
        string name="threads"+to_string(i+1);
        ThreadData *td=new ThreadData(name,&mutex);
        threads.emplace_back(name,route,td);
    }
    for(auto t:threads)
    {
        t.start();
    }
    sleep(5);
    for(auto t:threads)
    {
        t.stop();
    }
    sleep(5);
    for(auto t:threads)
    {
        t.join();
    }
}