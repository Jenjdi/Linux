#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cerrno>
#include <cstring>

#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define ZERO '\0'
#define SIZE 512
#define SPACE " "
#define NUM 64
#define Skip(p)               \
    do                        \
    {                         \
        p += strlen((p) - 1); \
        while (*p != '/')     \
        {                     \
            p--;              \
        }                     \
    } while (0)
char *cwd;
int lastcode = 0;
const char *getusername()
{
    const char *env = getenv("USER");
    if (env == NULL)
    {
        return "NONE";
    }
    return env;
}
// const char* gethost()
// {
//     const char* host=getenv("HOSTNAME");
//     if(host==NULL)
//     {
//         return "NONE";
//     }
//     return host;
// }虚拟机无法直接使用
const char *gethost()
{
    static char host[256];
    int n = gethostname(host, sizeof(host));
    if (n != 0)
    {
        return "NONE";
    }
    return host;
}
const char *getcwd()
{
    const char *name = getenv("PWD");
    if (name == NULL)
    {
        return "NONE";
    }
    return name;
}
const char *getHome()
{
    const char *home = getenv("HOME");
    if (home == NULL)
    {
        return "/";
    }
    return home;
}
void MakeCommandLine() // 输出命令行
{
    char line[SIZE];
    const char *hostname = gethost();
    const char *cwd = getcwd();
    const char *username = getusername();
    Skip(cwd);
    snprintf(line, sizeof(line), "[%s@%s:%s]# ", username, hostname, strlen(cwd) == 1 ? "/" : cwd + 1);
    printf("%s", line);
    fflush(stdout);
}
int getCommand(char usercommand[], size_t n) // 获取命令
{                                            // 这里的n不能省，因为如果在函数里面使用sizeof的话，由于传入的数组会退化为指针，因此sizeof会变为指针的大小而不是实际的大小
    char *s = fgets(usercommand, n, stdin);
    if (s == nullptr)
    {
        return -1;
    }
    usercommand[strlen(usercommand) - 1] = ZERO;
    // strlen取到的是最后一个元素\n，但是并不希望去到\n，\n的下标为strlen-1，将其设置为\0表示为字符串结尾
    // 由于至少按过一次回车，因此至少有一个\n，因此即使不输入，也不会越界
    return strlen(usercommand);
}
// 分割命令
char *gArg[NUM];
void splitcommand(char commandLine[])
{
    gArg[0] = strtok(commandLine, SPACE);
    // strtok会从传入的str中从头开始找分隔符，找到后在其后面添加\0，然后会在内部静态地记录下一个待处理的位置
    // 即分隔符后的第一个字符
    int index = 1;
    while ((gArg[index] = strtok(NULL, SPACE)) != NULL)
    // 当传递NULL为参数时，会自动从上次记录的字符串开始，继续向下分割，直到字符串中没有字符为止，返回NULL
    // 由于这里写成=，会先判断，分割之后，在最后的位置直接赋值为NULL
    {
        index++;
    }
}

void cd()
{
    const char *path = gArg[1];
    if (path == NULL)
    {
        path = getHome();
    }
    chdir(path);
    char tmp[NUM];
    getcwd(tmp, sizeof(tmp));
    char *env_var = NULL;
    asprintf(&cwd, "PWD=%s", tmp); // 自动分配足够的内存
    if (cwd)
    {
        putenv(cwd); // putenv() 负责释放内存
    }
}
void echo()
{
    if (gArg[1] == NULL)
    {
        printf("\n");
    }
    else
    {
        printf("%d\n", lastcode);
        lastcode = 0;
    }
}
int checkBuilding()
{
    int flag = 0;
    if (strcmp(gArg[0], "cd") == 0)
    {
        flag = 1;
        cd();
    }
    else if (strcmp(gArg[0], "echo")==0)
    {
        flag = 1;
        echo();
    }
    return flag;
}
void ExecuteCommand()
{
    pid_t pid;
    pid = fork();
    if (pid < 0)
    {
        exit(-1);
    }
    else if (pid == 0)
    {
        // child
        execvp(gArg[0], gArg); // 如果直接运行cd等内建命令，会导致子进程自己的pwd切换了，但是父进程并没有切换
        exit(errno);
    }
    else
    {
        // father
        int status;
        pid_t rid = waitpid(rid, &status, 0);
        if (rid > 0)
        {
            lastcode = WIFEXITED(status);
            if (lastcode != 0)
            {
                printf("%s:%s:%d\n", gArg[0], strerror(lastcode), lastcode);
            }
        }
    }
}

int main()
{
    int quit = 0;
    while (!quit)
    {
        MakeCommandLine();
        char usercommand[SIZE];
        int n = getCommand(usercommand, sizeof(usercommand));
        splitcommand(usercommand);
        if (checkBuilding())
        {
            continue;
        }
        ExecuteCommand();
    }
}