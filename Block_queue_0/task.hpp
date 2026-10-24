#include <iostream> // 修复独立包含时缺少标准输出流声明。
#pragma once
#include<functional>
using task_t=std::function<void()>;
void mission()
{
    std::cout<<"this is a task"<<std::endl;
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
    std::string debug()
    {
        std::string msg=std::to_string(_x)+"+"+std::to_string(_y)+"=";
        return msg;
    }
    private:
    int _x = 0;
    int _y = 0;
};
