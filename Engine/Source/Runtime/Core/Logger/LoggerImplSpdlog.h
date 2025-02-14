#pragma once

#include <spdlog/spdlog.h>

#include "LoggerImpl.h"

namespace Voxium::Core
{
    class LoggerImplSpdlog : public LoggerImpl
    {
    public:
        LoggerImplSpdlog() : logger_(nullptr) {}

        virtual ~LoggerImplSpdlog() override { Shutdown(); }

        virtual bool InitFromConfig(const std::string& config_file) override;

        virtual void
        Log(const char* file, int line, const char* function, LogLevel level, const std::string& message) override;

        virtual void Shutdown() override;

        virtual void SetLevel(LogLevel level) override;

        virtual LogLevel GetLevel() const override;

        virtual void FlushOn(LogLevel lvl) override;

    private:
        spdlog::level::level_enum ToSpdlogLevel(LogLevel level) const;

        LogLevel FromSpdlogLevel(spdlog::level::level_enum level) const;

    private:
        bool                            initialized_ = false;
        std::shared_ptr<spdlog::logger> logger_;
    };

} // namespace Voxium::Core
