#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <unordered_map>
#include <fstream>
#include<functional>
#include"Session.h"
const std::string base_sep = "\r\n";
const std::string line_sep = ": ";
const std::string space = " ";
const std::string defaultpath = "wwwroot";
const std::string homepage = "index.html";
const std::string version = "HTTP/1.0";
const std::string suffixsep=".";
const std::string arg_sep="?";
class HttpRequest;
class HttpResponse;
using func_t=std::function<HttpResponse(HttpRequest&)>;
class HttpRequest
{
private:
    std::string _request_line;
    std::vector<std::string> _request_header;
    std::string _blank_line;
    std::string _path;
    std::string _body_text;
    std::string _method;
    std::string _version;
    std::string _url;
    std::vector<std::string> _cookies;
    std::string _session;
    std::unordered_map<std::string, std::string> _header_kv;
    std::string _suffix;
    std::string GetLine(std::string &reqstr)
    {
        auto pos = reqstr.find(base_sep);
        if (pos == std::string::npos)
        {
            return std::string();
        }
        std::string line = reqstr.substr(0, pos);
        reqstr.erase(0, line.size() + base_sep.size());
        return line.empty() ? base_sep : line;
    }

public:
    HttpRequest()
        : _blank_line(base_sep),
          _path(defaultpath)
    {
    }
    void ParseReq()
    {
        std::stringstream ss(_request_line);
        ss >> _method >> _url >> _version; // 自动添加空格
        if(strcasecmp(_method.c_str(),"GET")==0)
        {
            auto pos=_url.find(arg_sep);
            if(pos!=std::string::npos)
            {
                _body_text=_url.substr(pos+arg_sep.size());
                _url.resize(pos);
            }
        }
        _path += _url;
        if (_path[_path.size() - 1] == '/') // 找到最后一个位置
        {
            _path += homepage;
        }
        auto pos = _path.rfind(suffixsep);
        if (pos == std::string::npos)
        {
            _suffix = ".default";
        }
        else
        {
            _suffix = _path.substr(pos);
        }
    }
    std::string GetRequestBody()
    {
        LOG(DEBUG,"Request method is %s,args is %s,request path is %s",_method.c_str(),_body_text.c_str(),_path.c_str());
        return _body_text;
    }
    void ParseHeader()
    {
        for (auto &head : _request_header)
        {
            auto pos = head.find(line_sep);
            if (pos == std::string::npos)
                continue;
            std::string key = head.substr(0, pos);
            std::string val = head.substr(pos + line_sep.size());
            if (key.empty() || val.empty())
            {
                continue;
            }
            _header_kv.insert(std::make_pair(key, val));
        }
        for(auto& cookie:_request_header)
        {
            auto pos=cookie.find("Cookie: ");
            if(pos==std::string::npos)
            {
                break;
            }
            _cookies.emplace_back(cookie.substr(pos+1));
            break;
        }
        for(auto& session:_cookies)
        {
            auto pos=session.find("sessionid=");
            if(pos==std::string::npos)
                break;
            _session=session.substr(pos+1);
            break;
        }
    }
    std::string Session()
    {
        return _session;
    }
    void Deserialize(std::string reqstr)
    {
        _request_line = GetLine(reqstr);
        
        std::string header;
        do
        {
            header = GetLine(reqstr);
            if (header.empty())
                break;
            if (header != base_sep)
            {
                _request_header.push_back(header);
            }
        } while (true);
        if (!reqstr.empty())
        {
            _body_text = reqstr;
        }
        ParseReq();
        ParseHeader();
    }
    std::string Suffix()
    {
        return _suffix;
    }
    void Print()
    {
        std::cout << "###" << _request_line << std::endl;
        for (auto &head : _request_header)
        {
            std::cout << "@@@" << head << std::endl;
        }
        std::cout << "***" << _blank_line << std::endl;
        std::cout << ">>>" << _body_text << std::endl;
        std::cout << "method:" << _method << std::endl;
        std::cout << "***url:" << _url << std::endl;
        std::cout << "***path:" << _path << std::endl;
        std::cout << "***version" << _version << std::endl;
        for (auto &head : _header_kv)
        {
            std::cout << head.first << ":" << head.second << std::endl;
        }
    }
    std::string Path()
    {
        LOG(DEBUG,"Request Want Path:%s",_path.c_str());
        return _path;
    }
    std::string Method()
    {
        LOG(DEBUG,"Request Want Method:%s",_method.c_str());
        return _method;
    }
    std::string Url()
    {
        LOG(DEBUG,"Request Want URL:%s",_url.c_str());
        return _url;
    }
};
class HttpResponse
{
private:
    std::string _version;
    int _status_code;
    std::string _desc;
    std::vector<std::pair<std::string, std::string>> _header_kv;
    //使用unordered_map会导致前面传入的相同的键被后面的覆盖

