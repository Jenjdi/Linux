#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cerrno>
#include <cstring>

#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

#define ZERO '\0'
#define SIZE 512
#define SPACE " "
#define NUM 64
#define None_Redir 0
#define In_Redir 1
#define Out_Redir 2
#define App_Redir 3

#define Skip(p)               \
    do                        \
    {                         \
        p += strlen((p) - 1); \
        while (*p != '/')     \
        {                     \
            p--;              \
        }                     \
    } while (0)
#define SkipSpace(cmd, pos)        \
    do                             \
    {                              \
        while (1)                  \
        {                          \
            if (isspace(cmd[pos])) \
            {                      \
                pos++;             \
            }                      \
            else                   \
            {                      \
                break;             \
            }                      \
        }                          \
    } while (0)

char *cwd;
int lastcode = 0;
int redir_type = None_Redir;
char *filename = NULL;
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
    else if (strcmp(gArg[0], "echo") == 0)
    {
        flag = 1;
        echo();
    }
    return flag;
}
void checkDir(char cmd[])
{
    int pos = 0;
    int end = strlen(cmd);
    while (pos < end)
    {
        if (cmd[pos] == '>')
        {
            if (cmd[pos + 1] == '>')
            {
                cmd[pos++] = 0;
                pos++;//由于有两个>，因此跳过两个字符
                redir_type = App_Redir;
                SkipSpace(cmd, pos);
                filename = cmd + pos;
            }
            else
            {
                cmd[pos++] = 0;
                redir_type = Out_Redir;
                SkipSpace(cmd, pos);
                filename = cmd + pos;
            }
        }
        else if (cmd[pos] == '<')
        {
            cmd[pos++] = 0; // 设置为0直接就将命令和文件分开了
            redir_type = In_Redir;
            SkipSpace(cmd, pos);
            filename = cmd + pos;
        }
        else
        {
            pos++;
        }
    }
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
        if(filename!=NULL)
        {
            if(redir_type==In_Redir)
            {
                int fd=open(filename,O_RDONLY);
                dup2(fd,0);//将文件中的内容输入到命令中，因此要更改的是输入
            }
            else if(redir_type==Out_Redir)
            {
                int fd=open(filename,O_CREAT|O_WRONLY|O_TRUNC,0666);
                dup2(fd,1);//将命令中的内容输出到文件中，因此要更改的是输出
            }
            else if(redir_type==App_Redir)
            {
                int fd=open(filename,O_CREAT|O_WRONLY|O_APPEND,0666);
                dup2(fd,1);//同样是输出，但是这里是追加而不是覆盖
            }
        }
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
        if (n <= 0)
            return 1;
        checkDir(usercommand);

        splitcommand(usercommand);
        if (checkBuilding())
        {
            continue;
        }
        ExecuteCommand();
    }
}