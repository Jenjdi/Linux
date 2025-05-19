#include<sys/sem.h>
#include<sys/types.h>
#include<iostream>
#include<string>
#include<cstring>
#define A 1
#define B 2
#define proj_id 0x77
using namespace std;
const char* path="./semphore.cpp";
class sem
{
    private:
    key_t _key;
    int _semid;
    struct sembuf* op;
    sem()
    {
        _key=ftok(path,proj_id);
        _semid=semget(_key,1,IPC_CREAT);
    }
    ~sem()
    {
        semctl(_semid,1,IPC_RMID);
    }
    void P()
    {
        semop(_semid,op,1);
    }
    void V()
    {
        
    }
};