#include<unistd.h>
#include<stdlib.h>
#include<wait.h>
#include<sys/types.h>
#include<stdio.h>
#include<string.h>
int main()
{
    printf("hello world\n");
    pid_t pid=fork();
    if(pid==0)
    {
        exit(0);
    }
    else{
        wait(NULL);
    }
}