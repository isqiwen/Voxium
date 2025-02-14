#include "Logger.h"
#include "LoggerImplSpdlog.h"

namespace Voxium::Core
{
    Logger::Logger(Token) : impl_(std::make_unique<LoggerImplSpdlog>()) {}

    Logger::~Logger() { Shutdown(); }

    bool Logger::InitFromConfig(const std::string& config_file) { return impl_->InitFromConfig(config_file); }

    void Logger::Logf(const char* file, int line, const char* function, LogLevel level, const std::string& message)
    {
        Log(file, line, function, level, message);
    }

    void Logger::Log(const char* file, int line, const char* function, LogLevel level, const std::string& message)
    {
        impl_->Log(file, line, function, level, message);
    }

    void Logger::Shutdown() { impl_->Shutdown(); }

    void Logger::FlushOn(LogLevel lvl) { impl_->FlushOn(lvl); }

    void Logger::SetLevel(LogLevel level) { impl_->SetLevel(level); }

    LogLevel Logger::GetLevel() const { return impl_->GetLevel(); }

} // namespace Voxium::Core
