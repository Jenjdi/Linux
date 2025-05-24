// // #include<iostream>
// // #include<signal.h>
// // using namespace std;
// // void printpending(sigset_t &pend)
// // {
// //     //遍历pending中的所有32位，从右向左进行遍历
// //     for(int signo=31;signo>=1;signo--)
// //     {
// //         if(sigismember(&pend,signo))
// //         {
// //             cout<<1;
// //         }
// //         else
// //         {
// //             cout<<0;
// //         }
// //     }
// // }
// // void handler(int sig)
// // {
// //     cout<<"cap sig:"<<sig<<endl;
// // }
// // int main()
// // {
// //     signal(SIGINT,handler);
// //     sigset_t block,old;
// //     sigemptyset(&block);
// //     sigemptyset(&old);
// //     sigaddset(&block,SIGINT);//并没有修改当前进程在操作系统内核中的block表，只是修改了block这个位图
// //     sigprocmask(SIG_BLOCK,&block,&old);//进入进程的block位图进行修改，将2号进程进行了屏蔽，然后将原来的操作写入old中
// //     //打印pending
// //     int cnt=5;
// //     while(true)
// //     {
// //         sigset_t pending;
// //         sigpending(&pending);//获取当前进程的pending位图
// //         printpending(pending);
// //         cout<<endl;
// //         cnt--;
// //         if(cnt==0)
// //         {
// //             sigprocmask(SIG_UNBLOCK,&block,&old);
// //         }
// //         sleep(1);
// //     }
// // }
// // #include<iostream>
// // #include<signal.h>
// // using namespace std;
// // //对当前正在处理的信号回自动屏蔽，处理完成后回自动解除屏蔽
// // void handler(int sig)
// // {
// //     cout<<"get sig:"<<sig<<endl;
// // }
// // int main()
// // {
// //     struct sigaction action,oaction;
// //     action.sa_handler=handler;//设置默认信号处理为handler
// //     sigemptyset(&action.sa_mask);//将掩码置空
// //     sigaddset(&action.sa_mask,3);//对其他信号也进行屏蔽，将信号添加进掩码中
// //     action.sa_flags=0;
// //     sigaction(2,&action,&oaction);
// // }
// #include<iostream>
// #include<signal.h>
// using namespace std;
// int gflag=0;
// void change(int sig)
// {
//     gflag=1;
//     cout<<sig;
// }
// int main()
// {
//     signal(SIGINT,change);
//     //while (!gflag);
//     while(1);
//     cout<<"process quit normal"<<endl;
    
// }
// #include<signal.h>
// #include<unistd.h>
// #include<iostream>
// using namespace std;
// void handler(int sig)
// {
//     cout<<"SIGINT catch"<<endl;
// }
// int main()
// {
//     struct sigaction action,oaction;
//     action.sa_handler=handler;
//     sigemptyset(&action.sa_mask);
//     sigaddset(&action.sa_mask,SIGINT);
//     action.sa_flags=0;
//     sigaction(SIGINT,&action,&oaction);
//     while (1);
// }
#include<signal.h>
#include<unistd.h>
#include<iostream>
using namespace std;
int main()
{
    sigset_t blockset,oldset;
    sigemptyset(&blockset);
    sigemptyset(&oldset);
    sigaddset(&blockset,2);
    sigaddset(&blockset,40);
    sigprocmask(SIG_BLOCK,&blockset,&oldset);
    while(1)
    {
        cout<<kill(getpid(),2)<<endl;
        cout<<kill(getpid(),40)<<endl;
        sleep(1);
    }
}