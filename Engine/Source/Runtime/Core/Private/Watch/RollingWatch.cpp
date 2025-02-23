#include "Watch/StopWatch.h"

namespace Voxium::Core
{

    RollingWatch::RollingWatch(int frameCount, float speed, bool direction, bool start, bool wrap, int startTime) :
        max_(frameCount * BASE_SPEED), speed_(BASE_SPEED / speed), direction_(direction), wrap_(wrap), min_(0), priorElapsed_(0)
    {
        // If direction is false (backwards), start at max - startTime; otherwise, start at startTime.
        if (!direction)
            setElapsed(max - startTime);
        else
            setElapsed(startTime);

        // If not starting, reset the stopwatch; else start it.
        if (!start)
            watch.Reset();
        else
            watch.Start();
    }

    // Constructor with fixed frameCount, speed, direction, start flag, and wrap flag (default startTime = 0)
    RollingWatch::RollingWatch(int frameCount, float speed, bool direction, bool start, bool wrap) : RollingWatch(frameCount, speed, direction, start, wrap, 0)
    {}

    // ----------------------------------------------------------------
    // Copy method: creates a copy of the current watch.
    // ----------------------------------------------------------------
    RollingWatch RollingWatch::Copy() const
    {
        // Create a new RollingWatch with the same configuration.
        RollingWatch rw(max / BASE_SPEED, BASE_SPEED / _speed, _direction, false, wrap, 0);
        rw.setElapsed(Elapsed());
        return rw;
    }

    // ----------------------------------------------------------------
    // Stopwatch control methods.
    // ----------------------------------------------------------------
    void RollingWatch::Start() { watch.Start(); }
    void RollingWatch::Stop() { watch.Stop(); }
    void RollingWatch::Reset()
    {
        watch.Reset();
        priorElapsed = 0;
    }
    void RollingWatch::Restart()
    {
        watch.Restart();
        priorElapsed = 0;
        if (!_direction) // If running backwards, reset elapsed to max.
            setElapsed(max);
    }
    void RollingWatch::Restart(int e)
    {
        watch.Reset();
        priorElapsed = e;
        watch.Start();
    }

    // ----------------------------------------------------------------
    // Direction control
    // ----------------------------------------------------------------
    // Get current direction.
    bool RollingWatch::Direction() const { return _direction; }
    // Set direction and restart stopwatch. (Wrap check omitted in this example.)
    void RollingWatch::Direction(bool d)
    {
        // Update accumulated elapsed time.
        priorElapsed = static_cast<long>(Elapsed());
        _direction   = d;
        watch.Restart();
    }
    // Set direction if different.
    void RollingWatch::SetDirection(bool d)
    {
        if (_direction != d)
            Direction(d);
    }
    void RollingWatch::SetDirectionNoStart(bool d)
    {
        if (_direction != d)
        {
            priorElapsed = static_cast<long>(Elapsed());
            _direction   = d;
        }
    }

    // ----------------------------------------------------------------
    // Speed control
    // ----------------------------------------------------------------
    float RollingWatch::Speed() const { return _speed; }
    void  RollingWatch::Speed(float s)
    {
        // Ensure new speed is finite.
        assert(std::isfinite(s));
        if (std::abs(_speed - s) < std::numeric_limits<float>::epsilon())
            return;
        priorElapsed = static_cast<long>(Elapsed());
        watch.Restart();
        _speed = s;
    }
    void RollingWatch::SetRateNoStart(float s)
    {
        if (std::abs(_speed - s) >= std::numeric_limits<float>::epsilon())
        {
            priorElapsed = static_cast<long>(Elapsed());
            _speed       = s;
        }
    }

    // ----------------------------------------------------------------
    // Elapsed time calculation (in milliseconds, adjusted by speed and direction)
    // ----------------------------------------------------------------
    float RollingWatch::Elapsed() const
    {
        // CurrentElapsed = watch.ElapsedMilliseconds * speed
        float currentElapsed = static_cast<float>(watch.ElapsedMilliseconds()) * _speed;
        if (_direction)
        {
            float current = static_cast<float>(priorElapsed) + currentElapsed;
            if (current > max)
            {
                if (wrap)
                {
                    float extra  = current - max;
                    priorElapsed = static_cast<long>(extra);
                    watch.Restart();
                    return static_cast<float>(priorElapsed) + static_cast<float>(watch.ElapsedMilliseconds()) * _speed;
                }
                else
                {
                    priorElapsed = max;
                    watch.Stop();
                    return static_cast<float>(priorElapsed);
                }
            }
            return current;
        }
        else
        {
            float current = static_cast<float>(priorElapsed) - currentElapsed;
            if (current < min)
            {
                if (wrap)
                {
                    Restart(max);
                    return static_cast<float>(priorElapsed) - static_cast<float>(watch.ElapsedMilliseconds()) * _speed;
                }
                else
                {
                    priorElapsed = min;
                    watch.Stop();
                    return static_cast<float>(priorElapsed);
                }
            }
            return current;
        }
    }

    // Setter for Elapsed, sets the priorElapsed.
    void RollingWatch::setElapsed(int e)
    {
        priorElapsed = e;
        assert(std::isfinite(static_cast<float>(priorElapsed)));
    }

    // ----------------------------------------------------------------
    // Progress calculation: returns a value between 0 and 1.
    // ----------------------------------------------------------------
    double RollingWatch::Progress() const
    {
        if (max == 0)
            return 1.0;
        return std::min(1.0, static_cast<double>(Elapsed()) / max);
    }
    float RollingWatch::ProgressF() const { return static_cast<float>(Progress()); }

    // ----------------------------------------------------------------
    // Running state
    // ----------------------------------------------------------------
    bool RollingWatch::IsRunning() const { return watch.IsRunning(); }

    // Getter and setter for max.
    int  RollingWatch::maxValue() const { return max; }
    void RollingWatch::setMax(int m) { max = m; }

    // 'min' is public in C#, here we assume min is 0.
    int  RollingWatch::MinValue() const { return min; }
    void RollingWatch::RestartWatch() { watch.Restart(); }

} // namespace Voxium::Core
