#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <unordered_map>
#include <fstream>
#include<functional>
#include"log.h"
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
    std::unordered_map<std::string, std::string> _header_kv;
    std::string _suffix;
    std::string GetLine(std::string &reqstr)
    {
        auto pos = reqstr.find(base_sep);
        if (pos == reqstr.npos)
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
            if(pos!=_url.npos)
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
        auto pos = _request_line.rfind(suffixsep);
        if (pos == _request_line.npos)
        {
            _suffix = ".default";
        }
        else
        {
            _suffix = _request_line.substr(pos);
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
            if (pos == head.npos)
                continue;
            std::string key = head.substr(0, pos);
            std::string val = head.substr(pos + line_sep.size());
            if (key.empty() || val.empty())
            {
                continue;
            }
            _header_kv.insert(std::make_pair(key, val));
        }
    }
    void Deserialize(std::string reqstr)
    {
        _request_line = GetLine(reqstr);
        ParseReq();
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
    std::string _status_code;
    std::string _desc;
    std::unordered_map<std::string, std::string> _header_kv;

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
        _status_code = std::to_string(code);
        _desc = desc;
    }
    void addBodytext(const std::string &body_text)
    {
        _resp_body_text = body_text;
    }
    void addHeader(const std::string &k, const std::string &v)
    {
        _header_kv[k] = v;
    }
    std::string Serialize()
    {
        // 状态行
        _status_line = version + space + _status_code + space + _desc + base_sep;
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
        _code_to_desc.insert(std::make_pair(404, "Not Found"));
        _code_to_desc.insert(std::make_pair(301, "Moved Permanently"));
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
        if (_path == "wwwroot/redir")
        {
            std::string redir = "https://www.qq.com";
            resp.addCode(301, _code_to_desc[301]);
            resp.addHeader("Location", redir);
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
        in.read((char*)content.c_str(), size);
        in.close();
        return content;
    }
};