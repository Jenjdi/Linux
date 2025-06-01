#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
std::string defaultip="127.0.0.1";
uint16_t defaultport=1234;
const int defaultfd=-1;
const int defaultfretry_interval=1;
const int defaultretries=5;
enum class status // 强类型枚举
{
    NEW,
    CONNECTING,
    CONNECTED,
    DISCONNECTED,
    CLOSED
};
class clientconnection
{
    private:
    int _sockfd=defaultfd;
    status _status;
    std::string _ip;
    uint16_t _port;
    int _retry_interval;
    int _max_retries;
    public:
    clientconnection(const std::string& ip=defaultip,const uint16_t port=defaultport)
    :_status(status::NEW),
    _ip(ip),
    _port(port),
    _retry_interval(defaultfretry_interval),
    _max_retries(defaultretries)
    {
    }
    status status()
    {
        return _status;
    }
    void connect()
    {
        if(_sockfd!=defaultfd)
        {
            close(_sockfd);
        }
        _sockfd=socket(AF_INET,SOCK_STREAM,0);
        struct sockaddr_in remote;
        memset(&remote,0,sizeof(remote));
        remote.sin_addr.s_addr=inet_addr(_ip.c_str());
        remote.sin_family=AF_INET;
        remote.sin_port=htons(_port);
        int n=::connect(_sockfd,(sockaddr*)&remote,sizeof(remote));
        if(n<0)
        {
            disconnect();
            _status=status::DISCONNECTED;
        }
        _status=status::CONNECTED;
    }
    void reconnect()
    {
        _status=status::CONNECTING;
        int cnt=0;
        while(cnt<_max_retries)
        {
            connect();
            if(_status==status::CONNECTED)
            {
                return;
            }
            cnt++;
            std::cout<<"断线重连中..."<<"("<<cnt<<")"<<std::endl;
            sleep(_retry_interval);
        }
        _status=status::CLOSED;
        std::cout<<"重连失败"<<std::endl;
    }
    void process()
    {
        while(true)
        {
            std::string message;
            ssize_t n=send(_sockfd,message.c_str(),message.size(),0);
            if(n>0)
            {
                char buf[128];
                ssize_t m=recv(_sockfd,buf,sizeof(buf)-1,0);
                if(m>0)
                {
                    buf[m]=0;
                    std::cout<<buf<<std::endl;
                }
                else
                {
                    _status=status::DISCONNECTED;
                    break;
                }
            }
            else
            {
                std::cout<<"send error"<<std::endl;
                _status=status::CLOSED;
                break;
            }

        }
    }
    void disconnect()
    {
        if(_sockfd!=defaultfd)
        {
            close(_sockfd);
            _sockfd=defaultfd;
            _status=status::CLOSED;
        }
    }
};
class tcpclient
{
public:
    tcpclient(const std::string& ip,const uint16_t port)
    :_connect(ip,port)
    {
    }
    void Execute()
    {
        while(true)
        {
            switch (_connect.status())
            {
            case status::NEW:
                _connect.connect();
                break;
            case status::CONNECTED:
            _connect.process();
            break;
            case status::DISCONNECTED:
            _connect.reconnect();
            case status::CLOSED:
            _connect.disconnect();
            default:
                break;
            }
        }
    }
    ~tcpclient()
    {

    }
private:
    clientconnection _connect;
};
int main()
{

}