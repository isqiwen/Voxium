#pragma once

#include <chrono>

#include "CoreMacros.h"

namespace Voxium::Core
{
    class CORE_API StopWatch
    {
    public:
        // Default constructor: resets the stopwatch.
        StopWatch();

        // Starts the stopwatch.
        void Start();

        // Stops the stopwatch and accumulates elapsed time.
        void Stop();

        // Resets the stopwatch to zero and stops it.
        void Reset();

        // Resets and starts the stopwatch.
        void Restart();

        // Returns the total elapsed time in milliseconds.
        long long ElapsedMilliseconds() const;

        // Returns whether the stopwatch is currently running.
        bool IsRunning() const;

    private:
        // Indicates if the stopwatch is currently running.
        bool running_ = false;

        // Accumulated elapsed time in milliseconds from previous runs.
        long long elapsed_ = 0;

        // The time point when Start() was last called.
        std::chrono::steady_clock::time_point startTime_;
    };

} // namespace Voxium::Core
