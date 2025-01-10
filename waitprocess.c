#include<stdio.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<unistd.h>
void childprocess()
{
    printf("child process pid=%d\n",getpid());
}
int main()
{
    int n =fork();
    if(n==0)
    {
        childprocess();
    }
    else{
        printf("father process pid=%d\n",getpid());
        wait(NULL);
    }
}