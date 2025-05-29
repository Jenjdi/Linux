#pragma once
class Socket
{
    //模板方法模式
    public:
    virtual void CreateSocketOrdie()=0;
    virtual void CreateBindOrdie()=0;
    virtual void CreateLisetenerOrdie()=0;
    virtual void accepter()=0;
    void BuildListenSocket()
    {
        CreateSocketOrdie();
        CreateBindOrdie();
        CreateLisetenerOrdie();
    }
};
class TCPSocket:public Socket
{
    

};
// Socket* tcpsock=new TCPSocket();
// tcpsock->BuildListenSocket();