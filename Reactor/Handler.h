#pragma once
#include "Connection.h"
#include <errno.h>
const static int buffersize = 512;
class Handler
{
private:
    handler_t _process;

public:
    Handler(handler_t process)
        : _process(process)
    {
    }
    void HandleRecv(Connection *conn)
    {
        errno=0;//防止上次设置的错误码影响到之后的判断
        // 直接读
        while (true)
        {
            char buf[buffersize];
            int n = recv(conn->Sockfd(), buf, sizeof(buf) - 1, 0);
            if (n > 0)
            {
                buf[n] = 0;
                conn->AppendInBuff(buf);
            }
            else
            {
                if (errno == EWOULDBLOCK)
                {
                    break;
                }
                else if (errno == EINTR)
                {
                    continue;
                }
                else
                {
                    conn->_except(conn);
                    return;
                }
            }
        }
        // 读取完毕后业务处理
        std::cout<<"Inbuffer is:"<<conn->Inbuff()<<std::endl;
        _process(conn);
    }
    void HandleSend(Connection *conn)
    {
        errno=0;
        // 直接写
        while (true)
        {
            int n = send(conn->Sockfd(), conn->OutBuff().c_str(), conn->OutBuff().size(), 0);
            if (n > 0)
            {
                conn->DiscardOutBuff(n);
                if (conn->OutBuff().empty())
                    return; // 为空直接返回
            }
            else if (n == 0)
            {
                break;
            }
            else
            {
                if (errno == EWOULDBLOCK)
                {
                    // 发送条件不满足
                    break;
                }
                else if (errno == EINTR)
                {
                    continue;
                }
                else
                {
                    // 出现错误，直接处理错误并返回
                    conn->_except(conn);
                    break;
                }
            }
        }

        //发送条件不满足并且缓冲区还有数据
        if (!conn->OutBuff().empty())
        {
            //开启对写事件的关心
            conn->_reactor->EnableConnectionReadWrite(conn->Sockfd(),true,true);
            //开启写事件关心后，当下一层循环时，epoll会自动执行HandleSend，也就会继续发送数据，直到数据被全部发送完毕
        }
        else//数据发送完毕后的处理
        {
            conn->_reactor->EnableConnectionReadWrite(conn->Sockfd(),true,false);
        }
    }
    void HandleExcept(Connection *conn)
    {
        conn->_reactor->DelConnection(conn->Sockfd());
    }
};