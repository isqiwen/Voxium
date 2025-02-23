#pragma once

#include <algorithm>
#include <cassert>
#include <chrono>
#include <cmath>
#include <cstdint>
#include <functional>
#include <thread>

namespace Voxium::Core
{

    class CustomWatch
    {
    public:
        // -------------------------------------------------------------
        // Constructors
        // -------------------------------------------------------------
        // Constructor: (min, seconds, direction, start)
        // 'seconds' is a double representing time in seconds, converted to milliseconds.
        CustomWatch(int min, double seconds, bool direction = true, bool start = false);

        // Constructor: (min, max, direction, start)
        CustomWatch(int min, int max, bool direction = true, bool start = false);

        // -------------------------------------------------------------
        // Copy method: returns a new CustomWatch with the same configuration.
        // -------------------------------------------------------------
        CustomWatch Copy() const;

        void Initialise(int min, int max, bool direction, bool start);

        // -------------------------------------------------------------
        // Recreate methods: reset the watch with new bounds or from another watch.
        // -------------------------------------------------------------
        void Recreate(int newMin, int newMax);
        void Recreate(int newMin, int newMax, int elapsed, bool direction);
        void Recreate(const CustomWatch& w);

        // -------------------------------------------------------------
        // Rate control methods
        // -------------------------------------------------------------
        void SetRate(double r);
        void SetRateNoStart(double r);

        // Getter and Setter for Rate
        double Rate();
        void   Rate(double newRate);

        // -------------------------------------------------------------
        // Stopwatch control methods using std::chrono
        // -------------------------------------------------------------
        void Start();
        void Stop();
        void Reset();
        void Restart();
        void Restart(int e);
        void RestartMax();

        // -------------------------------------------------------------
        // Direction control methods
        // -------------------------------------------------------------
        void SetDirection(bool d);
        void SetDirectionNoStart(bool d);

        // Getter and Setter for Direction property
        bool Direction() const;
        void Direction(bool d);

        // -------------------------------------------------------------
        // Elapsed time calculation
        // -------------------------------------------------------------
        // Returns the current elapsed time (in milliseconds) based on accumulated time and stopwatch.
        int Elapsed() const;
        // Setter for Elapsed: sets priorElapsed (simulating "Elapsed = value" in C#)
        void SetElapsed(int e);

        // -------------------------------------------------------------
        // Progress properties (derived from Elapsed)
        // -------------------------------------------------------------
        double  Progress() const;
        uint8_t ProgressB() const;
        float   ProgressF() const;
        double  ProgressEase() const;
        float   ProgressEaseF() const;
        float   ProgressFancyEaseF() const;
        float   ModernEaseF() const;
        double  ProgressQuad() const;
        float   ProgressQuadF() const;

        // -------------------------------------------------------------
        // Other properties
        // -------------------------------------------------------------
        // RawElapsed: returns the stopwatch's current measured elapsed time (in milliseconds)
        long RawElapsed() const;
        bool IsRunning() const;

        // Getter and setter for max property
        int  Max() const;
        void SetMax(int m);

        // Getter and setter for min property
        int  Min() const;
        void SetMin(int m);

    private:
        // -------------------------------------------------------------
        // Helper function to restart the stopwatch (reset startTime)
        // -------------------------------------------------------------
        void RestartWatch();

    private:
        // -------------------------------------------------------------
        // Member variables:
        // -------------------------------------------------------------
        // Stopwatch simulation using std::chrono:
        std::chrono::steady_clock::time_point startTime_ {std::chrono::steady_clock::now()};
        bool                                  isRunning_ = false;

        // priorElapsed: accumulated elapsed time in milliseconds before the current stopwatch run.
        mutable long priorElapsed_ = 0;

        // Rate factor (multiplier for elapsed time)
        double r_ = 1.0;

        // Direction flag: true means increasing, false means decreasing.
        bool direction_ = true;

        // Boundaries
        int min_ = 0;
        int max_ = 0;
    };

} // namespace Voxium::Core
