
#include<jsoncpp/json/json.h>
const std::string sep="\r\n";
//len\r\n{json}\r\nlen\r\n{json}\r\n
std::string Encode(const std::string& in)
{
    std::string len=std::to_string(in.size());
    return len+sep+in+sep;
}
std::string Decode(std::string &packagestream)
{
    // 分析
    auto pos = packagestream.find(sep);
    if (pos == std::string::npos)
        return std::string();
    std::string lenstr = packagestream.substr(0, pos);
    int len = std::stoi(lenstr);
    // 计算一个完整的报文应该是多长？？
    int total = lenstr.size() + len + 2 * sep.size();
    if (packagestream.size() < total)
        return std::string();

    // 提取
    std::string jsonstr = packagestream.substr(pos + sep.size(), len);
    packagestream.erase(0, total);
    return jsonstr;
}
class Request
{
    public:
    Request(){}
    ~Request(){}
    void Serialize(std::string* out)
    {
        Json::Value root;
        root["_x"]=_x;
        root["_y"]=_y;
        root["_oper"]=_oper;
        Json::FastWriter write;
        std::string s=write.write(root);
        *out=s;
    }
    void Deserialize(std::string& in)
    {
        Json::Value root;
        Json::Reader read;
        read.parse(in,root);
        _x=root["_x"].asInt();
        _y=root["_y"].asInt();
        _oper=root["_oper"].asInt();
    }
    void SetValue(int x,int y,char oper)
    {
        _x=x;
        _y=y;
        _oper=oper;
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
    Response(){}
    ~Response(){}
    void Serialize(std::string* out)
    {
        Json::Value root;
        root["_result"]=_result;
        root["_code"]=_code;
        root["_desc"]=_desc;
        Json::FastWriter write;
        std::string s=write.write(root);
        *out=s;
    }
    void Print()
    { 
        std::cout<<"result:"<<_result<<" "<<"code:"<<_code<<" "<<"desc:"<<_desc<<std::endl;
    }
    void Deserialize(const std::string& in)
    {
        Json::Value root;
        Json::Reader read;
        read.parse(in,root);
        _result=root["_result"].asInt();
        _code=root["_code"].asInt();
        _desc=root["_desc"].asString();    
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