#include "../http_v1/Socket.h"
#include <cassert>
// 回归：recv 的字节数决定结果长度，嵌入零字节也必须完整保留。
int main() {
 int sockets[2];assert(socketpair(AF_UNIX,SOCK_STREAM,0,sockets)==0);
 TcpSocket receiver(sockets[0]);const char payload[]={'a','\0','b'};
 assert(write(sockets[1],payload,sizeof(payload))==3);
 std::string result;assert(receiver.Recv(&result)==3);
 assert(result==std::string(payload,sizeof(payload)));
 receiver.Close();close(sockets[1]);
}
