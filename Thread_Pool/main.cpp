#include"thread_pool.h"
int main()
{
    unique_ptr<thread_pool> tp=make_unique<thread_pool>();
    tp->init();
    
}