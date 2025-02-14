#pragma once

#include <filesystem>
#include <format>
#include <memory>
#include <sstream>
#include <string>

#include "Core/Pattern/Singleton.h"

#include "LoggerLevel.h"

namespace Voxium::Core
{
    class LoggerImpl;

    class Logger final : public Voxium::Core::Pattern::Singleton<Logger>
    {
        friend class Voxium::Core::Pattern::Singleton<Logger>;

    private:
        Logger(Token);

        ~Logger();

    public:
        bool InitFromConfig(const std::string& config_file);

        void Shutdown();

        template<typename... Args>
        void Logf(const char*                 file,
                  int                         line,
                  const char*                 function,
                  LogLevel                    level,
                  std::format_string<Args...> fmt,
                  Args&&... args)
        {
            Log(file, line, function, level, std::format(fmt, std::forward<Args>(args)...));
        }

        void Logf(const char* file, int line, const char* function, LogLevel level, const std::string& message);

        void Log(const char* file, int line, const char* function, LogLevel level, const std::string& message);

        void SetLevel(LogLevel level);

        LogLevel GetLevel() const;

        void FlushOn(LogLevel lvl);

    private:
        std::unique_ptr<LoggerImpl> impl_;

    public:
        /// let logger like stream
        struct LogStream
        {
        public:
            LogStream(const char* file, int line, const char* function, LogLevel level) :
                file_(file), line_(line), function_(function), level_(level)
            {}

            ~LogStream() { Flush(); }

            template<typename T>
            LogStream& operator<<(const T& value)
            {
                stream_ << value;
                return *this;
            }

        private:
            void Flush() { Logger::Instance().Log(file_, line_, function_, level_, stream_.str().c_str()); }

        private:
            const char*        file_;
            int                line_;
            const char*        function_;
            LogLevel           level_;
            std::ostringstream stream_;
        };
    };

} // namespace Voxium::Core

#define __FILENAME__ (std::filesystem::path(__FILE__).filename().string().c_str())

#define LOG(level, fmt, ...) \
    Voxium::Core::Logger::Instance().Logf(__FILENAME__, __LINE__, __FUNCTION__, level, fmt, ##__VA_ARGS__)

#define LOG_TRACE(fmt, ...) LOG(Voxium::Core::LogLevel::Trace, fmt, ##__VA_ARGS__)
#define LOG_DEBUG(fmt, ...) LOG(Voxium::Core::LogLevel::Debug, fmt, ##__VA_ARGS__)
#define LOG_INFO(fmt, ...) LOG(Voxium::Core::LogLevel::Info, fmt, ##__VA_ARGS__)
#define LOG_WARN(fmt, ...) LOG(Voxium::Core::LogLevel::Warn, fmt, ##__VA_ARGS__)
#define LOG_ERROR(fmt, ...) LOG(Voxium::Core::LogLevel::Error, fmt, ##__VA_ARGS__)
#define LOG_FATAL(fmt, ...) LOG(Voxium::Core::LogLevel::Fatal, fmt, ##__VA_ARGS__)

#define LOG_IF(condition, level, fmt, ...) \
    if (condition) \
    { \
        LOG(level, fmt, ##__VA_ARGS__); \
    }

#define STREAM_TRACE() \
    Voxium::Core::Logger::LogStream(__FILENAME__, __LINE__, __FUNCTION__, Voxium::Core::LogLevel::Trace)
#define STREAM_DEBUG() \
    Voxium::Core::Logger::LogStream(__FILENAME__, __LINE__, __FUNCTION__, Voxium::Core::LogLevel::Debug)
#define STREAM_INFO() \
    Voxium::Core::Logger::LogStream(__FILENAME__, __LINE__, __FUNCTION__, Voxium::Core::LogLevel::Info)
#define STREAM_WARN() \
    Voxium::Core::Logger::LogStream(__FILENAME__, __LINE__, __FUNCTION__, Voxium::Core::LogLevel::Warn)
#define STREAM_ERROR() \
    Voxium::Core::Logger::LogStream(__FILENAME__, __LINE__, __FUNCTION__, Voxium::Core::LogLevel::Error)
#define STREAM_FATAL() \
    Voxium::Core::Logger::LogStream(__FILENAME__, __LINE__, __FUNCTION__, Voxium::Core::LogLevel::Fatal)
