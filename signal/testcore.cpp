// #include<iostream>
// #include<signal.h>
// using namespace std;
// void printpending(sigset_t &pend)
// {
//     //遍历pending中的所有32位，从右向左进行遍历
//     for(int signo=31;signo>=1;signo--)
//     {
//         if(sigismember(&pend,signo))
//         {
//             cout<<1;
//         }
//         else
//         {
//             cout<<0;
//         }
//     }
// }
// void handler(int sig)
// {
//     cout<<"cap sig:"<<sig<<endl;
// }
// int main()
// {
//     signal(SIGINT,handler);
//     sigset_t block,old;
//     sigemptyset(&block);
//     sigemptyset(&old);
//     sigaddset(&block,SIGINT);//并没有修改当前进程在操作系统内核中的block表，只是修改了block这个位图
//     sigprocmask(SIG_BLOCK,&block,&old);//进入进程的block位图进行修改，将2号进程进行了屏蔽，然后将原来的操作写入old中
//     //打印pending
//     int cnt=5;
//     while(true)
//     {
//         sigset_t pending;
//         sigpending(&pending);//获取当前进程的pending位图
//         printpending(pending);
//         cout<<endl;
//         cnt--;
//         if(cnt==0)
//         {
//             sigprocmask(SIG_UNBLOCK,&block,&old);
//         }
//         sleep(1);
//     }
// }
// #include<iostream>
// #include<signal.h>
// using namespace std;
// //对当前正在处理的信号回自动屏蔽，处理完成后回自动解除屏蔽
// void handler(int sig)
// {
//     cout<<"get sig:"<<sig<<endl;
// }
// int main()
// {
//     struct sigaction action,oaction;
//     action.sa_handler=handler;//设置默认信号处理为handler
//     sigemptyset(&action.sa_mask);//将掩码置空
//     sigaddset(&action.sa_mask,3);//对其他信号也进行屏蔽，将信号添加进掩码中
//     action.sa_flags=0;
//     sigaction(2,&action,&oaction);
// }
#include<iostream>
#include<signal.h>
using namespace std;
int gflag=0;
void change(int sig)
{
    gflag=1;
}
int main()
{
    signal(2,change);
    while (!gflag);
    cout<<"process quit normal"<<endl;
    
}