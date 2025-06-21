#include"Udp_Echo_Server.h"
#include<memory>
int main()
{
    dict d("./dict");
    func_t t=std::bind(&dict::translate,&d,std::placeholders::_1);
    std::unique_ptr<Server> ptr=std::make_unique<Server>(t);//服务器要接收所有地址发送来的消息
    ptr->init();
}