#include<fstream>
#include<string>
#include<unistd.h>
#include<bits/stdc++.h>
using namespace std;
const string default_path="./dict.txt";
const string sep=": ";
class dict
{
    public:
    void LoadDict(string path)
    {
        fstream in(path);
        if(!in.is_open())
        {
            exit(-1);
        }
        string line;
        while(getline(in,line))
        {
            if(line.empty())
            {
                continue;
            }
            auto pos=line.find(sep);
            if(pos==string::npos)
            {
                continue;
            }
            string key=line.substr(0,pos);
            if(key.empty())
            {
                continue;
            }
            string value=line.substr(pos+sep.size());
            if(value.empty())
            {
                continue;
            }
            _dict.insert(make_pair(key,value));
        }
        cout<<"Load Dict Success"<<endl;
        in.close();
    }
    string translate(const string word)
    {
        if(word.empty())
        {
            return "None";
        }
        auto iter=_dict.find(word);
        if(iter==_dict.end())
        {
            return "None";
        }
        else
        {
            return iter->second;
        }
    }
    dict(string path=default_path)
    :_dict_path(path)
    {
        LoadDict(_dict_path);
    }
    ~dict()
    {}
    private:
    unordered_map<string,string> _dict;
    string _dict_path;
};