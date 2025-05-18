// #include<iostream>
// #include<cstdio>
// #include<cstdlib>
// #include<string>
// #include<unistd.h>
// #include<sys/types.h>
// #include<sys/wait.h>
// #include<cerrno>
// using namespace std;
// void childprocess(int pipefd)
// {
//     char buffer[128];
//     ssize_t n=read(pipefd,buffer,sizeof(buffer)-1);
//     if(sizeof(buffer)>0)
//     {
//         buffer[n]=0;
//         cout<<buffer;
//     }

// }
// int main()
// {
//     int pipefd[2]={0};
//     int n=pipe(pipefd);
//     if(n==-1)
//     {
//         perror("pipe failed");
//     }
//     pid_t processid=fork();
//     if(processid==0)
//     {
//         close(pipefd[1]);
//         childprocess(pipefd[0]);
//         close(pipefd[0]);
//         exit(0);
//     }
//     close(pipefd[0]);
//     string buf="i am father\n";
//     int w=write(pipefd[1],buf.c_str(),buf.size());
//     close(1);
// }
#include <unistd.h>
#include <iostream>
#include <sys/wait.h>
#include <fcntl.h>
#include <string>
using namespace std;
//char buf[1024];
//fork之后子进程能拿到父进程的数据，但是这样并不能进行通信，因为只有子进程能拿到父进程的数据，而且还存在写时拷贝的问题
//进程之间写入的数据互相都看不到，因此不能通过一个全局变量来实现通信
string getMessage()
{
    static int cnt = 0;
    string messageid = to_string(cnt);
    cnt++;
    pid_t pid = getpid();
    string messagepid = to_string(pid);
    string ret = "messageid: " + messageid + " " + messagepid;
    return ret;
}
void childprocess(int rfd)
{
    char buffer[128];
    while (true)
    {
        ssize_t n = read(rfd, buffer, sizeof(buffer) - 1); // 因为读取的只是字符串，并没有\0，因此要预留出一个位置
        if (n > 0)
        {
            buffer[n] = 0; // write返回的是读取的字节数，因为char占1字节，因此第n个正好为最后一个字符，将其设置为0标识buffer的最后一位
            cout << "child get: " << buffer << endl;
            sleep(1);
        }
    }
}
void fatherprocess(int wfd)
{
    while (true)
    {
        string message = "father process" + getMessage();
        write(wfd, message.c_str(), message.size());
        sleep(1);
    }
}
int main()
{
    int pipefd[2];
    int fd = pipe(pipefd); // 成功返回0，否则失败，读文件描述符放到pipefd[0]，写文件描述符放到pipefd[1]
    if (fd != 0)
    {
        cerr << "create pipe failed" << endl;
    }
    pid_t pid = fork();
    if (pid == 0)
    {
        // child process
        close(pipefd[1]); // 因为子进程要读，所以把写文件描述符关闭
        childprocess(pipefd[0]);
        close(pipefd[0]);
        exit(0);
    }
    else
    {
        // father process
        close(pipefd[0]);
        fatherprocess(pipefd[1]);
        close(pipefd[1]);
        wait(nullptr);
    }
}