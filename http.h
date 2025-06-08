#include <string>
#include <unordered_map>
#include <vector>
#include <sstream>
using namespace std;
class HttpResponse
{
private:
    string _blank_line = "\r\n"; // 空行
    string _head_sep = " ";      // 请求行中的空格
    string session;              // Cookie中的session，用于较为安全的区分用户
    vector<string> _headers_kv;  // 请求头，包含Cookie等字段
    vector<string> cookies;      // Cookie
    string _method;              // 请求方法
    string _version;             // HTTP版本
    string _uri;                 // 请求路径
    string _body_text;           // 请求正文
    string _req_line;            // 整个HTTP请求报文
    vector<string> _req_head;
    string Getline()
    {
        
            auto pos = _req_line.find(_blank_line);
            if (pos == string::npos)
            {
                return string();
            }
            string tmp=_req_line.substr(0, pos);
            _req_head.push_back(tmp);
            _req_line.erase(0,tmp.size()+_blank_line.size());
        
    }

public:
    HttpResponse(const string &reqstr)
        : _req_line(reqstr)
    {
    }
};