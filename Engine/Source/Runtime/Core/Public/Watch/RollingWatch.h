#pragma once

#include <algorithm>
#include <cassert>
#include <cmath>
#include <functional>

#include "Watch/StopWatch.h"

namespace Voxium::Core
{

    // For demonstration, define a base speed constant.
    constexpr int BASE_SPEED = 100;

    class CORE_API RollingWatch
    {
    public:
        // ----------------------------------------------------------------
        // Constructors
        // ----------------------------------------------------------------
        // Constructor with fixed frameCount, speed, direction, start flag, wrap flag, and optional startTime (default = 0)
        RollingWatch(int frameCount, float speed, bool direction, bool start, bool wrap, int startTime = 0);

        // Constructor with fixed frameCount, speed, direction, start flag, and wrap flag (default startTime = 0)
        RollingWatch(int frameCount, float speed, bool direction, bool start, bool wrap);

        // ----------------------------------------------------------------
        // Copy method: creates a copy of the current watch.
        // ----------------------------------------------------------------
        RollingWatch Copy() const;

        // ----------------------------------------------------------------
        // Stopwatch control methods.
        // ----------------------------------------------------------------
        void Start();
        void Stop();
        void Reset();
        void Restart();
        void Restart(int e);

        // ----------------------------------------------------------------
        // Direction control
        // ----------------------------------------------------------------
        // Get current direction.
        bool Direction() const;
        // Set direction and restart stopwatch. (Wrap check omitted in this example.)
        void Direction(bool d);

        // Set direction if different.
        void SetDirection(bool d);
        void SetDirectionNoStart(bool d);

        // ----------------------------------------------------------------
        // Speed control
        // ----------------------------------------------------------------
        float Speed() const;
        void  Speed(float s);
        void  SetRateNoStart(float s);

        // ----------------------------------------------------------------
        // Elapsed time calculation (in milliseconds, adjusted by speed and direction)
        // ----------------------------------------------------------------
        float Elapsed() const;

        // Setter for Elapsed, sets the priorElapsed.
        void SetElapsed(int e);

        // ----------------------------------------------------------------
        // Progress calculation: returns a value between 0 and 1.
        // ----------------------------------------------------------------
        double Progress() const;
        float  ProgressF() const;

        // ----------------------------------------------------------------
        // Running state
        // ----------------------------------------------------------------
        bool IsRunning() const;

        // Getter and setter for max.
        int  MaxValue() const;
        void SetMax(int m);
        int  MinValue() const;

    private:
        // ----------------------------------------------------------------
        // Helper: Restart the stopwatch (update startTime).
        // ----------------------------------------------------------------
        void RestartWatch();

        // ----------------------------------------------------------------
        // Member variables:
        // ----------------------------------------------------------------
        StopWatch    watch_;        // Internal stopwatch object.
        int          min_ = 0;      // Lower bound (here assumed to be 0).
        int          max_;          // Upper bound, in milliseconds.
        bool         direction_;    // true: increasing, false: decreasing.
        bool         wrap_;         // Whether to wrap around on overflow/underflow.
        mutable long priorElapsed_; // Accumulated elapsed time from previous runs.
        float        speed_;        // Speed factor (adjusting time scale).
    };

} // namespace Voxium::Core
