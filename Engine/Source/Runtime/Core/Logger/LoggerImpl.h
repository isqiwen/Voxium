#pragma once

#include <string>

#include "LoggerLevel.h"

namespace Voxium::Core
{
    class LoggerImpl
    {
    public:
        virtual ~LoggerImpl() = default;

        virtual bool InitFromConfig(const std::string& config_file) = 0;

        virtual void
        Log(const char* file, int line, const char* function, LogLevel level, const std::string& message) = 0;

        virtual void Shutdown() = 0;

        virtual void SetLevel(LogLevel level) = 0;

        virtual LogLevel GetLevel() const = 0;

        virtual void FlushOn(LogLevel lvl) = 0;
    };

} // namespace Voxium::Core
