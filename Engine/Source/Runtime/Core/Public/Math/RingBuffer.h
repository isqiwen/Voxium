#pragma once

#include <cassert>
#include <stdexcept>
#include <vector>

#include "CoreMacros.h"

namespace Voxium::Core
{

    template<typename T>
    class RingBuffer
    {
    public:
        explicit RingBuffer(int capacity) { Reset(capacity); }

        RingBuffer(int capacity, const T& initialFill)
        {
            Reset(capacity);
            Fill(initialFill);
        }

        void Reset(int capacity)
        {
            if (capacity <= 0)
            {
                throw std::invalid_argument("Capacity must be greater than 0");
            }
            elems_.resize(capacity + 1);
            readPos_  = 0;
            writePos_ = 0;
        }

        void Reset() { Reset(static_cast<int>(elems_.size()) - 1); }

        T DequeueEnqueue(const T& item)
        {
            if (readPos_ == writePos_)
            {
                throw std::runtime_error("RingBuffer is empty");
            }
            T retv            = elems_[readPos_];
            readPos_          = (readPos_ + 1) % elems_.size();
            elems_[writePos_] = item;
            writePos_         = (writePos_ + 1) % elems_.size();
            return retv;
        }

        bool Enqueue(const T& item)
        {
            int nextWritePos = (writePos_ + 1) % elems_.size();
            if (nextWritePos == readPos_)
            {
                return false;
            }
            elems_[writePos_] = item;
            writePos_         = nextWritePos;
            return true;
        }

        bool Dequeue()
        {
            if (readPos_ == writePos_)
            {
                return false;
            }
            readPos_ = (readPos_ + 1) % elems_.size();
            return true;
        }

        bool Dequeue(T& item)
        {
            if (readPos_ == writePos_)
            {
                return false;
            }
            item     = elems_[readPos_];
            readPos_ = (readPos_ + 1) % elems_.size();
            return true;
        }

        int Count() const
        {
            int val = writePos_ - readPos_;
            if (val < 0)
            {
                val += static_cast<int>(elems_.size());
            }
            return val;
        }

        void Fill(const T& value)
        {
            while (!IsFull())
            {
                Enqueue(value);
            }
        }

        bool IsFull() const { return ((writePos_ + 1) % elems_.size()) == readPos_; }

        bool IsEmpty() const { return readPos_ == writePos_; }

        int Capacity() const { return static_cast<int>(elems_.size()) - 1; }

        T operator[](int index) const
        {
            if (index < 0 || index >= Count())
            {
                throw std::invalid_argument("Index out of range");
            }
            return elems_[(readPos_ + index) % elems_.size()];
        }

    private:
        std::vector<T> elems_;
        int            readPos_  = 0;
        int            writePos_ = 0;
    };

} // namespace Voxium::Core
