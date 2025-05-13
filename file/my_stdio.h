#pragma once

#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define NUM 1024
#define FLUSH_NOW  1//001
#define FLUSH_LINE 2//010
#define FLUSH_FULL 4//100   如果使用3的话，就是011，此时会包含上面两个的所以造成冲突

typedef struct _MyFile
{
    unsigned int flag;
    int fileno;
    char buff[NUM];
    int pos;
    int cap;
}MyFile;
MyFile* My_Open(const char* path,const char* flag)
{
    int flag0=0;
    int iscreate=0;
    mode_t mode=0666;
    if(strcmp(flag,"r")==0)
    {
        flag0=(O_RDONLY);
    }
    else if(strcmp(flag,"w")==0)
    {
        flag0=(O_CREAT|O_WRONLY|O_TRUNC);
        iscreate=1;
    }
    else if(strcmp(flag, "a") == 0)
    {
        flag0 = (O_WRONLY | O_CREAT | O_APPEND);
        iscreate = 1;
    }
    else
    {}
    int fd=0;
    if(iscreate)
    {
        fd=open(path,flag0,mode);
    }
    else
    {
        fd=open(path,flag0);
    }
    if(fd<0)
    {
        return NULL;
    }
    MyFile* file=(MyFile*)malloc(sizeof(MyFile));
    if(file==NULL)
    {
        return NULL;
    }
    file->fileno=fd;
    file->flag=FLUSH_LINE;
    file->cap=NUM;
    file->pos=0;
    return file;
}
void my_fflush(MyFile* fp)
{
    write(fp->fileno,fp->buff,fp->pos);
    fp->pos=0;//缓冲区刷新完毕后自动清空
}
void my_fclose(MyFile* fp)
{
    my_fflush(fp);
    close(fp->fileno);
    free(fp);
}
ssize_t my_fwrite(MyFile* fp,const char* data,int len)
{
    memcpy(fp->buff+fp->pos,data,len);
    fp->pos+=len;
    if((fp->flag&FLUSH_LINE)&&fp->buff[fp->pos-1]=='\n')
    {
        my_fflush(fp);
    }
    return len;
}