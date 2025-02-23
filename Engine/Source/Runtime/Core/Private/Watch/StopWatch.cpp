#include "Watch/StopWatch.h"

namespace Voxium::Core
{

    StopWatch::StopWatch() { Reset(); }

    void StopWatch::Start()
    {
        if (!running_)
        {
            startTime_ = std::chrono::steady_clock::now();
            running_   = true;
        }
    }

    void StopWatch::Stop()
    {
        if (running_)
        {
            auto now = std::chrono::steady_clock::now();
            elapsed_ += std::chrono::duration_cast<std::chrono::milliseconds>(now - startTime_).count();
            running_ = false;
        }
    }

    void StopWatch::Reset()
    {
        running_   = false;
        elapsed_   = 0;
        startTime_ = std::chrono::steady_clock::now();
    }

    void StopWatch::Restart()
    {
        Reset();
        Start();
    }

    long long StopWatch::ElapsedMilliseconds() const
    {
        if (running_)
        {
            auto now = std::chrono::steady_clock::now();
            return elapsed_ + std::chrono::duration_cast<std::chrono::milliseconds>(now - startTime_).count();
        }
        return elapsed_;
    }

    bool StopWatch::IsRunning() const { return running_; }

} // namespace Voxium::Core
