#include<stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include<string.h>
int main()
{

    umask(0);
    int fp=open("./log.txt",O_WRONLY|O_CREAT|O_APPEND,0666);
    if(fp<0)
    {
        perror("open");
        return 1;
    }
    
    
    const char* t="11fasd6\n";
    write(fp,t,strlen(t));
    close(fp);
    return 0;
}