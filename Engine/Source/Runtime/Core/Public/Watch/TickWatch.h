#pragma once

#include <functional>

#include "Watch/StopWatch.h"

namespace Voxium::Core
{

    // TickWatch provides a timer that checks if a given time threshold has passed.
    // The threshold can be fixed (via an int 'max') or dynamic (via a std::function<int()> 'maxFunc').
    class CORE_API TickWatch
    {
    public:
        // Constructor with a fixed max value (in milliseconds)
        explicit TickWatch(int maxValue);

        // Constructor with a dynamic max value provided by a function
        explicit TickWatch(std::function<int()> maxFunction);

        // Returns true if the elapsed time exceeds the threshold,
        // resets the timer (restarts the stopwatch) in that case.
        // If the watch is not running, returns false.
        bool Allow();

        // Control functions for the stopwatch
        void Start();
        void Restart();
        void Stop();
        void Reset();

        // Returns the current elapsed time in milliseconds.
        int Elapsed() const;

    private:
        StopWatch            watch_;   // Internal stopwatch object
        int                  max_;     // Fixed maximum threshold (in milliseconds)
        std::function<int()> maxFunc_; // Dynamic maximum threshold function (optional)
    };

} // namespace Voxium::Core
