#include "Math/CustomWatch.h"

namespace Voxium::Core
{

    CustomWatch::CustomWatch(int min, double seconds, bool direction, bool start) { Initialise(min, static_cast<int>(seconds * 1000), direction, start); }

    CustomWatch(int min, int max, bool direction, bool start) { Initialise(min, max, direction, start); }

    CustomWatch CustomWatch::Copy() const
    {
        CustomWatch cw(min_, max_, Direction(), false);
        cw.SetElapsed(Elapsed());
        return cw;
    }

    void CustomWatch::Initialise(int min, int max, bool direction, bool start)
    {
        min_       = min;
        max_       = max;
        direction_ = direction;

        if (!direction)
            priorElapsed_ = max;

        // Reset here as setting direction above starts the watch
        if (!start)
            watch.Stop();
    }

    void CustomWatch::Recreate(int newMin, int newMax)
    {
        SetMin(newMin);
        SetMax(newMax);
        if (Direction())
            Restart(Min());
        else
            Restart(Max());
    }

    void CustomWatch::Recreate(int newMin, int newMax, int elapsed, bool direction)
    {
        SetMin(newMin);
        SetMax(newMax);
        Reset();
        SetElapsed(elapsed);
        direction_ = direction;
        Start();
    }

    void CustomWatch::Recreate(const CustomWatch& w)
    {
        SetMin(w.Min());
        SetMax(w.Max());
        Direction(w.Direction());
        Reset();
        SetElapsed(w.Elapsed());
    }

    void CustomWatch::SetRate(double r)
    {
        if (Rate() != r)
            Rate(r);
    }

    void CustomWatch::SetRateNoStart(double r)
    {
        if (Rate() != r)
        {
            SetElapsed(Elapsed());
            r_ = r;
        }
    }

    double CustomWatch::Rate() const { return r_; }

    void CustomWatch::Rate(double newRate)
    {
        // Save current elapsed time before changing the rate.
        SetElapsed(Elapsed());
        RestartWatch(); // Restart stopwatch to measure new elapsed time
        r_ = newRate;
    }

    void CustomWatch::Start()
    {
        if (!isRunning_)
        {
            startTime_ = std::chrono::steady_clock::now();
            isRunning_ = true;
        }
    }

    void CustomWatch::Stop()
    {
        if (isRunning_)
        {
            // Simply mark as not running; Elapsed() will compute current elapsed.
            isRunning_ = false;
        }
    }

    void CustomWatch::Reset()
    {
        isRunning_    = false;
        startTime_    = std::chrono::steady_clock::now();
        priorElapsed_ = 0;
    }

    void CustomWatch::Restart()
    {
        Reset();
        Start();
        priorElapsed_ = 0;
    }

    void CustomWatch::Restart(int e)
    {
        Reset();
        priorElapsed_ = e;
        Start();
    }

    void CustomWatch::RestartMax() { Restart(Max()); }

    void CustomWatch::SetDirection(bool d)
    {
        if (Direction() != d)
            Direction(d);
        else
            Start();
    }

    void CustomWatch::SetDirectionNoStart(bool d)
    {
        if (Direction() != d)
        {
            SetElapsed(Elapsed());
            direction_ = d;
        }
    }

    bool CustomWatch::Direction() const { return direction_; }

    void CustomWatch::Direction(bool d)
    {
        SetElapsed(Elapsed());
        direction_ = d;
        RestartWatch(); // Restart stopwatch when direction changes.
    }

    int CustomWatch::Elapsed() const
    {
        long currentElapsed = 0;
        if (isRunning_)
        {
            auto now       = std::chrono::steady_clock::now();
            currentElapsed = static_cast<long>(std::chrono::duration_cast<std::chrono::milliseconds>(now - startTime_).count() * r_);
        }
        if (direction_)
        {
            if (priorElapsed_ + currentElapsed > Max())
            {
                // If overflow, stop stopwatch and return max.
                // Note: cannot modify mutable members in a const function in production code.
                return Max();
            }
            return static_cast<int>(priorElapsed_ + currentElapsed);
        }
        else
        {
            if (priorElapsed_ - currentElapsed < Min())
            {
                return min;
            }
            return static_cast<int>(priorElapsed_ - currentElapsed);
        }
    }

    void CustomWatch::SetElapsed(int e) { priorElapsed_ = e; }

    double CustomWatch::Progress() const { return Max() == 0 ? 1.0 : static_cast<double>(Elapsed()) / Max(); }

    uint8_t CustomWatch::ProgressB() const { return Max() == 0 ? 255 : static_cast<uint8_t>(Elapsed() / static_cast<double>(Max()) * 255.0); }

    float CustomWatch::ProgressF() const { return Max() == 0 ? 1.0f : static_cast<float>(Elapsed()) / Max(); }

    double CustomWatch::ProgressEase() const
    {
        double t = Progress();
        return t * (2 - t);
    }

    float CustomWatch::ProgressEaseF() const
    {
        float t = ProgressF();
        return t * (2 - t);
    }

    float CustomWatch::ProgressFancyEaseF() const
    {
        float t = ProgressF();
        return t >= 0.5f ? (-2 * t * t) + (4 * t) - 1 : t;
    }

    float CustomWatch::ModernEaseF() const
    {
        float x = ProgressF();
        return x < 0.5f ? 4 * x * x * x : 1 - std::pow(-2 * x + 2, 3) / 2;
    }

    double CustomWatch::ProgressQuad() const
    {
        double t = Progress();
        return t < 0.5 ? 2 * t * t : 1 - std::pow(-2 * t + 2, 2) / 2;
    }

    float CustomWatch::ProgressQuadF() const
    {
        float t = ProgressF();
        return t < 0.5f ? 2 * t * t : 1 - std::pow(-2 * t + 2, 2) / 2;
    }

    long CustomWatch::RawElapsed() const
    {
        if (isRunning_)
        {
            auto now = std::chrono::steady_clock::now();
            return static_cast<long>(std::chrono::duration_cast<std::chrono::milliseconds>(now - startTime_).count());
        }
        return 0;
    }

    bool CustomWatch::IsRunning() const { return isRunning_; }

    int CustomWatch::Max() const { return max_; }

    void CustomWatch::SetMax(int m) { max_ = m; }

    int CustomWatch::Min() const { return min_; }

    void CustomWatch::SetMin(int m) { min_ = m; }

    void CustomWatch::RestartWatch() { startTime_ = std::chrono::steady_clock::now(); }

} // namespace Voxium::Core
