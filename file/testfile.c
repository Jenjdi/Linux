// #include<stdio.h>
// #include <sys/types.h>
// #include <sys/stat.h>
// #include <fcntl.h>
// #include<string.h>
// #include<unistd.h>
// int main()
// {

//     // umask(0);
//     // int fp=open("./log.txt",O_WRONLY|O_CREAT|O_APPEND,0666);
//     // if(fp<0)
//     // {
//     //     perror("open");
//     //     return 1;
//     // }
    
    
//     // const char* t="11fasd6\n";
//     // write(fp,t,strlen(t));
//     // close(fp);
//     // return 0;
//     int fd=open("/dev/pts/4",O_WRONLY|O_APPEND);
//     if(fd==NULL)
//     {
//         return 1;
//     }
//     const char* message="123\n";
//     while (1)
//     {
//         write(fd,message,sizeof(message));
//         sleep(1);
//     }
//     return 0;
// }
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include<unistd.h>
#include <stdlib.h>
#include<string.h>
void test_dup()
{
    int fd=open("/dev/pts/6",O_CREAT|O_WRONLY|O_APPEND,0666);
    if(fd==-1)
    {
        return;
    }
    dup2(fd,1);//把输入到1中的内容重定向到fd中
    printf("test111");
    fflush(stdout);
    close(fd);
}
void test_flush()
{
    int fd=open("test",O_CREAT|O_WRONLY|O_APPEND,0666);
    dup2(fd,1);
    printf("printf\n");
    fprintf(stdout,"fprintf\n");
    const char* msg="write\n";
    write(1,msg,strlen(msg));
    fork();
}
int main()
{
    // close(1);
    // int fd = open("myfile", O_WRONLY|O_CREAT|O_TRUNC, 0644);
    // if(fd < 0)
    // {
    //     perror("open");
    //     return 1;
    // }
    // printf("fd: %d\n", fd);
    // fprintf(stdout,"fprintf,%d\n",fd);
    // //fflush(stdout);//fflush将内容刷新到语言的缓冲区，正常结束时操作系统会将缓冲区中的内容刷新到内核
    // //但是这里因为提前使用close将文件关闭了，因此操作系统刷新时无法将内容刷新到文件中，因而如果不加上fflush文件中就会没有内容
    // close(fd);//也可以将close去掉，让程序结束后操作系统自己将语言缓冲区中的内容刷新到文件中
    // exit(0);
    test_flush();
    
}