#pragma once

#include <algorithm>
#include <cassert>
#include <cmath>
#include <vector>

#include "RingBuffer.h"

namespace Voxium::Core
{

    class AccurateMovingAverage
    {
    public:
        explicit AccurateMovingAverage(int period) : buffer_(static_cast<size_t>(period)) { assert(period > 0); }

        [[nodiscard]] int Capacity() const { return static_cast<int>(buffer_.Capacity()); }

        [[nodiscard]] double Average() const
        {
            double total_ = 0.0;
            size_t cnt    = buffer_.Count();
            for (size_t i = 0; i < cnt; i++)
            {
                total_ += buffer_[i];
            }
            return total_ / std::max<size_t>(1, cnt);
        }

        void Add(double value)
        {
            if (buffer_.IsFull())
            {
                double oldVal = 0.0;
                buffer_.Dequeue(oldVal);
            }
            buffer_.Enqueue(value);
        }

        void EnsureSize(int capacity)
        {
            if (Capacity() != capacity)
                Resize(capacity);
        }

        void Resize(int capacity) { buffer_.Reset(static_cast<size_t>(capacity)); }

    private:
        RingBuffer<double> buffer_;
    };

    class MovingAverage
    {
    public:
        explicit MovingAverage(int period) : buffer_(static_cast<size_t>(period)), total_(0.0) { assert(period > 0); }

        [[nodiscard]] int Capacity() const { return static_cast<int>(buffer_.Capacity()); }

        [[nodiscard]] double Average() const { return total_ / std::max<size_t>(1, buffer_.Count()); }

        void Add(double value)
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

        void EnsureSize(int capacity)
        {
            if (Capacity() != capacity)
                Resize(capacity);
        }

        void Resize(int capacity)
        {
            buffer_.Reset(static_cast<size_t>(capacity));
            total_ = 0.0;
        }

    private:
        double             total_;
        RingBuffer<double> buffer_;
    };

} // namespace Voxium::Core
