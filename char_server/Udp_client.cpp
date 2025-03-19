#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <cstring>
#include "thread.hpp"
using namespace std;
int Initclient()
{

    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0)
    {
        cout << "Create Socket Failed" << endl;
        exit(1);
    }
    return sockfd;
}
void receivemessage(int sockfd, const string &name)
{
    while (true)
    {
        struct sockaddr_in peer;
        socklen_t len = sizeof(peer);
        char buffer[1024];
        int m = recvfrom(sockfd, buffer, sizeof(buffer) - 1, 0, (struct sockaddr *)&peer, &len);
        if (m > 0)
        {
            buffer[m] = 0;
            cout << buffer << endl;
        }
        else
        {
            cout << "receive failed" << endl;
            break;
        }
    }
}
void sendmessage(int sockfd, const string &serverip, uint16_t port, const string &name)
{
    struct sockaddr_in server;
    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(serverip.c_str());
    server.sin_port = htons(port);
    string prefix = name + "#";
    while (true)
    {
        string line;
        cout << prefix;
        getline(cin, line);
        int n = sendto(sockfd, line.c_str(), line.size(), 0, (struct sockaddr *)&server, sizeof(server));
        if (n <= 0)
        {
            break;
        }
    }
}
int main(int argc, char *argv[])
{
    int sockfd = Initclient();
    if (argc != 3)
    {
        cerr << "Usage" << argv[0] << "Server_IP,Server_Port" << endl;
        exit(0);
    }
    string ip = argv[1];
    uint16_t port = stoi(argv[2]);
    Thread recv("receiver-thread", bind(&receivemessage, sockfd, placeholders::_1));
    Thread sender("sender-thread", bind(&sendmessage, sockfd, ip, port, placeholders::_1));

    recv.start();
    sender.start();

    recv.join();
    sender.join();

    close(sockfd);
    return 0;
}
