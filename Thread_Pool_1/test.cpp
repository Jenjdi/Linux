#include "Thread_Pool.h"
#include <iostream>
#include <time.h>
class Task
{
public:
    Task()
    {
    }
    Task(int x, int y) : _x(x), _y(y)
    {
    }
    void Excute()
    {
        _result = _x + _y;
    }
    void operator()()
    {
        Excute();
    }
    std::string debug()
    {
        std::string msg = std::to_string(_x) + "+" + std::to_string(_y) + "=?";
        return msg;
    }
    std::string result()
    {
        std::string msg = std::to_string(_x) + "+" + std::to_string(_y) + "=" + std::to_string(_result);
        return msg;
    }

private:
    int _x;
    int _y;
    int _result;
};

int main()
{
    
    
    int cnt=10;
    while(cnt--)
    {
        sleep(1);
        Task t(1,1);
        Thread_Pool<Task>::GetInstance()->push(t);
    }
    //tp.stop();
    Thread_Pool<Task>::GetInstance()->stop();
    return 0;
}