    //std::vector<std::pair<std::string, std::string>> _header_kv;
    std::vector<std::string> _resp_head;
    std::string _resp_body_text;
    std::string _status_line;
    std::string _blank_line;
    std::string _suffix;

public:
    HttpResponse()
        : _version(version),
          _blank_line(base_sep)
    {
    }
    void addCode(int code, const std::string &desc)
    {
        _status_code = code;
        _desc = desc;
    }
    void addBodytext(const std::string &body_text)
    {
        _resp_body_text = body_text;
    }
    void addHeader(const std::string &k, const std::string &v)
    {
        _header_kv.push_back(std::make_pair(k,v));
    }
    std::string Serialize()
    {
        // 状态行
        _status_line = version + space + std::to_string(_status_code) + space + _desc + base_sep;
        // 报头
        for (auto &head : _header_kv)
        {
            std::string head_line = head.first + line_sep + head.second + base_sep;
            _resp_head.push_back(head_line);
        }
        // 空行和正文在上面的函数中已经添加过了
        // 开始构建序列化字符串
        std::string respstr = _status_line;
        for (auto &head : _resp_head)
        {
            respstr += head;
        }
        respstr += _blank_line;
        respstr += _resp_body_text;
        return respstr;
    }
};
class HttpServer
{
public:
    HttpServer()
    {
        _min_type.insert(std::make_pair(".jpg", "img/jpg"));
        _min_type.insert(std::make_pair(".html", "text/html"));
        _min_type.insert(std::make_pair(".png", "img/png"));
        _min_type.insert(std::make_pair(".default", "text/html"));

        _code_to_desc.insert(std::make_pair(100, "Continue"));
        _code_to_desc.insert(std::make_pair(200, "OK"));
        _code_to_desc.insert(std::make_pair(201, "Created"));
        _code_to_desc.insert(std::make_pair(301, "Moved Permanently"));
        _code_to_desc.insert(std::make_pair(302, "Found"));
        _code_to_desc.insert(std::make_pair(404, "Not Found"));
        _session=std::make_shared<SessionManager>();
    }
    std::string HttpHandler(std::string &in)
    {
        std::cout << "---------------------------------" << std::endl;
        HttpRequest req;
        req.Deserialize(in);
        _path = req.Path();
        req.GetRequestBody();
        std::string content = GetFileContent(_path);
        HttpResponse resp;
        req.Print();
        static int num=0;
        if (_path == "wwwroot/redir")
        {
            std::string redir = "https://www.qq.com";
            resp.addCode(301, _code_to_desc[301]);
            resp.addHeader("Location", redir);
        }
        else if(_path=="wwwroot/login")
        {
            
            if(req.Session().empty())
            {
                std::string user="user-"+std::to_string(num++);
                session_ptr s=std::make_shared<Session>(user,"login");
                std::string sessionid=_session->AddSessionID(s);
                resp.addHeader("sessionid=",sessionid);
            }
            else
            {
                std::string session=req.Session();
                session_ptr s=_session->GetSession(session);
                if(!session.empty())
                {
                    LOG(DEBUG,"%s is activing",s->_user.c_str());
                }
            }
        }
        else if(!req.GetRequestBody().empty())
        {
            if(_service.count(req.Path()))
            {
                resp=_service[req.Path()](req);
            }
        }
        else
        {
            if (content.empty())
            {
                content = GetFileContent("wwwroot/404.html");
                resp.addCode(404, "Not Found");
                resp.addHeader("Content-Length", std::to_string(content.size()));
                resp.addHeader("Content-Type", _min_type[".html"]);
                resp.addBodytext(content);
            }
            else
            {
                resp.addCode(200, "OK");
                resp.addHeader("Content-Length", std::to_string(content.size()));
                resp.addHeader("Content-Type", _min_type[req.Suffix()]);
                std::string cookieVal="user=123456; expires="+ExpireTime(60)+";";
                resp.addHeader("Set-Cookie",cookieVal);
                resp.addHeader("Ser-Cookie","password=1546;");
                resp.addBodytext(content);
            }
        }
        return resp.Serialize();
    }
    void InsertService(const std::string& req,func_t func)
    {
        std::string s=defaultpath+req;//浏览器请求的是wwwroot/login，因此加上前缀
        _service[s]=func;
    }
private:
    std::string _path;
    std::string _url;
    std::unordered_map<std::string, std::string> _min_type;
    std::unordered_map<int, std::string> _code_to_desc;
    std::unordered_map<std::string,func_t> _service;
    std::shared_ptr<SessionManager> _session;
    std::string GetFileContent(const std::string &path)
    {
        // 以文件流的方式读取文件
        std::ifstream in(path, std::ios::binary);
        if (!in.is_open())
            return std::string();
        in.seekg(0, in.end);   // 从头读到尾
        int size = in.tellg(); // 获取文件的偏移量（已经将读写位置移动到文件的结尾，因此偏移量就是整个文件的大小）
        in.seekg(0, in.beg);   // 将指针放回到文件开头
        std::string content;
        content.resize(size);
        in.read((char *)content.c_str(), size);
        in.close();
        return content;
    }
    std::string GetMonthName(int month)
    {
        std::vector<std::string> months={"Jan","Feb","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"};
        return months[month];
    }
    std::string GetWeekName(int day)
    {
        std::vector<std::string> week={"Sun","Mon","Feb","Wed","Thu","Fri","Sat"};
        return week[day];
    }
    std::string ExpireTime(int t)
    {
        time_t timeout=time(nullptr)+t;
        struct tm *tm=gmtime(&timeout);
        char timebuf[1024];
        snprintf(timebuf,sizeof(timebuf),"%s, %02d %s %d,%02d:%02d:%02d UTC",
        GetWeekName(tm->tm_wday).c_str(),
        tm->tm_mday,
        GetMonthName(tm->tm_mon).c_str(),
        tm->tm_year+1900,
        tm->tm_hour,
        tm->tm_min,
        tm->tm_sec
        );
        return timebuf;
    }
};
