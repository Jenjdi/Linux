#include"Epoll.hpp"
#include"Connecttion.hpp"
// 修复：为原先未命名的类补充名称，指针默认初始化为空。
class Reactor
{
    private:
    Connection* _conn = nullptr;
    Epoll _epoll;
    public:

};
