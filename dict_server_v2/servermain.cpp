#include"dict_server.h"
#include<memory>
int main()
{
    Dict d("./dict");
    func_t func=std::bind(&Dict::translate,&d,std::placeholders::_1);
    std::unique_ptr<Server> usvr=std::make_unique<Server>(func);
    usvr->Init();
    usvr->Loop();
}