#include"task.h"

#define tasknum 2
typedef void(*func_t)();

func_t tasks[tasknum]={nullptr};

void LoadTask()
{
    tasks[0]=DownLoad;
    tasks[1]=PrintLog;
}
void Handler()
{
    for(int i=0;i<tasknum;i++)
    {
        tasks[i]();
    }
}
void DoingThings()
{
    Handler();
}
void ChildRun()
{
    std::cout<<"child is running pid="<<getpid()<<std::endl;
    sleep(5);
}
int main()
{
    pid_t id=fork();
    if(id==0)
    {
        ChildRun();
        _exit(1);
    }
    LoadTask();
    while(1)
    {
        int status=0;
        pid_t rid=waitpid(id,&status,WNOHANG);
        if(rid==0)
        {
            std::cout<<"child is running"<<std::endl;
            DoingThings();
            sleep(1);
        }
        else if(rid>0)
        {
            if(WIFEXITED(status))
            {
                std::cout<<"child exit success,code="<<WEXITSTATUS(status)<<std::endl;
            }
            else
            {
                std::cout<<"child exit unnormal"<<std::endl;
            }
            break;
        }
        else
        {
            std::cout<<"wait failed"<<std::endl;
            break;
        }
    }
}