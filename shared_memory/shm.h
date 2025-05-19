// #include <iostream>
// #include <sys/types.h>
// #include <sys/ipc.h>
// #include <sys/shm.h>
// #include <unistd.h>
// #include <cstring>
// #include <cstdlib>
// #include <cstdio>
// #include <string>
// #define customer 1
// #define Creater 2
// #define Default -1
// using namespace std;
// const string pathname = "/home/zyf/shared_memory";
// const int proj_id = 0x11;
// class sharedMemory
// {
//     key_t getkey(string path, int Proj_id)
//     {
//         key_t k = ftok(path.c_str(), Proj_id);
//         if (k < 0)
//         {
//             perror("ftok failed");
//         }
//         return k;
//     }

// public:
//     bool getshmCreate(const string pathname, int proj_id)
//     {
//         _shmid = shmget(getkey(pathname, proj_id), 1024, IPC_CREAT | 0666);
//         if (_shmid == -1)
//         {
//             perror("shmget");
//             return 1;
//         }
//     }
//     void *getshmAddr()
//     {
//         return _shmaddr;
//     }
//     void ShmAttch()
//     {
//         _shmaddr = shmat(_shmid, nullptr, 0);
//         if (_shmaddr == nullptr)
//         {
//             perror("Attach Falied");
//         }
//     }
//     sharedMemory(int user, const string pathname, int proj_id)
//         : _user(user),
//           _proj_id(proj_id),
//           _path(pathname)
//     {
//         if (_user == Creater)
//         {
//             getshmCreate(pathname, proj_id);
//             ShmAttch();
//         }
//         else if (_user == customer)
//         {
//             getshmCreate(pathname, proj_id);
//             ShmAttch();
//         }
//     }
//     ~sharedMemory()
//     {
//         shmctl(_shmid, IPC_RMID, NULL);
//     }

// private:
//     int _user = Default;
//     int _shmid;
//     key_t _key;
//     string _path;
//     int _proj_id;
//     void *_shmaddr;
// };
#include <iostream>
#include <string>
#include<cstring>
#include <sys/shm.h>
#include <unistd.h>
#include <sys/ipc.h>
#include"fifo.hpp"
#define SHMSIZE 4096
#define creater 1
#define user 2
#define defaultshmid -1
using namespace std;
const string pathname = "/home/zyf/shared_memory";
const int proj_id = 0x11;

class SharedMemory
{
private:
    int _id;
    key_t key;
    int _shmid=defaultshmid;
    string _path;
    int _proj_id;
    void *_shmaddr;
    key_t getkey(string path, int proj_id)
    {
        return ftok(path.c_str(), proj_id);
    }
    void shmattach()
    {
        _shmaddr = shmat(_shmid, nullptr, 0);
        if (_shmaddr == nullptr)
        {
            cerr << "attach failed" << endl;
        }
    }
    void getshmcreate(key_t key)
    {
        if (_id == creater)
        {
            _shmid = shmget(key, SHMSIZE, IPC_CREAT);
            shmattach();
        }
        else if (_id == user)
        {
            _shmid = shmget(key, SHMSIZE, IPC_CREAT);
            shmattach();
        }
    }

public:
    SharedMemory(const string &path, int id, int proj_id)
        : _id(id),
          _path(path),
          _proj_id(proj_id)
    {
        key_t key = getkey(_path, _proj_id);
        if (key == -1)
        {
            exit(-1);
        }
        getshmcreate(key);
        if (_shmid == -1)
        {
            exit(-1);
        }
    }
    ~SharedMemory()
    {
        if (_id == creater)
        {
            shmctl(_shmid, IPC_RMID, nullptr);
        }
    }
    void *getshm()
    {
        return _shmaddr;
    }
    void clear()
    {
        if(_shmaddr!=nullptr)
        {
            memset(_shmaddr,0,sizeof(_shmaddr));
        }
    }
};