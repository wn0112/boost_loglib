#pragma once
/********************************* 
   * message cache size : 1000000
   * max message string length : 1024
**********************************/

#ifndef WLOG_H
#define WLOG_H

#ifdef _MSC_VER
#define DLL_EXPORT __declspec( dllexport )
#else
#define DLL_EXPORT
#endif
#include <ostream>
class Log;

// log level 
enum LOG_LVL {
    trace,
    debug,
    info,
    warning,
    error,
    fatal
};

class DLL_EXPORT WLogger
{
public:
    WLogger();
    WLogger(const std::string& file, int level, int fileCount, long long fileSize);
    ~WLogger();

    // max message length: 1024
    void trace(const char* fmt, ...);
    void info(const char* fmt, ...);
    void debug(const char* fmt, ...);
    void warning(const char* fmt, ...);
    void error(const char* fmt, ...);
    void fatal(const char* fmt, ...);

    //initial logger, works after setXXX() serial functions
    void init();

    // default: log.log
    void setLogFile(const std::string&);
    
    // default: 5
    void setRotateFileCount(int);
    
    // rotate file size, default: 10MB
    void setRotateFileSize(long long);
    
    // log level, default: info
    void setLogLevel(int);

    // time format, default: "%Y-%m-%d %H:%M:%S"
    void setTimeFormat(const std::string&);

    // enable async log, default: false
    void setAsync(bool);

    // send to console, default: true
    void setPrintToConsole(bool);

    // write message to file
    void flush(); 

private:
    Log* __logger;
    static const int BUFFSIZE = 1024;
    int __level; // default: info
};

#endif // WLOG_H
