#include"dict_server.hpp"
#include"dict.hpp"
int main()
{
    int port=10000;
    dict d("./dict.txt");
    func_t transfunc=bind(&dict::translate,&d,placeholders::_1);
    unique_ptr<dict_server> dcsv=make_unique<dict_server>(transfunc,port);
    
    dcsv->init();
    dcsv->start();
}