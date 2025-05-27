#include "log.h"
#include <unordered_map>
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
const std::string sep = ": ";
class dict
{
private:
    std::string _path;
    std::unordered_map<std::string, std::string> _dict;
    void load(const std::string &path)
    {
        std::ifstream in(path);
        if (!in.is_open())
        {
            LOG(FATAL, "file open failed");
            exit(0);
        }
        std::string line;
        while (std::getline(in, line))
        {
            if (line.empty())
                continue;

            auto it = line.find(sep);
            if (it == line.npos)
                continue;

            std::string key = line.substr(0, it);
            if (key.empty())
                continue;
            std::string val = line.substr(it + sep.size());
            if (key.empty())
                continue;
            _dict.insert({key,val});
        }
        LOG(DEBUG,"fail loaded");
        in.close();
    }

public:
    dict(const std::string& path)
    :_path(path)
    {
        load(_path);
    }
    std::string translate(const std::string& word)
    {
        if(!_dict.count(word)) return "no word";
        return _dict[word];
    }
};