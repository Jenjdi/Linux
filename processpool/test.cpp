#include "process.h"
// int i=0;
// void childprocesswrite(int wrfd)
// {
//     string messages="child process write message:pid=";

//     while(true)
//     {
//         string info=messages+to_string(getpid())+"messageid=:"+to_string(i);
//         ssize_t ret=write(wrfd,info.c_str(),info.size());
//         i++;
//     }

// }
// void fatherprocessread(int rfd)
// {
//     char buf[1024];
//     while(true)
//     {
//         sleep(2);
//         ssize_t ret=read(rfd,buf,sizeof(buf)-1);
//         if(ret>0)
//         {
//             buf[ret]='\0';
//         }
//         cout<<"father received:"<<buf<<endl;
//     }
// }
// int main()
// {
//     int pipefd[2];
//     int n=pipe(pipefd);
//     if(n==-1)
//     {
//         cout<<"error";
//         return -1;
//     }
//     pid_t child=fork();
//     if(child==0)
//     {
//         close(pipefd[0]);
//         childprocesswrite(pipefd[1]);
//         close(pipefd[1]);
//     }
//     else
//     {
//         close(pipefd[1]);
//         fatherprocessread(pipefd[0]);
//         close(pipefd[0]);
//         exit(0);
//         wait(NULL);
//     }

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

// private:
//     int _wfd;
//     pid_t _processid;
//     string _name;
// };
// void work(int rfd)
// {
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
//             close(pipefd[0]);
//             work(pipefd[1]);
//             close(pipefd[1]);
//             exit(0);
//         }
//         else
//         {
//             close(pipefd[1]);
//             string name = "child process-" + to_string(pid);
//             channel.push_back(Channel(pipefd[0], pid, name));
//         }
//     }
// }
// int main()
// {
//     vector<Channel> channel;
//     creatprocess(10, channel);
//     for (auto &ch : channel)
//     {
//         cout << "--------------------------" << endl;
//         cout << ch.getname() << endl;
//         cout << ch.getprocessid() << endl;
//         cout << ch.getWfd() << endl;
//     }
// }
int main(int argc,char* argv[])
{
    if(argc!=2)
    {
        cerr<<"usage "<<argv[0]<<" processnum"<<endl;
        return 1;
    }
    int num=stoi(argv[1]);
    vector<Channel> channel;
    //加载任务
    load();
    //创建子进程
    createprocess(num, &channel);
    //为每个进程分配任务
    //processctr(channel,4);
    //销毁子进程
    //cleanchannel(channel);
    process_ctrl(channel,5);
    CleanProcess(channel);
}
