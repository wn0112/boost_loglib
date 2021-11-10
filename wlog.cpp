#include "wlog.h"
#include "log_api.h"

WLogger::WLogger(): __level(2)
{
    __logger = new Log();
}

WLogger::WLogger(const std::string& file, int level, int fileCount, long long fileSize): __level(level)
{
    __logger = new Log(file, level, fileCount, fileSize);
}

WLogger::~WLogger()
{
    delete __logger;
}

void WLogger::init()
{
    __logger->init();
}

void WLogger::setLogFile(const std::string& file)
{
    __logger->setLogFile(file);
}

void WLogger::setRotateFileCount(int count)
{
    __logger->setRotateFileCount(count);
}

void WLogger::setRotateFileSize(long long size)
{
    __logger->setRotateFileSize(size);
}

void WLogger::setLogLevel(int level)
{
    __level = level;
    __logger->setLogLevel(level);
}

void WLogger::setTimeFormat(const std::string& fmt)
{
    __logger->setTimeFormat(fmt);
}

void WLogger::setAsync(bool enable)
{
    __logger->setAsync(enable);
}

void WLogger::setPrintToConsole(bool enable)
{
    __logger->setPrintToConsole(enable);
}

void WLogger::flush()
{
    __logger->flush();
}

void WLogger::trace(const char* fmt, ...)
{
    if (__level > trvl::severity_level::trace) 
        return;

    char buff[WLogger::BUFFSIZE];
    va_list ap;
    va_start(ap, fmt); 
    vsprintf_s(buff, WLogger::BUFFSIZE, fmt, ap);
    va_end(ap);
    __logger->write(buff, trvl::severity_level::trace);
}

void WLogger::info(const char* fmt, ...)
{
    if (__level > trvl::severity_level::info) 
        return;

    char buff[WLogger::BUFFSIZE];
    va_list ap;
    va_start(ap, fmt);
    vsprintf_s(buff, WLogger::BUFFSIZE, fmt, ap);
    va_end(ap);
    __logger->write(buff, trvl::severity_level::info);
}

void WLogger::debug(const char* fmt, ...)
{
    if (__level > trvl::severity_level::debug)
        return;

    char buff[WLogger::BUFFSIZE];
    va_list ap;
    va_start(ap, fmt);
    vsprintf_s(buff, WLogger::BUFFSIZE, fmt, ap);
    va_end(ap);
    __logger->write(buff, trvl::severity_level::debug);
}

void WLogger::warning(const char* fmt, ...)
{
    if (__level > trvl::severity_level::warning)
        return;

    char buff[WLogger::BUFFSIZE];
    va_list ap;
    va_start(ap, fmt);
    vsprintf_s(buff, WLogger::BUFFSIZE, fmt, ap);
    va_end(ap);
    __logger->write(buff, trvl::severity_level::warning);
}

void WLogger::error(const char* fmt, ...)
{
    if (__level > trvl::severity_level::error)
        return;

    char buff[WLogger::BUFFSIZE];
    va_list ap;
    va_start(ap, fmt);
    vsprintf_s(buff, WLogger::BUFFSIZE, fmt, ap);
    va_end(ap);
    __logger->write(buff, trvl::severity_level::error);
}

void WLogger::fatal(const char* fmt, ...)
{
    if (__level > trvl::severity_level::fatal)
        return;

    char buff[WLogger::BUFFSIZE];
    va_list ap;
    va_start(ap, fmt);
    vsprintf_s(buff, WLogger::BUFFSIZE, fmt, ap);
    va_end(ap);
    __logger->write(buff, trvl::severity_level::fatal);
}
