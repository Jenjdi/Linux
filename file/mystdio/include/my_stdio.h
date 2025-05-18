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
MyFile* My_Open(const char* path,const char* flag);
void my_fflush(MyFile* fp);
void my_fclose(MyFile* fp);
ssize_t my_fwrite(MyFile* fp,const char* data,int len);