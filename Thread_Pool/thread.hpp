#pragma once
#include<string>
#include<unistd.h>
#include<iostream>
#include<pthread.h>
#include<functional>
using namespace std;

using func_t=function<void()>;
class Thread
{
    public:
    void Excute()
    {
        _isrunning=true;
        _func();
        _isrunning=false;
    }
    string Name()
    {
        return _name;
    }
    public:
    Thread(const string& name,func_t func)
    :_name(name),_func(func)
    {}
    string status()
    {
        if(_isrunning) return "Thread is running";
        else return "Thread has stopped";
    }
    static void* ThreadRoutine(void* args)//隐含了当前对象的this指针，因此不能直接传入，需要static修饰
    {
        Thread* self=static_cast<Thread*>(args);
        self->Excute();
    }
    bool start()
    {
        int n=pthread_create(&_tid,nullptr,ThreadRoutine,this);//由于static修饰后无法访问当前对象的成员变量，因此将this指针传入
        if(n!=0)
        {
            return false;
        }
        return true;
    }
    void stop()
    {
        if(_isrunning)
        {
            _isrunning=false;
            pthread_cancel(_tid);
        }
    }
    void join()
    {
        if(!_isrunning)
        {
            pthread_join(_tid,nullptr);
        }
    }

    ~Thread()
    {
        
    }
    private:
    bool _isrunning;
    string _name;
    pthread_t _tid;
    func_t _func;
    
};