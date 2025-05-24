#include "block_queue.h"
#include "task.h"
#include <time.h>
using namespace std;

void *producer(void *arg)
{
    while (1)
    {
        srand(time(nullptr));
        int x = rand() % 10;
        int y = rand() % 10;
        Block_Queue<Task> *bq = static_cast<Block_Queue<Task> *>(arg);
        cout << "push task " << x << " and " << y << endl;

        bq->push(Task(x, y));
        sleep(1);
    }

    return nullptr;
}
void *consumer(void *arg)
{
    while (1)
    {
        Block_Queue<Task> *bq = static_cast<Block_Queue<Task> *>(arg);
        Task out;
        bq->pop(&out);
        cout << "execute task: ";
        out.Execute();
        sleep(1);
    }

    return nullptr;
}
int main()
{
    pthread_t cpid, ptid;
    Block_Queue<Task> *bq = new Block_Queue<Task>;
    pthread_create(&ptid, nullptr, producer, (void *)bq);
    pthread_create(&cpid, nullptr, consumer, (void *)bq);
    pthread_join(cpid, nullptr);
    pthread_join(ptid, nullptr);
}