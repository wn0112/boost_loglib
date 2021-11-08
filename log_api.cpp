#include "log_api.h"
#include <iostream>
#include <boost/filesystem.hpp>
#include <boost/log/sources/global_logger_storage.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/record_ordering.hpp>
#include <boost/log/sinks/text_ostream_backend.hpp>
#include <boost/log/attributes/named_scope.hpp>
#include <boost/log/attributes.hpp>
#include <boost/log/detail/format.hpp>
#include <boost/log/common.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/log/detail/thread_id.hpp>
#include <boost/log/sinks.hpp>
#include <boost/algorithm/string.hpp>

static const int QUEUE_SIZE = 1000000; // 指定队列大小可以避免内存无限膨涨
using boost::shared_ptr;

BOOST_LOG_ATTRIBUTE_KEYWORD(thread_id, "ThreadID", attrs::current_thread_id::value_type);
BOOST_LOG_ATTRIBUTE_KEYWORD(severity, "Severity", trvl::severity_level);
BOOST_LOG_ATTRIBUTE_KEYWORD(tag_attr, "Tag", std::string);

// 写日志文件用
typedef sinks::text_file_backend backend_t;
typedef sinks::asynchronous_sink<backend_t, sinks::bounded_ordering_queue<
    logging::attribute_value_ordering< unsigned int, std::less< unsigned int >>, 
    QUEUE_SIZE, sinks::block_on_overflow>> async_sink_t;
typedef sinks::synchronous_sink<backend_t> sync_sink_t;

// 写终端用
typedef sinks::text_ostream_backend backend_t2;
typedef sinks::synchronous_sink<backend_t2> sync_sink_t2;

Log::Log() : _fileCount(5),
            _fileSize(10),
            _log2Console(true),
            _enableAsync(false),
            _fmt("%Y-%m-%d %H:%M:%S"),
            _filename("log.log"),
            _logLevel(trvl::severity_level::info)
{
   
}

Log::Log(const std::string& file, int level, int fileCount, long long fileSize)
    : _fileCount(fileCount), _fileSize(fileSize), _logLevel(level), _filename(file), 
    _log2Console(true), _enableAsync(false), _fmt("%Y-%m-%d %H:%M:%S")
{

}

void Log::init()
{
    _logFmt =
        (
            expr::stream
            << expr::format_date_time< boost::posix_time::ptime >("TimeStamp", _fmt)
            << " [" << thread_id << "]"
            << "[" << severity
            << "] " << expr::smessage
            );

    // 打印到终端
    if (_log2Console) {
        auto pConsoleBackend = boost::make_shared<backend_t2>();
        pConsoleBackend->add_stream(boost::shared_ptr<std::ostream>(&std::cout, boost::null_deleter()));
        auto consoleSink = boost::make_shared<sync_sink_t2>(pConsoleBackend);
        consoleSink->set_formatter(_logFmt);
        consoleSink->set_filter(severity >= (trvl::severity_level)_logLevel);
        logging::core::get()->add_sink(consoleSink);
    }

    if (_enableAsync)
        addAsyncSink();
    else
        addSyncSink();

    boost::log::add_common_attributes();
    logging::core::get()->add_global_attribute("TimeStamp", attrs::local_clock());
    logging::core::get()->add_global_attribute("RecordID", attrs::counter< unsigned int >());
}

void Log::addAsyncSink()
{
    boost::shared_ptr< async_sink_t > sink(new async_sink_t(
        boost::make_shared< backend_t >(),
        keywords::order = logging::make_attr_ordering< unsigned int >("RecordID", std::less< unsigned int >())
    ));

    boost::filesystem::path _path(_filename);
    sink->locked_backend()->set_file_collector(sinks::file::make_collector(
        keywords::target = _path.parent_path().string(),           // 日志备份目标文件夹
        keywords::max_size = _fileSize * _fileCount * 1024 * 1024  // 所有日志加起来的最大大小,
    ));

    sink->locked_backend()->set_file_name_pattern(_path.string());
    sink->locked_backend()->set_rotation_size((long long)_fileSize * 1024 * 1024);
    sink->locked_backend()->set_target_file_name_pattern(_path.stem().string() + std::string("_%Y%m%d-%N") + _path.filename().extension().string());
    sink->locked_backend()->set_open_mode(std::ios::app);
    sink->set_filter(severity >= (trvl::severity_level)_logLevel && expr::has_attr(tag_attr) && tag_attr == _filename);
    sink->set_formatter(_logFmt);

    logging::core::get()->add_sink(sink);
}


void Log::addSyncSink()
{
    boost::shared_ptr< sync_sink_t > sink(new sync_sink_t(
        boost::make_shared< backend_t >(),
        keywords::order = logging::make_attr_ordering< unsigned int >("RecordID", std::less< unsigned int >())
    ));

    boost::filesystem::path _path(_filename);
    sink->locked_backend()->set_file_collector(sinks::file::make_collector(
        keywords::target = _path.parent_path().string(),           // 日志备份目标文件夹
        keywords::max_size = _fileSize * _fileCount * 1024 * 1024  // 所有日志加起来的最大大小,
    ));

    sink->locked_backend()->set_file_name_pattern(_path.string());
    sink->locked_backend()->set_rotation_size((long long)_fileSize * 1024 * 1024);
    sink->locked_backend()->set_target_file_name_pattern(_path.stem().string() + std::string("_%Y%m%d-%N") + _path.filename().extension().string());
    sink->locked_backend()->set_open_mode(std::ios::app);
    sink->set_filter(severity >= (trvl::severity_level)_logLevel && expr::has_attr(tag_attr) && tag_attr == _filename);
    sink->set_formatter(_logFmt);

    logging::core::get()->add_sink(sink);
}

void Log::setLogFile(const std::string& file)
{
    _filename = file;
}

void Log::setRotateFileCount(int count)
{
    _fileCount = count;
}

void Log::setRotateFileSize(long long size)
{
    _fileSize = size;
}

void Log::setLogLevel(int level)
{
    _logLevel = level;
}

void Log::setPrintToConsole(bool enable)
{
    _log2Console = enable;
}

void Log::setTimeFormat(const std::string& fmt)
{
    _fmt = fmt;
}

void Log::setAsync(bool enable)
{
    _enableAsync = enable;
}

void Log::write(const char* msg, trvl::severity_level level)
{
    BOOST_LOG_SCOPED_THREAD_TAG("Tag", _filename);
    BOOST_LOG_SEV(_logger, level) << msg;
}

void Log::flush()
{
    boost::shared_ptr< logging::core > core = logging::core::get();
    core->flush();
}

Log::~Log()
{

}
