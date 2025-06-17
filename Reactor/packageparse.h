#pragma once
#include"protocol.h"
#include"calculator.h"
#include"Connection.h"
using process_t=std::function<std::shared_ptr<Response>(std::shared_ptr<Request>)>;
class PackageParse
{
    private:
    Calculator _cal;
    public:
    void Execute(Connection* conn)
    {
        while(true)
        {
            
            std::string package=Decode(conn->Inbuff());//报文解析，提取报头和有效字段
            LOG(DEBUG,"decode success");
            if(package.empty())
            {
                LOG(DEBUG,"package empty");
                break;//读取到不完整的报文就重新读取
            }
            auto req=Factory::BuildRequest();
            req->Deserialize(package);//反序列化
            LOG(DEBUG,"deserialize success");
            auto res=_cal.calculator(req);//业务处理
            std::string respmessage;//序列化应答
            res->Serialize(&respmessage);
            respmessage=Encode(respmessage);//添加len长度报头

            //发回
            conn->AppendOutBuff(respmessage);
        }
        
        //走到这里说明至少处理了一个请求，至少会有一个应答
        //if(!conn->OutBuff().empty()) conn->_send(conn);//直接发送数据
        if(!conn->OutBuff().empty())
            conn->_reactor->EnableConnectionReadWrite(conn->Sockfd(),true,true);
    }
};