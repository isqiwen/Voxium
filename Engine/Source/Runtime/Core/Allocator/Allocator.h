#pragma once

#include <cstdlib>
#include <cstring>
#include <cmath>
#include <mutex>
#include <sstream>
#include <string>
#include <cassert>
#include <type_traits>

namespace Voxium::Core {

    inline std::size_t gUnsafeMemory{0};
    inline std::size_t gBufferMemory{0};
    inline std::size_t gInstanceMemory{0};
    inline std::size_t gIndirectMemory{0};
    inline std::size_t gSSBOMemory{0};
    inline std::size_t gUBOMemory{0};
    inline std::size_t gPBOUIMemory{0};
    inline std::size_t gTextureMemory{0};
    inline std::size_t gTextureUIMemory{0};
    inline std::size_t gTextureSVGMemory{0};
    inline std::size_t gTextureMinimapMemory{0};
    inline std::size_t gTextureSkyboxMemory{0};
    inline std::size_t gTextureRenderMemory{0};

    inline std::mutex gAllocatorMutex;

    inline double FormatMemory(std::size_t bytes) {
        return std::round((bytes / 1024.0 / 1024.0) * 10.0) / 10.0;
    }

    inline std::string MemoryString() {
        std::lock_guard<std::mutex> lock(gAllocatorMutex);
        std::ostringstream oss;
        oss << "Unsafe: " << FormatMemory(gUnsafeMemory) << " MB\n";
            << "Buffer: " << FormatMemory(gBufferMemory) << " MB\n"
            << "Instance: " << FormatMemory(gInstanceMemory) << " MB\n"
            << "Indirect: " << FormatMemory(gIndirectMemory) << " MB\n"
            << "SSBO: " << FormatMemory(gSSBOMemory) << " MB\n"
            << "UBO: " << FormatMemory(gUBOMemory) << " MB\n"
            << "PBOUI: " << FormatMemory(gPBOUIMemory) << " MB\n"
            << "Texture: " << FormatMemory(gTextureMemory) << " MB\n"
            << "TextureUI: " << FormatMemory(gTextureUIMemory) << " MB\n"
            << "TextureSVG: " << FormatMemory(gTextureSVGMemory) << " MB\n"
            << "TextureMinimap: " << FormatMemory(gTextureMinimapMemory) << " MB\n"
            << "TextureSkybox: " << FormatMemory(gTextureSkyboxMemory) << " MB\n"
            << "TextureRender: " << FormatMemory(gTextureRenderMemory) << " MB";
        return oss.str();
    }

    inline void CopyMemory(void* dest, const void* src, std::size_t count) {
        std::memcpy(dest, src, count);
    }

    inline void ZeroMemory(void* dest, std::size_t count) {
        std::memset(dest, 0, count);
    }

    inline void IncrementUnsafeMemory(std::size_t bytes) {
        std::lock_guard<std::mutex> lock(gAllocatorMutex);
        assert(bytes > 0);
        gUnsafeMemory += bytes;
    }

    inline void DecrementUnsafeMemory(std::size_t bytes) {
        std::lock_guard<std::mutex> lock(gAllocatorMutex);
        assert(gUnsafeMemory >= bytes);
        assert(bytes > 0);
        gUnsafeMemory -= bytes;
    }

    inline void* Alloc(std::size_t byteCount) {
        DEBUG_CALL(IncrementUnsafeMemory(byteCount));
        void* ptr = std::malloc(byteCount);
        if (!ptr) throw std::bad_alloc();
        return ptr;
    }

    inline void* AllocZeroed(std::size_t byteCount) {
        DEBUG_CALL(IncrementUnsafeMemory(byteCount));
        void* ptr = std::calloc(1, byteCount);
        if (!ptr) throw std::bad_alloc();
        return ptr;
    }

    template<typename T>
    inline T* AllocSmart(int amount) {
        static_assert(std::is_trivially_copyable_v<T>, "T must be trivially copyable");
        std::size_t byteCount = static_cast<std::size_t>(amount) * sizeof(T);
        DEBUG_CALL(IncrementUnsafeMemory(byteCount));
        void* ptr = std::malloc(byteCount);
        if (!ptr) throw std::bad_alloc();
        return reinterpret_cast<T*>(ptr);
    }

    template<typename T>
    inline T* AllocSmart(int amount, int& byteCount) {
        static_assert(std::is_trivially_copyable_v<T>, "T must be trivially copyable");
        byteCount = amount * static_cast<int>(sizeof(T));
        DEBUG_CALL(IncrementUnsafeMemory(byteCount));
        void* ptr = std::malloc(byteCount);
        if (!ptr) throw std::bad_alloc();
        return reinterpret_cast<T*>(ptr);
    }

    template<typename T>
    inline T* AllocZeroedSmart(int amount) {
        static_assert(std::is_trivially_copyable_v<T>, "T must be trivially copyable");
        std::size_t byteCount = static_cast<std::size_t>(amount) * sizeof(T);
        DEBUG_CALL(IncrementUnsafeMemory(byteCount));
        void* ptr = std::calloc(1, byteCount);
        if (!ptr) throw std::bad_alloc();
        return reinterpret_cast<T*>(ptr);
    }

    template<typename T>
    inline T* AllocZeroedSmart(int amount, int& byteCount) {
        static_assert(std::is_trivially_copyable_v<T>, "T must be trivially copyable");
        byteCount = amount * static_cast<int>(sizeof(T));
        DEBUG_CALL(IncrementUnsafeMemory(byteCount));
        void* ptr = std::calloc(1, byteCount);
        if (!ptr) throw std::bad_alloc();
        return reinterpret_cast<T*>(ptr);
    }

    template<typename T>
    inline T* Realloc(T* data, std::size_t byteCount, std::size_t oldLength) {
        if (oldLength > 0)
            DEBUG_CALL(DecrementUnsafeMemory(oldLength));
        DEBUG_CALL(IncrementUnsafeMemory(byteCount));
        if (data == nullptr) {
            void* ptr = std::malloc(byteCount);
            if (!ptr) throw std::bad_alloc();
            return reinterpret_cast<T*>(ptr);
        }
        void* ptr = std::realloc(data, byteCount);
        if (!ptr) throw std::bad_alloc();
        return reinterpret_cast<T*>(ptr);
    }

    inline void* Realloc(void* data, std::size_t byteCount, std::size_t oldLength) {
        if (oldLength > 0)
            DEBUG_CALL(DecrementUnsafeMemory(oldLength));
        DEBUG_CALL(IncrementUnsafeMemory(byteCount));
        if (data == nullptr) {
            void* ptr = std::malloc(byteCount);
            if (!ptr) throw std::bad_alloc();
            return ptr;
        }
        void* ptr = std::realloc(data, byteCount);
        if (!ptr) throw std::bad_alloc();
        return ptr;
    }

    template<typename T>
    inline void FreeSmart(T* data, int amount) {
        if (data != nullptr) {
            int byteCount = amount * static_cast<int>(sizeof(T));
            DEBUG_CALL(DecrementUnsafeMemory(byteCount));
            std::free(data);
        } else {
            assert(amount == 0);
        }
        data = nullptr;
    }

    template<typename T>
    inline void Free2(T* data, int bytes) {
        if (data != nullptr) {
            DEBUG_CALL(DecrementUnsafeMemory(bytes);
  )          std::free(data);
        } else {
            assert(bytes == 0);
        }
        data = nullptr;
    }
} // namespace Voxium::Core
