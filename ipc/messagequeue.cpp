#include <sys/msg.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <iostream>
#include <string>
#include <cstring>
using namespace std;
#define flag IPC_CREAT | IPC_CREAT
#define proj_id 0x66
#define A 1
#define B 2
const char *path = "./messagequeue";
class msg
{
private:
    key_t key;
    int msgid;
    struct msqid_ds ms;
    struct msgbuf recbuf;
    struct msgbuf sendbuf;
    msg()
    {
        key = ftok(path, proj_id);
        msgid = msgget(key, flag);
        if (msgid == -1)
        {
            cerr << "getmsg failed" << endl;
            exit(-1);
        }
    }
    ~msg()
    {
        msgctl(msgid, IPC_RMID, &ms);
    }
    void receive()
    {
        msgrcv(msgid, &recbuf, sizeof(recbuf), B, 0);
    }
    void send(string &str)
    {
        sendbuf.mtype = A;
        strcpy(sendbuf.mtext, str.c_str());
        int n = msgsnd(msgid, &sendbuf, sizeof(sendbuf), 0);
        if(n<0)
        {
            cerr<<"send failed"<<endl;
            return;
        }
    }
};