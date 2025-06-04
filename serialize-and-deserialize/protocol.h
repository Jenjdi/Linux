#include <jsoncpp/json/json.h>
#include<iostream>
#include<string>
#include<vector>
#define SELF 1
const std::string protosep = " ";
const std::string linesep="\r\n";
// len\r\n{json}\r\nlen\r\n{json}\r\n
std::string Encode(const std::string &in)
{
    std::string len = std::to_string(in.size());
    return len + linesep + in + linesep;
}
std::string Decode(std::string &packagestream)
{
    // 分析
    auto pos = packagestream.find(linesep);
    if (pos == std::string::npos)
        return std::string();
    std::string lenstr = packagestream.substr(0, pos);
    int len = std::stoi(lenstr);
    // 计算一个完整的报文应该是多长？？
    int total = lenstr.size() + len + 2 * linesep.size();
    if (packagestream.size() < total)
        return std::string();

    // 提取
    std::string jsonstr = packagestream.substr(pos + linesep.size(), len);
    packagestream.erase(0, total);
    return jsonstr;
}
class Request
{
public:
    Request() {}
    ~Request() {}
    void Serialize(std::string *out)
    {
        *out=std::to_string(_x)+protosep+_oper+protosep+std::to_string(_y);
#ifdef SELF
        
#else
        Json::Value root;
        root["_x"] = _x;
        root["_y"] = _y;
        root["_oper"] = _oper;
        Json::FastWriter write;
        std::string s = write.write(root);
        *out = s;
#endif
    }
    void Deserialize(std::string &in)
    {
#ifdef SELF
        auto left=in.find(protosep);
        if(left==in.npos)
            return;
        auto right=in.rfind(in);
        if(right==in.npos)
            return;
        if(left+protosep.size()+1!=right)//x + y，判定序列是否是正确的，left+1+1=right
            return;
        _x=std::stoi(in.substr(0,left));//substr是前闭后开的区间，刚好将x的值得到
        
        _y=std::stoi(in.substr(right+protosep.size()));

        _oper=in[left+protosep.size()];
#else
        Json::Value root;
        Json::Reader read;
        read.parse(in, root);
        _x = root["_x"].asInt();
        _y = root["_y"].asInt();
        _oper = root["_oper"].asInt();
#endif
    }
    void SetValue(int x, int y, char oper)
    {
        _x = x;
        _y = y;
        _oper = oper;
    }
    int X()
    {
        return _x;
    }
    int Y()
    {
        return _y;
    }
    char oper()
    {
        return _oper;
    }

private:
    int _x;
    int _y;
    char _oper;
};

class Response
{
public:
    Response() {}
    ~Response() {}
    void Serialize(std::string *out)
    {
        *out=std::to_string(_result)+protosep+std::to_string(_code);
#ifdef SELF
        
#else
        Json::Value root;
        root["_result"] = _result;
        root["_code"] = _code;
        root["_desc"] = _desc;
        Json::FastWriter write;
        std::string s = write.write(root);
        *out = s;
#endif
    }
    void Print()
    {
        std::cout << "result:" << _result << " " << "code:" << _code << " " << "desc:" << _desc << std::endl;
    }
    void Deserialize(const std::string &in)
    {
#ifdef SELF
        auto pos=in.find(protosep);
        _result=std::stoi(in.substr(0,pos));
        _code=std::stoi(in.substr(pos+protosep.size()));
#else
        Json::Value root;
        Json::Reader read;
        read.parse(in, root);
        _result = root["_result"].asInt();
        _code = root["_code"].asInt();
        _desc = root["_desc"].asString();
#endif
    }
    void setresult(int res)
    {
        _result=res;
    }
    void setcode(int code)
    {
        _code=code;
    }
    int getcode()
    {
        return _code;
    }
    int getresult()
    {
        return _result;
    }
    int _result;
    int _code;
    std::string _desc;
};
class Factory
{
public:
    static std::shared_ptr<Request> BuildRequest()
    {
        return std::make_shared<Request>();
    }
    static std::shared_ptr<Response> BuildResponse()
    {
        return std::make_shared<Response>();
    }
};