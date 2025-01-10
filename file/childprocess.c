#include<stdio.h>
#include<unistd.h>

void child()
{
  while(1)
  {
    int pid=getpid();
    printf("child process,pid=%d",pid);
    sleep(1);
  }
}
int main()
{

  int pid=fork();
  if(pid==0)
  {
    child();
  }
  else
  {
    printf("father process,pid=%d",getpid());
  }
}