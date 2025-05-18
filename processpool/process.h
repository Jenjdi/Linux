// #include <iostream>
// #include <cerrno>
// #include <unistd.h>
// #include <sys/types.h>
// #include <sys/wait.h>
// #include <string>
// #include <vector>
// #include <cstdlib>
// using namespace std;
// #define TaskNUM 4
// typedef void (*task_t)();
// task_t task[TaskNUM];
// void print()
// {
//     cout << "print task" << endl;
// }
// void Download()
// {
//     cout << "download task" << endl;
// }
// void Upload()
// {
//     cout << "upload task" << endl;
// }
// void Process()
// {
//     cout << "process task" << endl;
// }
// void load()
// {
//     srand(time(nullptr));
//     task[0] = print;
//     task[1] = Download;
//     task[2] = Upload;
//     task[3] = Process;
// }

// class Channel
// {
// public:
//     Channel(int wfd, pid_t pid, string &name)
//         : _wfd(wfd),
//           _processid(pid),
//           _name(name) {};
//     int getWfd()
//     {
//         return _wfd;
//     }
//     pid_t getprocessid()
//     {
//         return _processid;
//     }
//     string getname()
//     {
//         return _name;
//     }
//     void closeprocess()
//     {
//         close(_wfd);
//     }
//     void waitprocess()
//     {
//         pid_t rid = waitpid(_processid, nullptr, 0);
//         if (rid > 0)
//         {
//             cout << "wait" << rid << "success" << endl;
//         }
//     }

// private:
//     int _wfd;
//     pid_t _processid;
//     string _name;
// };
// void cleanchannel(vector<Channel> &channel)
// {
//     for (auto &ch : channel)
//     {
//         ch.closeprocess();
//     }
//     for (auto &ch : channel)
//     {
//         ch.waitprocess();
//     }
// }
// void Execute(int tasknum) // 执行任务
// {
//     if (tasknum > TaskNUM || tasknum < 0)
//     {
//         cerr << "task num error" << endl;
//         return;
//     }
//     task[tasknum]();
// }
// void work(int rfd) // 开始工作
// {
//     while (true)
//     {
//         int command = 0;
//         int n = read(rfd, &command, sizeof(command));
//         if (n == sizeof(command))
//         {
//             Execute(command);
//         }
//         else
//         {
//             break;
//         }
//     }
// }
// int selecttask()
// {
//     // 随机选择任务
//     int ret = rand() % TaskNUM;
//     return ret;
// }
// int selectchannel(int channelnum)
// {
//     // 循环选择进程分配
//     static int next = 0;
//     int ret = next;
//     next++;
//     next %= channelnum;
//     return ret;
// }
// void sendtask(Channel &channel, int tasknum)
// {
//     // 给信道发送任务
//     write(channel.getWfd(), &tasknum, sizeof(tasknum));
// }
// void processctrl_once(vector<Channel> &channel)
// {
//     sleep(1);
//     int tasknum = selecttask();
//     int channelnum = selectchannel(channel.size());
//     sendtask(channel[channelnum], tasknum);
//     cout<<endl;
//     cout<<"tasknum=:"<<tasknum<<"channel num=:"<<channelnum<<endl;
// }
// void processctr(vector<Channel> &channel, int times = -1)
// {
//     if (times > 0)
//     {
//         while (times--)
//         {
//             processctrl_once(channel);
//         }
//     }
//     else
//     {
//         while (true)
//         {
//             processctrl_once(channel);
//         }
//     }
// }

// void creatprocess(int num, vector<Channel> &channel)
// {

//     int pipefd[2] = {0};
//     for (int i = 0; i < num; i++)
//     {
//         ssize_t pipid = pipe(pipefd);
//         if (pipid == -1)
//         {
//             cerr << "error" << endl;
//             exit(1);
//         }
//         pid_t pid = fork();
//         if (pid == 0)
//         {
//             close(pipefd[1]);
//             work(pipefd[0]);
//             close(pipefd[0]);
//             exit(0);
//         }
//         close(pipefd[0]);
//         string name = "child process-" + to_string(pid);
//         channel.push_back(Channel(pipefd[1], pid, name));
//     }
// }
#include <iostream>
#include <string>
#include <vector>

