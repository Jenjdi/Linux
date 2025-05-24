#pragma once
#include<functional>
using task_t=std::function<void()>;
void mission()
{
    cout<<"this is a task"<<endl;
}
class task
{
    public:
    task()
    {}
    task(int x,int y)
    :_x(x),_y(y)
    {}
    int execute()
    {
        return _x+_y;
    }
    string debug()
    {
        string msg=to_string(_x)+"+"+to_string(_y)+"=";
        return msg;
    }
    private:
    int _x;
    int _y;
};