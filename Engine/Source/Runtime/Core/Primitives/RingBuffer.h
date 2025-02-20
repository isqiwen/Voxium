#pragma once

#include <vector>
#include <stdexcept>
#include <cassert>

namespace Voxium::Core {

    template<typename T>
    class RingBuffer {
    public:
        // 构造函数：传入容量（逻辑容量，内部存储空间分配为 capacity+1）
        explicit RingBuffer(int capacity) {
            Reset(capacity);
        }

        RingBuffer(int capacity, const T& initialFill) {
            Reset(capacity);
            Fill(initialFill);
        }

        void Reset(int capacity) {
            if (capacity <= 0) {
                throw std::invalid_argument("Capacity must be greater than 0");
            }
            // 内部数组大小为 capacity + 1
            elems_.resize(capacity + 1);
            readPos_ = 0;
            writePos_ = 0;
        }

        // 使用当前逻辑容量（elems_.size()-1）重置缓冲区
        void Reset() {
            Reset(static_cast<int>(elems_.size()) - 1);
        }

        // DequeueEnqueue：先从缓冲区中取出最旧元素，再将新元素放入队列尾部，并返回被移除的元素
        T DequeueEnqueue(const T& item) {
            if (readPos_ == writePos_) {
                throw std::runtime_error("RingBuffer is empty");
            }
            T retv = elems_[readPos_];
            readPos_ = (readPos_ + 1) % elems_.size();
            elems_[writePos_] = item;
            writePos_ = (writePos_ + 1) % elems_.size();
            return retv;
        }

        // Enqueue：尝试将 item 加入缓冲区，如果满了则返回 false
        bool Enqueue(const T& item) {
            int nextWritePos = (writePos_ + 1) % elems_.size();
            if (nextWritePos == readPos_) {
                return false; // 缓冲区已满
            }
            elems_[writePos_] = item;
            writePos_ = nextWritePos;
            return true;
        }

        // Dequeue：不返回值地从缓冲区移除最旧的元素，若为空则返回 false
        bool Dequeue() {
            if (readPos_ == writePos_) {
                return false;
            }
            readPos_ = (readPos_ + 1) % elems_.size();
            return true;
        }

        // Dequeue：将最旧元素移除，并将其值赋给 item；若为空则返回 false
        bool Dequeue(T& item) {
            if (readPos_ == writePos_) {
                return false;
            }
            item = elems_[readPos_];
            readPos_ = (readPos_ + 1) % elems_.size();
            return true;
        }

        // Count：返回缓冲区中当前存储的元素个数
        int Count() const {
            int val = writePos_ - readPos_;
            if (val < 0) {
                val += static_cast<int>(elems_.size());
            }
            return val;
        }

        // Fill：将缓冲区填充为指定值，直到满
        void Fill(const T& value) {
            while (!IsFull()) {
                Enqueue(value);
            }
        }

        // 判断缓冲区是否已满
        bool IsFull() const {
            return ((writePos_ + 1) % elems_.size()) == readPos_;
        }

        // 判断缓冲区是否为空
        bool IsEmpty() const {
            return readPos_ == writePos_;
        }

        // Capacity：返回逻辑容量（内部数组大小 - 1）
        int Capacity() const {
            return static_cast<int>(elems_.size()) - 1;
        }

        // 下标运算符：按逻辑顺序访问缓冲区中的元素
        T operator[](int index) const {
            if (index < 0 || index >= Count()) {
                throw std::invalid_argument("Index out of range");
            }
            return elems_[(readPos_ + index) % elems_.size()];
        }

    private:
        std::vector<T> elems_;
        int readPos_ = 0;
        int writePos_ = 0;
    };

}
