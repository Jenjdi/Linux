// #include <iostream>
// #include <sys/types.h>
// #include <sys/stat.h>
// #include <cerrno>
// #include <string>
// #include <cstdio>
// #include <unistd.h>
// #include <fcntl.h>
// #define creater 1
// #define customer 2
// #define Read O_RDONLY
// #define Write O_WRONLY
// #define DefautlFD -1
// #define Buffer_Size 4096
// using namespace std;
// const string commonPath = "./fifo";

// class Fifo
// {
// public:
//     bool OpenRead()
//     {
//         return OpenNamedPipe(Read);
//     }
//     bool OpenWrite()
//     {
//         return OpenNamedPipe(Write);
//     }
//     int WritePipe(const string &in)
//     {

//         int n = write(_fd, in.c_str(), in.size());
//         if (n == -1)
//         {
//             perror("write failed");
//             return -1;
//         }
//         return n;
//     }
//     int ReadPipe(string &out)
//     {
//         char buffer[Buffer_Size];
//         int n = read(_fd, buffer, sizeof(buffer));
//         if (n > 0)
//         {
//             buffer[n] = 0;
//             out = buffer;
//         }
//         return n;
//     }
//     Fifo(const string &path, int id) : _path(path), _id(id)
//     {
//         if (id == creater)
//         {
//             int res = mkfifo(_path.c_str(), 0666);
//             if (res != 0)
//             {
//                 perror("createfifo failed");
//             }
//         }
//     }
//     ~Fifo()
//     {
//         if (_id == creater)
//         {
//             int res = unlink(_path.c_str());
//             if (res != 0)
//             {
//                 perror("deletefifo failed");
//             }
//         }
//         if (_fd != DefautlFD)
//         {
//             close(_fd);
//         }
//     }

// private:
//     const string _path;
//     int _id;
//     int _fd = DefautlFD;
//     bool OpenNamedPipe(int mode)
//     {
//         _fd = open(_path.c_str(), mode);
//         if (_fd < 0)
//         {
//             return false;
//         }
//         return true;
//     }
// };
#pragma once
#include <iostream>
#include <string>

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define Write O_WRONLY
#define Read O_RDONLY
#define creater 1
#define user 2

using namespace std;
const string commonPath = "./fifo";
#define defaultFD -1
#define buffsize 4096

class Fifo
{
private:
    int _fd;
    string _path;
    int _id;
    bool OpenFifo(int mode)
    {
        _fd = open(_path.c_str(), mode);
        if (_fd < 0)
        {
            return false;
        }
        return true;
    }

public:
    Fifo(string path, int id)
        : _path(path),
          _id(id)
    {
        if (_id == creater)
        {
            int res = mkfifo(path.c_str(), 0666);
            if (res != 0)
            {
                cerr << "createfifo failed" << endl;
            }
        }
    }
    ~Fifo()
    {
        if (_id == creater) // 服务端进行生命周期的管理
        {
            int res = unlink(_path.c_str());
            if (res != 0)
            {
                cerr << "unlink failed" << endl;
            }
        }
        if (_fd != defaultFD)
        {
            close(_fd); // 当fd不为默认时，说明已经进行了创建，因此当退出时需要关闭文件描述符
        }
    }
    bool OpenRead()
    {
        return OpenFifo(Read);
    }
    bool OpenWrite()
    {
        return OpenFifo(Write);
    }
    int WriteFifo(const string &in)
    {
        int n = write(_fd, in.c_str(), sizeof(in));
        if (n == -1)
        {
            cerr << "Write failed" << endl;
            return -1;
        }
        return n;
    }
    int ReadFifo(string &out)
    {
        char buf[buffsize];
        int n = read(_fd, buf, sizeof(out));
        if (n > 0)
        {
            buf[n] = 0;
            out = buf;
        }
        return n;
    }
};