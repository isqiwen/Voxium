#pragma once

#include <algorithm>
#include <cassert>
#include <cmath>
#include <vector>

#include "CoreMacros.h"

#include "Math/RingBuffer.h"

namespace Voxium::Core
{

    class CORE_API AccurateMovingAverage
    {
    public:
        explicit AccurateMovingAverage(int period);

        [[nodiscard]] int    Capacity() const;
        [[nodiscard]] double Average() const;
        void                 Add(double value);
        void                 EnsureSize(int capacity);
        void                 Resize(int capacity);

    private:
        RingBuffer<double> buffer_;
    };

    class CORE_API MovingAverage
    {
    public:
        explicit MovingAverage(int period);

        [[nodiscard]] int    Capacity() const;
        [[nodiscard]] double Average() const;
        void                 Add(double value);
        void                 EnsureSize(int capacity);
        void                 Resize(int capacity);

    private:
        double             total_;
        RingBuffer<double> buffer_;
    };

} // namespace Voxium::Core
