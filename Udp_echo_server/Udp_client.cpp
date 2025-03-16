#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <cstring>
using namespace std;
int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        cerr << "Usage" << argv[0] << "Server_IP,Server_Port" << endl;
        exit(0);
    }
    string ip = argv[1];
    uint16_t port = stoi(argv[2]);
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0)
    {
        cout << "Create Socket Failed" << endl;
        exit(1);
    }
    struct sockaddr_in server;
    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(ip.c_str());
    server.sin_port = htons(port);

    while (1)
    {
        string line;
        cout << "Enter Messages" << endl;
        getline(cin, line);
        int n = sendto(sockfd, line.c_str(), line.size(), 0, (struct sockaddr *)&server, sizeof(server));

        if (n > 0)
        {
            struct sockaddr tmp;
            socklen_t len = sizeof(tmp);
            char buffer[1024];
            int m = recvfrom(sockfd, buffer, sizeof(buffer) - 1, 0, (struct sockaddr *)&server, &len);
            if (m > 0)
            {
                buffer[n] = 0;
                cout<<buffer<<endl;
            }
            else
            {
                break;
            }
        }
        else
        {
            break;
        }
    }

    close(sockfd);
    return 0;
}