#include <sys/wait.h>
#include <unistd.h>

#include "task.h"

void load()
{
    srand(time(nullptr));
    task[0] = print;
    task[1] = Download;
    task[2] = Upload;
    task[3] = Process;
}
class Channel // 用来管理所有进程
{
private:
    int _wfd;
    pid_t _processid;
    string _name;

public:
    Channel(int wfd, pid_t pid, const string &name)
        : _wfd(wfd),
          _processid(pid),
          _name(name) {}
    int getwfd()
    {
        return _wfd;
    }
    string getname()
    {
        return _name;
    }
    pid_t getpid()
    {
        return _processid;
    }
    void waitprocess()
    {
        int statistic;
        pid_t rid = waitpid(_processid, &statistic, 0);
        if (rid < 0)
        {
            cerr << "wait " << _processid << " failed" << endl;
        }
        else
        {
            cout<<"wait "<<_processid<<" success"<<endl;
        }
    }
    void closefd()
    {
        close(_wfd);
        //cout<<"close"<<_wfd<<"success"<<endl;
    }
};
int selecttask()
{
    return rand() % taskNum;
}
int selectchannel(int channelnum)
{
    static int next = 0;
    int ret = next;
    next++;
    next %= channelnum;
    return ret;
}
void sendtask(Channel &channel, int tasknum)
{
    write(channel.getwfd(), &tasknum, sizeof(tasknum));
}
void process_ctrl_once(vector<Channel> &channels)
{
    sleep(1);
    int tasknum = selecttask();
    int channelnum = selectchannel(channels.size());
    sendtask(channels[channelnum], tasknum);
    cout << "--------------------------------------------" << endl;
    cout << "task " << tasknum << " send to " << channelnum << " " << channels[channelnum].getname() << endl;
}
void process_ctrl(vector<Channel> &channels, int times = -1)
{
    if (times > 0)
    {
        while (times--)
        {
            process_ctrl_once(channels);
        }
    }
    else
    {
        while (true)
        {
            process_ctrl_once(channels);
        }
    }
}
void execute(int command)
{
    if (command < 0 || command > taskNum)
    {
        cerr << "execute failed" << endl;
        return;
    }
    task[command]();
}
void work(int rfd)
{
    while (true)
    {
        int command;
        int n = read(rfd, &command, sizeof(command));
        if (n == sizeof(command))
        {
            cout << "pid:" << getpid() << " is handling " << command << endl;
            execute(command);
        }
        else
        {
            cout<<"worker:"<<getpid()<<" quit success"<<endl;
            break;
        }
    }
}
// 命名规范：&表示输入输出型参数，const&表示输入型参数，*表示输出型参数
void createprocess(int num, vector<Channel> *channel) // 这里的channel用于输出，因此使用*
{
    for (int i = 0; i < num; i++)
    {
        int pipefd[2];
        if (pipe(pipefd) < 0)
        { // 创建新管道
            cerr << "pipe() error" << endl;
            exit(1);
        }
        pid_t pid = fork();
        if (pid == 0)
        {
            if(!channel->empty())//若为空，说明这是第一个创建的子进程，因此不做调整
            {
                for(auto& ch:*channel)
                {
                    ch.closefd();//从第二个进程开始，就会出现写端指向了不该去的地方，因此遍历所有的子进程，关闭写端
                    //因为父进程并不在channel中，因此不会将父进程的误关闭
                }
            }
            close(pipefd[1]);
            work(pipefd[0]);
            close(pipefd[0]);
            exit(0);
        }
        close(pipefd[0]);
        string name = "childprocess-" + to_string(pid);
        channel->emplace_back(Channel(pipefd[1], pid, name)); // 提高效率
    }
}
void CleanProcess(vector<Channel> &channels)
{
    for (auto &ch : channels)
    {
        ch.closefd();
        ch.waitprocess();
    }

}