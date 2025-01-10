#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <cerrno>
#include <string>
#include <cstdio>
#include <unistd.h>
#include <fcntl.h>
#define creater 1
#define customer 2
#define Read O_RDONLY
#define Write O_WRONLY
#define DefautlFD -1
#define Buffer_Size 4096
using namespace std;
const string commonPath = "./fifo";

class Fifo
{
public:
    
    bool OpenRead()
    {
        return OpenNamedPipe(Read);
    }
    bool OpenWrite()
    {
        return OpenNamedPipe(Write);
    }
    int WritePipe(const string &in)
    {
        
            int n = write(_fd, in.c_str(), in.size());
            if (n == -1)
            {
                perror("write failed");
                return -1;
            }
            return n;
    }
    int ReadPipe(string &out)
    {

            char buffer[Buffer_Size];
            int n = read(_fd, buffer,sizeof(buffer));
            if(n>0)
            {
                buffer[n]=0;
                out = buffer;
            }
            return n;


    }
    Fifo(const string &path, int id) : _path(path), _id(id)
    {
        if (id == creater)
        {
            int res = mkfifo(_path.c_str(), 0666);
            if (res != 0)
            {
                perror("createfifo failed");
            }
        }
    }
    ~Fifo()
    {
        if (_id == creater)
        {
            int res = unlink(_path.c_str());
            if (res != 0)
            {
                perror("deletefifo failed");
            }
        }
        if(_fd!=DefautlFD)
        {
            close(_fd);
        }
    }

private:
    const string _path;
    int _id;
    int _fd = DefautlFD;
    bool OpenNamedPipe(int mode)
    {
        _fd = open(_path.c_str(), mode);
        if (_fd < 0)
        {
            return false;
        }
        return true;
    }
};