#include "Math/MovingAverage.h"

namespace Voxium::Core
{

    AccurateMovingAverage::AccurateMovingAverage(int period) : buffer_(static_cast<size_t>(period))
    {
        assert(period > 0);
    }

    int AccurateMovingAverage::Capacity() const { return static_cast<int>(buffer_.Capacity()); }

    double AccurateMovingAverage::Average() const
    {
        double total_ = 0.0;
        size_t cnt    = buffer_.Count();
        for (size_t i = 0; i < cnt; i++)
        {
            total_ += buffer_[i];
        }
        return total_ / std::max<size_t>(1, cnt);
    }

    void AccurateMovingAverage::Add(double value)
    {
        if (buffer_.IsFull())
        {
            double oldVal = 0.0;
            buffer_.Dequeue(oldVal);
        }
        buffer_.Enqueue(value);
    }

    void AccurateMovingAverage::EnsureSize(int capacity)
    {
        if (Capacity() != capacity)
            Resize(capacity);
    }

    void AccurateMovingAverage::Resize(int capacity) { buffer_.Reset(static_cast<size_t>(capacity)); }

    MovingAverage::MovingAverage(int period) : buffer_(static_cast<size_t>(period)), total_(0.0) { assert(period > 0); }

    int MovingAverage::Capacity() const { return static_cast<int>(buffer_.Capacity()); }

    double MovingAverage::Average() const { return total_ / std::max<size_t>(1, buffer_.Count()); }

    void MovingAverage::Add(double value)
    {
        if (buffer_.IsFull())
        {
            double oldVal = 0.0;
            buffer_.Dequeue(oldVal);
            total_ -= oldVal;
        }
        total_ += value;
        buffer_.Enqueue(value);
    }

    void MovingAverage::EnsureSize(int capacity)
    {
        if (Capacity() != capacity)
            Resize(capacity);
    }

    void MovingAverage::Resize(int capacity)
    {
        buffer_.Reset(static_cast<size_t>(capacity));
        total_ = 0.0;
    }
} // namespace Voxium::Core
