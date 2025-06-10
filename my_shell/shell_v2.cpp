#include<cstdio>
#include<unistd.h>
#include<iostream>
#include<cstdlib>
#include<cstring>
#define SIZE 512
#define Sep " "
const char* GetUserName()
{
    char* user=getenv("USER");
    if(user==nullptr)
    {
        return "NONE";
    }
    return user;
}
const char* GetHostName()
{
    char* host=getenv("HOST");
    if(host==nullptr)
    {
        return "NONE";
    }
    return host;
}
const char* GetCwd()
{
    char* cwd=getenv("PWD");
    if(cwd==nullptr)
    {
        return "NONE";
    }
    return cwd;
}
void MakeCommandLine()
{
    const char* user=GetUserName();
    const char* host=GetHostName();
    const char* pwd=GetCwd();
    printf("%s@%s:%s#",host,user,pwd);
    
}
int GetCommand(char command[],int n)
{
    char* s=fgets(command,n,stdin);
    if(s==nullptr)
    {
        return -1;
    }
    command[strlen(command)-1]='\0';//将\n变成\0
    return strlen(command);
    
}
const char* SepCommand(char* command,int n)
{
    char* com=strtok(command,Sep);
    char* parameter=strtok(command,Sep);
    return com;
}
void ExecuteCommand(const char* command,int n)
{

}
int main()
{
    MakeCommandLine();
    char command[SIZE];
    int n=GetCommand(command,sizeof(command));
    if(n<0)
    {
        return 1;
    }
    //ExecuteCommand(command,sizeof(command));
    printf("%s\n",command);
    const char* com=SepCommand(command,sizeof(command));
    printf("%s\n",com);
}