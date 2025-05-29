#include <cstring>
#include <string>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <set>
#include "log.h"
#include <stdio.h>
class command
{
public:
    command() {}
    ~command() {}
    std::string Execute(const std::string &name)
    {
        std::string result;
        FILE *fp = popen(name.c_str(), "r"); // 声明在函数作用域内

        if (fp)
        {
            char line[128];
            while (std::fgets(line, sizeof(line) - 1, fp))
            {
                result += line;
            }
            return result;
        }

        return "execute error"; // 错误处理移至if块外
    }
    void Handler(int sockfd)
    {
        while (true)
        {
            char cmdbuf[128];
            int n = recv(sockfd, cmdbuf, sizeof(cmdbuf) - 1, 0);//面向字节流的接收
            if (n > 0)
            {
                cmdbuf[n] = 0;
                std::string result = Execute(cmdbuf);
                send(sockfd, result.c_str(), result.size(), 0);
            }
            else if (n == 0)
            {
                LOG(DEBUG, "client quit");
                break;
            }
        }
    }

private:
};