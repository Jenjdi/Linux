#include "log.h"
int main(int argc, char *argv[])
{
    if (argc == 2)
    {
        if (strcmp(argv[1], "screen") == 0)
        {
            EnableScreen();
        }
        else if (strcmp(argv[1], "file") == 0)
        {
            EnableFile();
        }
        else
        {
            printf("%s\n", argv[1]);
            std::cout << "wrong parameter" << std::endl;
            return 0;
        }
    }
    LOG(DEBUG, "hello %d", 10);
    LOG(ERROR,"hello error %d",10);
    LOG(DEBUG,"123456");
}