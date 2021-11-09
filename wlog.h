#pragma once
/********************************* 
   * 异步日志缓冲队列大小 : 1000000
   * 日志每行字符串最大长度: 1024
**********************************/

#ifndef WLOG_H
#define WLOG_H

#ifdef LOG_API
#else
#define LOG_API _declspec(dllimport)
#endif
#include <ostream>
class Log;

//日志等级 
enum LOG_LVL {
    trace,
    debug,
    info,
    warning,
    error,
    fatal
};

class LOG_API WLogger
{
public:
    WLogger();
    WLogger(const std::string& file, int level, int fileCount, long long fileSize);
    ~WLogger();

    // 格式化后的字符串最大长度: 1024
    void trace(const char* fmt, ...);
    void info(const char* fmt, ...);
    void debug(const char* fmt, ...);
    void warning(const char* fmt, ...);
    void error(const char* fmt, ...);
    void fatal(const char* fmt, ...);

    //初始化 logger; 先执行设置函数 setXXX(), 再 init(), 设置才会有效
    void init();

    //设定日志文件; 默认 log.log
    void setLogFile(const std::string&);
    
    //设置滚动日志的最大个数; 默认 5
    void setRotateFileCount(int);
    
    //设置单个日志文件大小, 单位 MB; 默认 10MB
    void setRotateFileSize(long long);
    
    //设置日志等级, 大于等于此等级的才记录; 默认 info 级别
    void setLogLevel(int);

    //设置日志时间格式, 默认: "%Y-%m-%d %H:%M:%S"
    void setTimeFormat(const std::string&);

    //是否启用异步日志; 默认 否
    void setAsync(bool);

    //是否输出到终端; 默认 是
    void setPrintToConsole(bool);

    // 立即刷新缓冲, 写入日志文件; 针对异步日志起作用
    void flush(); 

private:
    Log* __logger;
    static const int BUFFSIZE = 1024;
    int __level; // default: info
};

#endif // WLOG_H
