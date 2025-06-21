#include<cstring>
#include<string>
#include<unordered_map>
#include<iostream>
#include <fstream>
const std::string sep=": ";
class Dict
{
    private:
    std::unordered_map<std::string,std::string> _dict;
    std::string _path;
    void load(const std::string& path)
    {
        std::ifstream in(path);
        if(!in.is_open())
        {
            std::cout<<"file open failed"<<std::endl;
            exit(-1);
        }
        std::string line;
        while(std::getline(in,line))
        {
            if(line.empty())
            {
                continue;
            }
            auto it=line.find(sep);
            if(it==std::string::npos)
            {
                continue;
            }
            std::string key=line.substr(0,it);
            if(key.empty())
                continue;
            std::string val=line.substr(it+sep.size());
            if(val.empty())
                continue;
            _dict.insert({key,val});
        }
    }
    public:
    Dict(const std::string& path)
    :_path(path)
    {
        load(path);
    }
    std::string translate(const std::string& word)
    {
        if(!_dict.count(word))
        {
            return "no word";
        }
        return _dict[word];
    }
};