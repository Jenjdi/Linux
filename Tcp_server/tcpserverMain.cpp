#include"tcp_server.hpp"
#include<memory>
int main(int argc,char* argv[])
{
    if(argc!=2)
    {
        cout<<"lack of parameter"<<endl;
    }
    uint16_t port=stoi(argv[1]);
    unique_ptr<tcp_server> tcsv=make_unique<tcp_server>();
    tcsv->loop();
}