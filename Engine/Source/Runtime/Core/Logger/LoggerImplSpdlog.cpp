#include "LoggerImplSpdlog.h"

#include <filesystem>
#include <fstream>

#include <spdlog/async.h>
#include <spdlog/logger.h>
#include <spdlog/sinks/msvc_sink.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include <nlohmann/json.hpp>

namespace details
{
    std::string GetCurrentDateString()
    {
        auto        now      = std::chrono::system_clock::now();
        std::time_t now_time = std::chrono::system_clock::to_time_t(now);

        std::tm tm_time;
#if defined(_MSC_VER)
        localtime_s(&tm_time, &now_time);
#else
        localtime_r(&now_time, &tm_time);
#endif

        std::stringstream ss;
        ss << std::put_time(&tm_time, "%H:%M:%S");
        return ss.str();
    }

} // namespace details

namespace Voxium::Core
{
    bool LoggerImplSpdlog::InitFromConfig(const std::string& config_file)
    {
        if (initialized_)
        {
            return true;
        }

        try
        {
            // Read and parse the configuration file
            std::ifstream file(config_file);
            if (!file.is_open())
            {
                initialized_ = false;
                throw std::runtime_error("Unable to open log config file: " + config_file);
            }

            nlohmann::json config;
            file >> config;

            // Extract configuration
            std::string log_level_str        = config.value("log_level", "info");
            std::string log_flush_level_str  = config.value("log_flush_level", "warn");
            bool        enable_console_log   = config.value("enable_console_log", true);
            bool        enable_file_log      = config.value("enable_file_log", false);
            std::string log_file_path        = config.value("log_file_path", "logs/eurora.log");
            std::string log_pattern          = config.value("log_pattern", "%s(%#): [%l %D %T.%e %t %!] %v");
            size_t      max_file_size        = config.value("max_file_size", 50 * 1024 * 1024); // 50MB
            size_t      max_files            = config.value("max_files", 50);
            size_t      log_buffer_size      = config.value("log_bugger_size", 32 * 1024); // 32Kb
            size_t      log_thread_pool_size = config.value("log_thread_pool_size", 1);

            // Set log level and flush level
            spdlog::level::level_enum log_level   = spdlog::level::from_str(log_level_str);
            spdlog::level::level_enum flush_level = spdlog::level::from_str(log_flush_level_str);

            // check log path and try to create log directory
            namespace fs = std::filesystem;
            fs::path    log_path(log_file_path);
            fs::path    log_dir   = log_path.parent_path();
            std::string base_name = log_path.stem().string();
            std::string extension = log_path.extension().string();

            if (!fs::exists(log_dir))
            {
                fs::create_directories(log_dir);
            }

            // Generate a unique log file name for the same dayS
            std::string date_str = details::GetCurrentDateString();

            int      count = 0;
            fs::path unique_log_file;
            do
            {
                unique_log_file = log_dir / fmt::format("{}_{}_{}{}", base_name, date_str, count, extension);
                count++;
            } while (fs::exists(unique_log_file));

            // Create sinks based on the configuration
            spdlog::init_thread_pool(log_buffer_size, log_thread_pool_size);
            std::vector<spdlog::sink_ptr> sinks;
            if (enable_file_log)
            {
                auto file_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(
                    unique_log_file.string(), max_file_size, max_files);
                sinks.push_back(file_sink);
            }
            if (enable_console_log)
            {
#if defined(_DEBUG) && defined(_WIN32) && !defined(NO_CONSOLE_LOG)
                auto ms_sink = std::make_shared<spdlog::sinks::msvc_sink_mt>();
                sinks.push_back(ms_sink);
#endif //  _DEBUG

#if !defined(_WIN32) && !defined(NO_CONSOLE_LOG)
                auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
                sinks.push_back(console_sink);
#endif
            }

            // Initialize logger with sinks
            logger_ = std::make_shared<spdlog::async_logger>(
                "", sinks.begin(), sinks.end(), spdlog::thread_pool(), spdlog::async_overflow_policy::block);
            spdlog::set_default_logger(logger_);

            spdlog::info("Logger initialized successfully with configuration from {}", config_file);

            // Apply settings
            logger_->set_level(log_level);
            spdlog::set_pattern(log_pattern);
            spdlog::flush_on(flush_level);
        }
        catch (const std::exception& e)
        {
            spdlog::error("Failed to initialize logger from config: {}", e.what());
            initialized_ = false;
            return false;
        }

        initialized_ = true;

        return true;
    }

    void
    LoggerImplSpdlog::Log(const char* file, int line, const char* function, LogLevel level, const std::string& message)
    {
        if (!initialized_)
        {
            throw std::runtime_error("SpdLogger is not initialized.");
        }
        spdlog::source_loc loc {file, line, function};
        logger_->log(loc, ToSpdlogLevel(level), message);
    }

    void LoggerImplSpdlog::Shutdown()
    {
        initialized_ = false;
        spdlog::shutdown();
    }

    void LoggerImplSpdlog::SetLevel(LogLevel level)
    {
        if (!initialized_)
        {
            throw std::runtime_error("Logger is not initialized.");
        }
        logger_->set_level(ToSpdlogLevel(level));
    }

    LogLevel LoggerImplSpdlog::GetLevel() const
    {
        if (!initialized_)
        {
            throw std::runtime_error("Logger is not initialized.");
        }
        return FromSpdlogLevel(logger_->level());
    }

    void LoggerImplSpdlog::FlushOn(LogLevel lvl)
    {
        if (!initialized_)
        {
            throw std::runtime_error("Logger is not initialized.");
        }
        spdlog::flush_on(ToSpdlogLevel(lvl));
    }

    spdlog::level::level_enum LoggerImplSpdlog::ToSpdlogLevel(LogLevel level) const
    {
        switch (level)
        {
            case LogLevel::Trace:
                return spdlog::level::trace;
            case LogLevel::Debug:
                return spdlog::level::debug;
            case LogLevel::Info:
                return spdlog::level::info;
            case LogLevel::Warn:
                return spdlog::level::warn;
            case LogLevel::Error:
                return spdlog::level::err;
            case LogLevel::Fatal:
                return spdlog::level::critical;
            default:
                return spdlog::level::info;
        }
    }

    LogLevel LoggerImplSpdlog::FromSpdlogLevel(spdlog::level::level_enum level) const
    {
        switch (level)
        {
            case spdlog::level::trace:
                return LogLevel::Trace;
            case spdlog::level::debug:
                return LogLevel::Debug;
            case spdlog::level::info:
                return LogLevel::Info;
            case spdlog::level::warn:
                return LogLevel::Warn;
            case spdlog::level::err:
                return LogLevel::Error;
            case spdlog::level::critical:
                return LogLevel::Fatal;
            default:
                return LogLevel::Info;
        }
    }

} // namespace Voxium::Core
