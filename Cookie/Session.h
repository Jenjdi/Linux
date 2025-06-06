#include"log.h"
#include<unistd.h>
#include<ctime>
#include<memory>
#include<unordered_map>
#include<string>
class Session
{
    public:
    std::string _user;
    uint64_t _time;
    std::string _status;
    Session(const std::string& name,const std::string status)
    :_user(name),
    _status(status)
    {
        _time=time(nullptr);
    }
};
using session_ptr=std::shared_ptr<Session>;
class SessionManager
{
    private:
    std::unordered_map<std::string,session_ptr> _session;
    public:
    SessionManager()
    {
        srand(time(nullptr)^getpid());
    }
    std::string AddSessionID(session_ptr s)
    {
        uint32_t randsession=rand()+time(nullptr);
        std::string sessionid=std::to_string(randsession);
        _session.insert(std::make_pair(sessionid,s));
        return sessionid;
    }
    session_ptr GetSession(const std::string& sessionid)
    {
        if(_session.find(sessionid)==_session.end())
            return nullptr;
        else
            return _session[sessionid];
    }
};