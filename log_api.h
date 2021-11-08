#pragma once
#ifndef LOG_API_H
#define LOG_API_H
#include <string>
#include <boost/log/trivial.hpp>
#include <boost/log/sources/logger.hpp>
#include <boost/log/expressions.hpp>

namespace logging = boost::log;
namespace src = boost::log::sources;
namespace keywords = boost::log::keywords;
namespace sinks = boost::log::sinks;
namespace expr = boost::log::expressions;
namespace trvl = boost::log::trivial;
namespace attrs = boost::log::attributes;

class Log
{

public:
    Log();
    Log(const std::string& file, int level, int fileCount, long long fileSize);
    ~Log();

    //在使用之前必须先调用此函数
    void init();
    // 写日志
    void write(const char* msg, trvl::severity_level);
    //添加异步槽
    void addAsyncSink();
    //添加同步槽
    void addSyncSink();
    void setLogFile(const std::string&);
    void setRotateFileCount(int);
    void setRotateFileSize(long long);   // MB
    void setLogLevel(int);
    void setTimeFormat(const std::string&);
    void setAsync(bool);
    void setPrintToConsole(bool);
    void flush(); // 清空队列, 将缓存写入文件

private:
    bool _enableAsync;
    int _fileCount;
    int _logLevel;
    int _log2Console;
    long long _fileSize;
    std::string _filename;
    std::string _fmt;
    src::severity_logger<trvl::severity_level> _logger;
    logging::formatter _logFmt;
};

#endif //LOG_API_H
