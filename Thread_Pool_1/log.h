#include <unistd.h>
#include <sys/types.h>
#include <iostream>
#include <time.h>
#include <string>
#include <cstring>
#include <stdarg.h>
#include <sys/fcntl.h>
#include <fstream>
#define SCREEN 1
#define FILE 2
const std::string glogfile = "./log.txt";
enum
{
    DEBUG = 0,
    INFO,
    WARNING,
    ERROR,
    FATAL,
};
class LogMessage
{
public:
    pid_t _pid;
    std::string _filename;
    int _filenum;
    std::string _level;
    std::string _cur_time;
    std::string _message_info;
};
class Log
{
private:
    std::string GetCurTime()
    {
        time_t now = time(nullptr);
        struct tm *cur_time = localtime(&now);
        char buff[128];
        snprintf(buff, sizeof(buff), "%d-%02d-%02d %02d:%02d:%02d",
                 cur_time->tm_year + 1900,
                 cur_time->tm_mon,
                 cur_time->tm_mday,
                 cur_time->tm_hour,
                 cur_time->tm_min,
                 cur_time->tm_sec);
        return buff;
    }
    std::string leveltostring(int level)
    {
        switch (level)
        {
        case DEBUG:
            return "DEBUG";
        case INFO:
            return "INFO";
        case WARNING:
            return "WARNING";
        case ERROR:
            return "ERROR";
        case FATAL:
            return "FATAL";
        default:
            return "UNKNOWN";
        }
    }
    void FlushToScreen(const LogMessage &lg)
    {
        printf("[%s][%d][%s][%d][%s] %s\n",
               lg._level.c_str(),
               lg._pid,
               lg._filename.c_str(),
               lg._filenum,
               lg._cur_time.c_str(),
               lg._message_info.c_str());
    }
    void FlushToFile(const LogMessage &lg)
    {
        std::ofstream out(_logfile, std::ios::app);
        if (!out.is_open())
            return;
        char logtext[1024];
        snprintf(logtext, sizeof(logtext), "[%s][%d][%s][%d][%s] %s\n",
                 lg._level.c_str(),
                 lg._pid,
                 lg._filename.c_str(),
                 lg._filenum,
                 lg._cur_time.c_str(),
                 lg._message_info.c_str());
        out.write(logtext, strlen(logtext));
        out.close();
    }
    void FlushLog(const LogMessage &lg)
    {
        switch (_type)
        {
        case SCREEN:
            FlushToScreen(lg);
            break;
        case FILE:
            FlushToFile(lg);
            break;
        default:
            break;
        }
    }

public:
    Log(const std::string &file = glogfile)
        : _type(SCREEN),
          _logfile(file)
    {
    }
    ~Log()
    {}
    void Enable(int type)
    {
        _type = type;
    }
    void Message(const char *filename, int filenum, int level, const char *format, ...)
    {
        LogMessage lg;
        lg._pid = getpid();
        lg._filename = filename;
        lg._filenum = filenum;
        lg._level = leveltostring(level);
        lg._cur_time = GetCurTime();
        va_list ap;
        va_start(ap, format);
        char info[1024];
        vsnprintf(info, sizeof(info), format, ap); // 将va_list中的参数一个个提取出来放到info中
        va_end(ap);
        lg._message_info = info;
        FlushLog(lg);
    }

private:
    int _type;
    std::string _logfile;
};
Log lg;
#define LOG(level, format, ...)                                     \
    do                                                              \
    {                                                               \
        lg.Message(__FILE__, __LINE__, level, format, ##__VA_ARGS__); \
    } while (0)
#define EnableScreen()     \
    do                     \
    {                      \
        lg.Enable(SCREEN); \
    } while (0)
#define EnableFile()     \
    do                   \
    {                    \
        lg.Enable(FILE); \
    } while (0)
